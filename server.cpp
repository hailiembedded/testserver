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
#include <QHBoxLayout>

#include "unistd.h"
#include "server.h"

typedef union floatIntChange
{
    unsigned char fchar[4];
    float ffloat;
    int fint;
}floatIntChange;

typedef union boolChange
{
    bool fbool;
    unsigned char fchar;
}boolChange;

typedef union wholeChange
{
    unsigned char fchar[sizeof(OA_Profile_t)*2];
    OA_Profile_t fOAProfile;

}wholeChange;


void profileTochar(char * target, wholeChange source)
{
    char temp = 0;
    for (unsigned int i =0; i<sizeof(OA_Profile_t);i++)
    {
        temp =source.fchar[i]&0x0F;
        if (temp>=0 && temp<=9)
            target[i*2+1] = temp+'0';
        else
            target[i*2+1] = temp - 10 + 'A';
        temp = source.fchar[i]>>4;
        if (temp>=0 && temp<=9)
            target[i*2] = temp+'0';
        else
            target[i*2] = temp - 10 + 'A';
    }

}

void floatIntToChar(char * target, floatIntChange source)
{

    char temp;
    for (int i =0; i<=3;i++)
    {
        temp =source.fchar[i]&0x0F;
        if (temp>=0&& temp<=9)
            target[i*2+1] = temp+'0';
        else
            target[i*2+1] = temp - 10 + 'A';
        temp = source.fchar[i]>>4;
        if (temp>=0 && temp<=9)
            target[i*2] = temp+'0';
        else
            target[i*2] = temp - 10 + 'A';
    }
}//8 byte for a float or int

void boolToChar(char * target, boolChange source)
{

    char temp;

    temp =source.fchar&0x0F;
    if (temp>=0&& temp<=9)
        target[1] = temp+'0';
    else
        target[1] = temp - 10 + 'A';
    temp = source.fchar>>4;
    if (temp>=0 && temp<=9)
        target[0] = temp+'0';
    else
        target[0] = temp - 10 + 'A';

}//2 byte for a bool




