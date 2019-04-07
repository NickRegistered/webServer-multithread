#include "workthread.h"

workthread::workthread(int clientFd):clientFd(clientFd){

}

int workthread::receiveRequestion(){
    char clientIp[64] = {0};
    ssize_t len = recv(clientFd,recvBuff,BUFFER_SIZE,0);
    if(len == 0){
        sendMsg("one connectionclosed,IP: "\
                +QString::fromLocal8Bit(inet_ntop(AF_INET,&clientAddr.sin_addr.s_addr,clientIp,sizeof(clientIp)))
                 +"  Port: " + QString::number(ntohs(clientAddr.sin_port)) + "\n\n");
        return -1;
    }
    else if(len < 0) {
        sendMsg("\nreceive data from client failed! errno:"+QString::number(errno) +\
                "IP: "+QString::fromLocal8Bit(inet_ntop(AF_INET,&clientAddr.sin_addr.s_addr,clientIp,sizeof(clientIp)))
                +"  Port: " + QString::number(ntohs(clientAddr.sin_port)) + "\n\n");
        return -1;
    }
    sendMsg("receive data from IP: "\
            +QString::fromLocal8Bit(inet_ntop(AF_INET,&clientAddr.sin_addr.s_addr,clientIp,sizeof(clientIp)))
            +"  Port: " + QString::number(ntohs(clientAddr.sin_port)) + "\n\n");
    Requestion::resolve(recvBuff);
    sendMsg("Requestion Method:"+QString::fromLocal8Bit(Requestion::Method) + "\n"+\
            "Requestion URL" + QString::fromLocal8Bit(Requestion::URL)+"\n");
    return 0;
}

int workthread::responseRequestion(){
    int fd;
    size_t ret,sum;
    if((ret = Response::Header(Requestion::URL+1,Requestion::Extn,sendBuff))== (~0uL)){
        sendMsg("open " + QString::fromLocal8Bit(Requestion::URL+1) + " failed\n");
        return -1;
    }
    send(clientFd,sendBuff,ret,0);
    sendMsg("sending file: " + QString::fromLocal8Bit(Requestion::URL) + "\n");
    fd = open(Requestion::URL+1,O_RDONLY);
    sum = 0;
    while(ret = read(fd,sendBuff,BUFFER_SIZE)){
        send(clientFd,sendBuff,ret,0);
        sum += ret;
    }
    sendMsg("file sending succeeded!\n\n");
#ifdef DEBUG_ON
    cout<<Requestion::URL<<" "<<sum<<"Bytes\n";
#endif
    close(fd);
    return 0;
}

void workthread::run(){
    while(working){
        if(receiveRequestion() == -1 || responseRequestion() == -1)break;
    }
    jobDone(this);
    return;
}
