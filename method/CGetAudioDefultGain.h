#ifndef GetAudioDefultGain_H
#define GetAudioDefultGain_H
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

class CGetAudioDefultGain
{
public:
    CGetAudioDefultGain(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief GetHostDecInfo 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode GetAudioDefultGain(string &sResult);
	
	InterfaceResCode SetWebAudioSQLCreatAndUpdate(string &sResult);

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
	
    bool GetAudioDefultGainInner(CData& oResult, char* cResult, int ChnNum, int CtrlType);
	
	bool GetWebAudioDefultMuteInner(CData& oResult, char* cResult, int CtrlType,int ChnNum);
	
	bool SetWebAudioSQLCreatAndUpdateInner(CData& oResult, char* cResult);
	
	bool GetWebAudioAllMuteInner(CData& oResult, char* cResult, int CtrlType,int ChnNum);

	bool GetAudioAllGainInner(CData& oResult, char* cResult, int ChnNum, int CtrlType);

	bool GetWebAudioConfInner(CData& oResult, char* cResult, int cmd);

	bool GetWebMICWhineButtonInner(CData& oResult, char* cResult, int ChnNam);

	bool GetWebMICWhineGainInner(CData& oResult, char* cResult, int ChnNam);
	
	bool GetWebAudioMuteInner(CData& oResult, char* cResult, int CtrlType,int ChnNam);

	bool GetWebAudioGainInner(CData& oResult, char* cResult, int ChnNum, int CtrlType);

	bool GetWebAudioSPLInner( CData& oResult, char* cResult, int ChnNum);

	bool GetWebAudioPowerInner( CData& oResult, char* cResult, int ChnNum);
	
	bool GetWebAudioMotivationValInner( CData& oResult, char* cResult, int ChnNum);

	bool GetWebAudioMotivationTimeInner( CData& oResult, char* cResult, int ChnNum);

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
