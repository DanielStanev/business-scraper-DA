#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>

#include "core/BusinessScraperEngine.h"

class ResultsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultsWidget(QWidget *parent = nullptr);

    // Display results
    void displayResults(const SearchResults& results);
    void clearResults();

    // Export functionality
    void exportToCsv(const QString& filename);
    void exportToJson(const QString& filename);
    bool hasResults() const;

    // Get current results
    const SearchResults* getResults() const { return m_currentResults; }

signals:
    void exportRequested();

private slots:
    void onExportClicked();
    void onCopyClicked();
    void onCellDoubleClicked(int row, int column);
    void showContextMenu(const QPoint& pos);

private:
    void setupUI();
    void setupTable();
    void setupConnections();
    void populateTable(const std::vector<Business>& businesses);
    void resizeTableColumns();
    QString formatBusinessForClipboard(const Business& business) const;

    // UI Components
    QTableWidget* m_resultsTable;
    QLabel* m_summaryLabel;
    QPushButton* m_exportButton;
    QPushButton* m_copyButton;
    QGroupBox* m_resultsGroup;

    // Data
    const SearchResults* m_currentResults;

    // Table columns
    enum Columns {
        COL_NAME = 0,
        COL_ADDRESS,
        COL_PHONE,
        COL_EMAIL,
        COL_WEBSITE,
        COL_RATING,
        COL_COUNT
    };
};

#endif // RESULTSWIDGET_H
