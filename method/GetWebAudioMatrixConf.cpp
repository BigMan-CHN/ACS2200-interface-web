#include "GetWebAudioMatrixConf.h"
#include "MPSClient.h"

CGetWebAudioMatrixConf::CGetWebAudioMatrixConf(const std::string& ip, unsigned short port, int timeOut)
	:_result(cJSON_CreateObject())
	, _audioMatrixList(cJSON_CreateArray())
	, _mpsClient(new MPSClient(ip, port, timeOut))
{

}

CGetWebAudioMatrixConf::~CGetWebAudioMatrixConf()
{
}

bool CGetWebAudioMatrixConf::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}
// 
// 	if (_nbTaskList == NULL) {
// 		mLogWarn("cJSON _nbTaskList == NULL.");
// 		return false;
// 	}
// 
// 	//result中添加节点
// 
// 	cJSON_AddItemToObject(_result, GET_HOST_NBINFO_TASKLIST, _nbTaskList);
	return true;
}

std::string CGetWebAudioMatrixConf::ComposeResult()
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

	if (_audioMatrixList == NULL)
	{
		mLogError("_audioMatrixList is NULL !!!");
		return "";
	}

	cJSON_AddItemToObject(_result, GET_WEB_AUDIO_MATRIX_CONF_RES_AUDIOMATRIXLIST, _audioMatrixList);

	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	//cJSON_Delete(result);
	return res;
}

InterfaceResCode CGetWebAudioMatrixConf::GetWebAudioMatrixConf(string& sResult)
{
	mLogInfo("获取 音频矩阵参数...");

	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();

	//1. 调用类内部方法进一步封装业务请求
	if (!GetWebAudioMatrixConfInner(oResult, cResult))
	{
		mLogError("Failed to run  GetWebAudioMatrixConfInner(...)");
		return eInterfaceResCodeError;
	}

	//2. 解析响应,存入类型为cJson对象的成员变量中
	if (!_analyPara.ConvertSTRING2CJSONGetWebAudioMatrixConf(oResult, cResult, _result, _audioMatrixList)) {
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebAudioMatrixConf(...)");
		return eInterfaceResCodeError;
	}

	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();

	return eInterfaceResCodeSuccess;
}

bool CGetWebAudioMatrixConf::GetWebAudioMatrixConfInner(CData& oResult, char* cResult)
{
	mLogDebug("run GetWebAudioMatrixConfInner(...)");

	InterfaceResCode resCode = eInterfaceResCodeSuccess;

	//1. 封装业务请求
	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	
	snprintf(szReqBody, REQ_BODY_MAXLEN, INTERFACE_GETWEBAUDIOMATRIXCONF_SENDCMD);
	int realBodySize = strlen(szReqBody);

	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;  //不加密
	szReqCmd[1] = 2;  //Get请求, 3是SET, 4是SYS
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize;
	memcpy(&szReqCmd[4], szReqBody, realBodySize);

#if 0
	//2. 发送socket请求
	//MPSOperationRes opResCode = eMPSResultOK; //??
	ResponseCode resCode = = _mpsClient->GetConfigNew(szReqCmd, 4 + realBodySize, eMPSResultOK, oResult, cResult);
	if (resCode != eResponseCodeSuccess) {
		mLogError("GetConfig(...) error:" << resCode);
		return false;
	}
#else
	//dummy response
#endif

	return true;
}
