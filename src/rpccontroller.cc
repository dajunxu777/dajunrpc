#include "rpccontroller.h"

RpcController::RpcController()
{
    m_failed = false;
    m_errText = "";
}

// 下面函数均为基类虚函数
void RpcController::Reset()
{
    m_failed = false;
    m_errText = "";
}
bool RpcController::Failed() const
{
    return m_failed;
}
std::string RpcController::ErrorText() const
{
    return m_errText;
}
void RpcController::SetFailed(const std::string& reason)
{
    m_failed = true;
    m_errText = reason;
}

// 暂时用不上
void RpcController::StartCancel(){}
bool RpcController::IsCanceled() const{}
void RpcController::NotifyOnCancel(google::protobuf::Closure* callback){}