#include "CGetWebVideoOutPutConf.h"
#include <boost/lexical_cast.hpp>

CGetWebVideoOutPutConf::CGetWebVideoOutPutConf(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_total(cJSON_CreateObject())
    ,_addressList(cJSON_CreateArray())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CGetWebVideoOutPutConf::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    if(_addressList == NULL){
        mLogWarn("cJSON _addressList == NULL.");
        return false;
    }


    return true;
}

string CGetWebVideoOutPutConf::ComposeResult()
{
    cJSON * result = cJSON_CreateObject();
    if(ComposeResult(result) == false){
        cJSON_Delete(result);
        return "";
    }else{
        char *cRes = cJSON_Print(_result);
        string res = cRes;
        cJSON_Delete(result);
        return res;
    }
}
bool CGetWebVideoOutPutConf::GetWebVideoOutPutConfParamInner(CData& oResult, char* cResult, int chn_num)
{
			mLogDebug("run GetWebVideoOutPutConfParamInner(...)");
		
			//InterfaceResCode resCode = eInterfaceResCodeSuccess;
		
			//1. 封装业务请求
			char szReqBody[REQ_BODY_MAXLEN] = { 0 };
//			sprintf(szReqBody, "vid_dis_hd%d.luminance\nvid_dis_hd%d.contrast\nvid_dis_hd%d.hue\nvid_dis_hd%d.satuature\nvid_dis_hd%d.resolution\n", chn_num, chn_num, chn_num, chn_num, chn_num);
			sprintf(szReqBody, "vid_dis_hd1.all\n");
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
			ResponseCode resCode = _client->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
			if (resCode != eResponseCodeSuccess) {
				mLogError("GetConfig(...) error:" << resCode);
				return false;
			}
#else
			//dummy response
#endif
		
			return true;

}

InterfaceResCode CGetWebVideoOutPutConf::GetWebVideoOutPutConf(string &sResult)
{
	mLogInfo("GetWebVideoOutPutConf...");
	cJSON* temList[10];
	int chn_num = 2;

	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();
	string Method = "GetWebVideoOutPutConf";
	string IP,ChnName;
	for(int i = 1; i <= chn_num; i++)
	{
		 //用于生成节点，存储解析后的参数
		 temList[i] = cJSON_CreateObject();
	}


   //1. 调用类内部方法进一步封装业务请求
   if (!GetWebVideoOutPutConfParamInner(oResult, cResult,chn_num))
   {
  	 mLogError("Failed to run  GetWebVideoOutPutConfParamInner(...)");
	 return eInterfaceResCodeError;
   }
	 //2. 解析响应,存入类型为cJson对象的成员变量中
	if (!_AnalyParas.ConvertSTRING2CJSONGetWebParam(oResult, cResult, _result, _addressList, temList, Method, IP, chn_num))
	{
	 mLogError("Failed to run  ConvertSTRING2CJSONGetWebVideoOutPutConfListParam(...)");
	 return eInterfaceResCodeError;
	}

//    //2. 解析响应,存入类型为cJson对象的成员变量中
//   if (!_AnalyParas.ConvertSTRING2CJSONGetWebVideoOutPutConfListParam(oResult, cResult, _result, _addressList,temList,chn_num))
//   {
//	mLogError("Failed to run  ConvertSTRING2CJSONGetWebVideoOutPutConfListParam(...)");
//	return eInterfaceResCodeError;
//   }

	for(int i = 1; i <= chn_num; i++)
	{
		 switch(i)
		 {
		   case 1:
				ChnName = "VGA";
				break;
		   case 2:
				ChnName = "HDMI";
				break;
		   default:
				mLogError("please input the right Resolution!!!\n");
				return eInterfaceResCodeError;
		 }
			//添加通道号信息，对应发送命令中的通道号
			cJSON_AddItemToObject(temList[i], "ChnName", cJSON_CreateString(ChnName.c_str()));
			cJSON_AddItemToArray(_addressList, temList[i]);
	 }
	 cJSON_AddItemToObject(_result, "total", cJSON_CreateNumber((chn_num)));
	//result中添加节点
    cJSON_AddItemToObject(_result,"OutPutList",_addressList);	
	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	cJSON_Delete(_result);

	return eInterfaceResCodeSuccess;

}


