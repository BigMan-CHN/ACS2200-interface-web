#ifndef __SETWEBZKTDOWNLOAD__H__
#define __SETWEBZKTDOWNLOAD__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	中控下载方法类
*/

class MPSClient;
class CSetWebZKTDownload
{
public:
	CSetWebZKTDownload(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebZKTDownload();

	/**
	 * @brief SetWebZKTDownload 内部调用SetWebZKTDownloadInner实现功能
	 * @param sResult
	 * @param zKTUpdate
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebZKTDownload(std::string& sResult, const std::string& zktUpdate);

private:
	/**
	 * @brief SetWebZKTDownloadInner 构造、发送业务请求并获取响应
	 * @param zKTUpdate
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebZKTDownloadInner(const std::string& zktUpdate, CData& oResult, char* cResult);
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


#endif //__SETWEBZKTDOWNLOAD__H__
