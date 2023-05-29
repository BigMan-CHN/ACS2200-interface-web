#include "SetWebAudioParam.h"
#include "MPSClient.h"

CSetWebAudioParam::CSetWebAudioParam(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebAudioParam::~CSetWebAudioParam()
{
	
}

bool CSetWebAudioParam::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebAudioParam::ComposeResult()
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


InterfaceResCode CSetWebAudioParam::SetWebAudioParam(std::string& sResult,
	int soundNum, const std::string& soundSource, int codeMode,
	int sampleRate, 
	int rateSet)
{
	mLogInfo("设置 web 音频参数...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebAudioParamInner(soundNum, soundSource, codeMode, sampleRate, rateSet, oResult, cResult))
	{
		mLogError("SetWebAudioParamInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析响应, 默认不处理
	if (!_analyzePara.ConvertSTRING2CJSONSetWebAudioParam(oResult, cResult)) {
		mLogError("Failed to run  ConvertSTRING2CJSONSetWebAudioParam(...)");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}

bool CSetWebAudioParam::SetWebAudioParamInner(int soundNum, const std::string& soundSource, int codeMode,
	int sampleRate, int rateSet, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebAudioParamInner(...)");

	  string  encodeMode;
	  int vsampleRate;
	  //编码类型
	  switch(codeMode)
	{
	  case 1:
		   encodeMode = "AAC";
		   break;
	  case 2:
		   encodeMode = "G711U";
		   break;
	  case 3:
		   encodeMode = "G711A";
		   break;
	  default:
		   mLogError("please input the right encodeMode!!!\n");
		   return false;
	  }
	  //音频采样率
	   switch(sampleRate)
	  {
		case 1:
			 vsampleRate = 8000;
			 break;
		case 2:
			 vsampleRate = 11025;
			 break;
		case 3:
			 vsampleRate = 22050;
			 break;
		case 4:
			 vsampleRate = 24000;
			 break;
		case 5:
			 vsampleRate = 44100;
			 break;
		case 6:
			 vsampleRate = 48000;
			 break;
		default:
			 mLogError("please input the right vsampleRate!!!\n");
			 return false;
		}

	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	sprintf(szReqBody, INTERFACE_SETWEBAUDIOPARAM_SENDCMD, soundNum,
		soundSource.c_str(), soundNum, encodeMode.c_str(), vsampleRate, soundNum, rateSet);
	mLogDebug("INTERFACE_SETWEBAUDIOPARAM_SENDCMD = "<<szReqBody);
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
		mLogError("SetWebAudioParamInner Run Failed !!!");
		return false;
	}
#endif

	return true;
}

