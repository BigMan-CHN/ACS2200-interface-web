#ifndef __SetWebInfraredFileDel_H__
#define __SetWebInfraredFileDel_H__

#include <TransmitClient.h>
#include <LibXML.h>
#include <cJSON.h>
#include <stdio.h>

#include "interfaceDefines.h"
#include "CAnalyzeParaProc.h"
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <string>
#include <iostream>
#include <stdlib.h>

/*
	获取红外学习命令集方法类
*/

class MPSClient;
class CSetWebInfraredFileDel
{
public:
	CSetWebInfraredFileDel(const std::string& ip = MPS_IP, unsigned short port =MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebInfraredFileDel();
	/**
	 * @brief GetWebInfraredStudyList 内部调用GetWebInfraredStudyListInner实现功能
	 * @param sResult
	 * @param fileName
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebInfraredFileDel(string& sResult,string FileName);


private:
	/**
	 * @brief GetWebInfraredStudyListInner 构造、发送业务请求并获取响应
	 * @param fileName
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebInfraredFileDelInner(CData& oResult,char* cResult, string FileName);

	bool ComposeResult(cJSON* result);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();

	MPSClient* _mpsClient;
	cJSON* _result;
	cJSON* _ParamList;
	CAnalyzeParaProc	_analyPara;
};


#endif //__GETWEBINFRAREDSTUDYLIST_H__
