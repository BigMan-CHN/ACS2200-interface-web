#ifndef __GETWEBRTMPPARAM__H__
#define __GETWEBRTMPPARAM__H__

#include <TransmitClient.h>
#include <LibXML.h>
#include <cJSON.h>

#include "interfaceDefines.h"
#include "CAnalyzeParaProc.h"

/*
	获取RTMP参数方法类
*/

class MPSClient;
class CGetWebRtmpParam
{
public:
	CGetWebRtmpParam(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CGetWebRtmpParam();
	/**
	 * @brief GetWebRtmpParam 内部调用GetWebRtmpParamInner实现功能
	 * @param sResult
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode GetWebRtmpParam(string& sResult);

private:
	/**
	 * @brief GetWebRtmpParamInner 构造、发送业务请求并获取响应
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool GetWebRtmpParamInner(CData& oResult, char* cResult);
	bool ComposeResult(cJSON* result);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();

	MPSClient*			_mpsClient;
	cJSON*				_result;
	cJSON*				_ParamList;
	CAnalyzeParaProc	_analyPara;
};


#endif //__GETWEBRTMPPARAM__H__
