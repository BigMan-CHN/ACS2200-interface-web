#ifndef __GETWEBAUDIOCONFIG__H__
#define __GETWEBAUDIOCONFIG__H__

#include <TransmitClient.h>
#include <LibXML.h>
#include <cJSON.h>

#include "interfaceDefines.h"
#include "CAnalyzeParaProc.h"
#include "stu_sqlite.h"
#include "sqlite3.h"

/*
	获取音频参数方法类
*/

class MPSClient;
class CGetWebAudioConf
{
public:
	CGetWebAudioConf(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CGetWebAudioConf();
	/**
	 * @brief GetWebAudioConf 内部调用GetWebAudioConfInner实现功能
	 * @param sResult
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode GetWebAudioConf(string& sResult);

private:
	/**
	 * @brief GetWebAudioConfInner 构造、发送业务请求并获取响应
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool GetWebAudioConfInner(CData& oResult, char* cResult,int cmd);
	bool GetWebAudioMotivationTimeInner(CData& oResult, char* cResult, int ChnNum);
	bool ComposeResult(cJSON* result);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();

	MPSClient*			_mpsClient;
	cJSON*				_result;
	cJSON*				_audioParamList;
	CAnalyzeParaProc	_analyPara;
};


#endif //__GETWEBAUDIOCONFIG__H__
