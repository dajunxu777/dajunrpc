#pragma once
#include "rpcconfig.h"
#include "rpccontroller.h"
// dajunrpc框架的基础类 负责框架的初始化操作
class RpcApplication
{
public:
    static void Init(int argc, char** argv);
    static RpcApplication& GetInstance();
    static RpcConfig& GetConfig();
private:
    static RpcConfig m_config;
    static bool is_config;

    RpcApplication(){}
    RpcApplication(const RpcApplication&) = delete;
    RpcApplication& operator=(const RpcApplication&) = delete;
};