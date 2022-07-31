#include <iostream>
#include "rpcapplication.h"
#include "user.pb.h"
#include "rpcchannel.h"

int main(int argc, char** argv)
{
    // 整个程序启动以后，想使用dajunrpc框架进行rpc服务调用，一定需要先调用框架的初始化函数（只初始化一次）
    RpcApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new RpcChannel());
    
    // rpc方法请求
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    // rpc方法响应
    fixbug::LoginResponse response;
    // RpcChannel->RpcChannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送
    // 框架类需要重写RpcChannel::callMethod函数
    stub.Login(nullptr, &request, &response, nullptr);   

    // 一次rpc调用完成，读调用的结果
    if(0 == response.result().errcode())
    {
        std::cout<< "rpc login response success: "<< response.sucess() << std::endl;
    }
    else
    {
        std::cout<< "rpc login response error: "<< response.result().errmsg()<<std::endl;
    }
    
    return 0;
}