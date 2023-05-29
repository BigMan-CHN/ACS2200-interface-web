#include "CSetWebVideoChnConf.h"
#include <boost/lexical_cast.hpp>

CSetWebVideoChnConf::CSetWebVideoChnConf(string ip, int port, int timeOut)
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

bool CSetWebVideoChnConf::ComposeResult(cJSON *result)
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

string CSetWebVideoChnConf::ComposeResult()
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
bool CSetWebVideoChnConf::SetWebVideoChnConfInner(string ChnName,int ChnPort,int IsShow, CData& oResult, char* cResult)
{
		mLogDebug("run SetWebVideoChnConfInner(...)");
		string Enctype;
		int Type = 1;
		switch(Type)
		{
			case 1:
				Enctype = "UTF-8";
				break;
			case 2:
				Enctype = "GB2312";
				break;
			default:
		 	   mLogError("SetWebVideoChnConfInner  Enctype error:"<<Type);
            return false;
		}	
		char szReqBody[REQ_BODY_MAXLEN] = { 0 };
		if(ChnPort >= 2 && ChnPort <= 9)
		{
			ChnPort = ChnPort - 1;
			sprintf(szReqBody,"vid_cap%d.ch1.osd1.s_display=%d\nvid_cap%d.ch1.osd1.type=0\nvid_cap%d.ch1.osd1.char_set=%s\nvid_cap%d.ch1.osd1.content=%s\nvid_cap%d.ch1.osd1.position=%d\nvid_cap%d.ch1.osd1.size=32", \
				ChnPort, IsShow, ChnPort,ChnPort,Enctype.c_str(), ChnPort, ChnName.c_str(), ChnPort, 1, ChnPort);
		}
		else if(ChnPort == 1)
		{
			sprintf(szReqBody,"comp1.osd1.type=0\ncomp1.osd1.s_display=%d\ncomp1.osd1.position=1\ncomp1.osd1.char_set=%s\ncomp1.osd1.content=%s\ncomp1.osd1.size=32\n", \
				IsShow, Enctype.c_str(), ChnName.c_str());
		}
//		else if(ChnPort == 10)
//		{
//			sprintf(szReqBody,"comp2.osd1.type=0\ncomp2.osd1.s_display=%d\ncomp2.osd1.position=1\ncomp2.osd1.char_set=%s\ncomp2.osd1.content=%s\n", \
//				IsShow, Enctype.c_str(), ChnName.c_str());
//		}
		//sprintf(szReqBody,INTERFACE_SETWEB_VIDEO_CHN_CONF_SENDCMD, ChnPort, IsShow, ChnPort, ChnName.c_str(), ChnPort,Enctype.c_str());	
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
			mLogError("SetWebVideoChnConfInner Run Failed !!!");
			return false;
		}
#endif
			
		return true;

}
InterfaceResCode CSetWebVideoChnConf::SetWebVideoChnConf(string &sResult,string ChnName,int ChnPort,int IsShow)
{
    //TODO
	mLogInfo("SetWebVideoChnConf...");
	
	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	
	if (!SetWebVideoChnConfInner(ChnName, ChnPort, IsShow, oResult, cResult))
	{
		mLogError("SetWebVideoChnConfInner failed !!!");
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


