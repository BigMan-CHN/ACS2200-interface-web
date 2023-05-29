#ifndef __SETWEBVIDEOPARAM__H__
#define __SETWEBVIDEOPARAM__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/**
 *  设置视频参数方法类
 * 
 */

class MPSClient;
class CSetWebVideoParam
{
public:
	CSetWebVideoParam(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebVideoParam();

	/**
	 * @brief SetWebVideoParam 内部调用SetWebVideoParamInner实现功能
	 * @param sResult
	 * @param chnName
	 * @param zKTUpdate
	 * @param resolutionSet
	 * @param codeMode
	 * @param rateType
	 * @param rateSize
	 * @param frameSet
	 * @param gopSet
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebVideoParam(std::string& sResult,
		int chnNum,
		string chnName,
		int resolutionSet,
		int codeMode,
		int rateType,
		int rateSize,
		int frameSet,
		int gopSet);

private:
	/**
	 * @brief SetWebVideoParamInner 构造、发送业务请求并获取响应
	 * @param chnName
	 * @param zKTUpdate
	 * @param resolutionSet
	 * @param codeMode
	 * @param rateType
	 * @param rateSize
	 * @param frameSet
	 * @param gopSet
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebVideoParamInner(int chnNum, string chnName,
		int resolutionSet,
		int codeMode,
		int rateType,
		int rateSize,
		int frameSet,
		int gopSet,
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


#endif //__SETWEBVIDEOPARAM__H__
