#ifndef SERVER_H
#define SERVER_H

#include <QThread>
#include <fcntl.h>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include"workthread.h"
using namespace std;

const int MAX_CONNECT = 20;
class Server:public QThread
{
    Q_OBJECT
public:
    volatile bool working;

    Server();
    Server(const string root,const string IP,const unsigned short port);
    ~Server() override;
protected:
    void run() override;
    int srvFd;
private:
    const string root;
    const string ipAddr;
    const unsigned short port;

    struct sockaddr_in clientAddr;
    socklen_t nAddrLen;

private:
    int initServer();
    int acceptConnection();
//    int receiveRequestion(int clientFd);
//    int reseponseRequestion(int clientFd);

signals:
    void sendMsg(QString msg);
private slots:
    void killThread(workthread*);
};

#endif // SERVER_H
