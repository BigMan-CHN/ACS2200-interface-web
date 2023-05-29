#include "SetWebDiscipFileUpdate.h"
#include "MPSClient.h"

CSetWebDiscipFileUpdate::CSetWebDiscipFileUpdate(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebDiscipFileUpdate::~CSetWebDiscipFileUpdate()
{
	
}

bool CSetWebDiscipFileUpdate::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebDiscipFileUpdate::ComposeResult()
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


InterfaceResCode CSetWebDiscipFileUpdate::SetWebDiscipFileUpdate(std::string& sResult,
	const std::string& discipfileUpdate)
{
	mLogInfo("discipfile update...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebDiscipFileUpdateInner(discipfileUpdate, oResult, cResult))
	{
		mLogError("SetWebDiscipFileUpdateInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析响应, 默认不处理
	if (!_analyzePara.ConvertSTRING2CJSONSetWebDiscipFileUpdate(oResult, cResult)) {
		mLogError("Failed to run  ConvertSTRING2CJSONSetWebDiscipFileUpdate(...)");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}

bool CSetWebDiscipFileUpdate::SetWebDiscipFileUpdateInner(const std::string& discipfileUpdate, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebDiscipFileUpdateInner(...)");

	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	snprintf(szReqBody, REQ_BODY_MAXLEN, INTERFACE_SETWEBDISCIPFILEUPDATE_SENDCMD, discipfileUpdate.c_str());

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

