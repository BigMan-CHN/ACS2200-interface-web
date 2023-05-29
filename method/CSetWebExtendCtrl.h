#ifndef CSETWEBEXTENDCTRL_H
#define CSETWEBEXTENDCTRL_H
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

class CSetWebExtendCtrl
{
public:
    CSetWebExtendCtrl(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief SetWebExtendCtrlNetCfg 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebExtendCtrlNetCfg(string &sResult,string Ip,string GateWay,string NetMask,string MacAddr,string MainDNS,string BackupDNS);

    /**
     * @brief SetWebExtendCtrlSyncTime 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebExtendCtrlSyncTime(string &sResult,string SyncTime);

    /**
     * @brief SetWebExtendCtrlSyncTime 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebExtendCtrlDevID(string &sResult,string DevID);

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

    bool GetComposeResult(cJSON * result);
    /**
     * @brief ComposeResult 合并结果
     */
    string GetComposeResult();

    /**
     * @brief SetWebExtendNetCfg 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebExtendNetCfg(string Ip,string GateWay,string NetMask,string MacAddr,string MainDNS,string BackupDNS, CData &oResult, char *cResult);

    /**
     * @brief SetWebExtendSyncTime 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebExtendSyncTime(string SyncTime, CData &oResult, char *cResult);

    /**
     * @brief SetWebExtendDevID 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebExtendDevID(string DevID, CData &oResult, char *cResult);

private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;
    CAnalyzeParaProc _AnalyParas;

    //用于存储结果
    cJSON *_result;
    cJSON *_LogList;
};

#endif // CSETWEBEXTENDCTRL_H
