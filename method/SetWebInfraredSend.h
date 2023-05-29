#ifndef __SETWEBINFRAREDSEND__H__
#define __SETWEBINFRAREDSEND__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	红外命令集发送方法类
*/

class MPSClient;
class CSetWebInfraredSend
{
public:
	CSetWebInfraredSend(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebInfraredSend();

	/**
	 * @brief SetWebInfraredSend 内部调用SetWebInfraredSendInner实现功能
	 * @param sResult
	 * @param port
	 * @param fileName
	 * @param num
	 * @param order
	 * @param describe
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebInfraredSend(std::string& sResult,
		int port,
		const std::string& fileName,
		int num,
		const std::string& order,
		const std::string& describe);

private:
	/**
	 * @brief SetWebInfraredSendInner 构造、发送业务请求并获取响应
	 * @param port
	 * @param fileName
	 * @param num
	 * @param order
	 * @param describe
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebInfraredSendInner(int port,
		const std::string& fileName,
		int num,
		const std::string& order,
		const std::string& describe,
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


#endif //__SETWEBINFRAREDSEND__H__
