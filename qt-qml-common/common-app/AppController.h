#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H
#include <QObject>
#include <QString>

class AppController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString      appName     READ appName         WRITE setAppName    NOTIFY appNameChanged)
    Q_PROPERTY(int          count       READ getCount        WRITE setCount      NOTIFY countChanged)

public:
    explicit AppController(QObject *parent = nullptr);

    // Getter
    QString appName() const;
    int     getCount() const;

    // Setter
    void setAppName(const QString &name);
    void setCount(const int &count);

    // Hàm gọi từ QML
    Q_INVOKABLE void changeName(const QString &name);
    Q_INVOKABLE void changeCount(const int &count);

signals:
    void appNameChanged();
    void countChanged();

private:
    QString     m_appName;
    int         m_count = 0;
};

#endif // APPCONTROLLER_H
