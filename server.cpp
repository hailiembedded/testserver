/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QtNetwork>

#include <stdlib.h>

#include "unistd.h"
#include "server.h"


Server::Server(QWidget *parent)
:   QDialog(parent), tcpServer(0), networkSession(0)
{
    statusLabel = new QLabel;
    quitButton = new QPushButton(tr("Quit"));
    sendButton = new QPushButton(tr("Send"));
    paraComm = new QLineEdit("888");
    ip = new QLineEdit(tr("127.0.0.1"));
    port = new QLineEdit(tr("8800"));
    commandText = new QComboBox;
    commandText->addItem("*IDN?");
    commandText->addItem("PROC:ID");
    commandText->addItem("SYST:INTLOCK");
    commandText->addItem("ETH:ERROR");
    commandText->addItem("ETH:LED");
    commandText->addItem("CARD:UPDATE");
    commandText->addItem("SYST:LEGA:MODE");
    commandText->addItem("SYST:SCREEN:DIR");
    commandText->addItem("PROC:FAULT");
    commandText->addItem("PROC:STATUS");
    commandText->addItem("PROC:UPDATE");
    commandText->addItem("SDS");
    commandText->addItem("PROC:SHUT");


    serverConnection = new QTcpSocket(this);
    serverConnection->setSocketOption(QAbstractSocket::LowDelayOption, QVariant(1));
    serverConnection->setSocketOption(QAbstractSocket::KeepAliveOption, QVariant(1));

    connect(serverConnection, SIGNAL(connected()), this, SLOT(sendComm()));
    connect(serverConnection, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(connectServerError(QAbstractSocket::SocketError)));
    connect(serverConnection, SIGNAL(readyRead()), this, SLOT(serverResponse()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(clickSend()));


    quitButton->setAutoDefault(false);
    sendButton->setAutoDefault(false);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpened();
    }



        connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(setupCon()));


        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addStretch(10);
        buttonLayout->addWidget(ip);
        buttonLayout->addWidget(port);
        buttonLayout->addWidget(commandText);
        buttonLayout->addWidget(paraComm);
        buttonLayout->addWidget(sendButton);
        buttonLayout->addStretch(1);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(statusLabel);
        mainLayout->addLayout(buttonLayout);
        mainLayout->addWidget(quitButton);
        setLayout(mainLayout);

        setWindowTitle(tr("Test Server"));


        commReply.insert("IDN?", "0,SNTest,888,SWTest");
        commReply.insert(":SYST:IP:LIST?", "0,127.0.0.1, Local");
        commReply.insert(":ETHERNET:MODE?", "2");
        commReply.insert(":ETHERNET:NAME?", "Local, MAP20");
        commReply.insert(":ETHERNET:PROPERTY? 0", "Local,127.0.0.1,255.255.255.0,127.0.0.1,Test,127.0.0.1,127.0.0.1");
        commReply.insert(":ETHERNET:PROPERTY? 1", "Local,127.0.0.1,255.255.255.0,127.0.0.1,Test,127.0.0.1,127.0.0.1");


}



void Server::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }


    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, 8100)) {
        QMessageBox::critical(this, tr("Test Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the MAP GUI now.")
                         .arg(ipAddress).arg(tcpServer->serverPort()));

}


void Server::setupCon()
{
    clientConnection = tcpServer->nextPendingConnection();
    clientConnection->setSocketOption(QAbstractSocket::LowDelayOption, QVariant(1));
    clientConnection->setSocketOption(QAbstractSocket::KeepAliveOption, QVariant(1));

    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(replyComm()));

}

void Server::replyComm()
{
    QString inComm;
    QByteArray outReply;
    static int count=0;
    qDebug()<<"cout is" <<count++;
    blockSize = 1;
    if (clientConnection->bytesAvailable() < blockSize)
    {
        return;
    }else
    {   qDebug()<<"data length"<<clientConnection->bytesAvailable();
        inComm = clientConnection->readLine();
        qDebug()<<"readline length"<<inComm.length();
        qDebug()<<"before trim receive is "<< inComm;
        inComm = inComm.trimmed();
        qDebug()<<"after trim receive is"<< inComm;
        qDebug()<<"after trim is "<<inComm.length();
    }
    if (inComm.contains("PROC:ID WM", Qt::CaseInsensitive))
    {
        //connectHost;
        return;
    }
    else if (commReply.contains(inComm))
    {
        outReply.append(commReply.value(inComm)+"\n");
    }
    else
    {
        outReply.append("\n");
    }
    clientConnection->write(outReply);
}

void Server::clickSend()
{
    if ((QAbstractSocket::ConnectedState == serverConnection->state()) && (port->text().trimmed().toInt() == serverConnection->peerPort()))
    {

        sendComm();
    }
    else
    {
        serverConnection->abort();
        serverConnection->connectToHost(ip->text().trimmed(), port->text().toInt(), QIODevice::ReadWrite);

    }


}
void Server::sendComm()
{
    QByteArray sendComm;

    QDataStream out(&sendComm, QIODevice::ReadWrite);


    if ("888" == paraComm->text())
    {
        sendComm.append(commandText->currentText()+"\n");
        //out << commandText->currentText();
    }
    else
    {
        sendComm.append(commandText->currentText() + " "+paraComm->text().trimmed());
    }
    if ((QAbstractSocket::ConnectedState == serverConnection->state()) && (port->text().trimmed().toInt() == serverConnection->peerPort()))
    {
        serverConnection->write(sendComm);
        serverConnection->flush();
    }
    else
    {
        //connection didn't setup

    }
}

void Server::connectServerError(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err);
    QMessageBox::critical(this, tr("Test Server"),
                              tr("Unable to start the server: %1.")
                              .arg(serverConnection->errorString()));
}
void Server::serverResponse()
{
    QDataStream in(serverConnection);
    QString serverReply;
    in >> serverReply;

    qDebug()<<"Server reply is "<< serverReply;
}
