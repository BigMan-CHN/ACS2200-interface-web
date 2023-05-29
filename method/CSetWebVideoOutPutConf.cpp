#include "CSetWebVideoOutPutConf.h"
#include <boost/lexical_cast.hpp>

CSetWebVideoOutPutConf::CSetWebVideoOutPutConf(string ip, int port, int timeOut)
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

bool CSetWebVideoOutPutConf::ComposeResult(cJSON *result)
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

string CSetWebVideoOutPutConf::ComposeResult()
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
bool CSetWebVideoOutPutConf::SetWebVideoOutPutConffInner(string ChnName, int Luminance, int Contrast, int Hue, int Satuature, int OutpuResolution, CData& oResult, char* cResult)
{
			mLogDebug("run SetWebVideoOutPutConffInner(...)");
				
			char szReqBody[REQ_BODY_MAXLEN] = { 0 };
			string Resolution;
			int devname;
			//输出分辨率
			switch(OutpuResolution)
			{
			  case 1:
				   Resolution = "1080P60";
				   break;
			  case 2:
				   Resolution = "4k";
				   break;
			  default:
				   mLogError("please input the right Resolution!!!\n");
				   return false;
			}
			// 输出源 
			if (!strcmp(ChnName.c_str(),"VGA"))
			{
				devname = 1;
			}
			else if(!strcmp(ChnName.c_str(),"HDMI"))
			{
				devname = 2;
			}else{
			 mLogError("please input the right ChnName!!!\n");	
			 return false;
			}
			
			sprintf(szReqBody,"vid_dis_hd%d.luminance=%d\nvid_dis_hd%d.contrast=%d\nvid_dis_hd%d.hue=%d\nvid_dis_hd%d.satuature=%d\nvid_dis_hd%d.resolution=%s\n", devname, Luminance, devname, Contrast, devname, Hue, devname, Satuature, devname, Resolution.c_str());
			//sprintf(szReqBody,INTERFACE_SETWEB_VIDEO_OUTPUT_CONF_SENDCMD, devname, Resolution.c_str());
				
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
				mLogError("SetWebVideoOutPutConffInner Run Failed !!!");
				return false;
			}
#endif
				
			return true;

}
InterfaceResCode CSetWebVideoOutPutConf::SetWebVideoOutPutConf(string &sResult, string ChnName, int Luminance, int Contrast, int Hue, int Satuature, int OutpuResolution)
{
		//TODO
		mLogInfo("SetWebVideoOutPutConf...");
		
		CData oResult = SVSMAP();
		char cResult[RES_BUF_MAXLEN] = { 0 };
		
		if (!SetWebVideoOutPutConffInner(ChnName, Luminance, Contrast, Hue, Satuature, OutpuResolution, oResult, cResult))
		{
			mLogError("SetWebVideoOutPutConffInner failed !!!");
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


