#ifndef __SETWEBAUDIOMATRIXCONFIG__H__
#define __SETWEBAUDIOMATRIXCONFIG__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	设置音频矩阵参数方法类
*/

class MPSClient;
class CSetWebAudioMatrixConf
{
public:
	CSetWebAudioMatrixConf(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebAudioMatrixConf();
	/**
	 * @brief SetWebAudioMatrixConf 内部调用SetWebAudioMatrixConfInner实现功能
	 * @param sResult
	 * @param chnNum
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebAudioMatrixConf(std::string& sResult, const std::string& chnNum);

private:
	/**
	 * @brief SetWebAudioMatrixConfInner 构造、发送业务请求并获取响应
	 * @param chnNum
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebAudioMatrixConfInner(const std::string& chnNum, CData& oResult, char* cResult);

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


#endif //__SETWEBAUDIOMATRIXCONFIG__H__
