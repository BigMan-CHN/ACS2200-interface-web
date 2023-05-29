#ifndef CGETWEBHOSTCONFIG_H
#define CGETWEBHOSTCONFIG_H
/********************************************************
 * public c/c++ standard headers
 *******************************************************/
#include <stdio.h>
#include <iostream>
#include <string.h>

/********************************************************
 * boost usart headers
 *******************************************************/
#include <boost/asio.hpp>
#include <boost/bind.hpp>

/********************************************************
 * lib headers
 *******************************************************/
#include <log.h>
#include <TransmitClient.h>
#include <xmlParser.h>
#include <LibXML.h>
#include <Server.h>
#include <cJSON.h>
#include <ctime>

#include "interfaceDefines.h"
#include "MPSClient.h"
#include "CAnalyzeParaProc.h"

class CGetWebHostConfig
{
public:
    CGetWebHostConfig(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief GetHostConfig 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode GetWebHostConfig(string &sResult);

private:
    //合成返回信息
    bool ComposeResult(cJSON * result);
    string ComposeResult();

private:
    /**
     * @brief getMPSConfig 获取主机配置信息
     * @param oResult
     * @return true：成功，false：失败.
     */
    bool getMPSConfig(CData &oResult, char *cResult);

private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;
	
    MPSClient *_client;
    CAnalyzeParaProc _AnalyParas;

    //用于存储结果
    cJSON *_result;
    cJSON *_network;
    cJSON *_channels;
    cJSON *_sysinfo;
    cJSON *_diskinfo;
    cJSON *_decnum;
    cJSON *_otherinfo;
};

#endif // CGETWEBHOSTCONFIG_H
