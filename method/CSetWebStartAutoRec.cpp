#include "CSetWebStartAutoRec.h"
#include <boost/lexical_cast.hpp>

CSetWebStartAutoRec::CSetWebStartAutoRec(string ip, int port, int timeOut)
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

bool CSetWebStartAutoRec::ComposeResult(cJSON *result)
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
    //cJSON_AddItemToObject(_result,"MosaicList",_addressList);	
    return true;
}

string CSetWebStartAutoRec::ComposeResult()
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
bool CSetWebStartAutoRec::SetWebStartAutoRecInner(CData& oResult, char* cResult, int StartRec)
{
			mLogDebug("run SetWebStartAutoRecInner(...)");
		
			//InterfaceResCode resCode = eInterfaceResCodeSuccess;
		
			//1. 封装业务请求
			char szReqBody[REQ_BODY_MAXLEN] = { 0 };
			
			sprintf(szReqBody, INTERFACE_SETWEB_START_AUTOREC_SENDCMD, StartRec);
			int realBodySize = strlen(szReqBody);
		
			char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
			szReqCmd[0] = 0;  //不加密
			szReqCmd[1] = 3;  //2Get请求, 3是SET, 4是SYS
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

InterfaceResCode CSetWebStartAutoRec::SetWebStartAutoRec(string &sResult, int StartRec)
{
	   //TODO
			mLogInfo("GetWebMosaicList...");
			
			char cResult[RES_BUF_MAXLEN] = { 0 };
			CData oResult = SVSMAP();

			
			   //1. 调用类内部方法进一步封装业务请求
			   if (!SetWebStartAutoRecInner(oResult, cResult, StartRec))
			   {
				 mLogError("Failed to run  SetWebStartAutoRecInner(...)");
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
		

			//3. 将step2中获取的响应格式化为Json字符串
			sResult = ComposeResult();
			cJSON_Delete(_result);
			return eInterfaceResCodeSuccess;



#if 0	
	cJSON *listnode1 = NULL;
	cJSON *listnode2 = NULL;
	
	listnode1 = cJSON_CreateObject();
	listnode2 = cJSON_CreateObject();
	
	cJSON_AddNumberToObject(listnode1, "MosaicChn", 1);	
	cJSON_AddNumberToObject(listnode1, "MosaicPost", 0);
	cJSON_AddNumberToObject(listnode1, "MosaicNum", 8);
	cJSON_AddNumberToObject(listnode1, "MosaicType", 3);
	
	cJSON_AddNumberToObject(listnode2, "MosaicChn", 1);	
	cJSON_AddNumberToObject(listnode2, "MosaicPost", 0);
	cJSON_AddNumberToObject(listnode2, "MosaicNum", 7);
	cJSON_AddNumberToObject(listnode2, "MosaicType", 3);
	

	cJSON_AddNumberToObject(_result, "Total", 6);
	cJSON_AddItemToArray(_addressList, listnode1);
    cJSON_AddItemToArray(_addressList, listnode2);
#endif

}


