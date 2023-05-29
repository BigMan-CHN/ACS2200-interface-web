#ifndef __MUTLIPARTYOPERA__H__
#define __MUTLIPARTYOPERA__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"
/*
	多方互动联系人操作方法类
*/
class MPSClient;
class CMultiPartyOpera
{
public:
	CMultiPartyOpera(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CMultiPartyOpera();

	/**
	 * @brief SetWebZKTUpdate 内部调用SetWebZKTUpdateInner实现功能
	 * @param sResult
	 * @param zKTUpdate
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode InsertOneParty(std::string& sResult, 
		const std::string& name,
		const std::string& protocol,
		const std::string& address,
		const std::string& bitRate,
		const std::string& bufferLength,
		const std::string& transport);
	/**
	 * @brief SetWebZKTUpdate 内部调用SetWebZKTUpdateInner实现功能
	 * @param sResult
	 * @param zKTUpdate
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode DeleteOneParty(std::string& sResult, int id);
	/**
	 * @brief SetWebZKTUpdate 内部调用SetWebZKTUpdateInner实现功能
	 * @param sResult
	 * @param zKTUpdate
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode GetPartyList(std::string& sResult);

private:

	bool ComposeResult(cJSON* result);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();

	MPSClient* _mpsClient;
	CAnalyzeParaProc _analyzePara;
	cJSON* _partyList;
	cJSON* _result;
};


#endif //__MUTLIPARTYOPERA__H__
