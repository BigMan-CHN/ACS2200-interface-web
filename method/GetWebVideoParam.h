#ifndef __GETWEBVIDEOPARAM__H__
#define __GETWEBVIDEOPARAM__H__

#include <TransmitClient.h>
#include <LibXML.h>
#include <cJSON.h>

#include "interfaceDefines.h"
#include "CAnalyzeParaProc.h"

/*
	获取视频参数方法类
*/

class MPSClient;
class CGetWebVideoParam
{
public:
	CGetWebVideoParam(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CGetWebVideoParam();
	/**
	 * @brief GetWebVideoParam 内部调用GetWebVideoParamInner实现功能
	 * @param sResult
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode GetWebVideoParam(string& sResult);

private:
	/**
	 * @brief GetWebVideoParamInner 构造、发送业务请求并获取响应
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool GetWebVideoParamInner(CData& oResult, char* cResult,int chn_num);
	bool ComposeResult(cJSON* result);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();

	MPSClient*			_mpsClient;
	cJSON*				_result;
	cJSON*				_videoParamList;
	cJSON*				_node;
	CAnalyzeParaProc	_analyPara;
};


#endif //__GETWEBAUDIOCONFIG__H__
