#include "zookeeperutil.h"
#include "rpcapplication.h"
#include "semaphore.h"
#include <iostream>

// 全局的watcher观察器 zkserver
void global_watcher(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx)
{
    // 回调的消息类型是和会话相关的消息类型
    if(type == ZOO_SESSION_EVENT)
    {
        // zkclient和zkserver连接成功
        if(state == ZOO_CONNECTED_STATE)
        {
            sem_t* sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

ZkClient::ZkClient() : m_zhandle(nullptr)
{
}

ZkClient::~ZkClient()
{
    if (m_zhandle != nullptr)
    {
        zookeeper_close(m_zhandle); // 关闭句柄，释放资源 
    }
}


// 连接zkserver
void ZkClient::Start()
{
    std::string host = RpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string post = RpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    // post 出错
    std::string connstr = host + ":" + post;

    /**
     *  zookeeper_mt: 多线程版本
     *  zookeeper的API客户端提供了三个线程
     *  API调用线程
     *  网络I/O线程 pthread_create poll
     *  watcher回调线程 pthread_create
     */

    m_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    if(nullptr == m_zhandle)
    {
        std::cout<<"zookeeper_init error!"<<std::endl;
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zhandle, &sem);

    sem_wait(&sem);
    std::cout<< "zookeeper_init success!"<<std::endl;
}

void ZkClient::Create(const char* path, const char* data, int datalen, int state)
{
    char path_buffer[128] = {0};
    // 先判断path表示的znode节点是否存在，如果存在，就不再重复创建了
    int flag = zoo_exists(m_zhandle, path, 0, nullptr);

    if(ZNONODE == flag)
    {
        // 创建指定path的znode节点
        flag = zoo_create(m_zhandle, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, sizeof(path_buffer));
        if(ZOK == flag)
        {
            std::cout << "znode create success... path: " << path << std::endl;
        }
        else
        {
            std::cout << "flag:" << flag << std::endl;
			std::cout << "znode create error... path: " << path << std::endl;
			exit(EXIT_FAILURE);
        }
    }
}


std::string ZkClient::GetData(const char* path)
{
    char buffer[64] = {0};
    int bufferlen = sizeof(buffer);
    
    int flag = zoo_get(m_zhandle, path, 0 , buffer, &bufferlen, nullptr);

    if(ZOK != flag)
    {
        std::cout << "get znode error... path: " << path << std::endl;
		return "";
    }
    else
    {
        return buffer;
    }
}