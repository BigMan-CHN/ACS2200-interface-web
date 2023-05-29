#ifndef __SETWEBLOGFILECONF__H__
#define __SETWEBLOGFILECONF__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	日志设置方法类
*/

class MPSClient;
class CSetWebLogFileConf
{
public:
	CSetWebLogFileConf(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebLogFileConf();

	/**
	 * @brief SetWebLogFileConf 内部调用SetWebLogFileConfInner实现功能
	 * @param sResult
	 * @param logSize
	 * @param logSaveTime
	 * @param logLevel
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebLogFileConf(std::string& sResult,
		int logSize,
		int logSaveTime,
		int logLevel);

private:

	/**
	 * @brief SetWebLogFileConfInner 构造、发送业务请求并获取响应
	 * @param logSize
	 * @param logSaveTime
	 * @param logLevel
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebLogFileConfInner(int logSize,
		int logSaveTime,
		int logLevel,
		CData& oResult,
		char* cResult);
	
	bool ComposeResult(cJSON* result);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();

	MPSClient* _mpsClient;
	CAnalyzeParaProc _analyzePara;
	cJSON* _result;
};


#endif //__SETWEBVIDEOPARAM__H__
