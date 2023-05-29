#include "CGetWebMosaicList.h"
#include <boost/lexical_cast.hpp>

CGetWebMosaicList::CGetWebMosaicList(string ip, int port, int timeOut)
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

bool CGetWebMosaicList::ComposeResult(cJSON *result)
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
//    cJSON_AddItemToObject(_result,"MosaicList",_addressList);	
    return true;
}

string CGetWebMosaicList::ComposeResult()
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
bool CGetWebMosaicList::GetWebMosaicListParamInner(CData& oResult, char* cResult, int MosaicNum)
{
			mLogDebug("run GetWebMosaicListParamInner(...)");
		
			//InterfaceResCode resCode = eInterfaceResCodeSuccess;
		
			//1. 封装业务请求
			char szReqBody[REQ_BODY_MAXLEN] = { 0 };
			
			sprintf(szReqBody, INTERFACE_GETWEB_MASIC_SENDCMD, MosaicNum, MosaicNum, MosaicNum, MosaicNum);
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

InterfaceResCode CGetWebMosaicList::GetWebMosaicList(string &sResult)
{
	   //TODO
			mLogInfo("GetWebMosaicList...");
			cJSON* temList[10];
			int masic_num;
			
			char cResult[RES_BUF_MAXLEN] = { 0 };
			CData oResult = SVSMAP();
			 for(masic_num=1;masic_num<3;masic_num++)
			{
			
			   //1. 调用类内部方法进一步封装业务请求
			   if (!GetWebMosaicListParamInner(oResult, cResult, masic_num))
			   {
				 mLogError("Failed to run  GetWebCaptionListParamInner(...)");
				 return eInterfaceResCodeError;
			   }
				//用于生成节点，存储解析后的参数
				temList[masic_num] = cJSON_CreateObject();
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
		
				//2. 解析响应,存入类型为cJson对象的成员变量中
				if (!_AnalyParas.ConvertSTRING2CJSONGetWebMosaicListParam(oResult, cResult, _result, _addressList,temList[masic_num],masic_num))
			   {
				mLogError("Failed to run  ConvertSTRING2CJSONGetWebMosaicListParam(...)");
				return eInterfaceResCodeError;
			   }
			}
			 cJSON_AddItemToObject(_result, "total", cJSON_CreateNumber((masic_num-1)));
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
bool CGetWebMosaicList::GetWebNewMosaicListParamInner(CData& oResult, char* cResult, int MosaicNum)
{
			mLogDebug("run GetWebNewMosaicListParamInner(...)");
		
			//InterfaceResCode resCode = eInterfaceResCodeSuccess;
		
			//1. 封装业务请求
			char szReqBody[2048] = { 0 };
			char *ReqBody;
			sprintf(szReqBody, "mosaic%d.all\n", MosaicNum-3);
//			for(int i = MosaicNum-3; i <= MosaicNum; i++)
//			{
//				ReqBody = szReqBody + strlen(szReqBody);
//				sprintf(ReqBody, "mosaic%d.s_display\nmosaic%d.channel\nmosaic%d.mode\nmosaic%d.win=win_x,win_y,win_w,win_h\n", i, i, i, i);
//			}
//			mLogDebug("szReqBody:"<<szReqBody);
			//sprintf(szReqBody, INTERFACE_GETWEB_MASIC_SENDCMD, MosaicNum, MosaicNum, MosaicNum, MosaicNum);
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
			mLogDebug("szReqBody:"<<cResult[5]);
			if (resCode != eResponseCodeSuccess) {
				mLogError("GetConfig(...) error:" << resCode);
				return false;
			}
#else
			//dummy response
#endif
		
			return true;

}

InterfaceResCode CGetWebMosaicList::GetWebNewMosaicList(string &sResult, int ChnnelID)
{
	//TODO
	mLogInfo("GetWebNewMosaicList...");
	cJSON* temList[40];
	int masic_num;
	
	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();
	string Method = "GetWebMosaicList";
	string ip;
	masic_num = ChnnelID * 4;//返回当前预览通道的马赛克参数
	for(int i = masic_num-3; i <= masic_num; i++)
	{
		//用于生成节点，存储解析后的参数
		temList[i] = cJSON_CreateObject();
	}	
   //1. 调用类内部方法进一步封装业务请求
   	if(!GetWebNewMosaicListParamInner(oResult, cResult, masic_num))
   	{
		 mLogError("Failed to run  GetWebCaptionListParamInner(...)");
		 return eInterfaceResCodeError;
   	}

#if 0
	int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
	printf("#######getMPSConfig######## iRecvLength= %d\n",iRecvLength);
	for (int i = 0; i < iRecvLength; i++)
	{
		if (i < 10)
		{
			printf("%d", cResult[i]);
		}
		else 
		{
			printf("%c", cResult[i]);
		}
	}
#endif 

	//2. 解析响应,存入类型为cJson对象的成员变量中
//	if(!_AnalyParas.ConvertSTRING2CJSONGetWebMosaicListParam(oResult, cResult, _result, _addressList,temList[masic_num],masic_num))
//    {
//		mLogError("Failed to run  ConvertSTRING2CJSONGetWebMosaicListParam(...)");
//		return eInterfaceResCodeError;
//    }
	
	if(!_AnalyParas.ConvertSTRING2CJSONGetWebParam(oResult, cResult, _result, _addressList, temList, Method, ip, masic_num))
    {
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebParam(...)");
		return eInterfaceResCodeError;
    }
	//将获得的一组通道信息添加到_videoParamList节点
	for(int i = masic_num-3; i <= masic_num; i++)
	{
		cJSON_AddItemToArray(_addressList, temList[i]);					
	} 
	cJSON_AddItemToObject(_result, "total", cJSON_CreateNumber(masic_num));
	cJSON_AddItemToObject(_result,"MosaicList",_addressList);	
	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;

}


