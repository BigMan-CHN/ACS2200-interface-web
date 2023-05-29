#include "CGetWebDevID.h"
#include <boost/lexical_cast.hpp>

CGetWebDevID::CGetWebDevID(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_total(cJSON_CreateObject())
    ,_ParamList(cJSON_CreateArray())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CGetWebDevID::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    if(_total == NULL){
        mLogWarn("cJSON _total == NULL.");
        return false;
    }

    //result中添加节点
	//cJSON_AddItemToObject(_result,"Total",_total);
	//cJSON_AddNumberToObject(_result, "Total", 6);
    //cJSON_AddItemToObject(_result,"VideoChnList",_addressList);	
    return true;
}

string CGetWebDevID::ComposeResult()
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
bool CGetWebDevID::GetWebDevIDParamInner(CData& oResult, char* cResult)
{
		mLogDebug("run GetWebDevIDParamInner(...)");
	
		//InterfaceResCode resCode = eInterfaceResCodeSuccess;
	
		//1. 封装业务请求
		char szReqBody[REQ_BODY_MAXLEN] = { 0 };
		
		sprintf(szReqBody, INTERFACE_GETWEB_SYS_DVEID_SENDCMD);
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
InterfaceResCode CGetWebDevID::GetWebDevID(string &sResult)
{
	mLogInfo("获取 DEV ID...");
	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();
	string Method = "GetWebDevID";
	string ip;
	   //1. 调用类内部方法进一步封装业务请求
	if (!GetWebDevIDParamInner(oResult, cResult))
	{
	  	 mLogError("Failed to run  GetWebDevIDParamInner(...)");
		 return eInterfaceResCodeError;
	}

	  //2. 解析响应,存入类型为cJson对象的成员变量中
    //if (!_AnalyParas.ConvertSTRING2CJSONGetWebDevIDParam(oResult, cResult, _result, _addressList))
	if (!_AnalyParas.ConvertSTRING2CJSONGetWebParam( oResult,  cResult, _result, _ParamList, NULL, Method, ip, 0))
    {
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebParam(...)");
	    return eInterfaceResCodeError;
    }
		
	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	cJSON_Delete(_result);

	return eInterfaceResCodeSuccess;

}


