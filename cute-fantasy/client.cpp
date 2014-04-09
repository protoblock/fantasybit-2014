#include "client.h"
#include <nanomsg/pair.h>
#include <MsgSock.h>
#include <string>
#include <fb/ProtoData.pb.h>
#include <QtDebug>
#include <QProcess>>
#include <QCoreApplication>

using namespace fantasybit;
using namespace std;

Client::Client(const string &a, QObject *parent) :
                    address(a),
                    QObject(parent) , sock {AF_SP, NN_PAIR},
                    receiver{sock}, timer(new QTimer(this)), timer2(new QTimer(this))
{
    //std::string address= {"ipc:///tmp/test.ipc"};
    //sock.connect(address.c_str());
    //Receiver receiver{sock};
    QProcess process;// = new QProcess();
    process.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    int i = process.startDetached(QCoreApplication::applicationDirPath()+"/fantasybit");///usr/local/bin/fantasybit");///bin/bash", QStringList() << "-c" << "fantasybit");
    //int i = QProcess::execute(QString("/bin/bash -c fantasybit"));
    hbdata.set_type(InData_Type_HEARTBEAT);
    connect(timer2, SIGNAL(timeout()), this, SLOT(pollServer()));
    connect(timer, SIGNAL(timeout()), this, SLOT(heartbeat()));
    connect(this, SIGNAL(doPoll()), this, SLOT(pollServer()));
}

bool Client::event(QEvent *ev)
{
    if  (ev->type() == QEvent::ThreadChange)
    {
        QTimer::singleShot(0, this, SLOT(start()));
    }


    return QObject::event(ev);
}

//slots
void Client::start()
{
    InData ind{};
    sock.connect(address.c_str());
    indata.set_type(InData_Type_CONNECT);
    Sender::Send(sock,indata);
    timer->start(10000);
    timer2->start(500);
    //emit doPoll();
}

void Client::fromGUI(const QString &in)
{
    //todo
    Sender::Send(sock,indata);
}

void Client::toServer(const InData &in)
{
    Sender::Send(sock,in);
}

void Client::pollServer()
{
    if ( receiver.receive(outdata,NN_DONTWAIT))
    {
        //std::cout << outdata.DebugString() << " \n";
        //QString q
        qDebug() << QString::fromStdString(outdata.DebugString());
        emit onData(outdata);//QString::fromStdString(outdata.DebugString()));
    }
    //emit doPoll();

}

void Client::heartbeat()
{
    Sender::Send(sock,hbdata);
}

void Client::quit()
{
    indata.set_type(InData_Type_QUIT);
    Sender::Send(sock,indata);
}
