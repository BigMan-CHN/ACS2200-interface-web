#ifndef __SETWEBDISCIPFILEUPDATE__H__
#define __SETWEBDISCIPFILEUPDATE__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	纪律文件升级方法类
*/

class MPSClient;
class CSetWebDiscipFileUpdate
{
public:
	CSetWebDiscipFileUpdate(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebDiscipFileUpdate();
	/**
	 * @brief SetWebDiscipFileUpdate 内部调用SetWebDiscipFileUpdateInner实现功能
	 * @param sResult
	 * @param discipfileUpdate
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebDiscipFileUpdate(std::string& sResult, const std::string& discipfileUpdate);

private:
	/**
	 * @brief SetWebDiscipFileUpdateInner 构造、发送业务请求并获取响应
	 * @param discipfileUpdate
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebDiscipFileUpdateInner(const std::string& discipfileUpdate, CData& oResult, char* cResult);
	bool ComposeResult(cJSON* result);
	std::string ComposeResult();
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	MPSClient* _mpsClient;
	CAnalyzeParaProc _analyzePara;
	cJSON* _result;
};


#endif //__SETWEBDISCIPFILEUPDATE__H__
