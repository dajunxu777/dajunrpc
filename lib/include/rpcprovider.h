#pragma once
#include "google/protobuf/service.h"
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include "string.h"
#include "rpcapplication.h"
#include "google/protobuf/descriptor.h"
#include <unordered_map>

// 框架提供的专门服务发布rpc服务的网络对象类
class RpcProvider
{
public:
    // 框架提供给外部使用的，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service* service);
    
    // 启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();
private:
    // 组合EventLoop
    muduo::net::EventLoop m_eventLoop;

    // service服务类型信息
    struct ServiceInfo
    {
        google::protobuf::Service* m_service;   // 保存服务对象
        std::unordered_map<std::string, 
                const google::protobuf::MethodDescriptor*> m_methodMap; // 保存服务
    };
    
    // 存储注册成功的服务对象   ServiceInfo中有其所有方法信息
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;

    void OnConnection(const muduo::net::TcpConnectionPtr& conn);

    void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);

    // Closure的回调操作，用于序列化rpc的响应和网络发送
    void SendRpcResponse(const muduo::net::TcpConnectionPtr&, google::protobuf::Message*);
};