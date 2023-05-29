#ifndef CSETPOINT_H
#define CSETPOINT_H
/********************************************************
 * 用于控制主机开始录制和停止录制
 *******************************************************/
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
#include "CGetHostStatus.h"

class CSetpoint
{
public:
    CSetpoint(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);
    /**
     * @brief Setpoint 视频打点
     * @param chNum 0-n
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode Setpoint(string &sResult,int chNum = 0);

    string ComposeResult();

private:
    bool AddRootNode();

private:

    /**
     * @brief analyzeRes 分析sql执行结果
     * @param status
     * @return true：成功，false：失败.
     */
    bool analyzeRes(MPSMediaStatus &status);

private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;
    //用于存储结果
    cJSON *_result;

    long _duration;
    string _filePath;

    GetHostStatus _hostStatus;
};

#endif // CSETPOINT_H
