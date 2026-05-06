#include "ConfigDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#define CONFIG_FILE "config.json"

ConfigDialog::ConfigDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("LogSync Configuration");

    sourceEdit = new QLineEdit();
    destEdit   = new QLineEdit();

    QPushButton* srcBtn = new QPushButton("Browse...");
    QPushButton* dstBtn = new QPushButton("Browse...");
    QPushButton* saveBtn = new QPushButton("Save");

    connect(srcBtn, &QPushButton::clicked, this, &ConfigDialog::browseSource);
    connect(dstBtn, &QPushButton::clicked, this, &ConfigDialog::browseDest);
    connect(saveBtn, &QPushButton::clicked, this, &ConfigDialog::saveConfig);

    QHBoxLayout* srcLayout = new QHBoxLayout();
    srcLayout->addWidget(sourceEdit);
    srcLayout->addWidget(srcBtn);

    QHBoxLayout* dstLayout = new QHBoxLayout();
    dstLayout->addWidget(destEdit);
    dstLayout->addWidget(dstBtn);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(srcLayout);
    mainLayout->addLayout(dstLayout);
    mainLayout->addWidget(saveBtn);

    setLayout(mainLayout);

    loadConfig();
}

void ConfigDialog::browseSource() {
    sourceEdit->setText(QFileDialog::getExistingDirectory(this, "Select Source Folder"));
}

void ConfigDialog::browseDest() {
    destEdit->setText(QFileDialog::getExistingDirectory(this, "Select Destination Folder"));
}

void ConfigDialog::saveConfig() {
    QJsonObject obj;
    obj["source"] = sourceEdit->text();
    obj["dest"]   = destEdit->text();

    QFile f("config.json");
    f.open(QIODevice::WriteOnly);
    f.write(QJsonDocument(obj).toJson());
    f.close();

    emit configSaved();
    accept();
}

void ConfigDialog::loadConfig() {
    QFile f(CONFIG_FILE);
    if (!f.open(QIODevice::ReadOnly)) return;

    auto doc = QJsonDocument::fromJson(f.readAll());
    sourceEdit->setText(doc["source"].toString());
    destEdit->setText(doc["dest"].toString());
}

