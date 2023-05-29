#include "CGetWebVideoChnList.h"
#include <boost/lexical_cast.hpp>

CGetWebVideoChnList::CGetWebVideoChnList(string ip, int port, int timeOut)
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

bool CGetWebVideoChnList::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

//    if(_addressList == NULL){
//        mLogWarn("cJSON _addressList == NULL.");
//        return false;
//    }
//    if(_total == NULL){
//        mLogWarn("cJSON _total == NULL.");
//        return false;
//    }

    //result中添加节点
	//cJSON_AddItemToObject(_result,"Total",_total);
	//cJSON_AddNumberToObject(_result, "Total", 6);
    
    return true;
}

string CGetWebVideoChnList::ComposeResult()
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
bool CGetWebVideoChnList::GetWebVideoChnListParamInner(CData& oResult, char* cResult, int chn_num)
{
		mLogDebug("run GetWebVideoChnListParamInner(...)");
	
		//InterfaceResCode resCode = eInterfaceResCodeSuccess;
	
		//1. 封装业务请求
		char szReqBody[2048] = { 0 };
		char *ReqBody;
		//获取8个通道的参数
		memset(szReqBody, 0, 2048);
		for(int i = 1; i <= 8; i++)
		{
			ReqBody = szReqBody + strlen(szReqBody);
			sprintf(ReqBody, "vid_cap%d.ch1.osdall\n", i);
//			sprintf(ReqBody, "vid_cap%d.ch1.osd1.s_display\nvid_cap%d.ch1.osd1.content\nvid_cap%d.ch1.osd1.char_set\n", i, i, i);
		}
		ReqBody = szReqBody + strlen(szReqBody);
		sprintf(ReqBody, "comp1.osdall\n");
//		sprintf(ReqBody, "comp1.osd1.s_display\ncomp1.osd1.char_set\ncomp1.osd1.content\n");
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
InterfaceResCode CGetWebVideoChnList::GetWebVideoChnList(string &sResult)
{
	mLogInfo("GetWebVideoChnList...");
	cJSON* temList[15];
	int chn_num = 9;
	string Method = "GetWebVideoChnList";

	char cResult[1024*100] = { 0 };
	CData oResult = SVSMAP();
	 for(int i = 0; i <= chn_num; i++)
	 {
		 temList[i] = cJSON_CreateObject();
	 }

	   //1. 调用类内部方法进一步封装业务请求
	   if (!GetWebVideoChnListParamInner(oResult, cResult,chn_num))
	   {
	  	 mLogError("Failed to run  GetWebVideoChnListParamInner(...)");
		 return eInterfaceResCodeError;
	   }

	    //2. 解析响应,存入类型为cJson对象的成员变量中
	    if (!_AnalyParas.ConvertSTRING2CJSONGetWebVideoChnListParam(oResult, cResult,Method, _result, _addressList, temList, chn_num))
	   {
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebVideoChnListParam(...)");
		return eInterfaceResCodeError;
	   }
	//将节点添加到节点链表中
	for(int i = 1; i <= chn_num; i++)
	{
		cJSON_AddItemToArray(_addressList, temList[i]);	
	} 
	 cJSON_AddItemToObject(_result, "total", cJSON_CreateNumber(chn_num));
	 cJSON_AddItemToObject(_result,"VideoChnList",_addressList);	
	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	cJSON_Delete(_result);

	return eInterfaceResCodeSuccess;

}


