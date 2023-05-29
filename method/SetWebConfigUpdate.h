#ifndef __SETWEBCONFIGUPDATE__H__
#define __SETWEBCONFIGUPDATE__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	配置升级方法类
*/

class MPSClient;
class CSetWebConfigUpdate
{
public:
	CSetWebConfigUpdate(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebConfigUpdate();
	/**
	 * @brief SetWebConfigUpdate 内部调用SetWebConfigUpdateInner实现功能
	 * @param sResult
	 * @param cfgUpdate
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebConfigUpdate(std::string& sResult, const std::string& cfgUpdate);

private:
	/**
	 * @brief SetWebConfigUpdateInner 构造、发送业务请求并获取响应
	 * @param cfgUpdate
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebConfigUpdateInner(const std::string& cfgUpdate, CData& oResult, char* cResult);
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


#endif //__SETWEBCONFIGUPDATE__H__
