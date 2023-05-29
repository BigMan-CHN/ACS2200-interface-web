/********************************************************
* InterfafceProcess.h
* 接口功能
*
* Copyright (c) 2019, Tyvis
* All rights reserved.
*
* @version		V1.00
* @author		es
* @date			2019-5-17
 *******************************************************/
#ifndef INTERFACEPROCESS_H
#define INTERFACEPROCESS_H
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

#include "interfaceDefines.h"
#include "MPSClient.h"

#define MPS_IP      "127.0.0.1"
//#define MPS_PORT    5231
#define MPS_PORT    6180
#define MPS_TIMEOUT 15000

using namespace std;
using namespace SVS;

class InterfaceProcess
{
public:
    InterfaceProcess(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);
    
    /**
     * @brief processRequest 处理请求
     * @param requestStr 请求数据字符串
     * @return 返回信息字符串
     */
    string processRequest(string &requestStr);

    void debugMode();
public:

    /**
     * @brief HostStatusMethod 获取设备指定通道的直播状态
     * @param chState 获取的状态
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode HostStatusMethod(CHStatus &chState);
    InterfaceResCode HostStatusMethodV2(CHStatus &chState);

    /**
     * @brief HostInitMethod 打开设备指定通道的视频流
     * @param caseId 录制任务唯一值
     * @param resCaseId 返回的任务唯一值
     * @return 0：成功，!0：失败.
     */
    /*
    InterfaceResCode HostInitMethod(string caseId, string &resCaseId);
    //*/

    /**
     * @brief HostStartMethod 开始设备指定通道的视频录制
     * @param caseId 录制任务唯一值
     * @return 0：成功，!0：失败.
     */
    /*
    InterfaceResCode HostStartMethod( string caseId);
    //*/
    /**
     * @brief HostStopMethod 停止设备指定通道的视频录制
     * @param closeFlag true:停止录制同时关闭直播流,false:仅停止录制
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode HostStopMethod(bool closeFlag);

    /**
     * @brief HostCloseMethod 关闭设备指定通道的视频流
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode HostCloseMethod();
    /**
     * @brief GetTaskContentMethod 获取指定任务的内容列表
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode GetTaskContentMethod(string caseId, cJSON *result);
    InterfaceResCode GetTaskContentMethod(string caseId, string &result);

private:
    bool checkCaseIdisEnable(string caseId);
    bool checkChannelNumisEnable(int chNum);
private:
    /**
     * @brief socketSend 发送 socket 命令 到 MPS
     * @param sModule 模块
     * @param sMethod 方法
     * @param oParams 所需要的参数
     * @return 返回 MPS 返回的信息
     */
    CData socketSend(string sModule, string sMethod, CData oParams= SVSMAP());

private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;
    //debug
    bool _debugMode;
};

#endif // INTERFACEPROCESS_H
