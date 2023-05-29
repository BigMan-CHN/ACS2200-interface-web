#ifndef __GETWEBINFRAREDCREATE_H__
#define __GETWEBINFRAREDCREATE_H__

#include <TransmitClient.h>
#include <LibXML.h>
#include <cJSON.h>

#include "interfaceDefines.h"
#include "CAnalyzeParaProc.h"

/*
	红外命令集文件创建方法类
*/

class MPSClient;
class CSetWebInfraredCreate
{
public:
	CSetWebInfraredCreate(const std::string& ip = MPS_IP, unsigned short port =MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebInfraredCreate();
	/**
	 * @brief SetWebInfraredCreate 内部调用SetWebInfraredCreateInner实现功能
	 * @param sResult
	 * @param port
	 * @param fileName
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebInfraredCreate(string& sResult,
		int port,
		const std::string& fileName, string DevID, string DevType, string Manufacturer);


private:
	/**
	 * @brief SetWebInfraredCreateInner 构造、发送业务请求并获取响应
	 * @param port
	 * @param fileName
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebInfraredCreateInner(int port, 
		const std::string& fileName,
		CData& oResult,
		char* cResult, string DevID, string DevType, string Manufacturer);
	bool ComposeResult(cJSON* result);
	int code_convert(char* from_charset, char* to_charset, char* inbuf, int inlen, char* outbuf, int outlen);
	char* utf8_to_gb2312(char *inbuf);  
	char* gb2312_to_utf8(char *inbuf);   

	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();

	MPSClient* _mpsClient;
	cJSON* _result;
	CAnalyzeParaProc	_analyPara;
};


#endif //__GETWEBINFRAREDCREATE_H__
