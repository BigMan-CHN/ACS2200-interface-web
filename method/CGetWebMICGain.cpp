#include "CGetWebMICGain.h"
#include <boost/lexical_cast.hpp>

CGetWebMICGain::CGetWebMICGain(string ip, int port, int timeOut)
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

bool CGetWebMICGain::ComposeResult(cJSON *result)
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

string CGetWebMICGain::ComposeResult()
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
bool CGetWebMICGain::GetWebMICGainInner(CData& oResult, char* cResult, int ChnNam)
{
	mLogDebug("run GetWebMICGainInner(...)");
#if 0

	 int SerialID = 1;//串口透传，串口1
	 int  cCmdLength   = 0;
	 char cCmdSend[1024]   = {0};
	 //string Cmd[8] = {"<1,2,2,0,0>","<2,2,2,0,0>","<3,2,2,0,0>","<4,2,2,0,0>","<5,2,2,0,0>","<6,2,2,0,0>","<7,2,2,0,0>"};
	 //cCmdLength = strlen(Cmd.c_str());
	 //memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));

	 //<指令类型,操作类型,输入/输出,通道,值>
	 sprintf(cCmdSend,INTERFACE_GETWEB_MIC_GAIN_SENDCMD, ChnNam-1); 
	 cCmdLength = strlen(cCmdSend);
	 
	 MPSOperationRes opResCode = eMPSResultOK;
	 ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	 if(resCode != eResponseCodeSuccess){
		 mLogError("GetSerialNew(...) error:"<<resCode);
		 return false;
	 }
#else
	sqlite3 *reg_db = NULL;
	char DataBuf[1024];
	char *buf;
	char *register_db = "Audio_manage.db";
//	int ChnNum[5] = {1,16,28,1,4};
	
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表

	sprintf(stu.name,"Audio_MICWhineGain0");

	find_stu_db(reg_db, &stu, DataBuf);
	
#endif
	//判断是否获取变声调节成功
//	 char *arg[MAX_ARG_NUM] = {};
//	 for (int i = 0; i < MAX_ARG_NUM; i ++)
//	 {
//		 arg[i] = (char *)calloc(1, strlen(cResult)+1);
//		 if(NULL == arg[i]) 
//		 {
//		   mLogError("calloc is error\n");
//		   return false;
//		  }
//	  }
//	 sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
//	 if (strcmp("ok>", arg[5]))
//	 {
//		 mLogError("GetWebMICGainInner failed");
//		 return false;
//	 }
	 
	 char *arg[40] = {};
	 for (int i = 0; i < 40; i ++)
	 {
		 arg[i] = (char *)calloc(1, strlen(DataBuf)+1);
		 if(NULL == arg[i]) 
		 {
			mLogError("calloc is error\n");
			return false;
		 }
	 }
	 
	 //mLogDebug("cResult:"<<cResult);
	 
	 //命令格式cResult: "<9（指令类型）,2（操作类型，1:SET，2:GET）,2（输入2/输出1）,1（通道0~31）,80（值）,ok（状态）>"
	 sscanf(DataBuf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
	 arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9],\
	 arg[10], arg[11], arg[12], arg[13], arg[14], arg[15], arg[16], arg[17], arg[18], arg[19],\
	 arg[20], arg[21], arg[22], arg[23], arg[24], arg[25], arg[26], arg[27], arg[28], arg[29], \
	 arg[30], arg[31], arg[32], arg[33], arg[34], arg[35], arg[36], arg[37], arg[38], arg[39]);
 

	 
	 int chn = ChnNam + 3;
	 char tmp[10];
	 float Valtmp;
	 sprintf(tmp,"%s",arg[chn]);
	
	 sscanf(tmp,"%f",&Valtmp);
//	 mLogDebug("Valtmp:"<<Valtmp);
	 int Val = Valtmp*10;//转换为web音量取值范围-720-120
// 	 sprintf(cResult,"<9,2,2,%d,%s,ok>", ChnNum, arg[chn]);
	 sprintf(cResult,"<8,2,2,%d,%d,ok>", ChnNam, Val);
	 
		mLogDebug("cResult = "<<cResult);
 

	 cJSON_AddNumberToObject(_result, "MicVal", Val);

	 return true;

}

InterfaceResCode CGetWebMICGain::GetWebMICGain(string &sResult,int ChnNam)
{
    //TODO
    mLogInfo("GetWebMICGain...");
		CData oResult = SVSMAP();
		char cResult[4096] = {0};
		//printf("SetDecCtrlStart Dectype %d Channal %d CacheTime %d sAddr %s TcpMode %d\n", Dectype, Channal, CacheTime, sAddr.c_str(), TcpMode);
#if 1
		if(GetWebMICGainInner( oResult, cResult, ChnNam) == false){
			mLogError("Failed to run  GetWebMICGainInner(...)");
			return eInterfaceResCodeError118;
		}
#endif 

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


