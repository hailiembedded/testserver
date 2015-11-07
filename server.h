#pragma pack(4)
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

#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QHash>
#include<QtNetwork>
#include <QtGui>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QTcpServer;
class QNetworkSession;
class QTcpSocket;
class QLineEdit;
class QComboBox;
QT_END_NAMESPACE


/*
 * typedef struct __attribute__((__packed__))
{
    bool	laser_source_status;			// laser source status
    unsigned char    mode;//constant power/current/gain mode
    float    out_current_min;
    float   out_current_max;
    float   gain_min;
    float   gain_max;
    float   power_min;
    float   power_max;
    float   gain_threshold;
    float   gain_accuracy;
    float   output_power;
    float   output_power_threshold;
    float   output_power_accuracy;
    bool    ALS_enable;
    float   LOS_threshold;
    float   optical_power_value;
    float   optical_output_power_value;
    float   wav_length;
    float   ATI;


}OA_Profile_t ;

//! [0]
>>>>>>> 90ba0e9c01b2d792fbd3717ee1ea9a12007f2cdf
*/
class Server : public QDialog
{
    Q_OBJECT



public:
    Server(QWidget *parent = 0);


    typedef struct
    {
        bool	laser_source_status;			// laser source status
        unsigned char    mode;                           //constant power/current/gain mode
        float    out_current_min;
        float   out_current_max;
        float   gain_min;
        float   gain_max;
        float   power_min;
        float   power_max;
        float   gain_threshold;
        float   gain_accuracy;
        float   output_power;
        float   output_power_threshold;
        float   output_power_accuracy;
        bool    ALS_enable;
        float   LOS_threshold;
        float   optical_power_value;
        float   optical_output_power_value;
        float   wav_length;
        float   ATI;


    }OA_Profile_t;

private slots:
    void sessionOpened();
    void setupCon();
    void setupConc1();
    void replyComm();
    void replyCommc1();
    void sendComm();
    void clickSend();
    void connectServerError(QAbstractSocket::SocketError err);
    void serverResponse();
    void setupConCard();
    void replyCommCard();

private:
    QLabel *statusLabel;
    QPushButton *quitButton;
    QPushButton *sendButton;
    QLineEdit *ip;
    QLineEdit *port;
    QComboBox *commandText;
    QLineEdit *paraComm;
    QTcpServer *tcpServer;
    QTcpServer *tcpCardServer;
    QTcpServer *tcpServerc1;
    QNetworkSession *networkSession;
    QTcpSocket *tcpCard;
    QTcpSocket *clientConnection;
    QTcpSocket *clientConnectionc1;
    QTcpSocket *serverConnection;
    QTcpSocket *cardConnection;
    quint8 blockSize;

    QHash<QString, QString> commReply;
    QHash<QString, QString> cardReply;
    QHash<QString, QString> commReplyc1;
    OA_Profile_t test;

    OA_Profile_t testProfile ;



};
//! [0]

#endif
