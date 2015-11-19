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
    :   QDialog(parent), chasisServer(0), networkSession(0)
{



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

    quitButton->setAutoDefault(false);
    sendButton->setAutoDefault(false);
    connect(sendButton, SIGNAL(clicked()), this, SLOT(sendTestEvent()));

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


    //        connect(chasisServerc1, SIGNAL(newConnection()), this, SLOT(setupConc1()));



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


    chasisReply.insert("*IDN?", "JDSU,MAP-200,888,1.01");
    chasisReply.insert(":SYST:IP:LIST?", "0,127.0.0.1, Local");
    chasisReply.insert(":ETHERNET:MODE?", "2");
    chasisReply.insert(":ETHERNET:NAME?", "Local, MAP20");
    chasisReply.insert(":ETHERNET:PROPERTY? 0", "Local,127.0.0.1,255.255.255.0,127.0.0.1,Test,127.0.0.1,127.0.0.1");
    chasisReply.insert(":ETHERNET:PROPERTY? 1", "Local,127.0.0.1,255.255.255.0,127.0.0.1,Test,127.0.0.1,127.0.0.1");
    chasisReply.insert(":SYST:ETH:MAC?", "AA:BB:CC:DD");
    chasisReply.insert("isu:etal:vhard?", "10");
    chasisReply.insert("PROC:LIC:STATUS?","VALID");
    chasisReply.insert(":PROC:CARD?", "1 136 8801,2 65505 0");



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


    chasisServer = new QTcpServer(this);

    if (!chasisServer->listen(QHostAddress::Any, 8000)) {
        QMessageBox::critical(this, tr("Test Server"),
                              tr("Unable to start the server: %1.")
                              .arg(chasisServer->errorString()));
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



    connect(chasisServer, SIGNAL(newConnection()), this, SLOT(chasisSetupCon()));
    connect(tcpCardServer, SIGNAL(newConnection()), this, SLOT(CardSetupCon()));

    //    chasisServerc1 = new QTcpServer(this);
    //    if (!chasisServerc1->listen(QHostAddress::Any, 8801)) {
    //        QMessageBox::critical(this, tr("Test Server"),
    //                              tr("Unable to start the server: %1.")
    //                              .arg(chasisServerc1->errorString()));
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
                         .arg(ipAddress).arg(chasisServer->serverPort()));

}


void Server::sendTestEvent()
{
    sendComm(profile+'\n');
}
void Server::chasisSetupCon()
{
    chasisConnection = chasisServer->nextPendingConnection();
    chasisConnection->setSocketOption(QAbstractSocket::LowDelayOption, QVariant(1));
    chasisConnection->setSocketOption(QAbstractSocket::KeepAliveOption, QVariant(1));

    connect(chasisConnection, SIGNAL(disconnected()),
            chasisConnection, SLOT(deleteLater()));
    connect(chasisConnection, SIGNAL(readyRead()), this, SLOT(chasisReplyComm()));

}


void Server::CardSetupCon()
{
    static int count = 0;
    tempConnection = tcpCardServer->nextPendingConnection();
    tempConnection->setSocketOption(QAbstractSocket::LowDelayOption, QVariant(1));
    tempConnection->setSocketOption(QAbstractSocket::KeepAliveOption, QVariant(1));

    connect(tempConnection, SIGNAL(disconnected()),
            tempConnection, SLOT(deleteLater()));
    connect(tempConnection, SIGNAL(readyRead()), this, SLOT(commFromCard()));
    qDebug() << "setup Card "<< ++count;
}



void Server::chasisReplyComm()
{
    QString inComm;
    QByteArray outReply;
    blockSize = 1;
    if (chasisConnection->bytesAvailable() < blockSize)
    {
        return;
    }else
    {

        while (chasisConnection->canReadLine())
        {
            inComm = chasisConnection->readLine();
            qDebug() << "chasis command"<<inComm;
            inComm = inComm.trimmed();
            if (chasisReply.contains(inComm))
            {
                outReply.append(chasisReply.value(inComm)+"\n");
                chasisConnection->write(outReply);
                chasisConnection->flush();
                qDebug()<<">>>>>>>>>>>from chasis after trim send is"<< outReply;
            }
        }
    }

}

void Server::sendComm(QString comm)
{
    QByteArray sendComm;

    sendComm.append(comm);

    cardEventConnection->write(sendComm);
    cardEventConnection->flush();

}

void Server::connectServerError(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err);
    QMessageBox::critical(this, tr("Test Server"),
                          tr("Unable to start the server: %1.")
                          .arg(chasisConnection->errorString()));
}

void Server::commFromCard()
{
    tempConnection = qobject_cast<QTcpSocket *>(sender());
    QString inComm;
    QByteArray outReply;
    blockSize = 1;
    if (tempConnection->bytesAvailable() < blockSize)
    {
        return;
    }else
    {

        while (tempConnection->canReadLine())
        {
            inComm = tempConnection->readLine();
            inComm = inComm.trimmed();
            qDebug() << "%%%%%%%%%%%%%"<<inComm;
            if (inComm == ":proc:gui:command")
            {

                cardCommConnection = tempConnection;
                tempConnection->disconnect(SIGNAL(readyRead()));
                connect(cardCommConnection, SIGNAL(readyRead()), this, SLOT(cardCommRece()));
                qDebug() <<"command seting";

            }
            else if (inComm == ":proc:gui:event")
            {
                cardEventConnection = tempConnection;
                tempConnection->disconnect(SIGNAL(readyRead()));
                qDebug() <<"event seting";

            }

        }
    }

}

void Server::cardCommRece()
{
    qDebug() <<"something receive form command channel";
}
Server::~Server()
{

}
