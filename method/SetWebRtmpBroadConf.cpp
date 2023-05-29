#include "SetWebRtmpBroadConf.h"
#include "MPSClient.h"

CSetWebRtmpBroadConf::CSetWebRtmpBroadConf(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebRtmpBroadConf::~CSetWebRtmpBroadConf()
{
	
}

bool CSetWebRtmpBroadConf::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebRtmpBroadConf::ComposeResult()
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


InterfaceResCode CSetWebRtmpBroadConf::SetWebRtmpBroadConf(std::string& sResult,
	int isEnabled, const std::string& broadAddr)
{
	mLogInfo("SetWebRtmpBroadConf...");
	cJSON* temList[15];
	string Method = "SetWebRtmpBroadConf";
	string IP;

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebRtmpBroadConfInner(isEnabled, broadAddr, oResult, cResult))
	{
		mLogError("SetWebRtmpBroadConfInner failed !!!");
		return eInterfaceResCodeError;
	}

	if (cResult[5] != 0)
	{
		mLogError("error: cResult[5] = "<<cResult[5]);
		return eInterfaceResCodeError;
	}


	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}

bool CSetWebRtmpBroadConf::SetWebRtmpBroadConfInner(int isEnabled, const std::string& broadAddr,
	CData& oResult, char* cResult)
{
	mLogDebug("run SetWebRtmpBroadConfInner(...)");

	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	sprintf(szReqBody,"net_send47.on=%d\nnet_send47.url=%s\n", isEnabled,broadAddr.c_str());

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
		mLogError("SetWebRtmpBroadConfInner Run Failed !!!");
		return false;
	}
#endif

	return true;
}
InterfaceResCode CSetWebRtmpBroadConf::GetWebRtmpBroadConf(std::string& sResult)
{
	mLogInfo("GetWebRtmpBroadConf...");
	cJSON* temList[15];
	string Method = "GetWebRtmpBroadConf";
	string IP;

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!GetWebRtmpBroadConfInner( oResult, cResult))
	{
		mLogError("GetWebRtmpBroadConfInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析响应
	if (!_analyzePara.ConvertSTRING2CJSONGetWebParam(oResult, cResult, _result, NULL, NULL, Method, IP, 0)) {
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebParam(...)");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}

bool CSetWebRtmpBroadConf::GetWebRtmpBroadConfInner(CData& oResult, char* cResult)
{
	mLogDebug("run GetWebRtmpBroadConfInner(...)");

	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	sprintf(szReqBody,"net_send47.on\nnet_send47.url\n");

	int realBodySize = strlen(szReqBody);

	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;
	szReqCmd[1] = 2;
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize & 0xff;

	memcpy(&szReqCmd[4], szReqBody, realBodySize);
#if 1
	 MPSOperationRes opResCode = eMPSResultOK;
	ResponseCode resCode = _mpsClient->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
	if (resCode != eResponseCodeSuccess)
	{
		mLogError("GetWebRtmpBroadConfInner Run Failed !!!");
		return false;
	}
#endif

	return true;
}

