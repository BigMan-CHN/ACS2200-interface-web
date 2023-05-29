#include "SetWebLogFileConf.h"
#include "MPSClient.h"

CSetWebLogFileConf::CSetWebLogFileConf(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebLogFileConf::~CSetWebLogFileConf()
{
	
}

bool CSetWebLogFileConf::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebLogFileConf::ComposeResult()
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


InterfaceResCode CSetWebLogFileConf::SetWebLogFileConf(std::string& sResult,
	int logSize , int logSaveTime, int logLevel)
{
	mLogInfo("设置 日志参数...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebLogFileConfInner(logSize, logSaveTime, logLevel, oResult, cResult))
	{
		mLogError("SetWebLogFileConfInner failed !!!");
		return eInterfaceResCodeError;
	}

	//解析响应, 默认不处理
	if (!_analyzePara.ConvertSTRING2CJSONSetWebLogFileConf(oResult, cResult)) {
		mLogError("Failed to run  ConvertSTRING2CJSONSetWebLogFileConf(...)");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	return eInterfaceResCodeSuccess;
}

bool CSetWebLogFileConf::SetWebLogFileConfInner(int logSize, int logSaveTime,
	int logLevel, CData& oResult, char* cResult)
{
	mLogDebug("run SetWebLogFileConfInner(...)");
	string log_Level;
	switch(logLevel)
	{
	  case 1:
		   log_Level = "debug";
		   break;
	  case 2:
		   log_Level = "info";
		   break;
	  case 3:
		   log_Level = "error";
		   break;
	  case 4:
		   log_Level = "warning";
		   break;
	  default:
		   mLogError("please input the right log_Level!!!");
		   return false;
	  }
	mLogDebug("logSize = "<<logSize);

	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	sprintf(szReqBody, "syslog.partition_size=%dM\nsyslog.time_limit=%d\nsyslog.option=%s\n", logSize, logSaveTime, log_Level.c_str());
	//sprintf(szReqBody, INTERFACE_SETWEBLOGFILECONF_SENDCMD, logSize, logSaveTime, log_Level.c_str());
	mLogDebug("szReqBody = "<<szReqBody);

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
		mLogError("SetWebLogFileConfInner Run Failed !!!");
		return false;
	}
#endif

	return true;
}

