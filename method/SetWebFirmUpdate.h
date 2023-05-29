#ifndef __SETWEBFIRMUPDATE__H__
#define __SETWEBFIRMUPDATE__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	固件升级方法类
*/

class MPSClient;
class CSetWebFirmUpdate
{
public:
	CSetWebFirmUpdate(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebFirmUpdate();

	/**
	 * @brief SetWebFirmUpdate 内部调用SetWebFirmUpdateInner实现功能
	 * @param sResult
	 * @param firmUpdate
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebFirmUpdate(std::string& sResult, const std::string& firmUpdate);

private:
	/**
	 * @brief SetWebFirmUpdateInner 构造、发送业务请求并获取响应
	 * @param firmUpdate
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebFirmUpdateInner(const std::string& firmUpdate, CData& oResult, char* cResult);
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


#endif //__SETWEBFIRMUPDATE__H__
