#ifndef CSETRECCTRL_H
#define CSETRECCTRL_H
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

#define INFO_LIST_MAX_BUFF 1024*1024

class CSetRecCtrl
{
public:
    CSetRecCtrl(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief SetRecCtrlStart 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetRecCtrlStart(string &sResult,int RecMode,int ChannelID, string TaskId, string Title);

    /**
     * @brief SetRecCtrlStop 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetRecCtrlStop(string &sResult,int RecMode, string TaskId);

    /**
     * @brief GetRecCtrlInfoList 获取主机配置信息
     * @param GetMode
     * @param TaskId
     * @param StartTime
     * @param EndTime
     * @return true：成功，false：失败.
     */
    InterfaceResCode GetRecCtrlInfoList(string &sResult,int GetMode,string TaskId, string StartTime, string EndTime);

    /**
     * @brief SetRecCtrlTaskDel 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetRecCtrlTaskDel(string &sResult, string TaskId);
	InterfaceResCode GetWebRecordTime(string &sResult);
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
     * @brief ComposeResult 合并结果
     * @param result
     * @return true：成功，false：失败.
     */
    bool GetComposeResult(cJSON * result);
    /**
     * @brief ComposeResult 合并结果
     */
    string GetComposeResult();
    /**
     * @brief SetRecStart 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetRecStart(int RecMode,int ChannelID, string TaskId, string Title, CData &oResult, char *cResult);

    /**
     * @brief SetRecStop 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetRecStop(int RecMode, string TaskId, CData &oResult, char *cResult);

    /**
     * @brief GetRecCtrlInfoList 获取主机配置信息
     * @param sResult
     * @param GetMode
     * @param TaskId
     * @param StartTime
     * @param EndTime
     * @return true：成功，false：失败.
     */
    bool GetRecInfoList(int GetMode,string TaskId, string StartTime, string EndTime,CData &oResult, char *cResult);
    /**
     * @brief SetRecTaskDel 获取主机配置信息
     * @param TaskId
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetRecTaskDel(string TaskId, CData &oResult, char *cResult);
	bool GetWebRecordTimeInner(CData &oResult, char *cResult);
private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;
    CAnalyzeParaProc _AnalyParas;

    //用于存储结果
    cJSON *_result;
    cJSON *_VideoFiles;
};

#endif // CSETRECCTRL_H
