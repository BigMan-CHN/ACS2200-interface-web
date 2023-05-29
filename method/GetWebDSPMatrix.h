#ifndef __GetWebDSPMatrix__H__
#define __GetWebDSPMatrix__H__

#include <string.h>
#include <iostream>
#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include<stdio.h>
#include<stdlib.h>

/*
	设置红外命令集学习方法类
*/


class CGetWebDSPMatrix
{
public:
	CGetWebDSPMatrix(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CGetWebDSPMatrix();
	/**
	 * @brief SetWebInfraredStudy 内部调用SetWebInfraredStudyInner实现功能
	 * @param sResult
	 * @param port
	 * @param fileName
	 * @param num
	 * @param order
	 * @param describe
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode GetWebDSPMatrix(std::string& sResult);

private:
	/**
	 * @brief SetWebInfraredStudyInner 构造、发送业务请求并获取响应
	 * @param port
	 * @param fileName
	 * @param num
	 * @param order
	 * @param describe
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool GetWebDSPMatrixInner(CData& oResult, char* cResult, int OutputType);

	bool ComposeResult(cJSON* result);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();
	//xmlXPathObjectPtr get_nodeset(xmlDocPtr doc, const xmlChar *szXpath); 

	
	MPSClient* _mpsClient;
	CAnalyzeParaProc _analyzePara;
	cJSON* _addressList;
	cJSON* _result;
};


#endif //__SETWEBINFRAREDSTUDY__H__
