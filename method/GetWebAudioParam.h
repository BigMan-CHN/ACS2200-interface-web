#ifndef __GETWEBAUDIOPARAM__H__
#define __GETWEBAUDIOPARAM__H__

#include <TransmitClient.h>
#include <LibXML.h>
#include <cJSON.h>

#include "interfaceDefines.h"
#include "CAnalyzeParaProc.h"

/*
	获取音频参数方法类
*/

class MPSClient;
class CGetWebAudioParam
{
public:
	CGetWebAudioParam(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CGetWebAudioParam();
	/**
	 * @brief GetWebAudioParam 内部调用GetWebAudioParamInner实现功能
	 * @param sResult
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode GetWebAudioParam(string& sResult);

private:
	/**
	 * @brief GetWebAudioParamInner 构造、发送业务请求并获取响应
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool GetWebAudioParamInner(CData& oResult, char* cResult, int chn_num);
	bool ComposeResult(cJSON* result);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();

	MPSClient*			_mpsClient;
	cJSON*				_result;
	cJSON* _audioParamList;
	CAnalyzeParaProc	_analyPara;
};


#endif //__GETWEBAUDIOPARAM__H__
