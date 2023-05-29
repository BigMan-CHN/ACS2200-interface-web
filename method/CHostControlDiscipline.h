#ifndef CHOSTCONTROLDISCIPLINE_H
#define CHOSTCONTROLDISCIPLINE_H
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

// Http media Discipline work state 播放法庭纪律状态
typedef enum HttpMediaDisciplineStatus{
    eHttpDisciplineStatusStop = 0, //停止播放法庭纪律
    eHttpDisciplineStatusStart = 1, //正在播放法庭纪律
    eHttpDisciplinetatusSize
}HttpMediaDisciplineStatus;

static string getHttpMediaDisciplineStatusStr(HttpMediaDisciplineStatus & s){
    switch (s){
        case eHttpDisciplineStatusStop:return "空闲,未播放法庭纪律";
        case eHttpDisciplineStatusStart:return "正在播放法庭纪律";
        default:return "无此状态";
    }
}

static HttpMediaDisciplineStatus CovertMediaDisciplineStatus(MPSMediaDisciplineStatus &s){
    switch (s){
        case eMPSDisciplineStatusStop:return eHttpDisciplineStatusStop;
        case eMPSDisciplineStatusStart:return eHttpDisciplineStatusStart;
        default:return eHttpDisciplinetatusSize;//"无此状态";
    }
}
class CHostControlDiscipline
{
public:
    CHostControlDiscipline(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);
    /**
     * @brief StartDisciplie 开始播放法庭纪律
     * @return
     */
    InterfaceResCode StartDisciplie();
    /**
     * @brief StopDisciplie 停止播放法庭纪律
     * @return
     */
    InterfaceResCode StopDisciplie();
    /**
     * @brief GetDisciplieStatus 获取播放法庭纪律状态
     * @param sResult
     * @return
     */
    InterfaceResCode GetDisciplieStatus(string &sResult);
private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;

    cJSON *_result;
};

#endif // CHOSTCONTROLDISCIPLINE_H
