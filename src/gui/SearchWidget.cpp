#include "gui/SearchWidget.h"
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QApplication>
#include <QtCore/QSettings>

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget(parent)
    , m_keywordEdit(nullptr)
    , m_locationEdit(nullptr)
    , m_resultsSpinBox(nullptr)
    , m_webScrapingCheckBox(nullptr)
    , m_formatComboBox(nullptr)
    , m_searchButton(nullptr)
    , m_clearButton(nullptr)
    , m_searchGroup(nullptr)
    , m_optionsGroup(nullptr)
{
    setupUI();
    setupConnections();
    loadSettings();
    validateInput();
}

void SearchWidget::setupUI()
{
    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    // Search parameters group
    m_searchGroup = new QGroupBox("Search Parameters", this);
    QFormLayout* searchLayout = new QFormLayout(m_searchGroup);

    // Keyword input
    m_keywordEdit = new QLineEdit(this);
    m_keywordEdit->setPlaceholderText("e.g., restaurants, coffee shops");
    searchLayout->addRow("&Keyword:", m_keywordEdit);

    // Location input
    m_locationEdit = new QLineEdit(this);
    m_locationEdit->setPlaceholderText("e.g., New York, NY");
    searchLayout->addRow("&Location:", m_locationEdit);

    // Results count input
    m_resultsSpinBox = new QSpinBox(this);
    m_resultsSpinBox->setRange(1, 100);
    m_resultsSpinBox->setValue(20);
    m_resultsSpinBox->setToolTip("Maximum number of results to return");
    searchLayout->addRow("Max &Results:", m_resultsSpinBox);

    mainLayout->addWidget(m_searchGroup);

    // Options group
    m_optionsGroup = new QGroupBox("Options", this);
    QFormLayout* optionsLayout = new QFormLayout(m_optionsGroup);

    // Web scraping option
    m_webScrapingCheckBox = new QCheckBox("Enable web scraping for additional data", this);
    m_webScrapingCheckBox->setChecked(true);
    m_webScrapingCheckBox->setToolTip("Scrape business websites for additional contact information");
    optionsLayout->addRow(m_webScrapingCheckBox);

    // Output format
    m_formatComboBox = new QComboBox(this);
    m_formatComboBox->addItems({"CSV", "JSON", "YAML", "XML"});
    m_formatComboBox->setToolTip("Output format for exported results");
    optionsLayout->addRow("Export &Format:", m_formatComboBox);

    mainLayout->addWidget(m_optionsGroup);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    m_searchButton = new QPushButton("&Search", this);
    m_searchButton->setMinimumHeight(35);
    m_searchButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #007ACC;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #005A9E;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #004080;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #CCCCCC;"
        "    color: #666666;"
        "}"
    );

    m_clearButton = new QPushButton("&Clear", this);
    m_clearButton->setMinimumHeight(35);

    buttonLayout->addWidget(m_searchButton);
    buttonLayout->addWidget(m_clearButton);

    mainLayout->addLayout(buttonLayout);

    // Add stretch to push everything to the top
    mainLayout->addStretch();

    // Set reasonable minimum width
    setMinimumWidth(300);
    setMaximumWidth(400);
}

void SearchWidget::setupConnections()
{
    // Connect input validation
    connect(m_keywordEdit, &QLineEdit::textChanged, this, &SearchWidget::onFieldChanged);
    connect(m_locationEdit, &QLineEdit::textChanged, this, &SearchWidget::onFieldChanged);

    // Connect buttons
    connect(m_searchButton, &QPushButton::clicked, this, &SearchWidget::onSearchClicked);
    connect(m_clearButton, &QPushButton::clicked, [this]() {
        m_keywordEdit->clear();
        m_locationEdit->clear();
        m_resultsSpinBox->setValue(20);
        m_webScrapingCheckBox->setChecked(true);
        m_formatComboBox->setCurrentIndex(0);
        validateInput();
    });

    // Enter key in line edits triggers search
    connect(m_keywordEdit, &QLineEdit::returnPressed, this, &SearchWidget::onSearchClicked);
    connect(m_locationEdit, &QLineEdit::returnPressed, this, &SearchWidget::onSearchClicked);
}

void SearchWidget::onSearchClicked()
{
    if (isValid()) {
        saveSettings();
        emit searchRequested();
    }
}

void SearchWidget::onFieldChanged()
{
    validateInput();
}

void SearchWidget::validateInput()
{
    bool valid = !m_keywordEdit->text().trimmed().isEmpty() &&
                 !m_locationEdit->text().trimmed().isEmpty();

    updateSearchButton();

    // Visual feedback for required fields
    QString requiredStyle = "QLineEdit { border: 2px solid #FF6B6B; }";
    QString normalStyle = "";

    m_keywordEdit->setStyleSheet(m_keywordEdit->text().trimmed().isEmpty() ? requiredStyle : normalStyle);
    m_locationEdit->setStyleSheet(m_locationEdit->text().trimmed().isEmpty() ? requiredStyle : normalStyle);
}

void SearchWidget::updateSearchButton()
{
    m_searchButton->setEnabled(isValid());
}

SearchOptions SearchWidget::getSearchOptions() const
{
    SearchOptions options;
    options.keyword = m_keywordEdit->text().trimmed().toStdString();
    options.location = m_locationEdit->text().trimmed().toStdString();
    options.max_radius = 5000; // Default radius since UI control is removed
    options.max_results = m_resultsSpinBox->value();
    options.enhance_with_web_scraping = m_webScrapingCheckBox->isChecked();
    return options;
}

OutputFormat SearchWidget::getOutputFormat() const
{
    switch (m_formatComboBox->currentIndex()) {
        case 0: return OutputFormat::CSV;
        case 1: return OutputFormat::JSON;
        case 2: return OutputFormat::YAML;
        case 3: return OutputFormat::XML;
        default: return OutputFormat::CSV;
    }
}

void SearchWidget::setSearchEnabled(bool enabled)
{
    m_searchButton->setEnabled(enabled && isValid());
    m_keywordEdit->setEnabled(enabled);
    m_locationEdit->setEnabled(enabled);
    m_resultsSpinBox->setEnabled(enabled);
    m_webScrapingCheckBox->setEnabled(enabled);
    m_formatComboBox->setEnabled(enabled);
    m_clearButton->setEnabled(enabled);
}

bool SearchWidget::isValid() const
{
    return !m_keywordEdit->text().trimmed().isEmpty() &&
           !m_locationEdit->text().trimmed().isEmpty();
}

void SearchWidget::loadSettings()
{
    QSettings settings;
    settings.beginGroup("SearchWidget");

    m_keywordEdit->setText(settings.value("keyword", "").toString());
    m_locationEdit->setText(settings.value("location", "").toString());
    m_resultsSpinBox->setValue(settings.value("maxResults", 20).toInt());
    m_webScrapingCheckBox->setChecked(settings.value("webScraping", true).toBool());
    m_formatComboBox->setCurrentIndex(settings.value("format", 0).toInt());

    settings.endGroup();
}

void SearchWidget::saveSettings()
{
    QSettings settings;
    settings.beginGroup("SearchWidget");

    settings.setValue("keyword", m_keywordEdit->text());
    settings.setValue("location", m_locationEdit->text());
    settings.setValue("maxResults", m_resultsSpinBox->value());
    settings.setValue("webScraping", m_webScrapingCheckBox->isChecked());
    settings.setValue("format", m_formatComboBox->currentIndex());

    settings.endGroup();
}
