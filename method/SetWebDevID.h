#ifndef __SETWEBDEVID__H__
#define __SETWEBDEVID__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	ID管理方法类
*/

class MPSClient;
class CSetWebDevID
{
public:
	CSetWebDevID(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebDevID();
	/**
	 * @brief SetWebDevID 内部调用SetWebDevIDInner实现功能
	 * @param sResult
	 * @param devId
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebDevID(std::string& sResult, string devId);

private:
	/**
	 * @brief SetWebDevIDInner 构造、发送业务请求并获取响应
	 * @param devId
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebDevIDInner(string devId, CData& oResult, char* cResult);
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


#endif //__SETWEBDEVID__H__
