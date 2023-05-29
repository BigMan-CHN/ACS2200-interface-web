#include "SetWebRecoverCFGFile.h"
#include "MPSClient.h"

CSetWebRecoverCFGFile::CSetWebRecoverCFGFile(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebRecoverCFGFile::~CSetWebRecoverCFGFile()
{
	
}

bool CSetWebRecoverCFGFile::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebRecoverCFGFile::ComposeResult()
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


InterfaceResCode CSetWebRecoverCFGFile::SetWebRecoverCFGFile(std::string& sResult, const std::string& recoverCFG)
{
	mLogInfo("设置 web 视频参数...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebRecoverCFGFileInner(recoverCFG, oResult, cResult))
	{
		mLogError("SetWebVideoParamInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析响应, 默认不处理
	if (!_analyzePara.ConvertSTRING2CJSONSetWebRecoverCFGFile(oResult, cResult)) {
		mLogError("Failed to run  ConvertSTRING2CJSONSetWebRecoverCFGFile(...)");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}

bool CSetWebRecoverCFGFile::SetWebRecoverCFGFileInner(const std::string& recoverCFG, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebRecoverCFGFileInner(...)");

	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	snprintf(szReqBody, REQ_BODY_MAXLEN, INTERFACE_SETWEBRECOVERCFGFILE_SENDCMD, recoverCFG.c_str());

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

