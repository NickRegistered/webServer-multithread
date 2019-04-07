#include "server.h"
Server::Server(const string root,const string IP,\
               const unsigned short port):\
    root(root),ipAddr(IP),port(port)
{

}

int Server::initServer()
{
    int ret,flag;
    srvFd = socket(AF_INET,SOCK_STREAM,0);
    if(srvFd == -1){
        sendMsg("Create server socket failed!\n");
        return -1;
    }
    sendMsg("Create server socket OK!\n");

    /*set address reuse*/
    setsockopt(srvFd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));

    /*set server's address & port*/
    struct sockaddr_in srvAddr;
    memset(&srvAddr,0,sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    srvAddr.sin_port = htons(port);
    bind(srvFd,(sockaddr*)&srvAddr,sizeof(srvAddr));

    if((ret = listen(srvFd,MAX_CONNECT))==-1){
        sendMsg("Listen failed!\n");
        return -1;
    }
    sendMsg("Server listen startup OK!\n");
    sendMsg("Server startup OK!\n\n");
    return 0;
}

int Server::acceptConnection(){
    nAddrLen = sizeof(clientAddr);
    int clientFd = accept(srvFd,(sockaddr*)&clientAddr,&nAddrLen);
    if(clientFd == -1){
        sendMsg("accept failed!\n");
        return -1;
    }
    char clientIp[64] = {0};
    sendMsg("New Client IP: "+QString::fromLocal8Bit(inet_ntop(AF_INET,&clientAddr.sin_addr.s_addr,clientIp,sizeof(clientIp)))
            +"  Port: " + QString::number(ntohs(clientAddr.sin_port)) + "\n");

   sendMsg("\nOne new session is established!\n");
    return clientFd;
}

void Server::run(){
    int ret;
    string filename;
    working = ((ret = initServer()) != -1);
    chdir(root.c_str());
    while(working){
        if((ret =(acceptConnection()) != -1)){
            workthread* p_work = new workthread(ret);
            p_work->working = true;
            p_work->start();
        }
        else continue;
    }
    return;
}

Server::~Server(){
    close(srvFd);
}

void Server::killThread(workthread* p_work){
    p_work->working = false;
    p_work->wait();
    delete p_work;
}
