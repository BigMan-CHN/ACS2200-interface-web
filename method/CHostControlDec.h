#ifndef CHOSTCONTROLDEC_H
#define CHOSTCONTROLDEC_H
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


// Http media decode work state 解码状态
typedef enum HttpMediaDecodeStatus{
    eHttpDecodeStatusStop = 0, //停止解码
    eHttpDecodeStatusStart	= 1, //正在解码
    eHttpDecodeStatusSize
}HttpMediaDecodeStatus;


static string getHttpMediaDecodeStatusStr(HttpMediaDecodeStatus & s){
    switch (s){
        case eHttpDecodeStatusStop:return "空闲,未解码";
        case eHttpDecodeStatusStart:return "正在解码";
        default:return "无此状态";
    }
}

static HttpMediaDecodeStatus CovertMediaDecodeStatus(MPSMediaDecodeStatus &s){
    switch (s){
        case eMPSDecodeStatusStop:return eHttpDecodeStatusStop;
        case eMPSDecodeStatusStart:return eHttpDecodeStatusStart;
        default:return eHttpDecodeStatusSize;//"无此状态";
    }
}
class CHostControlDec
{
public:
    CHostControlDec(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);
    /**
     * @brief StartDec 控制设备进行解码
     * @param chNum 解码通道号，1-4
     * @param url 解码地址
     * @param cacheTime 缓冲时间，单位毫秒
     * @param tcpMode 是否启用tcp模式
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode StartDec(int chNum, string url,int cacheTime,bool tcpMode);
    /**
     * @brief StartDec 控制设备停止解码
     * @param chNum
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode StopDec(int chNum);

    /**
     * @brief GetDecStatus
     * @param chNum
     * @param sResult
     * @return
     */
    InterfaceResCode GetDecStatus(int chNum,string &sResult);

private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;

    cJSON *_result;
};

#endif // CHOSTCONTROLDEC_H
