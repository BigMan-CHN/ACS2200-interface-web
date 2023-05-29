#include "CSetWebUserPassword.h"
#include <boost/lexical_cast.hpp>

CSetWebUserPassword::CSetWebUserPassword(string ip, int port, int timeOut)
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

bool CSetWebUserPassword::ComposeResult(cJSON *result)
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
    if(_total == NULL){
        mLogWarn("cJSON _total == NULL.");
        return false;
    }

    //result中添加节点
	//cJSON_AddItemToObject(_result,"Total",_total);
    //cJSON_AddItemToObject(_result,"AddressList",_addressList);	
    return true;
}

string CSetWebUserPassword::ComposeResult()
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
bool CSetWebUserPassword::SetWebUserPasswordInner(string UserName, string OldPassword, string NewPassword, string VerifyPassword, CData& oResult, char* cResult)
{
		mLogDebug("run SetWebUserPasswordInner(...)");
	
		char szReqBody[REQ_BODY_MAXLEN] = { 0 };
		//新密码替换成旧密码
		OldPassword = NewPassword;
		if(!strcmp(UserName.c_str(), "admin"))
		{
			sprintf(szReqBody, INTERFACE_SETWEB_PASSWORD_SENDCMD, UserName.c_str(), OldPassword.c_str());
			//sprintf(szReqBody, "web.root_username=%s\nweb.root_passwd=%s\n", UserName.c_str(), OldPassword.c_str());
		}
		else
		{
			//sprintf(szReqBody, INTERFACE_SETWEB_PASSWORD_SENDCMD, UserName.c_str(), OldPassword.c_str());
			sprintf(szReqBody, "web.root_username=%s\nweb.root_passwd=%s\n", UserName.c_str(), OldPassword.c_str());
		}
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
		ResponseCode resCode = _client->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
		if (resCode != eResponseCodeSuccess)
		{
			mLogError("SetWebUserPasswordInner Run Failed !!!");
			return false;
		}
#endif
	
		return true;

}
bool CSetWebUserPassword::GetWebUserPasswordParamInner(string UserName, CData& oResult, char* cResult)
{
		mLogDebug("run GetWebUserPasswordParamInner(...)");
	
		//InterfaceResCode resCode = eInterfaceResCodeSuccess;
	
		//1. 封装业务请求
		char szReqBody[REQ_BODY_MAXLEN] = { 0 };
		if(!strcmp(UserName.c_str(), "admin"))
		{
			sprintf(szReqBody, INTERFACE_GETUSER_OLDPASSWORD_PARAM_SENDCMD);
			//sprintf(szReqBody, "web.root_username\nweb.root_passwd\n");
		}
		else
		{
			//sprintf(szReqBody, INTERFACE_SETWEB_PASSWORD_SENDCMD);
			sprintf(szReqBody, "web.root_passwd\n");
		}	
		//sprintf(szReqBody, INTERFACE_GETUSER_OLDPASSWORD_PARAM_SENDCMD);
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

InterfaceResCode CSetWebUserPassword::GetWebUserPassword(string &Oldpassword,string UserName)
{
	mLogInfo("获取 旧密码...");

	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();
	   //1. 调用类内部方法进一步封装业务请求
	if (!GetWebUserPasswordParamInner(UserName, oResult, cResult))
	{
	   mLogError("Failed to run  GetWebUserPasswordParamInner(...)");
	   return eInterfaceResCodeError;
	}

	 //2. 解析响应,存入类型为cJson对象的成员变量中
	if (!_AnalyParas.ConvertSTRING2CJSONGetWebUserPasswordParam(oResult, cResult, Oldpassword))
	{
	    mLogError("Failed to run  ConvertSTRING2CJSONGetWebAudioParam(...)");
		return eInterfaceResCodeError;
    }
	
	return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetWebUserPassword::SetWebUserPassword(string &sResult,string UserName,string OldPassword,string NewPassword,string VerifyPassword)
{

		mLogInfo("设置 web 视频参数...");
		InterfaceResCode resCode = eInterfaceResCodeSuccess;
		CData oResult = SVSMAP();
		char cResult[RES_BUF_MAXLEN] = { 0 };
		string Oldpasswd;
		//比较新密码和确认密码是否相同
		if(strcmp(NewPassword.c_str(),VerifyPassword.c_str()))
		{
		  	mLogError("SetWebUserPasswordInner NewPassword and VerifyPassword not matching !!!");
			return eInterfaceResCodeError;
		}
		//获得旧密码
		resCode = GetWebUserPassword(Oldpasswd, UserName);
		if(resCode)
		{
		    mLogError("SetWebUserPasswordInner GetWebUserPassword failed !!!");
			return eInterfaceResCodeError;
		}
	
		//mLogDebug("####SetWebUserPass#### !!!"<<OldPassword.c_str());
		//mLogDebug("####SetWebUserPass#### !!!"<<Oldpasswd.c_str());
		//比较旧密码是否相同
		if(strcmp(OldPassword.c_str(),Oldpasswd.c_str()))
		{
		  	mLogError("SetWebUserPasswordInner OldPassword  not same !!!");
			return eInterfaceResCodeError;
		}
		//设置新密码
		if (!SetWebUserPasswordInner(UserName, OldPassword, NewPassword, VerifyPassword, oResult, cResult))
		{
			mLogError("SetWebUserPasswordInner failed !!!");
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
		sResult = ComposeResult();
		cJSON_Delete(_result);

		return eInterfaceResCodeSuccess;

}


