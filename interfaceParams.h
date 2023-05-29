/********************************************************
* InterfafceParams.h
* 解析接口参数
*
* Copyright (c) 2019, Tyvis
* All rights reserved.
*
* @version		V1.00
* @author		es
* @date			2019-5-17
 *******************************************************/
#ifndef INTERFACEPARAMS_H
#define INTERFACEPARAMS_H
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

#define CHANNEL_NUM_ERROR       9000 /* 获取通道号参数 错误 */

class InterfaceParams
{
public:
    InterfaceParams();
    /**
     * @brief getMethod 获取method值
     * @param sMethod
     * @return true：成功，false：失败.
     */
    bool getMethod(string &sMethod);

    /**
     * @brief getChannelNum 获取ChannelNum值
     * @param chNum
     * @return true：成功，false：失败.
     */
    bool getChannelNum(int &chNum);

    /**
     * @brief analysisParams 分析data获取接口参数
     * @param json data
     * @return true：成功，false：失败.
     */
    bool analysisParams(char * json);
    /**
     * @brief analysisParams 分析data获取接口参数
     * @param sJsonData data
     * @return true：成功，false：失败.
     */
    bool analysisParams(string sJsonData);

	/**
	  * @brief getParamsNodeValueofInt 获取params节点下指定node的值 int型
	  * @param sNode 节点名称
	  * @param iValue node的值 (INT_MIN,INTMAX]
	  * @return true：成功，false：失败.
	  */
	 bool getParamsNodeValueofInt(string sNode, int &iValue);
	 /**
	  * @brief getParamsNodeValueofStr 获取params节点下指定node的值 string型
	  * @param sNode 节点名称
	  * @param sValue node的值
	  * @return true：成功，false：失败.
	  */
	 bool getParamsNodeValueofStr(string sNode, string &sValue);
	 /**
	  * @brief getParamsNodeValueofDbl 获取params节点下指定node的值 double型
	  * @param sNode 节点名称
	  * @param dValue node的值
	  * @return true：成功，false：失败.
	  */
	 bool getParamsNodeValueofDbl(string sNode,double &dValue);
	
	 /**
	  * @brief getParamsNodeValueofBool 获取params节点下指定node的值 bool型
	  * @param sNode 节点名称
	  * @param bValue node的值
	  * @return true：成功，false：失败.
	  */
	 bool getParamsNodeValueofBool(string sNode,bool &bValue);
	/**
	  * @brief getParamsNodeValueofArry 获取params节点下JSON数组
	  * @param sNode 节点名称
	  * @param iValue node的值 (INT_MIN,INTMAX]
	  * @param ArryNumbers Json数组成员个数
	  * @param ArryNode Json指针数组
	  * @return true：成功，false：失败.
	  */
	 bool getParamsNodeValueofArry(string sNode, int &ArryNumbers, cJSON *ArryNode[]); 
	/**
	  * @brief getParamsNodeValueofInt 获取JsonNode节点下指定node的值 int型
	  * @param sNode 节点名称
	  * @param iValue node的值 (INT_MIN,INTMAX]
	  * @return true：成功，false：失败.
	  */
	 bool getNodeValueofInt(cJSON *JsonNode, string sNode, int &iValue);
	 /**
	  * @brief getParamsNodeValueofStr 获取JsonNode节点下指定node的值 string型
	  * @param sNode 节点名称
	  * @param sValue node的值
	  * @return true：成功，false：失败.
	  */
	 bool getNodeValueofStr(cJSON *JsonNode, string sNode, string &iValue);
	 /**
	  * @brief getParamsNodeValueofDbl 获取JsonNode节点下指定node的值 double型
	  * @param sNode 节点名称
	  * @param dValue node的值
	  * @return true：成功，false：失败.
	  */
	 bool getNodeValueofDbl(cJSON *JsonNode, string sNode, double &iValue);
	
	 /**
	  * @brief getParamsNodeValueofBool 获取JsonNode节点下指定node的值 bool型
	  * @param sNode 节点名称
	  * @param bValue node的值
	  * @return true：成功，false：失败.
	  */
	 bool getNodeValueofBool(cJSON *JsonNode, string sNode, bool &iValue);

    void clearAll();
private:
    /**
     * @brief getMethodfromRoot 从data根节点root中获取method参数
     * @return true：成功，false：失败.
     */
    bool getMethodfromRoot();
    /**
     * @brief getParamsfromRoot 从data根节点root中获取param参数
     * @return true：成功，false：失败.
     */
    bool getParamsfromRoot();
    /**
     * @brief getChannelNumfromRoot 从data根节点root中获取channelNum参数
     * @return true：成功，false：失败.
     */
    bool getChannelNumfromRoot();
private:
    string _sJsonData; /* 待分析的data */
    cJSON *_root; /* json格式的data */
    cJSON *_params; /* root中的param节点*/
    string _method; /* root中的method节点信息*/
    int _channelNum; /* root中的channelNum节点信息*/

//测试功能
public:
    /**
     * @brief getParamseNodeofTestDebug 获取节点TEST_DEBUG值
     * @param sVal
     * @return true：成功，false：失败.
     */
    bool getParamseNodeofTestDebug(string &sVal);
};

#endif // INTERFACEPARAMS_H
