#ifndef __GETWEBRTSPPARAM__H__
#define __GETWEBRTSPPARAM__H__

#include <TransmitClient.h>
#include <LibXML.h>
#include <cJSON.h>

#include "interfaceDefines.h"
#include "CAnalyzeParaProc.h"

/*
	获取RTSP参数方法类
*/

class MPSClient;
class CGetWebRtspParam
{
public:
	CGetWebRtspParam(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CGetWebRtspParam();
	/**
	 * @brief GetWebRtspParam 内部调用GetWebRtspParamInner实现功能
	 * @param sResult
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode GetWebRtspParam(string& sResult);
	InterfaceResCode SetWebRtspUserInfo(string& sResult, int ChannelNum, string UserName, string Password, int Encrypt );

private:
	/**
	 * @brief GetWebRtspParamInner 构造、发送业务请求并获取响应
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool GetWebRtspParamInner(CData& oResult, char* cResult,int chn_num);
	bool SetWebRtspUserInfoInner(CData& oResult, char* cResult, int ChannelNum, string UserName, string Password, int Encrypt );
	bool ComposeResult(cJSON* result);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();

	MPSClient*			_mpsClient;
	cJSON*				_result;
	cJSON*         _rtspParamList;
	CAnalyzeParaProc	_analyPara;
};


#endif //__GETWEBRTSPPARAM__H__
