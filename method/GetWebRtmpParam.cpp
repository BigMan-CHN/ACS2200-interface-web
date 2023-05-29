#include "GetWebRtmpParam.h"
#include "MPSClient.h"

CGetWebRtmpParam::CGetWebRtmpParam(const std::string& ip, unsigned short port, int timeOut)
	:_result(cJSON_CreateObject())
	, _ParamList(cJSON_CreateArray())
	, _mpsClient(new MPSClient(ip, port, timeOut))
{

}

CGetWebRtmpParam::~CGetWebRtmpParam()
{
}

bool CGetWebRtmpParam::ComposeResult(cJSON* result)
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

std::string CGetWebRtmpParam::ComposeResult()
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

	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	//cJSON_Delete(result);
	return res;
}

InterfaceResCode CGetWebRtmpParam::GetWebRtmpParam(string& sResult)
{
	mLogInfo("GetWebRtmpParam...");

	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();
	cJSON* temList[15];
	int chn_num = 12;
	string Method = "GetWebRtmpParam";
	string ip;
	 //用于生成节点，存储解析后的参数
	 for(int i = 1; i <= chn_num; i++)
	 {
		 temList[i] = cJSON_CreateObject();
	 }
	//1. 调用类内部方法进一步封装业务请求
	if (!GetWebRtmpParamInner(oResult, cResult))
	{
		mLogError("Failed to run  GetWebRtmpParamInner(...)");
		return eInterfaceResCodeError;
	}

	//2. 解析响应,存入类型为cJson对象的成员变量中
	if (!_analyPara.ConvertSTRING2CJSONGetWebRtmpParam( oResult,  cResult, _result, _ParamList, temList, Method, ip, chn_num)) {
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebRtmpParam(...)");
		return eInterfaceResCodeError;
	}

	char ReqBody[REQ_BODY_MAXLEN] = { 0 };
	
	for (int i = 1; i <= chn_num; i++)
	{
		memset(ReqBody,0,REQ_BODY_MAXLEN);
		switch(i)
		{
			case 1:
				sprintf(ReqBody, "rtmp://%s:1935/live/comp", ip.c_str());
				cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("comp"));
				break;
			case 2:
				sprintf(ReqBody, "rtmp://%s:1935/live/vl1-1", ip.c_str());
				cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl1-1"));
				break;
			case 3:
				sprintf(ReqBody, "rtmp://%s:1935/live/vl2-1", ip.c_str());
				cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl2-1"));
				break;
			case 4:
				sprintf(ReqBody, "rtmp://%s:1935/live/vl3-1", ip.c_str());
				cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl3-1"));
				break;
			case 5:
				sprintf(ReqBody, "rtmp://%s:1935/live/vl4-1", ip.c_str());
				cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl4-1"));
				break;
			case 6:
				sprintf(ReqBody, "rtmp://%s:1935/live/vl5-1", ip.c_str());
				cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl5-1"));
				break;
			case 7:
				sprintf(ReqBody, "rtmp://%s:1935/live/vl6-1", ip.c_str());
				cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl6-1"));
				break;
			case 8:
				sprintf(ReqBody, "rtmp://%s:1935/live/vl7-1", ip.c_str());
				cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl7-1"));
				break;
			case 9:
				sprintf(ReqBody, "rtmp://%s:1935/live/vl8-1", ip.c_str());
				cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl8-1"));
				break;
//			case 9:
//				sprintf(ReqBody, "rtmp://%s:1935/live/comp", ip.c_str());
//				cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("comp"));
//				break;
			case 10:
				sprintf(ReqBody, "rtmp://%s:1935/live/vn1-1", ip.c_str());
				cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vn1-1"));
				break;
			case 11:
				sprintf(ReqBody, "rtmp://%s:1935/live/vn2-1", ip.c_str());
				cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vn2-1"));
				break;
			case 12:
				sprintf(ReqBody, "rtmp://%s:1935/live/vn3-1", ip.c_str());
				cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vn3-1"));
				break;
		}
		
		cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_NUM, cJSON_CreateNumber(i));
		cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_ISHTTP, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_ISHLS, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_ISRTMP, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_ISRTSP, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_ISBROADCAST, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_BROADCASTADDR, cJSON_CreateString("233.233.2.1:2000"));
		cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_ISPUSHSTREAM, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(temList[i], GET_WEB_RTMP_PARAM_RES_PUSHADDR, cJSON_CreateString(ReqBody));


		cJSON_AddItemToArray(_ParamList, temList[i]);
	}
	cJSON_AddItemToObject(_result, GET_WEB_RTMP_PARAM_RES_TOTAL, cJSON_CreateNumber(atoi("12")));
	cJSON_AddItemToObject(_result, GET_WEB_RTMP_PARAM_RES_RTMPPARAMLIST, _ParamList);
	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	cJSON_Delete(_result);

	return eInterfaceResCodeSuccess;
}

bool CGetWebRtmpParam::GetWebRtmpParamInner(CData& oResult, char* cResult)
{
	mLogDebug("run GetWebRtmpParamInner(...)");

	

	//1. 封装业务请求
	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	
	sprintf(szReqBody, INTERFACE_GETWEBRTMPPARAM_SENDCMD, 1);
	//sprintf(szReqBody, "eth1.conf\n");
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
