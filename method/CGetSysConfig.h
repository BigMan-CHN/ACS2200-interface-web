#ifndef CGETSYSCONFIG_H
#define CGETSYSCONFIG_H
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

class CGetSysConfig
{
public:
    CGetSysConfig(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    InterfaceResCode GetSysConfig(string &sResult);

private:
    /**
     * @brief GetMPSSysConfig 获取MPS系统配置
     * @param oResult
     * @return true：成功，false：失败.
     */
    bool GetMPSSysConfig(CData &oResult);

    /**
     * @brief convertCData2CJSON 转换CData到cJSON
     * @param oResult
     * @return
     */
    bool convertCData2CJSON(CData &oResult);

    /**
     * @brief convertNetworkCData2CJSON
     * @param oNetwork
     * @param cNetwork
     * @return
     */
    bool convertNetworkCData2CJSON(CData &oNetwork,cJSON *cNetwork);

    bool convertSystemCData2CJSON(CData &oSys,cJSON *cSys);
    bool convertNtpCData2CJSON(CData &oNtp,cJSON *cNtp);
    bool convertChannelCData2CJSON(CData &oChannel,cJSON *cChannel);
    bool convertMulticastCData2CJSON(CData &oMulticast,cJSON *cMulticast);
private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;
    //用于存储结果
    cJSON *_result;
};

#endif // CGETSYSCONFIG_H
