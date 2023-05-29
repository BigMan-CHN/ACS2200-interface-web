#ifndef __SETWEBINFRAREDDEL__H__
#define __SETWEBINFRAREDDEL__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"
#include <libxml/parser.h>
#include <libxml/xpath.h>
/*
	红外命令集删除方法类
*/

class MPSClient;
class CSetWebInfraredDel
{
public:
	CSetWebInfraredDel(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebInfraredDel();
	/**
	 * @brief SetWebInfraredDel 内部调用SetWebInfraredDelInner实现功能
	 * @param sResult
	 * @param port
	 * @param fileName
	 * @param num
	 * @param order
	 * @param describe
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebInfraredDel(std::string& sResult,
		int port,
		const std::string& fileName,
		int num,
		const std::string& order,
		const std::string& describe);

private:
	/**
	 * @brief SetWebInfraredDelInner 构造、发送业务请求并获取响应
	 * @param port
	 * @param fileName
	 * @param num
	 * @param order
	 * @param describe
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebInfraredDelInner(int port,
		const std::string& fileName,
		int num,
		const std::string& order,
		const std::string& describe,
		CData& oResult,
		char* cResult);

	bool ComposeResult(cJSON* result);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();
	xmlXPathObjectPtr get_nodeset(xmlDocPtr doc, const xmlChar *szXpath);

	
	MPSClient* _mpsClient;
	CAnalyzeParaProc _analyzePara;
	cJSON* _result;
};


#endif //__SETWEBINFRAREDDEL__H__
