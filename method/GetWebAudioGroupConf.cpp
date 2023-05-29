#include "GetWebAudioGroupConf.h"
#include "MPSClient.h"

CGetWebAudioGroupConf::CGetWebAudioGroupConf(const std::string& ip, unsigned short port, int timeOut)
	:_result(cJSON_CreateObject())
	, _audioGroupList(cJSON_CreateArray())
	, _mpsClient(new MPSClient(ip, port, timeOut))
{

}

CGetWebAudioGroupConf::~CGetWebAudioGroupConf()
{
}

bool CGetWebAudioGroupConf::ComposeResult(cJSON* result)
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

std::string CGetWebAudioGroupConf::ComposeResult()
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

	if (_audioGroupList == NULL)
	{
		mLogError("_audioGroupList is NULL !!!");
		return "";
	}

	cJSON_AddItemToObject(_result, GET_WEB_AUDIO_GROUP_CONF_RES_AUDIOGROUPLIST, _audioGroupList);

	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	//cJSON_Delete(result);
	return res;
}

InterfaceResCode CGetWebAudioGroupConf::GetWebAudioGroupConf(string& sResult)
{
	mLogInfo("获取 音频分组信息...");

	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();

	//1. 调用类内部方法进一步封装业务请求
	if (!GetWebAudioGroupConfInner(oResult, cResult))
	{
		mLogError("Failed to run  GetWebAudioGroupConfInner(...)");
		return eInterfaceResCodeError;
	}

	//2. 解析响应,存入类型为cJson对象的成员变量中
	if (!_analyPara.ConvertSTRING2CJSONGetWebAudioGroupConf(oResult, cResult, _result, _audioGroupList)) {
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebAudioGroupConf(...)");
		return eInterfaceResCodeError;
	}

	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();

	return eInterfaceResCodeSuccess;
}

bool CGetWebAudioGroupConf::GetWebAudioGroupConfInner(CData& oResult, char* cResult)
{
	mLogDebug("run GetWebAudioGroupConfInner(...)");

	InterfaceResCode resCode = eInterfaceResCodeSuccess;

	//1. 封装业务请求
	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	
	snprintf(szReqBody, REQ_BODY_MAXLEN, INTERFACE_GETWEBAUDIOGROUPCONF_SENDCMD);
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
