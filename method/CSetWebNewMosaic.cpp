#include "CSetWebNewMosaic.h"
#include <boost/lexical_cast.hpp>

CSetWebNewMosaic::CSetWebNewMosaic(string ip, int port, int timeOut)
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

bool CSetWebNewMosaic::ComposeResult(cJSON *result)
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
   // cJSON_AddItemToObject(_result,"AddressList",_addressList);	
    return true;
}

string CSetWebNewMosaic::ComposeResult()
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
bool CSetWebNewMosaic::SetWebMosaicParamInner(CData& oResult, char* cResult, string MosaicChn,int MosaicDisplay,int MosaicNum,int MosaicType, string MosaicWinX, string MosaicWinY, string MosaicWinH, string MosaicWinW)
{
				mLogDebug("run SetWebNewMosaicParamInner(...)");
			
				//InterfaceResCode resCode = eInterfaceResCodeSuccess;
			
				//1. 封装业务请求
				char szReqBody[REQ_BODY_MAXLEN] = { 0 };
				
				sprintf(szReqBody, INTERFACE_SETWEB_MASIC_SENDCMD, MosaicNum, MosaicDisplay, MosaicNum, MosaicChn.c_str(), MosaicNum, MosaicWinX.c_str(), MosaicWinY.c_str(), MosaicWinW.c_str(), MosaicWinH.c_str());
				int realBodySize = strlen(szReqBody);
			
				char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
				szReqCmd[0] = 0;  //不加密
				szReqCmd[1] = 3;  //Get请求, 3是SET, 4是SYS
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

InterfaceResCode CSetWebNewMosaic::SetWebMosaic(string &sResult,string MosaicChn,int MosaicDisplay,int MosaicNum,int MosaicType, string MosaicWinX, string MosaicWinY, string MosaicWinH, string MosaicWinW)
{
		   //TODO
				mLogInfo("SetWebNewMosaic...");
				cJSON* temList[10];
				int masic_num;
				
				char cResult[RES_BUF_MAXLEN] = { 0 };
				CData oResult = SVSMAP();
				
				   //1. 调用类内部方法进一步封装业务请求
				   if (!SetWebMosaicParamInner(oResult, cResult,MosaicChn, MosaicDisplay, MosaicNum, MosaicType,  MosaicWinX, MosaicWinY, MosaicWinH, MosaicWinW))
				   {
					 mLogError("Failed to run  SetWebNewMosaicParamInner(...)");
					 return eInterfaceResCodeError;
				   }
			
				//将step2中获取的响应格式化为Json字符串
				sResult = ComposeResult();
				cJSON_Delete(_result);
				return eInterfaceResCodeSuccess;
	
#if 0

	cJSON *listnode1 = NULL;
	cJSON *listnode2 = NULL;
	
	listnode1 = cJSON_CreateObject();
	listnode2 = cJSON_CreateObject();
	
	cJSON_AddNumberToObject(listnode1, "Num", 1);
	cJSON_AddStringToObject(listnode1, "Title", "192.168.1.164");
	cJSON_AddStringToObject(listnode1, "InterAddr", "rtsp://192.168.1.164/3");
	cJSON_AddNumberToObject(listnode1, "InterProtocol", 0);
	cJSON_AddNumberToObject(listnode1, "CallRate", 2048);
	cJSON_AddNumberToObject(listnode1, "LengthBuff", 300);
	cJSON_AddNumberToObject(listnode1, "InterOptions", 0);
	
	cJSON_AddNumberToObject(listnode2, "Num", 1);
	cJSON_AddStringToObject(listnode2, "Title", "192.168.1.164");
	cJSON_AddStringToObject(listnode2, "InterAddr", "rtsp://192.168.1.164/3");
	cJSON_AddNumberToObject(listnode2, "InterProtocol", 0);
	cJSON_AddNumberToObject(listnode2, "CallRate", 2048);
	cJSON_AddNumberToObject(listnode2, "LengthBuff", 300);
	cJSON_AddNumberToObject(listnode2, "InterOptions", 0);

	cJSON_AddItemToArray(_addressList, listnode1);
    cJSON_AddItemToArray(_addressList, listnode2);
#endif 
}
bool CSetWebNewMosaic::SetWebNewMosaicParamInner(CData& oResult, char* cResult,int ArryNumbers,string MosaicChn[],int MosaicDisplay[],int MosaicNum[],int MosaicType[], string MosaicWinX[], string MosaicWinY[], string MosaicWinH[], string MosaicWinW[])
{
	mLogDebug("run SetWebMosaicParamInner(...)");

	//InterfaceResCode resCode = eInterfaceResCodeSuccess;

	//1. 封装业务请求
	char szReqBody[2048] = { 0 };
	char *ReqBody;
	//防止马赛克位置叠加后，马赛克设置失败
//	sprintf(szReqBody, "mosaic1.s_display=0\nmosaic2.s_display=0\nmosaic3.s_display=0\nmosaic4.s_display=0\n");
//	sprintf(szReqBody, "mosaic1.win=1/99,1/99,10/99,10/99\nmosaic2.win=98/99,1/99,10/99,10/99\nmosaic3.win=1/99,98/99,10/99,10/99\nmosaic4.win=98/99,98/99,10/99,10/99\n");
//	for(int i = 0; i < ArryNumbers; i++)
//	{
//		ReqBody = szReqBody + strlen(szReqBody);
//		sprintf(ReqBody, "mosaic%d.s_display=0\n", MosaicNum[i]);
//	}
	
//	for(int i = 0; i < 4; i++)
//	{
//		ReqBody = szReqBody + strlen(szReqBody);
//		sprintf(ReqBody, "mosaic%d.s_display=0\n", MosaicNum[0]+i);
//	}

	for(int i = 0; i < ArryNumbers; i++)
	{
		ReqBody = szReqBody + strlen(szReqBody);
		sprintf(ReqBody, "mosaic%d.s_display=%d\nmosaic%d.channel=%s\nmosaic%d.win=%s,%s,%s,%s\n", MosaicNum[i], MosaicDisplay[i], MosaicNum[i], MosaicChn[i].c_str(), MosaicNum[i], MosaicWinX[i].c_str(), MosaicWinY[i].c_str(), MosaicWinW[i].c_str(), MosaicWinH[i].c_str());
	}
	for(int i = 0; i < ArryNumbers; i++)
	{
		ReqBody = szReqBody + strlen(szReqBody);
		sprintf(ReqBody, "mosaic%d.s_display=%d\nmosaic%d.channel=%s\nmosaic%d.win=%s,%s,%s,%s\n", MosaicNum[i], MosaicDisplay[i], MosaicNum[i], MosaicChn[i].c_str(), MosaicNum[i], MosaicWinX[i].c_str(), MosaicWinY[i].c_str(), MosaicWinW[i].c_str(), MosaicWinH[i].c_str());
	}
	//sprintf(szReqBody, "mosaic%d.s_display=%d\nmosaic%d.channel=%s\nmosaic%d.win=%s,%s,%s,%s\n", MosaicNum, MosaicDisplay, MosaicNum, MosaicChn.c_str(), MosaicNum, MosaicWinX.c_str(), MosaicWinY.c_str(), MosaicWinW.c_str(), MosaicWinH.c_str());
	int realBodySize = strlen(szReqBody);
	mLogDebug("SetWebMosaicParamInner sendcmd = "<<szReqBody);
	char szReqCmd[1024] = { 0 };
	szReqCmd[0] = 0;  //不加密
	szReqCmd[1] = 3;  //Get请求, 3是SET, 4是SYS
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

InterfaceResCode CSetWebNewMosaic::SetWebNewMosaic(string &sResult,int ArryNumbers,string MosaicChn[],int MosaicDisplay[],int MosaicNum[],int MosaicType[], string MosaicWinX[], string MosaicWinY[], string MosaicWinH[], string MosaicWinW[])
{
		   //TODO
	mLogInfo("SetWebNewMosaic...");
	cJSON* temList[10];
	int masic_num;
	
	char cResult[1024] = { 0 };
	CData oResult = SVSMAP();
	
   //1. 调用类内部方法进一步封装业务请求
   if (!SetWebNewMosaicParamInner(oResult, cResult, ArryNumbers, MosaicChn, MosaicDisplay, MosaicNum, MosaicType,  MosaicWinX, MosaicWinY, MosaicWinH, MosaicWinW))
   {
	 mLogError("Failed to run  SetWebNewMosaicParamInner(...)");
	 return eInterfaceResCodeError;
   }

	//将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;

}


