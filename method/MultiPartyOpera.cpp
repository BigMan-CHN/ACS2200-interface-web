#include "MultiPartyOpera.h"
#include "MPSClient.h"
#include "XMLDefines.h"
#include "XMLOpera.h"

CMultiPartyOpera::CMultiPartyOpera(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
	, _partyList(cJSON_CreateArray())
{
	CXMLOpera::SetFilePath(PARTYLIST_FILE_PATH);
}

CMultiPartyOpera::~CMultiPartyOpera()
{
	
}

InterfaceResCode CMultiPartyOpera::InsertOneParty(std::string& sResult, const std::string& name, const std::string& protocol, const std::string& address, const std::string& bitRate, const std::string& bufferLength, const std::string& transport)
{
	mLogDebug("insert one party .");
	//1. 直接调用即可
	CXMLOpera::InsertOneParty(name, address, protocol, bitRate, bufferLength, transport);

	char* cRes = cJSON_Print(_result);
	sResult = cRes;
	return eInterfaceResCodeSuccess;
}

InterfaceResCode CMultiPartyOpera::DeleteOneParty(std::string& sResult, int id)
{
	mLogDebug("delete one party .");

	CXMLOpera::DelOneParty(id);

	char* cRes = cJSON_Print(_result);
	sResult = cRes;
	return eInterfaceResCodeSuccess;
}

InterfaceResCode CMultiPartyOpera::GetPartyList(std::string& sResult)
{
	mLogDebug("get party list .");

	CXMLOpera::LoadPartyList(_result, _partyList);

	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}

bool CMultiPartyOpera::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CMultiPartyOpera::ComposeResult()
{
// 	cJSON* result = cJSON_CreateObject();
// 	if (ComposeResult(result) == false) {
// 		cJSON_Delete(result);
// 		return "";
// 	}
// 	else {
// 		char* cRes = cJSON_Print(_result);
// 		std::string res = cRes;
// 		cJSON_Delete(result);
// 		return res;
// 	}

	//assume the setmethod's response code is always okay. -_-!
	if (_partyList == NULL)
	{
		mLogError("_partyList is NULL !!!");
		return "";
	}

	cJSON_AddItemToObject(_result, GET_WEB_ADDR_LIST_RES_PARTYLIST, _partyList);

	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	//cJSON_Delete(result);
	return res;
}

