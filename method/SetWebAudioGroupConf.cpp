#include "SetWebAudioGroupConf.h"
#include "MPSClient.h"

CSetWebAudioGroupConf::CSetWebAudioGroupConf(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebAudioGroupConf::~CSetWebAudioGroupConf()
{
	
}

bool CSetWebAudioGroupConf::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebAudioGroupConf::ComposeResult()
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


InterfaceResCode CSetWebAudioGroupConf::SetWebAudioGroupConf(std::string& sResult, int groupName, const std::string& chnNum)
{
	mLogInfo("设置 WebAudioConf...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebAudioGroupConfInner(groupName, chnNum, oResult, cResult))
	{
		mLogError("SetWebAudioGroupConfInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析响应, 默认不处理
	if (!_analyzePara.ConvertSTRING2CJSONSetWebAudioGroupConf(oResult, cResult)) {
		mLogError("Failed to run  ConvertSTRING2CJSONSetWebAudioGroupConf(...)");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}

bool CSetWebAudioGroupConf::SetWebAudioGroupConfInner(int groupName, const std::string& chnNum, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebAudioGroupConfInner(...)");

	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	snprintf(szReqBody, REQ_BODY_MAXLEN, INTERFACE_SETWEBAUDIOGROUPCONF_SENDCMD, groupName, chnNum.c_str());
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

