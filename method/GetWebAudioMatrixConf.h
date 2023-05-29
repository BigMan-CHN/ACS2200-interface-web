#ifndef __GETWEBAUDIOMATRIXCONFIG_H__
#define __GETWEBAUDIOMATRIXCONFIG_H__

#include <TransmitClient.h>
#include <LibXML.h>
#include <cJSON.h>

#include "interfaceDefines.h"
#include "CAnalyzeParaProc.h"

/*
	获取音频矩阵参数方法类
*/

class MPSClient;
class CGetWebAudioMatrixConf
{
public:
	CGetWebAudioMatrixConf(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CGetWebAudioMatrixConf();
	/**
	 * @brief GetWebAudioMatrixConf 内部调用GetWebAudioMatrixConfInner实现功能
	 * @param sResult
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode GetWebAudioMatrixConf(string& sResult);

private:
	/**
	 * @brief GetWebAudioMatrixConfInner 构造、发送业务请求并获取响应
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool GetWebAudioMatrixConfInner(CData& oResult, char* cResult);
	bool ComposeResult(cJSON* result);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();

	MPSClient*			_mpsClient;
	cJSON*				_result;
	cJSON*				_audioMatrixList;
	CAnalyzeParaProc	_analyPara;
};


#endif //__GETWEBAUDIOMATRIXCONFIG_H__
