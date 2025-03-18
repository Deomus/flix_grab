#include "single_instance.h"

#define SOCKET_TIMEOUT              (500)

SingleInstance::SingleInstance(QObject *parent) 
    : QObject(parent) {
    connect(&server_, &QLocalServer::newConnection, this, &SingleInstance::onNewConnection);
}

SingleInstance::~SingleInstance() {
    server_.close();
}

void SingleInstance::listen(QString name) {
    server_.removeServer(name);
    server_.listen(name);

    qDebug() << "Listening for: " << name;
    qDebug() << server_.errorString();
}

bool SingleInstance::tryNew(QString name, QString message) {
    QSystemSemaphore semaphore(QString("%1_sem").arg(name), 1);  // create semaphore
    semaphore.acquire(); // Raise the semaphore, barring other instances to work with shared memory

    qDebug() << "Checking for previous instance...";

    QLocalSocket socket;
    socket.connectToServer(name, QLocalSocket::ReadWrite);

    if(socket.waitForConnected()) {
        qDebug() << "Sending args to previous instance...";

        socket.write(message.toLocal8Bit());
        socket.waitForBytesWritten();
    }
    else if (socket.error() == QLocalSocket::ServerNotFoundError) {
        listen(name);
        return true;
    }
    return false;
}

void SingleInstance::onNewConnection() {
    emit newInstance();

    QLocalSocket* socket = server_.nextPendingConnection();
    connect(socket, &QLocalSocket::readyRead, [=]() {
        qDebug() << "Args: " << socket->readAll();
        socket->close();
        socket->deleteLater();
    });
}

