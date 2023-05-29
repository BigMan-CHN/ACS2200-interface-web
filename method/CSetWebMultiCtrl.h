#ifndef CSETWEBMULTICTRL_H
#define CSETWEBMULTICTRL_H
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

class CSetWebMultiCtrl
{
public:
    CSetWebMultiCtrl(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief SetDecCtrlStart 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebMultiCtrlStart(string &sResult,int Dectype,int DecID, string DecUrl);

    /**
     * @brief SetDecCtrlStop 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebMultiCtrlStop(string &sResult,int Dectype,int DecID);

    /**
     * @brief SetWebMultiCtrlRtsp 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebMultiCtrlRtsp(string &sResult,int DecMethod,int DecID,int DecMode,int DecFormat,string DecUrl);

    /**
     * @brief SetWebMultiCtrlH323 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebMultiCtrlH323(string &sResult,int DecID,int VideoEncType,int AudioEncType,string DecUrl,string Contact,string SendVideoSrc,string SendAudioSrc);
    
    /**
     * @brief SetWebMultiCtrlSip 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebMultiCtrlSip(string &sResult,int DecMode,int DecID, string DecUrl, string UserName, string UserPw);

    /**
     * @brief SetWebMultiCtrlSipList 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebMultiCtrlSipList(string &sResult,int DecType,int DecMethod,int Rate,int LenBuf,string DecUrl,string Title);
	InterfaceResCode SetWebStartDecNew(string &sResult, int ArryNumbers, int DecType[], int DecID[], string DecUrl[]);

	InterfaceResCode SetWebStopDecNew(string &sResult, int ArryNumbers, int DecType[], int DecID[]);

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
     * @brief SetDecStart 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebMultiStart(int Dectype,int DecID, string DecUrl, CData &oResult, char *cResult);

    /**
     * @brief SetDecStop 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebMultiStop(int Dectype, int DecID, CData &oResult, char *cResult);

        /**
     * @brief SetDecStop 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebMultiRtsp(int DecType,int DecID,int DecMode,int DecFormat,string DecUrl, CData &oResult, char *cResult);

        /**
     * @brief SetWebMultiH323 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebMultiH323(int DecID,int VideoEncType,int AudioEncType,string DecUrl,string Contact,string SendVideoSrc,string SendAudioSrc, CData &oResult, char *cResult);

    /**
     * @brief SetWebMultiSip 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebMultiSip(int DecMode, int DecID, string DecUrl, string UserName, string UserPw, CData &oResult, char *cResult);

    /**
     * @brief SetWebMultiSipList 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebMultiSipList(int DecType,int DecMethod,int Rate,int LenBuf,string DecUrl,string Title,CData &oResult,char *cResult);
	bool SetWebStartDecNewInner(CData& oResult, char* cResult, int ArryNumbers, int DecType, int DecID, string DecUrl, char *cmdstr);

	bool SetWebStopDecNewInner(CData& oResult, char* cResult, int ArryNumbers, int DecType, int DecID);
	
	bool SetWebStartChangeInner(CData& oResult, char* cResult, char *cmdstr);

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

#endif // CSETWEBMULTICTRL_H
