#include "GetWebVideoParam.h"
#include "MPSClient.h"

CGetWebVideoParam::CGetWebVideoParam(const std::string& ip, unsigned short port, int timeOut)
	:_result(cJSON_CreateObject())
	,_videoParamList(cJSON_CreateArray())
	,_node(cJSON_CreateObject())
	, _mpsClient(new MPSClient(ip, port, timeOut))
{

}

CGetWebVideoParam::~CGetWebVideoParam()
{
}

bool CGetWebVideoParam::ComposeResult(cJSON* result)
{
 	if (_result == NULL || result == NULL) {
		mLogError("cJSON result == NULL.");
 		result = NULL;
 		return false;
 	}
 
 	if (_videoParamList == NULL) {
 		mLogWarn("cJSON _videoParamList == NULL.");
 		return false;
 	}
 
	return true;
}

std::string CGetWebVideoParam::ComposeResult()
{
 	cJSON* result = cJSON_CreateObject();
 	if (ComposeResult(result) == false) {
 		cJSON_Delete(result);
 		return "";
 	}
 	else {
 		char* cRes = cJSON_Print(_result);
 		std::string res = cRes;
 		cJSON_Delete(result);
 		return res;
 	}
/*
	if (_videoParamList == NULL)
	{
		mLogError("_videoParamList is NULL !!!");
		return "";
	}

	cJSON_AddItemToObject(_result, GET_WEB_VIDEO_PARAM_RES_VIDEOPARAMLIST, _videoParamList);

	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	//cJSON_Delete(result);
	return res;
*/	
}

InterfaceResCode CGetWebVideoParam::GetWebVideoParam(string& sResult)
{
	mLogInfo("获取 GetWebVideoParam视频参数...");
	int chn_num = 12;
	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();
	cJSON* temList[15];
	string Method = "GetWebVideoParam";
	string ip;
	//用于生成节点，存储解析后的参数
    for(int i = 1; i <= chn_num; i++)
    {
		temList[i] = cJSON_CreateObject();
    }
	//1. 调用类内部方法进一步封装业务请求
	  if (!GetWebVideoParamInner(oResult, cResult,chn_num))
	  {
		mLogError("Failed to run  GetWebVideoParamInner(...)");
		return eInterfaceResCodeError;
	  }
#if 0	  
	  	int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
		printf("getMPSConfig\n");
		for (int i = 0; i < iRecvLength; i++)
		{
			if (i < 4)
			{
				printf("%d", cResult[i]);
			}
			else 
			{
				printf("%c", cResult[i]);
			}
		}
#endif	  
	  //2. 解析响应,存入类型为cJson对象的成员变量中
	  if (!_analyPara.ConvertSTRING2CJSONGetWebVideoParam(oResult, cResult, _result, _videoParamList, temList, Method, ip, chn_num)) {
	  	  mLogError("Failed to run  ConvertSTRING2CJSONGetWebVideoParam(...)");
		  return eInterfaceResCodeError;
	  } 
  	//将节点添加到节点链表中
	for(int i = 1; i <= chn_num; i++)
	{

		cJSON_AddItemToArray(_videoParamList, temList[i]);	
					
	} 
	 //result中添加节点
	cJSON_AddItemToObject(_result, "total", cJSON_CreateNumber((chn_num)));
 	cJSON_AddItemToObject(_result, GET_WEB_VIDEO_PARAM_RES_VIDEOPARAMLIST, _videoParamList);
	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	//删除根节点，释放资源
	cJSON_Delete(_result);

	return eInterfaceResCodeSuccess;
}

bool CGetWebVideoParam::GetWebVideoParamInner(CData& oResult, char* cResult,int chn_num)
{
	mLogDebug("run GetWebVideoParamInner(...)");

	//InterfaceResCode resCode = eInterfaceResCodeSuccess;

	//1. 封装业务请求
	char szReqBody[2048] = { 0 };
	char *ReqBody;
	//获取8个通道的参数
	memset(szReqBody, 0, 2048);
	sprintf(szReqBody,"vid_enc100.all\n");
//	for(int i = 1; i <= chn_num; i++)
//	{
//		ReqBody = szReqBody + strlen(szReqBody);
//		sprintf(ReqBody,"vid_enc%d.channel\nvid_enc%d.resolution\nvid_enc%d.vid_payload\nvid_enc%d.bitrate_mode\nvid_enc%d.bitrate\nvid_enc%d.fps\nvid_enc%d.gop\n",i,i,i,i,i,i,i);
//	}
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

#endif	
	return true;
}
