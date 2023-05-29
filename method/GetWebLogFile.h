#ifndef __GETWEBLOGFILE__H__
#define __GETWEBLOGFILE__H__

#include <TransmitClient.h>
#include <LibXML.h>
#include <cJSON.h>

#include "interfaceDefines.h"
#include "CAnalyzeParaProc.h"

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
using namespace std;
/*
	获取日志方法类
*/

//class MPSClient;
class CGetWebLogFile
{
public:
	CGetWebLogFile(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CGetWebLogFile();
	/**
	 * @brief GetWebLogFile 内部调用GetWebLogFileInner实现功能
	 * @param sResult
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode GetWebLogFile(string& sResult);

private:
	/**
	 * @brief GetWebLogFileInner 构造、发送业务请求并获取响应
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool GetWebLogFileInner(CData& oResult, char* cResult);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	bool ComposeResult(cJSON* result);
	std::string ComposeResult();
    void getFiles(std::string path, std::vector<std::string> &files, std::vector<std::string> &filesPath);

	

	MPSClient*			_mpsClient;
	cJSON*				_result;
	cJSON*				_logList;
	CAnalyzeParaProc	_analyPara;
};


#endif //__GETWEBLOGFILE__H__
