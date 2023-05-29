/********************************************************
* InterfafceResponse.h
* 生成json返回信息
*
* Copyright (c) 2019, Tyvis
* All rights reserved.
*
* @version		V1.00
* @author		es
* @date			2019-5-17
 *******************************************************/
#ifndef INTERFACERESPONSE_H
#define INTERFACERESPONSE_H

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
#include <cJSON.h>

/********************************************************
 * headers
 *******************************************************/
#include "interfaceDefines.h"

class interfaceResponse
{
public:
    /**
     * @brief interfaceResponse
     * @param flag true：成功，false：失败.
     * @param error
     * @param results
     */
    interfaceResponse(bool flag = true, string error="", InterfaceResCode code = eInterfaceResCodeSuccess, cJSON *results=NULL);
    /**
     * @brief responsePrint 转化json型返回信息为json string型
     * @param format true:带格式转化,false:不带格式转化
     * @return 转化后的字符串
     */
    string responsePrint(bool format = false);

    /**
     * @brief simpleResponsePrintofSuccess 快速生成成功返回信息
     * @param format true:带格式转化,false:不带格式转化
     * @return
     */
    string simpleResponsePrintofSuccess(bool format = false);

    /**
     * @brief simpleResponsePrintofSuccess 快速生成失败返回信息
     * @param format true:带格式转化,false:不带格式转化
     * @return
     */
    string simpleResponsePrintofFalied(string error,InterfaceResCode code = eInterfaceResCodeSuccess, bool format = false);

public:
    /**
     * @brief setHostStatusResResults 设置hostStatus返回信息
     * @param state channelStatus节点值
     */
    void setHostStatusResResults(CHStatus state);

public:
    /**
     * @brief setResponseSuccessFlag 设置返回信息为成功
     */
    void setResponseSuccessFlag();

    /**
     * @brief setResponseFaliedFlag 设置返回信息为失败
     */
    void setResponseFaliedFlag();

    /**
     * @brief setResponseError 设置返回信息中error节点错误信息
     * @param error错误信息
     */
    void setResponseError(string error);
    /**
    * @brief setResponseCode 设置返回信息中code节点错误信息
    * @param code错误信息
    */
   void setResponseCode(InterfaceResCode code);

    /**
     * @brief setResponseResults 设置返回信息
     * @param sResults json字符串
     * @return
     */
    bool setResponseResults(string sResults);
    void setResponseResults(cJSON * results);

    /**
     * @brief addResponseResultsNode 添加返回信息到results节点
     * @param name 节点名称
     * @param sValue 节点值
     * @return true：成功，false：失败.
     */
    bool addResponseResultsNode(string name,string sVal);
    bool addResponseResultsNode(string name, int iVal);
    bool addResponseResultsNode(string name,double dVal);
    bool addResponseResultsNode(string name,bool bVal);

    /**
     * @brief clearResponse 清除返回信息内容
     */
    void clearResponse();

    /**
     * @brief clearAllMsg 清除所有信息
     */
    void clearAllMsg();
private:
    /**
     * @brief responseAddFlag 添加flag节点到root根节点
     */
    void responseAddFlag();
    /**
     * @brief responseAddFlag 添加error节点到root根节点
     */
    void responseAddError();
    /**
     * @brief responseAddCode 添加code节点到root根节点
     */
    void responseAddCode();
    /**
     * @brief responseAddFlag 添加results节点到root根节点
     */
    void responseAddResults();

private:
    cJSON *_root; /* 生成的json返回信息根节点 */
    bool _flag; /* flag节点 */
    string _error; /* error节点 */
    InterfaceResCode _code;  /* code节点 */
    cJSON *_results; /* results节点 */
};

#endif // INTERFACERESPONSE_H
