#ifndef CDISCIPLINE_H
#define CDISCIPLINE_H
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

class CDiscipLineCtrl
{
public:
    CDiscipLineCtrl(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief SetRecCtrlStart 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetDiscipLineCtrlStart(string &sResult,bool LoopFlag, int PlayList);

    /**
     * @brief SetRecCtrlStop 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetDiscipLineCtrlStop(string &sResult);

    /**
     * @brief SetRecCtrlTaskDel 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode GetDiscipLineCtrlStatus(string &sResult);
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
    bool SetDiscipLineStart(bool LoopFlag, int PlayList, CData &oResult, char *cResult);

    /**
     * @brief SetRecStop 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetDiscipLineStop(CData &oResult, char *cResult);

    /**
     * @brief SetRecTaskDel 获取主机配置信息
     * @param TaskId
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool GetDiscipLineStatus(CData &oResult, char *cResult);
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
