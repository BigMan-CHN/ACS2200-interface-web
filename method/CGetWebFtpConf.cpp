#include "CGetWebFtpConf.h"
#include <boost/lexical_cast.hpp>

CGetWebFtpConf::CGetWebFtpConf(string ip, int port, int timeOut)
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

bool CGetWebFtpConf::ComposeResult(cJSON *result)
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

    //result中添加节点
	//cJSON_AddItemToObject(_result,"Total",_total);
    //cJSON_AddItemToObject(_result,"AddressList",_addressList);	
    return true;
}

string CGetWebFtpConf::ComposeResult()
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
bool CGetWebFtpConf::GetWebFtpConfParamInner(CData& oResult, char* cResult)
{
	mLogDebug("run GetWebFtpConfParamInner(...)");
			
	int netport = 1;
	//1. 封装业务请求
	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
				
	sprintf(szReqBody, INTERFACE_GETWEB_FTP_CONF_SENDCMD,netport);
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

InterfaceResCode CGetWebFtpConf::GetWebFtpConf(string &sResult)
{
	mLogInfo("获取 Ftp 参数...");

	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();
	string Method = "GetWebFtpConf";

	//1. 调用类内部方法进一步封装业务请求
	if (!GetWebFtpConfParamInner(oResult, cResult))
	{
	  mLogError("Failed to run  GetWebFtpConfParamInner(...)");
      return eInterfaceResCodeError;
	}

	//2. 解析响应,存入类型为cJson对象的成员变量中
	if (!_AnalyParas.ConvertSTRING2CJSONGetWebFtpConfListParam(oResult, cResult, _result, _addressList, Method, LocallIP))
	{
	   mLogError("Failed to run  ConvertSTRING2CJSONGetWebFtpConfListParam(...)");
	   return eInterfaceResCodeError;
	}
	cJSON_AddItemToObject(_result, "FtpServerIp", cJSON_CreateString(LocallIP.c_str()));
	cJSON_AddItemToObject(_result, "FtpName", cJSON_CreateString("ftp"));
	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	cJSON_Delete(_result);

	return eInterfaceResCodeSuccess;

}


