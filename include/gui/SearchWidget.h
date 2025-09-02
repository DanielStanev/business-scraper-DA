#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>

#include "core/BusinessScraperEngine.h"

class SearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchWidget(QWidget *parent = nullptr);

    // Get current search options
    SearchOptions getSearchOptions() const;
    OutputFormat getOutputFormat() const;

    // UI state management
    void setSearchEnabled(bool enabled);
    bool isValid() const;

    // Load/save settings
    void loadSettings();
    void saveSettings();

signals:
    void searchRequested();

private slots:
    void onSearchClicked();
    void onFieldChanged();
    void validateInput();

private:
    void setupUI();
    void setupConnections();
    void updateSearchButton();

    // Search parameters
    QLineEdit* m_keywordEdit;
    QLineEdit* m_locationEdit;
    QSpinBox* m_resultsSpinBox;

    // Options
    QCheckBox* m_webScrapingCheckBox;
    QComboBox* m_formatComboBox;

    // Actions
    QPushButton* m_searchButton;
    QPushButton* m_clearButton;

    // Layout groups
    QGroupBox* m_searchGroup;
    QGroupBox* m_optionsGroup;
};

#endif // SEARCHWIDGET_H
