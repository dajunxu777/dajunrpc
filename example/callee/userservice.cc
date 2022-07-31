#include <iostream>
#include <string>
#include "user.pb.h"
#include "rpcapplication.h"
#include "rpcprovider.h"


/**
 *  UserService原来是一个本地服务，提供了两个进程内的本地方法，Login和GetFriendLists
 */

class UserService : public fixbug::UserServiceRpc   // 使用在rpc服务发布端（rpc服务提供者）
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service : Login"<< std::endl;
        std::cout << "name:" << name << "pwd" << pwd <<std::endl;
        return true;
    }

    /**
     *  重写基类 UserServiceRpc 的虚函数 下面这些方法是框架直接调用的
     *  1. caller -->  Login(LoginRequest) --> muduo --> callee
     *  2. callee -->  Login(LoginRequest) --> 交到下面重写的Login方法上
     */

    virtual void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done) override
    {
        // 框架给业务上报了请求参数LoginRequest，应用获取相应数据做本地服务
        std::string name = request->name();
        std::string pwd = request->pwd();
        
        // 本地业务
        bool login_result = Login(name, pwd);

        // 响应写入 包括写入错误码
        fixbug::ResultCode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_sucess(login_result);

        // 执行回调操作 执行响应对象数据的序列化和网络发送（都是由框架来完成的）
        done->Run();
    }

};

// 使用rpc框架
int main(int argc, char** argv)
{
    // 调用框架的初始化操作
    RpcApplication::Init(argc, argv);

    // 把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());

    // 启动一个rpc服务发布节点 Run以后， 进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();

    return 0;
}