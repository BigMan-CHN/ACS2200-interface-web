#ifndef SetWebSyncTime_H
#define SetWebSyncTime_H
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
#include<stdlib.h>

#include<sys/time.h>
#include<unistd.h>
#include <time.h>

#include "interfaceDefines.h"
#include "MPSClient.h"
#include "CAnalyzeParaProc.h"

class CSetWebSyncTime
{
public:
    CSetWebSyncTime(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief GetHostDecInfo 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebSyncTime(string &sResult,string NtpIp,string SysTime, int Mode);
	InterfaceResCode GetWebSyncTime(string &sResult);

private:
    /**
     * @brief ComposeResult 合并结果
     * @param result
     * @return true：成功，false：失败.
     */
    bool ComposeResult(cJSON * result);
    /**
     * @brief ComposeResult 合并结果
     */
    string ComposeResult();
	bool SetWebSyncTimeInner(string NtpIp, string SysTime, int Mode, CData& oResult, char* cResult);
	bool GetWebSyncTimeInner( CData& oResult, char* cResult);

private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;
    CAnalyzeParaProc _AnalyParas;

    //用于存储结果
    cJSON *_result;
    cJSON *_total;
    cJSON *_addressList;
};

#endif // CGETHOSTCHNINFO_H
