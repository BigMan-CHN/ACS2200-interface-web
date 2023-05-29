#ifndef __SETWEBRTSPPARAM__H__
#define __SETWEBRTSPPARAM__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	设置RTSP参数方法类
*/

class MPSClient;
class CSetWebRtspParam
{
public:
	CSetWebRtspParam(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebRtspParam();

	/**
	 * @brief SetWebRtspParam 内部调用SetWebRtspParamInner实现功能
	 * @param sResult
	 * @param soundNum
	 * @param rtspSource
	 * @param rtspUrl
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebRtspParam(std::string& sResult,
		int RtspNum,
		const std::string& rtspSource,
		const std::string& rtspUrl);

private:
	/**
	 * @brief SetWebRtspParamInner 构造、发送业务请求并获取响应
	 * @param soundNum
	 * @param rtspSource
	 * @param rtspUrl
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebRtspParamInner(int RtspNum, 
		const std::string& rtspSource, 
		const std::string& rtspUrl,
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


#endif //__SETWEBRTSPPARAM__H__
