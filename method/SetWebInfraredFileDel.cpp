#include "SetWebInfraredFileDel.h"
#include "MPSClient.h"

CSetWebInfraredFileDel::CSetWebInfraredFileDel(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	, _result(cJSON_CreateObject())
	, _ParamList(cJSON_CreateArray())
{
}

CSetWebInfraredFileDel::~CSetWebInfraredFileDel()
{
}


bool CSetWebInfraredFileDel::ComposeResult(cJSON* result)
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

std::string CSetWebInfraredFileDel::ComposeResult()
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

	if (_ParamList == NULL)
	{
		mLogError("_videoParamList is NULL !!!");
		return "";
	}

	//cJSON_AddItemToObject(_result, "ParamList", _ParamList);

	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	//cJSON_Delete(result);
	return res;
}

InterfaceResCode CSetWebInfraredFileDel::SetWebInfraredFileDel(string& sResult, string FileName)
{
	mLogInfo("获取 SetWebInfraredFileDel...");

	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();

	//1. 调用类内部方法进一步封装业务请求
	if (!SetWebInfraredFileDelInner(oResult, cResult, FileName))
	{
		mLogError("Failed to run  SetWebInfraredFileDelInner(...)");
		return eInterfaceResCodeError;
	}
#if 0
	//2. 解析响应,存入类型为cJson对象的成员变量中
	if (!_analyPara.ConvertSTRING2CJSONGetWebInfraredStudyList(oResult, cResult, _result, _ParamList)) {
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebInfraredStudyList(...)");
		return eInterfaceResCodeError;
	}
#endif
	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	cJSON_Delete(_result);

	return eInterfaceResCodeSuccess;
}


bool CSetWebInfraredFileDel::SetWebInfraredFileDelInner(CData& oResult, char* cResult, string FileName)
{
	mLogDebug("run GetWebInfraredFileInfListInner(...)");
	char PathFile[1024]   = {0};
	sprintf( PathFile, "/root/Communicate_Schedule_exe/config/hongwai/%s", FileName.c_str()); 
	
	if( remove(PathFile) == 0 )
        mLogDebug("Removed success ! filename="<<FileName.c_str());
    else
        mLogError("Removed failed ! filename="<<FileName.c_str());

	return true;
}
