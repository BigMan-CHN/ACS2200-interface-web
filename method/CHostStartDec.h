#ifndef CHOSTSTARTDEC_H
#define CHOSTSTARTDEC_H
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

class CHostStartDec
{
public:
    CHostStartDec(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);


    /**
     * @brief StartDec 控制设备进行解码
     * @param chNum
     * @param url
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode StartDec(int chNum, string url);

private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;
};

#endif // CHOSTSTARTDEC_H
