#include "CSetWebStorageConf.h"
#include <boost/lexical_cast.hpp>

CSetWebStorageConf::CSetWebStorageConf(string ip, int port, int timeOut)
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

bool CSetWebStorageConf::ComposeResult(cJSON *result)
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

string CSetWebStorageConf::ComposeResult()
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
bool CSetWebStorageConf::SetWebStorageConfInner(int StorageThreshold,int FileType,int AutoDel,int SegSize, CData& oResult, char* cResult)
{
		mLogDebug("run SetWebStorageConfInner(...)");
			
		char szReqBody[REQ_BODY_MAXLEN] = { 0 };
/*		string file_type;
		switch(FileType)
	  {
		case 1:
			 file_type = "MP4(H264)";
			 break;
		case 2:
			 file_type = "MP4(AAC)";
			 break;
		case 3:
			 file_type = "MP4(AAC+H264)";
			 break;
		default:
			 printf("please input the right file_type!!!\n");
		}
*/		
		sprintf(szReqBody,INTERFACE_SETWEB_STORAGE_CONF_SENDCMD,FileType, StorageThreshold, AutoDel, SegSize, SegSize);
			
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
			mLogError("SetWebStorageConfInner Run Failed !!!");
			return false;
		}
#endif
			
		return true;

}

InterfaceResCode CSetWebStorageConf::SetWebStorageConf(string &sResult,int StorageThreshold,int FileType,int AutoDel,int SegSize)
{
		//TODO
		mLogInfo("设置  StorageConf  参数...");
		
		CData oResult = SVSMAP();
		char cResult[RES_BUF_MAXLEN] = { 0 };
		
		if (!SetWebStorageConfInner(StorageThreshold, FileType, AutoDel, SegSize, oResult, cResult))
		{
			mLogError("SetWebStorageConfInner failed !!!");
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


