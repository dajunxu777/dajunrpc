#include <iostream>
#include "string.h"
#include <echo_service.pb.h>
#include "rpcapplication.h"
#include "rpcprovider.h"

class EchoService : public fixbug::EchoServiceRpc
{
    void Echo(::google::protobuf::RpcController* controller,
                       const ::fixbug::EchoRequest* request,
                       ::fixbug::EchoResponse* response,
                       ::google::protobuf::Closure* done)
                       {
                            std::string request_str = request->message();
                            std::cout<<request_str<<std::endl;
                            response->set_message(request_str);
                            done->Run();
                       }
    void Add(::google::protobuf::RpcController* controller,
                       const ::fixbug::AddRequest* request,
                       ::fixbug::AddResponse* response,
                       ::google::protobuf::Closure* done)
                       {
                            int a = request->a();
                            int b = request->b();
                            response->set_result(a + b);
                            done->Run();
                       }
};

int main(int argc, char** argv)
{
    RpcApplication::Init(argc, argv);

    RpcProvider provider;

    provider.NotifyService(new EchoService());

    provider.Run();
    
    return 0;
}