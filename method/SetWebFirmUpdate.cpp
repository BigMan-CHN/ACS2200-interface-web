#include "SetWebFirmUpdate.h"
#include "MPSClient.h"

CSetWebFirmUpdate::CSetWebFirmUpdate(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebFirmUpdate::~CSetWebFirmUpdate()
{
	
}

bool CSetWebFirmUpdate::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebFirmUpdate::ComposeResult()
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
	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	return res;
}


InterfaceResCode CSetWebFirmUpdate::SetWebFirmUpdate(std::string& sResult, const std::string& firmUpdate)
{
	mLogInfo("firm update...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebFirmUpdateInner(firmUpdate, oResult, cResult))
	{
		mLogError("SetWebFirmUpdateInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析响应, 默认不处理
	if (!_analyzePara.ConvertSTRING2CJSONSetWebFirmUpdate(oResult, cResult)) {
		mLogError("Failed to run  ConvertSTRING2CJSONSetWebFirmUpdate(...)");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}

bool CSetWebFirmUpdate::SetWebFirmUpdateInner(const std::string& firmUpdate, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebFirmUpdateInner(...)");

	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	snprintf(szReqBody, REQ_BODY_MAXLEN, INTERFACE_SETWEBFIRMUPDATE_SENDCMD, firmUpdate.c_str());

	int realBodySize = strlen(szReqBody);

	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;
	szReqCmd[1] = 3;
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize & 0xff;

	memcpy(&szReqCmd[4], szReqBody, realBodySize);
#if 0
	// MPSOperationRes opResCode = eMPSResultOK;
	ResponseCode resCode = _mpsClient->GetConfigNew(szReqCmd, 4 + realBodySize, eMPSResultOK, oResult, cResult);
	if (resCode != eResponseCodeSuccess)
	{
		mLogError("SetHostNBInfoInner Run Failed !!!");
		return false;
	}
#endif

	return true;
}

