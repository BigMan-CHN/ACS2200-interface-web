#include "SetWebRtspParam.h"
#include "MPSClient.h"

CSetWebRtspParam::CSetWebRtspParam(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebRtspParam::~CSetWebRtspParam()
{
	
}

bool CSetWebRtspParam::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebRtspParam::ComposeResult()
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


InterfaceResCode CSetWebRtspParam::SetWebRtspParam(std::string& sResult,
	int RtspNum, const std::string& rtspSource, const std::string& rtspUrl)
{
	mLogInfo("设置 web rtsp参数...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebRtspParamInner(RtspNum, rtspSource, rtspUrl, oResult, cResult))
	{
		mLogError("SetWebRtspParamInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析响应, 默认不处理
	if (!_analyzePara.ConvertSTRING2CJSONSetWebRtspParam(oResult, cResult)) {
		mLogError("Failed to run  ConvertSTRING2CJSONSetWebRtspParam(...)");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}

bool CSetWebRtspParam::SetWebRtspParamInner(int RtspNum, const std::string& rtspSource,
	const std::string& rtspUrl, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebRtspParamInner(...)");

	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	sprintf(szReqBody, INTERFACE_SETWEBRTSPPARAM_SENDCMD, RtspNum,
		rtspSource.c_str(), RtspNum, rtspUrl.c_str());

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

