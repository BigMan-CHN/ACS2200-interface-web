#ifndef __SETWEBZKTUPDATE__H__
#define __SETWEBZKTUPDATE__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"
/*
	中控升级方法类
*/
class MPSClient;
class CSetWebZKTUpdate
{
public:
	CSetWebZKTUpdate(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebZKTUpdate();

	/**
	 * @brief SetWebZKTUpdate 内部调用SetWebZKTUpdateInner实现功能
	 * @param sResult
	 * @param zKTUpdate
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebZKTUpdate(std::string& sResult, const std::string& zKTUpdate);

private:
	/**
	 * @brief SetWebZKTUpdateInner 构造、发送业务请求并获取响应
	 * @param zKTUpdate
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebZKTUpdateInner(const std::string& zKTUpdate, CData& oResult, char* cResult);

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


#endif //__SETWEBZKTUPDATE__H__
