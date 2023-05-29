#ifndef __SETWEBBACKUPCFGFILE__H__
#define __SETWEBBACKUPCFGFILE__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	备份文件方法类
*/

class MPSClient;
class CSetWebBackupCfgFile
{
public:
	CSetWebBackupCfgFile(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebBackupCfgFile();

	/**
	 * @brief SetWebBackupCfgFile 内部调用SetWebBackupCfgFileInner实现功能
	 * @param sResult
	 * @param backUPCfg
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebBackupCfgFile(std::string& sResult, const std::string& backUPCfg);

private:
	/**
	 * @brief SetWebBackupCfgFileInner 构造、发送业务请求并获取响应
	 * @param backUPCfg
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebBackupCfgFileInner(const std::string& backUPCfg, CData& oResult, char* cResult);
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


#endif //__SETWEBBACKUPCFGFILE__H__
