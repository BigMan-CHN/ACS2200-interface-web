#ifndef __SETWEBAUDIOPARAM__H__
#define __SETWEBAUDIOPARAM__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	设置音频参数方法类
*/

class MPSClient;
class CSetWebAudioParam
{
public:
	CSetWebAudioParam(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebAudioParam();
	/**
	 * @brief SetWebAudioParam 内部调用SetWebAudioParamInner实现功能
	 * @param sResult
	 * @param soundNum
	 * @param soundSource
	 * @param codeMode
	 * @param gain
	 * @param sampleRate
	 * @param soundChn
	 * @param rateSet
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebAudioParam(std::string& sResult,
		int soundNum,
		const std::string& soundSource,
		int codeMode,
		int sampleRate,
		int rateSet);

private:
	/**
	 * @brief SetWebAudioParamInner 构造、发送业务请求并获取响应
	 * @param soundNum
	 * @param soundSource
	 * @param codeMode
	 * @param gain
	 * @param sampleRate
	 * @param soundChn
	 * @param rateSet
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebAudioParamInner(int soundNum,
		const std::string& soundSource,
		int codeMode,
		int sampleRate,
		int rateSet,
		CData& oResult,
		char* cResult);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	bool ComposeResult(cJSON* result);
	std::string ComposeResult();

	MPSClient* _mpsClient;
	CAnalyzeParaProc _analyzePara;
	cJSON* _result;
};


#endif //__SETWEBAUDIOPARAM__H__
