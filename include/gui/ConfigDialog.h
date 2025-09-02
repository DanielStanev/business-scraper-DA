#ifndef CONFIG_DIALOG_H
#define CONFIG_DIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMessageBox>

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = nullptr);

    QString getApiKey() const;
    void setApiKey(const QString &apiKey);

private slots:
    void onSaveClicked();
    void onCancelClicked();
    void onTestApiKey();

private:
    void setupUI();
    void loadCurrentConfig();
    bool saveConfig();

    QLineEdit* m_apiKeyEdit;
    QPushButton* m_saveButton;
    QPushButton* m_cancelButton;
    QPushButton* m_testButton;
    QLabel* m_statusLabel;
};

#endif // CONFIG_DIALOG_H
