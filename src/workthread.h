#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include <fcntl.h>
#include <iostream>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include "requestion.h"
#include "response.h"
using namespace std;

const int BUFFER_SIZE = 1024;

class workthread:public QThread
{
    Q_OBJECT
private:
    int clientFd;
    struct sockaddr_in clientAddr;
    socklen_t nAddrLen;

    char recvBuff[BUFFER_SIZE];
    char sendBuff[BUFFER_SIZE];
public:
    volatile bool working;

public:
    workthread(int clientFd);
    ~workthread() override;
protected:
    void run() override;
private:
    int receiveRequestion();
    int responseRequestion();
signals:
    void sendMsg(QString msg);
    void jobDone(workthread*);
};

#endif // WORKTHREAD_H
