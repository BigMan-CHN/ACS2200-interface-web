#ifndef __GetWebInfraredFileInf_H__
#define __GetWebInfraredFileInf_H__

#include <TransmitClient.h>
#include <LibXML.h>
#include <cJSON.h>
#include <stdio.h>

#include "interfaceDefines.h"
#include "CAnalyzeParaProc.h"
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <ctime>   
#include <sys/types.h> 
#include <sys/stat.h>
#include <dirent.h>
#include <cstddef>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>


/*
	获取红外学习命令集方法类
*/

class MPSClient;
class CGetWebInfraredFileInf
{
public:
	CGetWebInfraredFileInf(const std::string& ip = MPS_IP, unsigned short port =MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CGetWebInfraredFileInf();
	/**
	 * @brief GetWebInfraredStudyList 内部调用GetWebInfraredStudyListInner实现功能
	 * @param sResult
	 * @param fileName
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode GetWebInfraredFileInf(string& sResult);


private:
	/**
	 * @brief GetWebInfraredStudyListInner 构造、发送业务请求并获取响应
	 * @param fileName
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool GetWebInfraredFileInfListInner(CData& oResult,char* cResult);

	bool ComposeResult(cJSON* result);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();
	xmlXPathObjectPtr get_nodeset(xmlDocPtr doc, const xmlChar *szXpath);
	void getFiles(std::string path, std::vector<std::string> &files, std::vector<std::string> &filesPath);

	MPSClient* _mpsClient;
	cJSON* _result;
	cJSON* _ParamList;
	CAnalyzeParaProc	_analyPara;
};


#endif //__GETWEBINFRAREDSTUDYLIST_H__
