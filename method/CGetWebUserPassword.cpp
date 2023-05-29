#include "CGetWebUserPassword.h"
#include <boost/lexical_cast.hpp>

CGetWebUserPassword::CGetWebUserPassword(string ip, int port, int timeOut)
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

bool CGetWebUserPassword::ComposeResult(cJSON *result)
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

string CGetWebUserPassword::ComposeResult()
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

InterfaceResCode CGetWebUserPassword::GetWebUserPassword(string &sResult)
{
    //TODO
    mLogInfo("获取添加后的通讯录列表信息...");
		CData oResult = SVSMAP();
		char cResult[4096] = {0};
		//printf("SetDecCtrlStart Dectype %d Channal %d CacheTime %d sAddr %s TcpMode %d\n", Dectype, Channal, CacheTime, sAddr.c_str(), TcpMode);
#if 0
		if(SetWebExtendNetCfg(Ip, GateWay, NetMask, MacAddr, MainDNS, BackupDNS, oResult, cResult) == false){
			mLogError("Failed to run  getMPSConfig(...)");
			return eInterfaceResCodeError;
		}
#endif 
		
		;
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
	
#if 0
		if( _AnalyParas.ConvertSTRING2CJSONSetWebExtendCtrl(oResult, cResult) == false){
			mLogError("Failed to run  ConvertSTRING2CJSONSetWebExtendCtrl(...)");
			return eInterfaceResCodeError;
		}
#endif 

	cJSON_AddStringToObject(_result, "UserName", "admin");
	cJSON_AddStringToObject(_result, "OldPassword", "12345");
	cJSON_AddStringToObject(_result, "NewPassword", "admin");
	cJSON_AddStringToObject(_result, "VerifyPassword", "admin");


    sResult = ComposeResult();
	cJSON_Delete(_result);
    return eInterfaceResCodeSuccess;
}


