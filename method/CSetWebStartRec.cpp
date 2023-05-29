#include "CSetWebStartRec.h"
#include <boost/lexical_cast.hpp>

CSetWebStartRec::CSetWebStartRec(string ip, int port, int timeOut)
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

bool CSetWebStartRec::ComposeResult(cJSON *result)
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

string CSetWebStartRec::ComposeResult()
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
bool CSetWebStartRec::SetWebStartRecInner(CData& oResult, char* cResult, int VideoEncChnnel, int IfRec)
{
			mLogDebug("run SetWebStartRecInner(...)");
		
			//InterfaceResCode resCode = eInterfaceResCodeSuccess;
		
			//1. 封装业务请求
			char szReqBody[REQ_BODY_MAXLEN] = { 0 };
//			////录制的视频与编码通道号相对应，与web界面不符合，需转换对应关系
//			switch(VideoEncChnnel)
//			{
//				case 1:
//					VideoEncChnnel = 2;
//					break;
//				case 2:
//					VideoEncChnnel = 3;
//					break;
//				case 3:
//					VideoEncChnnel = 4;
//					break;
//				case 4:
//					VideoEncChnnel = 5;
//					break;
//				case 5:
//					VideoEncChnnel = 6;
//					break;
//				case 6:
//					VideoEncChnnel = 7;
//					break;
//				case 7:
//					VideoEncChnnel = 8;
//					break;
//				case 8:
//					VideoEncChnnel = 9;
//					break;
//				case 9:
//					VideoEncChnnel = 1;
//					break;			
//			}
			sprintf(szReqBody, "net_send%d.rec_is_select=%d\n", VideoEncChnnel, IfRec);
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

InterfaceResCode CSetWebStartRec::SetWebStartRec(string &sResult, int VideoEncChnnel, int IfRec)
{
	   //TODO
			mLogInfo("SetWebStartRec...");
		
			char cResult[RES_BUF_MAXLEN] = { 0 };
			CData oResult = SVSMAP();

		
			   //1. 调用类内部方法进一步封装业务请求
			   if (!SetWebStartRecInner(oResult, cResult, VideoEncChnnel,IfRec))
			   {
				 mLogError("Failed to run  SetWebStartRecInner(...)");
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


}


