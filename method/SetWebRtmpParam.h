#ifndef __SETWEBRTMPPARAM__H__
#define __SETWEBRTMPPARAM__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	设置RTMP参数方法类
*/

class MPSClient;
class CSetWebRtmpParam
{
public:
	CSetWebRtmpParam(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebRtmpParam();

	/**
	 * @brief SetWebRtmpParam 内部调用SetWebRtmpParamInner实现功能
	 * @param sResult
	 * @param num
	 * @param rtmpName
	 * @param isHttp
	 * @param isHls
	 * @param isRtmp
	 * @param isRtsp
	 * @param isBroadcast
	 * @param broadcastAddr
	 * @param isPushStream
	 * @param pushAddr
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebRtmpParam(std::string& sResult,
		int num,
		const std::string& rtmpName,
		int isHttp,
		int isHls,
		int isRtmp,
		int isRtsp,
		int isBroadcast,
		const std::string& broadcastAddr,
		int isPushStream,
		const std::string& pushAddr);

private:
	/**
	 * @brief SetWebRtmpParamInner 构造、发送业务请求并获取响应
	 * @param num
	 * @param rtmpName
	 * @param isHttp
	 * @param isHls
	 * @param isRtmp
	 * @param isRtsp
	 * @param isBroadcast
	 * @param broadcastAddr
	 * @param isPushStream
	 * @param pushAddr
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebRtmpParamInner(int num,
		const std::string& rtmpName,
		int isHttp,
		int isHls,
		int isRtmp,
		int isRtsp, 
		int isBroadcast,
		const std::string& broadcastAddr,
		int isPushStream, 
		const std::string& pushAddr,
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


#endif //__SETWEBRTMPPARAM__H__