Server::Server(QWidget *parent)
    :   QDialog(parent), tcpServer(0), networkSession(0)
{

    QString profile;
    char profileData[sizeof(OA_Profile_t)*2+1] = {'\0'};

    testProfile.laser_source_status = false;			// laser source status
    testProfile.mode = 3;//constant power/current/gain mode
    testProfile.out_current_min = -100.5;
    testProfile.out_current_max = 100.5;
    testProfile.gain_min = -10.5;
    testProfile.gain_max = 10.5;
    testProfile.power_min = -23;
    testProfile.power_max = 27.8;
    testProfile.gain_threshold = 439.9;
    testProfile.gain_accuracy = 34.50;
    testProfile.output_power = 14.5;
    testProfile.output_power_threshold = 120.56;
    testProfile.output_power_accuracy = 12.6;
    testProfile.ALS_enable = true;
    testProfile.LOS_threshold = 98.9;
    testProfile.optical_power_value = 369.5;
    testProfile.optical_output_power_value = 96.7;
    testProfile.wav_length = 100.5;
    testProfile.ATI = 125.9;



    wholeChange testWhole;
    testWhole.fOAProfile = testProfile;
    profileTochar(profileData, testWhole);

    qDebug() << "strlen (profileDate)"<< strlen(profileData);
    qDebug() << "sizeof (profileDate)"<< sizeof(OA_Profile_t);
    qDebug() << "sizeof (whole)"<< sizeof(wholeChange);

    profile.append("1234567890");

    profile.append(profileData);





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

    connect(tcpCardServer, SIGNAL(newConnection()), this, SLOT(setupConCard()));

    //        connect(tcpServerc1, SIGNAL(newConnection()), this, SLOT(setupConc1()));



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

    setWindowTitle(tr("EDFA Simulate Server"));


    commReply.insert("*IDN?", "JDSU,MAP-200,888,1.01");
    commReply.insert(":SYST:IP:LIST?", "0,127.0.0.1, Local");
    commReply.insert(":ETHERNET:MODE?", "2");
    commReply.insert(":ETHERNET:NAME?", "Local, MAP20");
    commReply.insert(":ETHERNET:PROPERTY? 0", "Local,127.0.0.1,255.255.255.0,127.0.0.1,Test,127.0.0.1,127.0.0.1");
    commReply.insert(":ETHERNET:PROPERTY? 1", "Local,127.0.0.1,255.255.255.0,127.0.0.1,Test,127.0.0.1,127.0.0.1");
    commReply.insert(":SYST:ETH:MAC?", "AA:BB:CC:DD");
    commReply.insert("isu:etal:vhard?", "10");
    commReply.insert("PROC:LIC:STATUS?","VALID");
    commReply.insert(":PROC:CARD?", "1 136 8801,2 65505 0");



    //        commReplyc1.insert(":IDN?", "0,FFL,888,1");
    //        commReplyc1.insert(":config?", "1 2 3 4 5 6 7 8 9 10 11 12 12 13 14");
    //        commReplyc1.insert(":info?", "1,2,3,4,5,6,7,8,9 10 11 12 12 13 14");
    //        commReplyc1.insert(":proc:ndev?", "1");

    cardReply.insert(":idn?", "pad,EDFA,pad,0.0.1");
    cardReply.insert(":proc:ndev?", "1");
    cardReply.insert(":config?","1 EDFA 888.88 10.5 20.5 100.1 200.1 25.5 30.5 50.5 60.5 1.2 2.5 10.1 15.2 yes yes yes yes");
    cardReply.insert(":info?", "ABCDEF,LUCK,0.0.1,pad,pad,pad,pad,pad");
    cardReply.insert(":proc:prof? 1", profile );
    cardReply.insert(":proc:license:list?","0" );
    cardReply.insert(":lock?","0" );

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

    if (!tcpServer->listen(QHostAddress::Any, 8000)) {
        QMessageBox::critical(this, tr("Test Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }

    tcpCardServer = new QTcpServer(this);
    if (!tcpCardServer->listen(QHostAddress::Any, 8201)) {
        QMessageBox::critical(this, tr("Test Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpCardServer->errorString()));
        close();
        return;
    }

    //    tcpServerc1 = new QTcpServer(this);
    //    if (!tcpServerc1->listen(QHostAddress::Any, 8801)) {
    //        QMessageBox::critical(this, tr("Test Server"),
    //                              tr("Unable to start the server: %1.")
    //                              .arg(tcpServerc1->errorString()));
    //        close();
    //        return;
    //    }

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


void Server::setupConCard()
{
    static int count = 0;
    cardConnection = tcpCardServer->nextPendingConnection();
    cardConnection->setSocketOption(QAbstractSocket::LowDelayOption, QVariant(1));
    cardConnection->setSocketOption(QAbstractSocket::KeepAliveOption, QVariant(1));

    connect(cardConnection, SIGNAL(disconnected()),
            cardConnection, SLOT(deleteLater()));
    connect(cardConnection, SIGNAL(readyRead()), this, SLOT(replyCommCard()));
    qDebug() << "setup Card "<< ++count;
}

//void Server::setupConc1()
//{
//    clientConnectionc1 = tcpServerc1->nextPendingConnection();
//    clientConnectionc1->setSocketOption(QAbstractSocket::LowDelayOption, QVariant(1));
//    clientConnectionc1->setSocketOption(QAbstractSocket::KeepAliveOption, QVariant(1));

//    connect(clientConnectionc1, SIGNAL(disconnected()),
//            clientConnectionc1, SLOT(deleteLater()));
//    connect(clientConnectionc1, SIGNAL(readyRead()), this, SLOT(replyCommc1()));
//    qDebug()<<"card connected";

//}
//void Server::replyCommc1()
//{
//    QString inComm;
//    QByteArray outReply;
//    blockSize = 1;
//    if (clientConnectionc1->bytesAvailable() < blockSize)
//    {
//        return;
//    }else
//    {
//        inComm = clientConnectionc1->readLine();
//        inComm = inComm.trimmed();
//        qDebug()<<"after trim receive is"<< inComm;

//    }
//    if (inComm.contains("PROC:ID WM", Qt::CaseInsensitive))
//    {
//        //connectHost;
//        return;
//    }
//    else if (commReplyc1.contains(inComm))
//    {
//        outReply.append(commReplyc1.value(inComm)+"\n");
//    }
//    else
//    {
//        outReply.append("\n");
//    }
//    clientConnectionc1->write(outReply);
//}



void Server::replyComm()
{
    QString inComm;
    QByteArray outReply;
    blockSize = 1;
    if (clientConnection->bytesAvailable() < blockSize)
    {
        return;
    }else
    {
        inComm = clientConnection->readAll();
        inComm = inComm.trimmed();
//        qDebug()<<"<<<<<<<<<<<from chasis after trim receive is"<< inComm;
//        if (inComm.contains('\n'))
//        {
//            qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!"<<inComm.indexOf('\n')<< "length"<<inComm.length();
//        }
        qDebug() <<"before inComm length is "<<inComm.length();
        inComm = inComm.right(inComm.length() - inComm.indexOf('\n') - 1);
        qDebug()<<"after adjust is "<< inComm;
    }
    if (inComm.contains("PROC:ID WM", Qt::CaseInsensitive))
    {
        //connectHost;
        return;
    }
    else if (commReply.contains(inComm))
    {
        outReply.append(commReply.value(inComm)+"\n");
        clientConnection->write(outReply);
        clientConnection->flush();
        qDebug()<<">>>>>>>>>>>from chasis after trim send is"<< outReply;
    }
    else
    {
        //outReply.append("\n"); don't need to reply
    }

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
    QString serverReply,temp;


    while (!in.atEnd()){
        in >> temp;
        serverReply += temp;
    }

    qDebug()<<"Server reply is "<< serverReply;
}

void Server::replyCommCard()
{
    cardConnection = qobject_cast<QTcpSocket *>(sender());
    QString inComm;
    QByteArray outReply;
    blockSize = 1;
    if (cardConnection->bytesAvailable() < blockSize)
    {
        return;
    }else
    {
        inComm = cardConnection->readAll();
        inComm = inComm.trimmed();
        qDebug() <<"before inComm length is "<<inComm.length();
        inComm = inComm.right(inComm.length() - inComm.indexOf('\n') - 1);
        qDebug()<<"after adjust is "<< inComm;
    }
    if (inComm.contains("PROC:ID WM", Qt::CaseInsensitive))
    {
        //connectHost;
        return;
    }
    else if (cardReply.contains(inComm))
    {
        outReply.append(cardReply.value(inComm)+"\n");
        cardConnection->write(outReply);
        cardConnection->flush();
        qDebug()<<">>>>>>>>>>from card after trim reply is"<< outReply;
    }
    else
    {
        //outReply.append("\n"); don;t need to reply;
    }


}
Server::~Server()
{

}
