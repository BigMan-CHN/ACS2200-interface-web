#ifndef __SETWEBRTMPBROADCONF__H__
#define __SETWEBRTMPBROADCONF__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	RTMP直播设置方法类
*/

class MPSClient;
class CSetWebRtmpBroadConf
{
public:
	CSetWebRtmpBroadConf(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebRtmpBroadConf();

	/**
	 * @brief SetWebRtmpBroadConf 内部调用SetWebRtmpBroadConfInner实现功能
	 * @param sResult
	 * @param isEnabled
	 * @param broadAddr
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebRtmpBroadConf(std::string& oResult, int isEnabled, const std::string& broadAddr);
	InterfaceResCode GetWebRtmpBroadConf(std::string& oResult);


private:

	/**
	 * @brief SetWebRtmpBroadConfInner 构造、发送业务请求并获取响应
	 * @param isEnabled
	 * @param broadAddr
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebRtmpBroadConfInner(int isEnabled, const std::string& broadAddr, CData& oResult, char* cResult);
	bool GetWebRtmpBroadConfInner(CData& oResult,char* cResult);


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


#endif //__SETWEBRTMPBROADCONF__H__
