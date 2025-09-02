#include "gui/MainWindow.h"
#include "gui/SearchWidget.h"
#include "gui/ResultsWidget.h"
#include "gui/ConfigDialog.h"
#include "gui/StyleManager.h"
#include "core/BusinessScraperEngine.h"
#include "utils/ConfigManager.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QSplitter>
#include <QtCore/QThread>
#include <QtCore/QStandardPaths>
#include <QtCore/QSettings>
#include <QtCore/QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_searchWidget(nullptr)
    , m_resultsWidget(nullptr)
    , m_splitter(nullptr)
    , m_statusLabel(nullptr)
    , m_progressBar(nullptr)
    , m_resultCountLabel(nullptr)
    , m_engine(nullptr)
    , m_lastResults(nullptr)
    , m_searchInProgress(false)
    , m_statusTimer(new QTimer(this))
{
    setupUI();
    setupMenuBar();
    setupStatusBar();
    setupConnections();

    // Initialize business engine
    m_engine = new BusinessScraperEngine();

    // Load configuration
    ConfigManager config;
    if (config.load_config()) {
        QString apiKey = QString::fromStdString(config.get_api_key());
        if (!apiKey.isEmpty()) {
            m_engine->set_api_key(apiKey.toStdString());
        } else {
            QMessageBox::warning(this, "Configuration Warning",
                "Google Maps API key not found in config.ini.\n"
                "Please configure your API key before searching.");
        }
    } else {
        QMessageBox::critical(this, "Configuration Error",
            "Could not load configuration file.\n"
            "Please ensure config.ini exists and is readable.");
    }

    // Set up engine callbacks
    m_engine->set_status_callback([this](const std::string& message) {
        QMetaObject::invokeMethod(this, "onStatusUpdate", Qt::QueuedConnection,
            Q_ARG(QString, QString::fromStdString(message)));
    });

    updateWindowTitle();
    resize(1200, 800);
}

MainWindow::~MainWindow()
{
    delete m_lastResults;
}

void MainWindow::setupUI()
{
    // Create central widget and main layout
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create main splitter
    m_splitter = new QSplitter(Qt::Horizontal, this);

    // Create widgets
    m_searchWidget = new SearchWidget(this);
    m_resultsWidget = new ResultsWidget(this);

    // Add widgets to splitter
    m_splitter->addWidget(m_searchWidget);
    m_splitter->addWidget(m_resultsWidget);

    // Set splitter proportions (30% search, 70% results)
    m_splitter->setSizes({300, 700});
    m_splitter->setStretchFactor(0, 0);
    m_splitter->setStretchFactor(1, 1);

    // Set up main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(m_splitter);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    // Apply professional styling
    StyleManager::applyProfessionalStyle(this);
}

void MainWindow::setupMenuBar()
{
    // File menu
    QMenu* fileMenu = menuBar()->addMenu("&File");

    QAction* exportAction = fileMenu->addAction("&Export Results...");
    exportAction->setShortcut(QKeySequence::SaveAs);
    exportAction->setStatusTip("Export search results to file");
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportResults);

    fileMenu->addSeparator();

    QAction* configAction = fileMenu->addAction("&Configuration...");
    configAction->setStatusTip("Configure application settings");
    connect(configAction, &QAction::triggered, this, &MainWindow::openConfiguration);

    fileMenu->addSeparator();

    QAction* exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip("Exit the application");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // Help menu
    QMenu* helpMenu = menuBar()->addMenu("&Help");

    QAction* licenseAction = helpMenu->addAction("&License Information");
    licenseAction->setStatusTip("View license information");
    connect(licenseAction, &QAction::triggered, this, &MainWindow::showLicenseDialog);

    helpMenu->addSeparator();

    QAction* aboutAction = helpMenu->addAction("&About");
    aboutAction->setStatusTip("About this application");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
}

