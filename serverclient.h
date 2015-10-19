#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H

#include <QObject>

class ServerClient : public QObject
{
    Q_OBJECT
public:
    explicit ServerClient(QObject *parent = 0);

signals:

public slots:

};

#endif // SERVERCLIENT_H
