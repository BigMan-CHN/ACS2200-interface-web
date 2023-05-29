#ifndef __SETWEBRECOVERCFGFILE__H__
#define __SETWEBRECOVERCFGFILE__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	恢复文件方法类
*/

class MPSClient;
class CSetWebRecoverCFGFile
{
public:
	CSetWebRecoverCFGFile(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebRecoverCFGFile();

	/**
	 * @brief SetWebRecoverCFGFile 内部调用SetWebRecoverCFGFileInner实现功能
	 * @param sResult
	 * @param recoverCFG
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebRecoverCFGFile(std::string& sResult, const std::string& recoverCFG);

private:
	/**
	 * @brief SetWebRecoverCFGFileInner 构造、发送业务请求并获取响应
	 * @param recoverCFG
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebRecoverCFGFileInner(const std::string& recoverCFG, CData& oResult, char* cResult);
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


#endif //__SETWEBRECOVERCFGFILE__H__
