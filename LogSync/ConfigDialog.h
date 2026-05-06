#pragma once
#include <QDialog>

class QLineEdit;

class ConfigDialog : public QDialog {
    Q_OBJECT
public:
    ConfigDialog(QWidget* parent = nullptr);

private slots:
    void browseSource();
    void browseDest();
    void saveConfig();

signals:
    void configSaved();

private:
    QLineEdit* sourceEdit;
    QLineEdit* destEdit;

    void loadConfig();
};