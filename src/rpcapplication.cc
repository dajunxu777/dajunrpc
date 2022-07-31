#include "rpcapplication.h"
#include <iostream>
#include <unistd.h>
#include "string.h"

RpcConfig RpcApplication::m_config;
bool RpcApplication::is_config = false;

void ShowArgsHelp()
{
    std::cout<< "format: command -i <configfile>" <<std::endl;
}

void RpcApplication::Init(int argc, char** argv)
{
    if(argc < 2)
    {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string config_file;
    while((c = getopt(argc, argv, "i:")) != -1)
    {
        switch(c)
        {
            case 'i':
                config_file = optarg;
                break;
            case '?':
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            case ':':
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }

    // 加载配置文件 rpcserver_ip rpcserver_port zookeeper_ip zookeeper_port 
    // 只会加载一次不需要考虑线程安全，可以使用静态变量判断是否加载过，程序更加健壮
    if(!is_config)
    {
        m_config.LoadConfigFile(config_file.c_str());
        is_config = true;
    }
    else
    {
        std::cout<<"repeated load!"<<std::endl;
        return;
    }
    

    std::cout << "rpcserverip:" << m_config.Load("rpcserverip") << std::endl;
    std::cout << "rpcserverport:" << m_config.Load("rpcserverport") << std::endl;
    std::cout << "zookeeperip:" << m_config.Load("zookeeperip") << std::endl;
    std::cout << "zookeeperport:" << m_config.Load("zookeeperport") << std::endl;
}

RpcApplication& RpcApplication::GetInstance()
{
    static RpcApplication app;
    return app;
}

RpcConfig& RpcApplication::GetConfig()
{
    return m_config;
}