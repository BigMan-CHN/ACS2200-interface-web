#ifndef CSETWEBAUDIOCTRL_H
#define CSETWEBAUDIOCTRL_H
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

class CSetWebAudioCtrl
{
public:
    CSetWebAudioCtrl(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief SetWebAudioCtrlMicInput 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebAudioCtrlMicInput(string &sResult,int VolSize,int VolChn);

    /**
     * @brief SetWebAudioCtrlLineInput 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebAudioCtrlLineInput(string &sResult,int VolSize,int VolChn);

    /**
     * @brief SetWebAudioCtrlLineInput 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebAudioCtrlDecInput(string &sResult,int VolSize,int VolChn);

    /**
     * @brief SetWebAudioCtrlBalanceOutput 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebAudioCtrlBalanceOutput(string &sResult,int VolSize,int VolChn);

    /**
     * @brief SetWebAudioCtrlLineOutput 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebAudioCtrlLineOutput(string &sResult,int VolSize,int VolChn);

    /**
     * @brief SetWebAudioCtrlDecOutput 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebAudioCtrlDecOutput(string &sResult,int VolSize,int VolChn);

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
     * @brief SetWebAudioCtrlMicInput 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebAudioMicInput(int VolSize,int VolChn, CData &oResult, char *cResult);

    /**
     * @brief SetWebAudioLineInput 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebAudioLineInput(int VolSize,int VolChn, CData &oResult, char *cResult);

    /**
     * @brief SetWebAudioLineInput 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebAudioDecInput(int VolSize,int VolChn, CData &oResult, char *cResult);

    /**
     * @brief SetWebAudioBalanceOutput 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebAudioBalanceOutput(int VolSize,int VolChn, CData &oResult, char *cResult);


    /**
     * @brief SetWebAudioLineOutput 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebAudioLineOutput(int VolSize,int VolChn, CData &oResult, char *cResult);

    /**
     * @brief SetWebAudioDecOutput 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebAudioDecOutput(int VolSize,int VolChn, CData &oResult, char *cResult);

private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;
    CAnalyzeParaProc _AnalyParas;

    //用于存储结果
    cJSON *_result;
};

#endif // CSETWEBAUDIOCTRL_H
