#include "CGetWebWhineList.h"
#include <boost/lexical_cast.hpp>

CGetWebWhineList::CGetWebWhineList(string ip, int port, int timeOut)
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

bool CGetWebWhineList::ComposeResult(cJSON *result)
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
    cJSON_AddItemToObject(_result,"WhineList",_addressList);	
    return true;
}

string CGetWebWhineList::ComposeResult()
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

InterfaceResCode CGetWebWhineList::GetWebWhineList(string &sResult,int WhineChn,int WhineType)
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

	
	cJSON *listnode1 = NULL;
	cJSON *listnode2 = NULL;
	
	listnode1 = cJSON_CreateObject();
	listnode2 = cJSON_CreateObject();
	
	cJSON_AddNumberToObject(listnode1, "WhineChn", 1);
	cJSON_AddNumberToObject(listnode1, "WhineType", 0);

	cJSON_AddNumberToObject(listnode2, "WhineChn", 1);
	cJSON_AddNumberToObject(listnode2, "WhineType", 1);



	cJSON_AddNumberToObject(_result, "Total", 6);
	cJSON_AddItemToArray(_addressList, listnode1);
    cJSON_AddItemToArray(_addressList, listnode2);

	

    sResult = ComposeResult();
	cJSON_Delete(_result);
    return eInterfaceResCodeSuccess;
}


