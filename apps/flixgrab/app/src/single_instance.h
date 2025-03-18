#pragma once

#include <QtCore>
#include <QtNetwork>

class SingleInstance : public QObject
{
    Q_OBJECT
public:
    explicit SingleInstance(QObject *parent = 0);

    ~SingleInstance();

public:

    bool    tryNew(QString name, QString message);

signals:

    void    newInstance();

private slots:

    void    onNewConnection();
    
private:
    void    listen(QString name);

private:

    QLocalServer server_;

};
