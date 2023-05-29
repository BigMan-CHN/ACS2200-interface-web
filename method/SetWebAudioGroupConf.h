#ifndef __SETWEBAUDIOGROUPCONF__H__
#define __SETWEBAUDIOGROUPCONF__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

class MPSClient;
class CSetWebAudioGroupConf
{
public:
	CSetWebAudioGroupConf(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebAudioGroupConf();
	/**
	 * @brief SetWebAudioGroupConf 内部调用SetWebAudioGroupConfInner实现功能
	 * @param sResult
	 * @param groupName
	 * @param chnNum
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebAudioGroupConf(std::string& sResult,
		int groupName,
		const std::string& chnNum);

private:
	/**
	 * @brief SetWebAudioGroupConfInner 构造、发送业务请求并获取响应
	 * @param groupName
	 * @param chnNum
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebAudioGroupConfInner(int groupName,
			const std::string& chnNum,
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


#endif //__SETWEBAUDIOGROUPCONF__H__
