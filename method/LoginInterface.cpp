#include "LoginInterface.h"
#include <boost/lexical_cast.hpp>

LoginInterface::LoginInterface(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_params(cJSON_CreateObject())
    ,_channal(cJSON_CreateArray())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool LoginInterface::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    if(_channal == NULL){
        mLogWarn("cJSON network == NULL.");
        return false;
    }

    //result中添加节点

    //cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_PARAMS,_params);
	//cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_WEB_CHN,_channal);
    return true;
}

string LoginInterface::ComposeResult()
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
bool LoginInterface::LoginCheckPasswordParamInner(CData& oResult, char* cResult,string &name,string &password)
{
		mLogDebug("run LoginCheckPasswordParamInner(...)");
		
		//1. 封装业务请求
		char szReqBody[REQ_BODY_MAXLEN] = { 0 };
		if(!strcmp(name.c_str(), "admin"))
		{
			sprintf(szReqBody, INTERFACE_GETUSERNAME_PASSWORD_PARAM_SENDCMD);
			//sprintf(szReqBody, "web.root_username\nweb.root_passwd\n");
		}
		else
		{
			//sprintf(szReqBody, INTERFACE_SETWEB_PASSWORD_SENDCMD);
			sprintf(szReqBody, "web.root_username\nweb.root_passwd\n");
		}	
		//sprintf(szReqBody, INTERFACE_GETUSERNAME_PASSWORD_PARAM_SENDCMD);
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

InterfaceResCode LoginInterface::LoginCheck(string &sResult,string &name,string &password)
{
    //TODO
    mLogInfo("用户登录界面验证...");
	
	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();
	string oldpassword,username;
    //1. 调用类内部方法进一步封装业务请求
	if (!LoginCheckPasswordParamInner(oResult, cResult, name, password))
	{
		  mLogError("Failed to run  GetWebUserPasswordParamInner(...)");
		  return eInterfaceResCodeError;
	}
	 //2. 解析响应,存入类型为cJson对象的成员变量中
	if (!_AnalyParas.ConvertSTRING2CJSONLoginCheckParam(oResult, cResult, oldpassword, username))
	{
	    mLogError("Failed to run  ConvertSTRING2CJSONLoginCheckParam(...)");
		return eInterfaceResCodeError;
    }
	//printf("username=%s ,name=%s , oldpassword=%s, password=%s/n",username.c_str(),name.c_str(),oldpassword.c_str(),password.c_str());
	//比较用户名
	if(strcmp(username.c_str(),name.c_str()))
	{
		mLogError("LoginCheck username  not matching !!!");
		//cJSON_AddItemToObject(_result, AUDIO_CHN_CONF_PARAMS_VAL, cJSON_CreateNumber(atoi(arg[i+4])));
		return eInterfaceResCodeUserNameError;
	}
	//比较密码
	if(strcmp(oldpassword.c_str(),password.c_str()))
	{
		mLogError("LoginCheck password  not matching !!!");
		return eInterfaceResCodePassWordError;
	}

    sResult = ComposeResult();
	cJSON_Delete(_result);
    return eInterfaceResCodeSuccess;
}


