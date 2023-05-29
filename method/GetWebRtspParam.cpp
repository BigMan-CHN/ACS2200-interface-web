#include "GetWebRtspParam.h"
#include "MPSClient.h"

CGetWebRtspParam::CGetWebRtspParam(const std::string& ip, unsigned short port, int timeOut)
	:_result(cJSON_CreateObject())
	, _rtspParamList(cJSON_CreateArray())
	, _mpsClient(new MPSClient(ip, port, timeOut))
{

}

CGetWebRtspParam::~CGetWebRtspParam()
{
}

bool CGetWebRtspParam::ComposeResult(cJSON* result)
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

std::string CGetWebRtspParam::ComposeResult()
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

	if (_rtspParamList == NULL)
	{
		mLogError("_audioParamList is NULL !!!");
		return "";
	}

	

	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	//cJSON_Delete(result);
	return res;
}

InterfaceResCode CGetWebRtspParam::GetWebRtspParam(string& sResult)
{
	mLogInfo("GetWebRtspParam...");
	cJSON* temList[15];
	int chn_num = 13;
	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();
	
	string Method = "GetWebRtspParam";
	string ip;
	 //用于生成节点，存储解析后的参数
	 for(int i = 1; i <= chn_num; i++)
	 {
		 temList[i] = cJSON_CreateObject();
	 }

	  //1. 调用类内部方法进一步封装业务请求
	  if (!GetWebRtspParamInner(oResult, cResult,chn_num))
	  {
		mLogError("Failed to run  GetWebRtspParamInner(...)");
		return eInterfaceResCodeError;
	  }

	  //2. 解析响应,存入类型为cJson对象的成员变量中
	  if (!_analyPara.ConvertSTRING2CJSONGetWebRtspParam(oResult, cResult, _result, _rtspParamList, temList, Method, ip, chn_num))
	  {
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebRtspParam(...)");
		return eInterfaceResCodeError;
	  }
  	 //将节点添加到节点链表中
	for(int i = 1; i <= chn_num; i++)
	{
		cJSON_AddItemToArray(_rtspParamList, temList[i]);					
	} 
	cJSON_AddItemToObject(_result, "total", cJSON_CreateNumber((chn_num)));
	cJSON_AddItemToObject(_result, GET_WEB_RTSP_PARAM_RES_RTSP_PARAMLIST, _rtspParamList);
	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	//删除根节点，释放资源
	cJSON_Delete(_result);

	return eInterfaceResCodeSuccess;
}

bool CGetWebRtspParam::GetWebRtspParamInner(CData& oResult, char* cResult,int chn_num)
{
	mLogDebug("run GetWebRtspParamInner(...)");

	//InterfaceResCode resCode = eInterfaceResCodeSuccess;

	//1. 封装业务请求
	char szReqBody[2048] = { 0 };
	char *ReqBody;
	//获取8个通道的参数
	memset(szReqBody, 0, 2048);
	sprintf(szReqBody, "net_send100.urlall\n");
//	for(int i = 1; i <= chn_num; i++)
//	{
//		ReqBody = szReqBody + strlen(szReqBody);
//		sprintf(ReqBody, INTERFACE_GETWEBRTSPPARAM_SENDCMD, i, i);
//	}

	//sprintf(szReqBody, INTERFACE_GETWEBRTSPPARAM_SENDCMD, chn_num, chn_num);
	int realBodySize = strlen(szReqBody);

	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;  //不加密
	szReqCmd[1] = 2;  //Get请求, 3是SET, 4是SYS
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize;
	memcpy(&szReqCmd[4], szReqBody, realBodySize);

#if 1
	//2. 发送socket请求
	MPSOperationRes opResCode = eMPSResultOK; //??
	ResponseCode resCode = _mpsClient->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
	if (resCode != eResponseCodeSuccess) {
		mLogError("GetConfig(...) error:" << resCode);
		return false;
	}
#else
	//dummy response
#endif

	return true;
}
InterfaceResCode CGetWebRtspParam::SetWebRtspUserInfo(string& sResult, int ChannelNum, string UserName, string Password, int Encrypt )
{
	mLogInfo("SetWebRtspUserInfo...");
	cJSON* temList[15];
	int chn_num = 14;
	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();
	

	  //1. 调用类内部方法进一步封装业务请求
	  if (!SetWebRtspUserInfoInner(oResult, cResult, ChannelNum, UserName, Password, Encrypt))
	  {
		mLogError("Failed to run  SetWebRtspUserInfoInner(...)");
		return eInterfaceResCodeError;
	  }

	  //2. 解析响应,存入类型为cJson对象的成员变量中
//	  if (!_analyPara.ConvertSTRING2CJSONGetWebRtspParam(oResult, cResult, _result, _rtspParamList, temList, Method, ip, chn_num))
//	  {
//		mLogError("Failed to run  ConvertSTRING2CJSONGetWebRtspParam(...)");
//		return eInterfaceResCodeError;
//	  }

	sResult = ComposeResult();
	//删除根节点，释放资源
	cJSON_Delete(_result);

	return eInterfaceResCodeSuccess;
}

bool CGetWebRtspParam::SetWebRtspUserInfoInner(CData& oResult, char* cResult, int ChannelNum, string UserName, string Password, int Encrypt )
{
	mLogDebug("run GetWebRtspParamInner(...)");

	//InterfaceResCode resCode = eInterfaceResCodeSuccess;

	//1. 封装业务请求
	char szReqBody[2048] = { 0 };
	char *ReqBody;
	//获取8个通道的参数
	memset(szReqBody, 0, 2048);
	sprintf(szReqBody, "net_send%d.usr=%s,%s\nnet_send%d.encrypt=%d\n",ChannelNum, UserName.c_str(), Password.c_str(), ChannelNum, Encrypt);
//	for(int i = 1; i <= chn_num; i++)
//	{
//		ReqBody = szReqBody + strlen(szReqBody);
//		sprintf(ReqBody, "net_send%d.name\nnet_send%d.url\n", i, i);
//	}
	int realBodySize = strlen(szReqBody);

	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;  //不加密
	szReqCmd[1] = 3;  //Get请求, 3是SET, 4是SYS
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize;
	memcpy(&szReqCmd[4], szReqBody, realBodySize);

#if 1
	//2. 发送socket请求
	MPSOperationRes opResCode = eMPSResultOK; //??
	ResponseCode resCode = _mpsClient->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
	if (resCode != eResponseCodeSuccess) {
		mLogError("GetConfig(...) error:" << resCode);
		return false;
	}
#else
	//dummy response
#endif

	return true;
}

