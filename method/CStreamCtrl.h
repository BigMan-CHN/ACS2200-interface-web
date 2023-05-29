#ifndef CSTREAMCTRL_H
#define CSTREAMCTRL_H
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

class CStreamCtrl
{
public:
    CStreamCtrl(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief SetRecCtrlStart 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetStreamCtrlRtmpPush(string &sResult, int RtmpID, int Channel, string Addr);

    /**
     * @brief SetRecCtrlStop 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetStreamCtrlOpen(string &sResult, bool AllFlag, int ChannelID);

    /**
     * @brief SetRecCtrlTaskDel 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetStreamCtrlClose(string &sResult, bool AllFlag, int ChannelID);
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
    /**
     * @brief SetRecStart 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetStreamRtmpPush(int RtmpID, int Channel, string Addr, CData &oResult, char *cResult);

    /**
     * @brief SetRecStop 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetStreamOpen(bool AllFlag, int ChannelID, CData &oResult, char *cResult);

    /**
     * @brief SetRecTaskDel 获取主机配置信息
     * @param TaskId
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetStreamClose(bool AllFlag, int ChannelID, CData &oResult, char *cResult);
private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;
    CAnalyzeParaProc _AnalyParas;

    //用于存储结果
    cJSON *_result;
    cJSON *_statusinfo;
};

#endif // CDISCIPLINE_H
