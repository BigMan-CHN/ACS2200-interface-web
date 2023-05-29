#ifndef CWebAudioMute_H
#define CWebAudioMute_H
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
#include "stu_sqlite.h"
#include "sqlite3.h"

class CWebAudioMute
{
public:
    CWebAudioMute(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief GetHostDecInfo 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebAudioMute(string &sResult,int MuteVal,int CtrlType, int ChnNam);
	
	InterfaceResCode GetWebAudioMute(string &sResult,int CtrlType,int ChnNam);

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
	
    bool SetWebAudioMuteInner(CData& oResult, char* cResult,int CtrlType, int MuteVal, int ChnNam);
	
	bool GetWebAudioMuteInner(CData& oResult, char* cResult, int CtrlType, int ChnNam);

private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;
    CAnalyzeParaProc _AnalyParas;

    //用于存储结果
    cJSON *_result;
    cJSON *_total;
    cJSON *_addressList;
};

#endif // CGETHOSTCHNINFO_H
