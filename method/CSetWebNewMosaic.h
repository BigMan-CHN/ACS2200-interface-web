#ifndef SetWebNewMosaic_H
#define SetWebNewMosaic_H
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

class CSetWebNewMosaic
{
public:
    CSetWebNewMosaic(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief GetHostDecInfo 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebMosaic(string &sResult,string MosaicChn,int MosaicDisplay,int MosaicNum,int MosaicType, string MosaicWinX, string MosaicWinY, string MosaicWinH, string MosaicWinW);
	InterfaceResCode SetWebNewMosaic(string &sResult, int ArryNumbers, string MosaicChn[],int MosaicDisplay[],int MosaicNum[],int MosaicType[], string MosaicWinX[], string MosaicWinY[], string MosaicWinH[], string MosaicWinW[]);

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

	bool SetWebMosaicParamInner(CData& oResult, char* cResult, string MosaicChn,int MosaicDisplay,int MosaicNum,int MosaicType, string MosaicWinX, string MosaicWinY, string MosaicWinH, string MosaicWinW);
	bool SetWebNewMosaicParamInner(CData& oResult, char* cResult, int ArryNumbers,string MosaicChn[],int MosaicDisplay[],int MosaicNum[],int MosaicType[], string MosaicWinX[], string MosaicWinY[], string MosaicWinH[], string MosaicWinW[]);

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
