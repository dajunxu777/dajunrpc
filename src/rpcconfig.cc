#include "rpcconfig.h"
#include <string>
#include <iostream>

// 负责解析加载配置文件
void RpcConfig::LoadConfigFile(const char* config_file)
{
    FILE* pf = fopen(config_file, "r");
    if(nullptr == pf)
    {
        std::cout<<config_file<<"is note exist!"<<std::endl;
        exit(EXIT_FAILURE);
    }

    // 1. 注释 2. 正确的配置项 = 3. 去掉开头多余空格

    while(!feof(pf))
    {
        char buf[512] = {0};
        fgets(buf, 512, pf);    // 读一行

        // 去掉字符串前面多余的空格
        std::string read_buf(buf);
        Trim(read_buf);

        // 判断#注释
        if(read_buf[0] == '#' || read_buf.empty())
        {
            continue;
        }

        // 解析配置项
        int idx = read_buf.find('=');
        if(idx == std::string::npos)
        {
            continue;
        }

        std::string key;
        std::string value;
        key = read_buf.substr(0, idx);
        Trim(key);
        read_buf = read_buf.substr(idx + 1);
        //std::cout<<read_buf<<std::endl;
        Trim(read_buf);
        //std::cout<<read_buf<<std::endl;
        value = read_buf;
        // 去掉结尾\n
        int len = value.size();
        if(value[len - 1] == '\n')
        {
            value = value.substr(0, len - 1);
        }

        m_configMap.insert({key, value});
    }

    fclose(pf);
}

// 查询配置项信息
std::string RpcConfig::Load(const std::string& key)
{
    auto it = m_configMap.find(key);
    if(it == m_configMap.end())
    {
        return "";
    }
    return it->second;
    // if(!m_configMap.count(key))
    // {
    //     return "";
    // }
    // return m_configMap[key];
}

void RpcConfig::Trim(std::string& s)
{
    if(!s.empty())
    {
        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
    }
}