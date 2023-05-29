#include "CGetWebCaptionList.h"
#include <boost/lexical_cast.hpp>

CGetWebCaptionList::CGetWebCaptionList(string ip, int port, int timeOut)
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

bool CGetWebCaptionList::ComposeResult(cJSON *result)
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
    cJSON_AddItemToObject(_result,"CaptionList",_addressList);	
    return true;
}

string CGetWebCaptionList::ComposeResult()
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
bool CGetWebCaptionList::GetWebCaptionListParamInner(CData& oResult, char* cResult, int OverlayChn, int OverlayNum, int SelectType)
{
		mLogDebug("run GetWebCaptionListParamInner(...)");
	
		//InterfaceResCode resCode = eInterfaceResCodeSuccess;
	
		//1. 封装业务请求
		char szReqBody[2048] = { 0 };
		char *ReqBody;
		if(SelectType == 0)
		{
			sprintf(szReqBody,"vid_cap%d.ch1.osdall\n",OverlayChn);
//			memset(szReqBody, 0, 2048);
//			for(int i = 2; i <= OverlayNum; i++)
//			{
//				ReqBody = szReqBody + strlen(szReqBody);
//				sprintf(ReqBody,  "vid_cap%d.ch1.osd%d.char_set\nvid_cap%d.ch1.osd%d.type\nvid_cap%d.ch1.osd%d.s_display\nvid_cap%d.ch1.osd%d.position\nvid_cap%d.ch1.osd%d.content\nvid_cap%d.ch1.osd%d.color\nvid_cap%d.ch1.osd%d.size\n", 
//					OverlayChn, i,OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i);
//			}
		}
		else if(SelectType == 1)
		{
			sprintf(szReqBody, "comp%d.osdall\n",OverlayChn);
//			memset(szReqBody, 0, 2048);
//			for(int i = 2; i <= OverlayNum; i++)
//			{
//				ReqBody = szReqBody + strlen(szReqBody);
//				sprintf(ReqBody, "comp%d.osd%d.type\ncomp%d.osd%d.s_display\ncomp%d.osd%d.position\ncomp%d.osd%d.content\ncomp%d.osd%d.x\ncomp%d.osd%d.y\ncomp%d.osd%d.w\ncomp%d.osd%d.h\ncomp%d.osd%d.char_set\ncomp%d.osd%d.color\ncomp%d.osd%d.size\n",
//					OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i);
//			}

		}
		else if(SelectType == 2)
		{
			memset(szReqBody, 0, 2048);
			for(int i = 2; i <= OverlayNum; i++)
			{
				ReqBody = szReqBody + strlen(szReqBody);
				sprintf(ReqBody, "vid_enc%d.osd_on\nvid_enc%d.osd%d.s_display\nvid_enc%d.osd%d.char_set\nvid_enc%d.osd%d.position\nvid_enc%d.osd%d.type\nvid_enc%d.osd%d.color\nvid_enc%d.osd%d.size\nvid_enc%d.osd%d.content\n",\
					OverlayChn, OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i, OverlayChn, i);

				ReqBody = szReqBody + strlen(szReqBody);
				sprintf(ReqBody, "vid_enc%d.substream.osd_on\nvid_enc%d.superstream.osd_on\n",OverlayChn, OverlayChn);


			}

		}
		else
		{
			mLogError("GetWebCaptionListParamInner SelectType = " << SelectType);
			return false;
		}
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

InterfaceResCode CGetWebCaptionList::GetWebCaptionList(string &sResult, int OverlayChn, int SelectType)
{
    //TODO
    mLogInfo("GetWebCaptionList...");
		cJSON* temList[10];
		int osd_num = 5;
		int comp_osd_num = 5;
		char cResult[RES_BUF_MAXLEN] = { 0 };
		CData oResult = SVSMAP();
		string Method = "GetWebCaptionList";
	if(SelectType == 0)
	{
		for(int i = 2; i <= osd_num; i++)
	 	{
		 	temList[i] = cJSON_CreateObject();
	 	}
		 //for(osd_num=2;osd_num<6;osd_num++)
		
		
		   //1. 调用类内部方法进一步封装业务请求
		   if (!GetWebCaptionListParamInner(oResult, cResult, OverlayChn, osd_num, SelectType))
		   {
			 mLogError("Failed to run  GetWebCaptionListParamInner(...)");
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
	
			//2. 解析响应,存入类型为cJson对象的成员变量中
			if (!_AnalyParas.ConvertSTRING2CJSONGetWebCaptionListParam(oResult, cResult, Method, _result, _addressList, temList, osd_num))
		   {
			mLogError("Failed to run  ConvertSTRING2CJSONGetWebCaptionListParam(...)");
			return eInterfaceResCodeError;
		   }
		  //将节点添加到节点链表中
		 for(int i = 2; i <= osd_num; i++)
		 {
			cJSON_AddItemToArray(_addressList, temList[i]);					
		 }
		  cJSON_AddItemToObject(_result, "total", cJSON_CreateNumber((osd_num-1)));
	}
	else if(SelectType == 1)
	{
				for(int i = 2; i <= comp_osd_num; i++)
				{
					temList[i] = cJSON_CreateObject();
				}
				 //for(osd_num=2;osd_num<6;osd_num++)
				
				
				   //1. 调用类内部方法进一步封装业务请求
				   if (!GetWebCaptionListParamInner(oResult, cResult, OverlayChn, comp_osd_num, SelectType))
				   {
					 mLogError("Failed to run  GetWebCaptionListParamInner2(...)");
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
			
					//2. 解析响应,存入类型为cJson对象的成员变量中
					if (!_AnalyParas.ConvertSTRING2CJSONGetWebCaptionListParam(oResult, cResult, Method, _result, _addressList, temList, comp_osd_num))
				   {
					mLogError("Failed to run  ConvertSTRING2CJSONGetWebCaptionListParam2(...)");
					return eInterfaceResCodeError;
				   }
				  //将节点添加到节点链表中
				 for(int i = 2; i <= comp_osd_num; i++)
				 {
					cJSON_AddItemToArray(_addressList, temList[i]); 				
				 }

		cJSON_AddItemToObject(_result, "total", cJSON_CreateNumber((comp_osd_num-1)));
	}
	else if(SelectType == 2)
	{
				for(int i = 2; i <= comp_osd_num; i++)
				{
					temList[i] = cJSON_CreateObject();
				}
				 //for(osd_num=2;osd_num<6;osd_num++)
				
				
				   //1. 调用类内部方法进一步封装业务请求
				   if (!GetWebCaptionListParamInner(oResult, cResult, OverlayChn, comp_osd_num, SelectType))
				   {
					 mLogError("Failed to run  GetWebCaptionListParamInner2(...)");
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
			
					//2. 解析响应,存入类型为cJson对象的成员变量中
					if (!_AnalyParas.ConvertSTRING2CJSONGetWebCaptionListParam(oResult, cResult, Method, _result, _addressList, temList, comp_osd_num))
				   {
					mLogError("Failed to run  ConvertSTRING2CJSONGetWebCaptionListParam2(...)");
					return eInterfaceResCodeError;
				   }
				  //将节点添加到节点链表中
				 for(int i = 2; i <= comp_osd_num; i++)
				 {
					cJSON_AddItemToArray(_addressList, temList[i]); 				
				 }

		cJSON_AddItemToObject(_result, "total", cJSON_CreateNumber((comp_osd_num-1)));
	}
	else
	{
		mLogError("Don't support SelectType = "<<SelectType);
		return eInterfaceResCodeError;
	}
		
		//3. 将step2中获取的响应格式化为Json字符串
		sResult = ComposeResult();
		cJSON_Delete(_result);
		return eInterfaceResCodeSuccess;
#if 0
	
	cJSON *listnode1 = NULL;
	cJSON *listnode2 = NULL;
	
	listnode1 = cJSON_CreateObject();
	listnode2 = cJSON_CreateObject();
	
	cJSON_AddNumberToObject(listnode1, "OverlayChn", 1);	
	cJSON_AddNumberToObject(listnode1, "OverlayPost", 0);
	cJSON_AddNumberToObject(listnode1, "OverlayNum", 8);
	cJSON_AddNumberToObject(listnode1, "OverlayType", 3);
	cJSON_AddStringToObject(listnode1, "OverlayContent", "打发斯蒂芬aaadfdf");
	
	cJSON_AddNumberToObject(listnode2, "OverlayChn", 1);	
	cJSON_AddNumberToObject(listnode2, "OverlayPost", 0);
	cJSON_AddNumberToObject(listnode2, "OverlayNum", 8);
	cJSON_AddNumberToObject(listnode2, "OverlayType", 3);
	cJSON_AddStringToObject(listnode2, "OverlayContent", "打发斯蒂芬aaadfdf");

	cJSON_AddNumberToObject(_result, "Total", 6);
	cJSON_AddItemToArray(_addressList, listnode1);
    cJSON_AddItemToArray(_addressList, listnode2);

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
#endif
}


