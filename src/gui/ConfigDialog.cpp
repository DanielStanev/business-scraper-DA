#include "gui/ConfigDialog.h"
#include "utils/ConfigManager.h"
#include <QtWidgets/QApplication>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>

ConfigDialog::ConfigDialog(QWidget *parent)
    : QDialog(parent)
    , m_apiKeyEdit(nullptr)
    , m_saveButton(nullptr)
    , m_cancelButton(nullptr)
    , m_testButton(nullptr)
    , m_statusLabel(nullptr)
{
    setWindowTitle("Configuration");
    setModal(true);
    setFixedSize(500, 320);

    setupUI();
    loadCurrentConfig();
}

void ConfigDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(16, 16, 16, 16);

    // API Key group
    QGroupBox* apiGroup = new QGroupBox("Google Maps API Configuration");
    QFormLayout* apiLayout = new QFormLayout(apiGroup);
    apiLayout->setVerticalSpacing(12);
    apiLayout->setHorizontalSpacing(8);

    m_apiKeyEdit = new QLineEdit();
    m_apiKeyEdit->setEchoMode(QLineEdit::Password);
    m_apiKeyEdit->setPlaceholderText("Enter your Google Maps API key...");
    m_apiKeyEdit->setMinimumHeight(28);

    m_testButton = new QPushButton("Test Key");
    m_testButton->setToolTip("Test if the API key is valid");
    m_testButton->setMinimumHeight(28);

    QHBoxLayout* keyLayout = new QHBoxLayout();
    keyLayout->setSpacing(8);
    keyLayout->addWidget(m_apiKeyEdit);
    keyLayout->addWidget(m_testButton);

    apiLayout->addRow("API Key:", keyLayout);

    // Status label
    m_statusLabel = new QLabel();
    m_statusLabel->setStyleSheet("QLabel { color: blue; font-style: italic; }");
    apiLayout->addRow(m_statusLabel);

    // Instructions
    QLabel* instructionsLabel = new QLabel(
        "To get a Google Maps API key:\n"
        "1. Go to Google Cloud Console\n"
        "2. Enable Places API and Maps JavaScript API\n"
        "3. Create credentials (API Key)\n"
        "4. Restrict the key to Places API for security"
    );
    instructionsLabel->setWordWrap(true);
    instructionsLabel->setStyleSheet("QLabel { color: gray; font-size: 10px; }");
    instructionsLabel->setMinimumHeight(80);

    mainLayout->addWidget(apiGroup);
    mainLayout->addWidget(instructionsLabel);
    mainLayout->addStretch(); // Add stretch to push buttons to bottom

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(8);
    buttonLayout->addStretch();

    m_cancelButton = new QPushButton("Cancel");
    m_saveButton = new QPushButton("Save");
    m_saveButton->setDefault(true);

    // Set minimum button sizes
    m_cancelButton->setMinimumSize(80, 32);
    m_saveButton->setMinimumSize(80, 32);

    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_saveButton);

    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(m_saveButton, &QPushButton::clicked, this, &ConfigDialog::onSaveClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &ConfigDialog::onCancelClicked);
    connect(m_testButton, &QPushButton::clicked, this, &ConfigDialog::onTestApiKey);
    connect(m_apiKeyEdit, &QLineEdit::textChanged, [this]() {
        m_statusLabel->clear();
    });
}

void ConfigDialog::loadCurrentConfig()
{
    ConfigManager config;

    // Try to load from current directory first, then from app directory
    QString configPath = "config.ini";
    if (!QFile::exists(configPath)) {
        configPath = QApplication::applicationDirPath() + "/../config.ini";
    }

    if (config.load_config(configPath.toStdString())) {
        QString apiKey = QString::fromStdString(config.get_api_key());
        if (!apiKey.isEmpty()) {
            m_apiKeyEdit->setText(apiKey);
            m_statusLabel->setText("Current API key loaded");
            m_statusLabel->setStyleSheet("QLabel { color: green; font-style: italic; }");
        }
    }
}

bool ConfigDialog::saveConfig()
{
    QString apiKey = m_apiKeyEdit->text().trimmed();

    if (apiKey.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input",
            "Please enter a valid API key.");
        return false;
    }

    ConfigManager config;
    config.set_api_key(apiKey.toStdString());

    // Save to the project root directory
    QString configPath = QApplication::applicationDirPath() + "/../config.ini";

    // Ensure the directory exists
    QDir dir = QFileInfo(configPath).absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    if (config.save_config(configPath.toStdString())) {
        m_statusLabel->setText("Configuration saved successfully!");
        m_statusLabel->setStyleSheet("QLabel { color: green; font-style: italic; }");
        return true;
    } else {
        QMessageBox::critical(this, "Save Error",
            QString("Failed to save configuration:\n%1")
            .arg(QString::fromStdString(config.last_error())));
        return false;
    }
}

QString ConfigDialog::getApiKey() const
{
    return m_apiKeyEdit->text().trimmed();
}

void ConfigDialog::setApiKey(const QString &apiKey)
{
    m_apiKeyEdit->setText(apiKey);
}

void ConfigDialog::onSaveClicked()
{
    if (saveConfig()) {
        accept();
    }
}

void ConfigDialog::onCancelClicked()
{
    reject();
}

void ConfigDialog::onTestApiKey()
{
    QString apiKey = m_apiKeyEdit->text().trimmed();

    if (apiKey.isEmpty()) {
        m_statusLabel->setText("Please enter an API key first");
        m_statusLabel->setStyleSheet("QLabel { color: red; font-style: italic; }");
        return;
    }

    m_testButton->setEnabled(false);
    m_testButton->setText("Testing...");
    m_statusLabel->setText("Testing API key validity...");
    m_statusLabel->setStyleSheet("QLabel { color: blue; font-style: italic; }");

    // For now, just validate the format (starts with AIza and reasonable length)
    bool validFormat = apiKey.startsWith("AIza") && apiKey.length() >= 35 && apiKey.length() <= 45;

    if (validFormat) {
        m_statusLabel->setText("API key format appears valid");
        m_statusLabel->setStyleSheet("QLabel { color: green; font-style: italic; }");
    } else {
        m_statusLabel->setText("API key format appears invalid (should start with 'AIza')");
        m_statusLabel->setStyleSheet("QLabel { color: orange; font-style: italic; }");
    }

    m_testButton->setEnabled(true);
    m_testButton->setText("Test Key");
}
