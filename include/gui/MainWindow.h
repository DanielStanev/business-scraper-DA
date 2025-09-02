#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QLabel>
#include <QTimer>

class SearchWidget;
class ResultsWidget;
class BusinessScraperEngine;
class ConfigDialog;
struct SearchResults;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSearchRequested();
    void onSearchCompleted();
    void onStatusUpdate(const QString& message);
    void showAboutDialog();
    void showLicenseDialog();
    void exportResults();
    void openConfiguration();

private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void setupConnections();
    void updateWindowTitle(const QString& subtitle = QString());

    // UI Components
    SearchWidget* m_searchWidget;
    ResultsWidget* m_resultsWidget;
    QSplitter* m_splitter;

    // Status bar components
    QLabel* m_statusLabel;
    QProgressBar* m_progressBar;
    QLabel* m_resultCountLabel;

    // Business logic
    BusinessScraperEngine* m_engine;
    SearchResults* m_lastResults;

    // State
    bool m_searchInProgress;
    QTimer* m_statusTimer;
};

#endif // MAINWINDOW_H
