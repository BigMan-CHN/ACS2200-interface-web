#ifndef CSETWEBEFFECTCTRL_H
#define CSETWEBEFFECTCTRL_H
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

class CSetWebEffectCtrl
{
public:
    CSetWebEffectCtrl(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief SetEffectCtrlNew 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebEffectCtrlNew(string &sResult,int Chn, int Post,int Num, int Type, string Content,int OverlayDisplay, int Color, int FontSize, int SelectType, int TimeORText);

    /**
     * @brief SetEffectCtrlNew 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebEffectCtrlCaption(string &sResult,int Chn, int Post,int Num, int Type, string Content);

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
     * @brief SetEffectNew 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebEffectNew( int OverlayDisplay,int Chn, int Post, int Num, int Type, string Content, int Color, int FontSize, int SelectType, int TimeORText, CData &oResult, char *cResult);

    /**
     * @brief SetEffectCaption 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebEffectCaption(int Chn, int Post, int Num, int Type, string Content, CData &oResult, char *cResult);

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

#endif // CSETWEBEFFECTCTRL_H
