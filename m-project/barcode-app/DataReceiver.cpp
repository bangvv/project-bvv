#include "DataReceiver.h"

DataReceiver::DataReceiver(QObject *parent) : QObject(parent) {}

QString DataReceiver::time() {
    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString("dd-MM-yyyy HH:mm:ss");
    m_time = timestamp;
    return m_time;
}
void DataReceiver::saveToCsv(const QString &operatorName, const QString &serial, const QString &result, const QString &customPath)
{
    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString("dd-MM-yyyy HH:mm:ss");
    QString fileName = now.toString("dd-MM-yyyy") + ".csv";

    QString path = customPath.isEmpty()
            ? QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            : customPath;

    QDir dir(path);
    if (!dir.exists()) dir.mkpath(".");

    QFile file(path + "/" + fileName);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);

        // Nếu file mới, ghi Header đúng cấu trúc mới
        if (file.size() == 0) {
            out << "Date,Operator,Serial,Result\n";
        }

        // Ghi dữ liệu theo thứ tự: Date, Operator, Serial, Result
        out << timestamp << ","
            << operatorName << ","
            << serial << ","
            << result << "\n";

        file.close();
    }

    m_time = timestamp;
    emit timeChanged();
}