void MainWindow::setupStatusBar()
{
    // Status label
    m_statusLabel = new QLabel("Ready");
    statusBar()->addWidget(m_statusLabel, 1);

    // Progress bar
    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    m_progressBar->setFixedWidth(200);
    statusBar()->addPermanentWidget(m_progressBar);

    // Result count label
    m_resultCountLabel = new QLabel();
    statusBar()->addPermanentWidget(m_resultCountLabel);
}

void MainWindow::setupConnections()
{
    // Connect search widget signals
    connect(m_searchWidget, &SearchWidget::searchRequested,
            this, &MainWindow::onSearchRequested);

    // Connect results widget signals
    connect(m_resultsWidget, &ResultsWidget::exportRequested,
            this, &MainWindow::exportResults);

    // Status timer for auto-clearing status messages
    m_statusTimer->setSingleShot(true);
    connect(m_statusTimer, &QTimer::timeout, [this]() {
        if (!m_searchInProgress) {
            m_statusLabel->setText("Ready");
        }
    });
}

void MainWindow::onSearchRequested()
{
    if (m_searchInProgress) {
        return;
    }

    if (!m_searchWidget->isValid()) {
        QMessageBox::warning(this, "Invalid Input",
            "Please fill in all required fields before searching.");
        return;
    }

    // Clear previous results
    m_resultsWidget->clearResults();
    delete m_lastResults;
    m_lastResults = nullptr;

    // Update UI state
    m_searchInProgress = true;
    m_searchWidget->setSearchEnabled(false);
    m_progressBar->setVisible(true);
    m_progressBar->setRange(0, 0); // Indeterminate progress
    m_resultCountLabel->clear();

    // Get search options
    SearchOptions options = m_searchWidget->getSearchOptions();

    // Start search in background thread
    QThread* searchThread = QThread::create([this, options]() {
        SearchResults results = m_engine->search_businesses(options);

        // Store results and signal completion
        QMetaObject::invokeMethod(this, [this, results]() {
            m_lastResults = new SearchResults(results);
            onSearchCompleted();
        }, Qt::QueuedConnection);
    });

    connect(searchThread, &QThread::finished, searchThread, &QThread::deleteLater);
    searchThread->start();

    updateWindowTitle("Searching...");
}

void MainWindow::onSearchCompleted()
{
    // Update UI state
    m_searchInProgress = false;
    m_searchWidget->setSearchEnabled(true);
    m_progressBar->setVisible(false);

    if (m_lastResults && m_lastResults->success) {
        // Display results
        m_resultsWidget->displayResults(*m_lastResults);

        // Update status
        QString statusText = QString("Found %1 businesses").arg(m_lastResults->total_found);
        if (m_lastResults->enhanced_count > 0) {
            statusText += QString(" (%1 enhanced with website data)").arg(m_lastResults->enhanced_count);
        }
        m_statusLabel->setText(statusText);
        m_resultCountLabel->setText(QString("%1 results").arg(m_lastResults->total_found));

        updateWindowTitle(QString("%1 results").arg(m_lastResults->total_found));

        // Auto-clear status after 10 seconds
        m_statusTimer->start(10000);

    } else {
        // Show error
        QString error = m_lastResults ? QString::fromStdString(m_lastResults->error_message) : "Unknown error";
        QMessageBox::critical(this, "Search Failed",
            QString("Search failed: %1").arg(error));

        m_statusLabel->setText("Search failed");
        updateWindowTitle();
    }
}

void MainWindow::onStatusUpdate(const QString& message)
{
    m_statusLabel->setText(message);
}

void MainWindow::showAboutDialog()
{
    QMessageBox::about(this, "About Business Scraper",
        "<h3>Business Scraper v1.0</h3>"
        "<p>A professional business search and data collection tool.</p>"
        "<p>Built with:</p>"
        "<ul>"
        "<li>Qt 6 Framework (LGPL)</li>"
        "<li>Google Maps API</li>"
        "<li>libcurl for web requests</li>"
        "<li>jsoncpp for JSON parsing</li>"
        "</ul>"
        "<p>© 2025 Daniel Stanev</p>");
}

