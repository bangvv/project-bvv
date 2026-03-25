#ifndef DATARECEIVER_H
#define DATARECEIVER_H
#include <QObject>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>

class DataReceiver : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString      time     READ time      NOTIFY timeChanged)
public:
    explicit DataReceiver(QObject *parent = nullptr);

    // Getter
    QString time();

    Q_INVOKABLE void saveToCsv(const QString &operatorName,
                               const QString &serial,
                               const QString &result,
                               const QString &customPath);
signals:
    void timeChanged();
private:
    QString m_time = "";
};
#endif // DATARECEIVER_H
