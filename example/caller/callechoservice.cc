#include <iostream>
#include "rpcapplication.h"
#include "echo_service.pb.h"
#include "rpcchannel.h"

int main(int argc, char** argv)
{
    RpcApplication::Init(argc, argv);

    fixbug::EchoServiceRpc_Stub stub(new RpcChannel());

    RpcController controller;

    fixbug::AddRequest add_request;
    fixbug::EchoRequest echo_request;

    std::cout<<"请输入您想调用哪个方法："<<"1. Add "<< "2. Echo"<<std::endl;
    int m = 0;
    std::cin>>m;
    if(m == 1)
    {
        std::cout<<"调用 Add 方法"<<std::endl;
        std::cout<<"请输入两个数"<<std::endl;
        int aa = 0;
        int bb = 0;
        std::cin>>aa>>bb;
        add_request.set_a(aa);
        add_request.set_b(bb);
        fixbug::AddResponse add_response;
        stub.Add(&controller, &add_request, &add_response, nullptr);
        if(controller.Failed())
        {
            std::cout << controller.ErrorText() << std::endl;
        }
        else
        {
            std::cout<<aa<<" + "<<bb<<" 得到的结果为："<<add_response.result()<<std::endl;
        }
        
    }
    else if(m == 2)
    {
        std::cout<<"调用 Echo 方法"<<std::endl;
        std::cout<<"请输入:"<<std::endl;
        std::string echo_str;
        std::cin>>echo_str;
        echo_request.set_message(echo_str);
        fixbug::EchoResponse echo_response;
        stub.Echo(&controller, &echo_request, &echo_response, nullptr);
        
        if(controller.Failed())
        {
            std::cout << controller.ErrorText() << std::endl;
        }
        else
        {
            std::cout<<echo_response.message()<<std::endl;
        }
    }
    else
    {
        std::cout<<"您的输入有误"<<std::endl;
        return -1;
    }

    return 0;
}