void MainWindow::showLicenseDialog()
{
    QString licenseText =
        "<h3>Business Scraper - License Information</h3>"
        "<h4>Main Application License:</h4>"
        "<p><b>Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</b></p>"
        "<p>Copyright © 2025 Daniel Stanev</p>"
        "<p>You are free to share and adapt this material for non-commercial purposes with proper attribution, "
        "under the same license terms.</p>"
        "<p>License URL: <a href='https://creativecommons.org/licenses/by-nc-sa/4.0/'>CC BY-NC-SA 4.0</a></p>"
        "<br>"
        "<h4>Third-Party Components:</h4>"
        "<p><b>Qt Framework</b> - GNU Lesser General Public License (LGPL) v3<br>"
        "Dynamically linked libraries for GUI functionality.<br>"
        "Source: <a href='https://www.qt.io/download-open-source'>Qt Open Source</a></p>"
        "<br>"
        "<p><b>libcurl</b> - MIT/X derivative license (curl license)<br>"
        "HTTP client functionality for API requests.<br>"
        "Source: <a href='https://curl.se/libcurl/'>libcurl</a></p>"
        "<br>"
        "<p><b>jsoncpp</b> - MIT License<br>"
        "JSON parsing functionality.<br>"
        "Source: <a href='https://github.com/open-source-parsers/jsoncpp'>jsoncpp</a></p>"
        "<br>"
        "<h4>LGPL Compliance:</h4>"
        "<p>Qt libraries are provided as separate files and can be replaced in accordance with LGPL v3 requirements.</p>"
        "<br>"
        "<p><i>For complete license terms, see the LICENSE and LICENSE_THIRD_PARTY files included with this application.</i></p>";

    QMessageBox licenseBox(this);
    licenseBox.setWindowTitle("License Information");
    licenseBox.setTextFormat(Qt::RichText);
    licenseBox.setText(licenseText);
    licenseBox.setIcon(QMessageBox::Information);
    licenseBox.exec();
}

void MainWindow::exportResults()
{
    if (!m_lastResults || !m_lastResults->success || m_lastResults->businesses.empty()) {
        QMessageBox::information(this, "No Results",
            "No search results available to export.");
        return;
    }

    OutputFormat format = m_searchWidget->getOutputFormat();
    QString defaultExt;
    QString filter;

    switch (format) {
        case OutputFormat::CSV:
            defaultExt = "csv";
            filter = "CSV Files (*.csv)";
            break;
        case OutputFormat::JSON:
            defaultExt = "json";
            filter = "JSON Files (*.json)";
            break;
        case OutputFormat::YAML:
            defaultExt = "yml";
            filter = "YAML Files (*.yml)";
            break;
        case OutputFormat::XML:
            defaultExt = "xml";
            filter = "XML Files (*.xml)";
            break;
    }

    QString filename = QFileDialog::getSaveFileName(this,
        "Export Results", QString("business_results.%1").arg(defaultExt), filter);

    if (!filename.isEmpty()) {
        QString content = QString::fromStdString(m_engine->format_results(*m_lastResults, format));

        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << content;

            m_statusLabel->setText(QString("Results exported to %1").arg(QFileInfo(filename).fileName()));
            m_statusTimer->start(5000);
        } else {
            QMessageBox::critical(this, "Export Error",
                QString("Could not write to file: %1").arg(filename));
        }
    }
}

void MainWindow::openConfiguration()
{
    ConfigDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        // Reload configuration after user saves
        ConfigManager config;
        QString configPath = QApplication::applicationDirPath() + "/../config.ini";

        if (config.load_config(configPath.toStdString())) {
            QString apiKey = QString::fromStdString(config.get_api_key());
            if (!apiKey.isEmpty()) {
                m_engine->set_api_key(apiKey.toStdString());
                m_statusLabel->setText("Configuration updated successfully");

                // Clear any previous configuration warnings
                statusBar()->clearMessage();
            }
        }
    }
}

void MainWindow::updateWindowTitle(const QString& subtitle)
{
    QString title = "Business Scraper";
    if (!subtitle.isEmpty()) {
        title += QString(" - %1").arg(subtitle);
    }
    setWindowTitle(title);
}
