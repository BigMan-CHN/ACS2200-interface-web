#ifndef CSETDECCTRL_H
#define CSETDECCTRL_H
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

class CSetDecCtrl
{
public:
    CSetDecCtrl(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief SetDecCtrlStart 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetDecCtrlStart(string &sResult,int Dectype,int Channal, int CacheTime, string sAddr, bool TcpMode);

    /**
     * @brief SetDecCtrlStop 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetDecCtrlStop(string &sResult,int Dectype,int Channal);

    InterfaceResCode SetDecCtrlAddPic(string &sResult,int Channal,string imgPath);

    InterfaceResCode SetDecCtrlClrPic(string &sResult,int Channal);

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
    bool SetDecStart(int Dectype,int Channal, int CacheTime, string sAddr, bool TcpMode, CData &oResult, char *cResult);

    /**
     * @brief SetDecStop 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetDecStop(int Dectype, int Channal, CData &oResult, char *cResult);

    bool SetDecAddPic(int Channal, string imgPath, CData &oResult, char *cResult);

    bool SetDecClrPic(int Channal, CData &oResult, char *cResult);

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

#endif // CSETDECCTRL_H
