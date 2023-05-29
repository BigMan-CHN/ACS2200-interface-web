/********************************************************
* cgiAnalysis.h
* 分析获取请求数据
*
* Copyright (c) 2019, Tyvis
* All rights reserved.
*
* @version		V1.00
* @author		es
* @date			2019-5-17
 *******************************************************/
#ifndef CGIANALYSIS_H
#define CGIANALYSIS_H
/********************************************************
 * public c/c++ standard headers
 *******************************************************/
#include <stdio.h>
#include <iostream>
#include <string.h>

/********************************************************
 * CGI headers
 *******************************************************/
//#include <cgi.h>

/********************************************************
 * boost usart headers
 *******************************************************/
#include <boost/asio.hpp>
#include <boost/bind.hpp>

/********************************************************
 * lib headers
 *******************************************************/
#include <log.h>


#define MAX_BUFFER_SIZE     8192    /* post 数据最大长度 */

class CgiAnalysis
{
    typedef enum RequestMethod{
        eRequestGet,
        eRequestPost
    }RequestMethod;

public:
    CgiAnalysis();

public:
    /**
     * @brief GetCgiData 获取cgi请求数据
     * @param fp 数据位置
     * @param requestMethod 请求类型 GET POST
     * @return 返回数据指针
     */
    char* GetCgiData();
    char* GetCgiData(FILE* fp, char* requestMethod);
    char* GetCgiData(FILE* fp, RequestMethod method);
    /**
     * @brief clearCgiData 释放申请的内存
     */
    void clearCgiData();

    /**
     * @brief getClientIp 获取客户端ip
     * @return ip
     */
    string getClientIp();
private:
    int _maxBufferSize; /* post 数据最大长度 */
    char* _inputBuffer; /* 接收数据存储指针 */
    string _clientIp;
};

#endif // CGIANALYSIS_H
