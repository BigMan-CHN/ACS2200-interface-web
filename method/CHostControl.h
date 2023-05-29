#ifndef CHOSTCONTROL_H
#define CHOSTCONTROL_H

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

class CHostControl
{
public:
    CHostControl(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief HostInit 打开直播流
     * @param caseid
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode HostInit(int chNum, string caseid);

    /**
     * @brief HostStart 开始录制
     * @param caseid
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode HostStart(int chNum,string caseid);

    /**
     * @brief HostStart 停止录制
     * @param int chNum
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode HostStop(int chNum);

    /**
     * @brief HostClose 关闭直播流
     * @param int chNum
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode HostClose(int chNum);

    /**
     * @brief HostShutdown 关闭主机(未经过MPS)
     * @return
     */
    InterfaceResCode HostShutdown();
    /**
     * @brief HostReboot 重启主机(未经过MPS)
     * @return
     */
    InterfaceResCode HostReboot();

protected:
    /**
     * @brief Close2Init 无此事件或已经结束 状态下打开直播流
     * @param caseid
     * @return
     */
    InterfaceResCode Close2Init(int chNum, string caseid);
    /**
     * @brief Ready2Init 准备就绪 状态下打开直播流
     * @param caseid
     * @return
     */
    InterfaceResCode Ready2Init(int chNum, string caseid);
    /**
     * @brief Running2Init 正在工作 状态下打开直播流
     * @param caseid
     * @return
     */
    InterfaceResCode Running2Init(int chNum, string caseid);
    /**
     * @brief Pause2Init 暂停 状态下打开直播流
     * @param caseid
     * @return
     */
    InterfaceResCode Pause2Init(int chNum, string caseid);
    /**
     * @brief Stop2Init 已经停止 状态下打开直播流
     * @param caseid
     * @return
     */
    InterfaceResCode Stop2Init(int chNum, string caseid);

protected:
    /**
     * @brief Close2Start 无此事件或已经结束 状态下开启录制
     * @param caseid
     * @return
     */
    InterfaceResCode Close2Start(int chNum, string caseid);
    /**
     * @brief Ready2Start 准备就绪 状态下开启录制
     * @param caseid
     * @return
     */
    InterfaceResCode Ready2Start(int chNum,string caseid);
    /**
     * @brief Running2Start 正在工作 状态下开启录制
     * @param caseid
     * @return
     */
    InterfaceResCode Running2Start(int chNum,string caseid);
    /**
     * @brief Pause2Start 暂停 状态下开启录制
     * @param caseid
     * @return
     */
    InterfaceResCode Pause2Start(int chNum,string caseid);
    /**
     * @brief Stop2Start 已经停止 状态下开启录制
     * @param caseid
     * @return
     */
    InterfaceResCode Stop2Start(int chNum,string caseid);

protected:
    /**
     * @brief Close2Stop 无此事件或已经结束 状态下停止录制
     * @param
     * @return
     */
    InterfaceResCode Close2Stop(int chNum);

    /**
     * @brief Ready2Stop 准备就绪 状态下停止录制
     * @param
     * @return
     */
    InterfaceResCode Ready2Stop(int chNum);
    /**
     * @brief Running2Stop 正在工作 状态下停止录制
     * @param
     * @return
     */
    InterfaceResCode Running2Stop(int chNum);
    /**
     * @brief Pause2Stop 暂停 状态下停止录制
     * @param
     * @return
     */
    InterfaceResCode Pause2Stop(int chNum);
    /**
     * @brief Stop2Stop 已经停止 状态下停止录制
     * @param
     * @return
     */
    InterfaceResCode Stop2Stop(int chNum);

protected:
    /**
     * @brief Close2Close 无此事件或已经结束 状态下 无此事件或已经结束
     * @param
     * @return
     */
    InterfaceResCode Close2Close(int chNum);

    /**
     * @brief Ready2Close 准备就绪 状态下 无此事件或已经结束
     * @param
     * @return
     */
    InterfaceResCode Ready2Close(int chNum);
    /**
     * @brief Running2Close 正在工作 状态下 无此事件或已经结束
     * @param
     * @return
     */
    InterfaceResCode Running2Close(int chNum);
    /**
     * @brief Pause2Close 暂停 状态下 无此事件或已经结束
     * @param
     * @return
     */
    InterfaceResCode Pause2Close(int chNum);
    /**
     * @brief Stop2Close 已经停止 状态下 无此事件或已经结束
     * @param
     * @return
     */
    InterfaceResCode Stop2Close(int chNum);

private:
    InterfaceResCode MediaPrepareAll(int chNum,string caseId);
    InterfaceResCode MediaStartAll(int chNum,string caseId);
    InterfaceResCode MediaStopAll(int chNum);
    InterfaceResCode MediaStopAll(int chNum,bool closeFlag);
    InterfaceResCode MediaCloseAll(int chNum);

private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;
    //用于存储结果
    cJSON *_result;

    GetHostStatus _hostStatus;
};

#endif // CHOSTCONTROL_H
