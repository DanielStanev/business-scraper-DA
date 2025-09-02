#include "gui/ResultsWidget.h"
#include "gui/TableItemDelegate.h"
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenu>
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>

ResultsWidget::ResultsWidget(QWidget *parent)
    : QWidget(parent)
    , m_resultsTable(nullptr)
    , m_summaryLabel(nullptr)
    , m_exportButton(nullptr)
    , m_copyButton(nullptr)
    , m_resultsGroup(nullptr)
    , m_currentResults(nullptr)
{
    setupUI();
    setupTable();
    setupConnections();
}

void ResultsWidget::setupUI()
{
    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    // Results group
    m_resultsGroup = new QGroupBox("Search Results", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(m_resultsGroup);

    // Summary label
    m_summaryLabel = new QLabel("No search performed yet.", this);
    m_summaryLabel->setStyleSheet("QLabel { color: #666666; font-style: italic; }");
    groupLayout->addWidget(m_summaryLabel);

    // Results table
    m_resultsTable = new QTableWidget(this);
    groupLayout->addWidget(m_resultsTable);

    // Button layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    m_copyButton = new QPushButton("&Copy Selected", this);
    m_copyButton->setEnabled(false);
    m_copyButton->setToolTip("Copy selected business to clipboard");

    m_exportButton = new QPushButton("&Export All...", this);
    m_exportButton->setEnabled(false);
    m_exportButton->setToolTip("Export all results to file");

    buttonLayout->addWidget(m_copyButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_exportButton);

    groupLayout->addLayout(buttonLayout);
    mainLayout->addWidget(m_resultsGroup);
}

void ResultsWidget::setupTable()
{
    // Set up columns
    m_resultsTable->setColumnCount(COL_COUNT);

    QStringList headers;
    headers << "Business Name" << "Address" << "Phone" << "Email" << "Website" << "Rating";
    m_resultsTable->setHorizontalHeaderLabels(headers);

    // Configure table appearance
    m_resultsTable->setAlternatingRowColors(true);
    m_resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_resultsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_resultsTable->setSortingEnabled(true);
    m_resultsTable->setContextMenuPolicy(Qt::CustomContextMenu);

    // Enable scroll bars when needed
    m_resultsTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_resultsTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Configure headers
    QHeaderView* header = m_resultsTable->horizontalHeader();
    header->setStretchLastSection(false);

    // Set specific widths for better readability
    header->setSectionResizeMode(COL_NAME, QHeaderView::Interactive);
    header->setSectionResizeMode(COL_ADDRESS, QHeaderView::Interactive);
    header->setSectionResizeMode(COL_PHONE, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(COL_EMAIL, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(COL_WEBSITE, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(COL_RATING, QHeaderView::ResizeToContents);

    // Set minimum widths for name and address columns
    header->resizeSection(COL_NAME, 250);      // Business Name: 250px minimum
    header->resizeSection(COL_ADDRESS, 300);   // Address: 300px minimum

    // Enable word wrapping for better text display
    m_resultsTable->setWordWrap(true);

    // Set default row height for better text visibility
    m_resultsTable->verticalHeader()->setDefaultSectionSize(40);

    // Set custom item delegate for proper in-cell editing
    m_resultsTable->setItemDelegate(new TableItemDelegate(this));
}

void ResultsWidget::setupConnections()
{
    connect(m_exportButton, &QPushButton::clicked, this, &ResultsWidget::onExportClicked);
    connect(m_copyButton, &QPushButton::clicked, this, &ResultsWidget::onCopyClicked);
    connect(m_resultsTable, &QTableWidget::cellDoubleClicked,
            this, &ResultsWidget::onCellDoubleClicked);
    connect(m_resultsTable, &QTableWidget::customContextMenuRequested,
            this, &ResultsWidget::showContextMenu);

    // Enable copy button when selection changes
    connect(m_resultsTable, &QTableWidget::itemSelectionChanged, [this]() {
        m_copyButton->setEnabled(m_resultsTable->currentRow() >= 0);
    });
}

void ResultsWidget::displayResults(const SearchResults& results)
{
    m_currentResults = &results;

    if (!results.success || results.businesses.empty()) {
        clearResults();
        m_summaryLabel->setText("No businesses found.");
        return;
    }

    // Update summary
    QString summary = QString("Found %1 businesses").arg(results.total_found);
    if (results.enhanced_count > 0) {
        summary += QString(" (%1 enhanced with website data)").arg(results.enhanced_count);
    }
    m_summaryLabel->setText(summary);
    m_summaryLabel->setStyleSheet("QLabel { color: #333333; font-weight: bold; }");

    // Populate table
    populateTable(results.businesses);

    // Enable export button
    m_exportButton->setEnabled(true);

    // Resize columns to content
    resizeTableColumns();
}

void ResultsWidget::populateTable(const std::vector<Business>& businesses)
{
    m_resultsTable->setRowCount(static_cast<int>(businesses.size()));
    m_resultsTable->setSortingEnabled(false); // Disable while populating

    for (size_t i = 0; i < businesses.size(); ++i) {
        const Business& business = businesses[i];
        int row = static_cast<int>(i);

        // Name
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(business.name()));
        nameItem->setToolTip(QString::fromStdString(business.name()));
        m_resultsTable->setItem(row, COL_NAME, nameItem);

        // Address
        QTableWidgetItem* addressItem = new QTableWidgetItem(QString::fromStdString(business.address()));
        addressItem->setToolTip(QString::fromStdString(business.address()));
        m_resultsTable->setItem(row, COL_ADDRESS, addressItem);

        // Phone
        QString phone = QString::fromStdString(business.phone_number());
        if (phone.isEmpty() && !business.additional_numbers().empty()) {
            phone = QString::fromStdString(business.additional_numbers()[0]);
        }
        QTableWidgetItem* phoneItem = new QTableWidgetItem(phone);
        phoneItem->setToolTip(phone);
        m_resultsTable->setItem(row, COL_PHONE, phoneItem);

        // Email
        QString email = QString::fromStdString(business.email());
        if (email.isEmpty() && !business.additional_emails().empty()) {
            email = QString::fromStdString(business.additional_emails()[0]);
        }
        QTableWidgetItem* emailItem = new QTableWidgetItem(email);
        emailItem->setToolTip(email);
        m_resultsTable->setItem(row, COL_EMAIL, emailItem);

        // Website
        QString website = QString::fromStdString(business.website());
        QTableWidgetItem* websiteItem = new QTableWidgetItem(website);
        websiteItem->setToolTip(website);
        if (!website.isEmpty()) {
            websiteItem->setForeground(QColor(0, 123, 204)); // Blue color for links
        }
        m_resultsTable->setItem(row, COL_WEBSITE, websiteItem);

        // Rating
        QString rating = business.rating() > 0 ? QString::number(business.rating(), 'f', 1) : "";
        QTableWidgetItem* ratingItem = new QTableWidgetItem(rating);
        if (business.rating() > 0) {
            ratingItem->setData(Qt::UserRole, business.rating()); // For proper sorting
        }
        m_resultsTable->setItem(row, COL_RATING, ratingItem);
    }

    m_resultsTable->setSortingEnabled(true);
}

void ResultsWidget::clearResults()
{
    m_currentResults = nullptr;
    m_resultsTable->setRowCount(0);
    m_summaryLabel->setText("No search performed yet.");
    m_summaryLabel->setStyleSheet("QLabel { color: #666666; font-style: italic; }");
    m_exportButton->setEnabled(false);
    m_copyButton->setEnabled(false);
}

void ResultsWidget::resizeTableColumns()
{
    m_resultsTable->resizeColumnsToContents();

    // Ensure name and address columns have reasonable minimum widths
    if (m_resultsTable->columnWidth(COL_NAME) < 200) {
        m_resultsTable->setColumnWidth(COL_NAME, 200);
    }
    if (m_resultsTable->columnWidth(COL_ADDRESS) < 250) {
        m_resultsTable->setColumnWidth(COL_ADDRESS, 250);
    }
}

void ResultsWidget::onExportClicked()
{
    emit exportRequested();
}

void ResultsWidget::onCopyClicked()
{
    int currentRow = m_resultsTable->currentRow();
    if (currentRow < 0 || !m_currentResults ||
        currentRow >= static_cast<int>(m_currentResults->businesses.size())) {
        return;
    }

    const Business& business = m_currentResults->businesses[currentRow];
    QString clipboardText = formatBusinessForClipboard(business);

    QApplication::clipboard()->setText(clipboardText);

    // Show temporary message
    QMessageBox::information(this, "Copied", "Business information copied to clipboard.");
}

void ResultsWidget::onCellDoubleClicked(int row, int column)
{
    if (!m_currentResults || row < 0 || row >= static_cast<int>(m_currentResults->businesses.size())) {
        return;
    }

    const Business& business = m_currentResults->businesses[row];

    // Handle different column double-clicks
    if (column == COL_WEBSITE && !business.website().empty()) {
        // Open website in browser
        QString url = QString::fromStdString(business.website());
        if (!url.startsWith("http")) {
            url = "https://" + url;
        }
        QDesktopServices::openUrl(QUrl(url));
    } else if (column == COL_EMAIL && !business.email().empty()) {
        // Open email client
        QString email = QString::fromStdString(business.email());
        QDesktopServices::openUrl(QUrl("mailto:" + email));
    } else {
        // Copy business info to clipboard
        QString clipboardText = formatBusinessForClipboard(business);
        QApplication::clipboard()->setText(clipboardText);
    }
}

void ResultsWidget::showContextMenu(const QPoint& pos)
{
    int row = m_resultsTable->rowAt(pos.y());
    if (row < 0 || !m_currentResults || row >= static_cast<int>(m_currentResults->businesses.size())) {
        return;
    }

    const Business& business = m_currentResults->businesses[row];

    QMenu menu(this);

    QAction* copyAction = menu.addAction("Copy Business Info");
    connect(copyAction, &QAction::triggered, this, &ResultsWidget::onCopyClicked);

    if (!business.website().empty()) {
        QAction* websiteAction = menu.addAction("Open Website");
        connect(websiteAction, &QAction::triggered, [this, &business]() {
            QString url = QString::fromStdString(business.website());
            if (!url.startsWith("http")) {
                url = "https://" + url;
            }
            QDesktopServices::openUrl(QUrl(url));
        });
    }

    if (!business.email().empty()) {
        QAction* emailAction = menu.addAction("Send Email");
        connect(emailAction, &QAction::triggered, [&business]() {
            QString email = QString::fromStdString(business.email());
            QDesktopServices::openUrl(QUrl("mailto:" + email));
        });
    }

    menu.exec(m_resultsTable->mapToGlobal(pos));
}

QString ResultsWidget::formatBusinessForClipboard(const Business& business) const
{
    QString text;
    text += QString("Name: %1\n").arg(QString::fromStdString(business.name()));
    text += QString("Address: %1\n").arg(QString::fromStdString(business.address()));

    if (!business.phone_number().empty()) {
        text += QString("Phone: %1\n").arg(QString::fromStdString(business.phone_number()));
    }

    if (!business.email().empty()) {
        text += QString("Email: %1\n").arg(QString::fromStdString(business.email()));
    }

    if (!business.website().empty()) {
        text += QString("Website: %1\n").arg(QString::fromStdString(business.website()));
    }

    if (business.rating() > 0) {
        text += QString("Rating: %1\n").arg(business.rating(), 0, 'f', 1);
    }

    return text;
}

bool ResultsWidget::hasResults() const
{
    return m_currentResults && m_currentResults->success && !m_currentResults->businesses.empty();
}
