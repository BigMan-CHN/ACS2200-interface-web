#include "SetWebDevID.h"
#include "MPSClient.h"

CSetWebDevID::CSetWebDevID(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebDevID::~CSetWebDevID()
{
	
}

bool CSetWebDevID::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebDevID::ComposeResult()
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


InterfaceResCode CSetWebDevID::SetWebDevID(std::string& sResult, string devId)
{
	mLogInfo("设置 web 视频参数...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebDevIDInner(devId, oResult, cResult))
	{
		mLogError("SetWebDevIDInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析响应, 默认不处理
	if (!_analyzePara.ConvertSTRING2CJSONSetWebDevID(oResult, cResult)) {
		mLogError("Failed to run  ConvertSTRING2CJSONSetWebDevID(...)");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}

bool CSetWebDevID::SetWebDevIDInner(string devId, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebDevIDInner(...)");

	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	sprintf(szReqBody, INTERFACE_SETWEBDEVID_SENDCMD, devId.c_str());

	int realBodySize = strlen(szReqBody);

	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;
	szReqCmd[1] = 3;
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize & 0xff;

	memcpy(&szReqCmd[4], szReqBody, realBodySize);
#if 1
    MPSOperationRes opResCode = eMPSResultOK;
	ResponseCode resCode = _mpsClient->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
	if (resCode != eResponseCodeSuccess)
	{
		mLogError("SetHostNBInfoInner Run Failed !!!");
		return false;
	}
#endif

	return true;
}

