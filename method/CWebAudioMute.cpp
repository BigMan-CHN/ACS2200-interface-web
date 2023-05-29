#include "CWebAudioMute.h"
#include <boost/lexical_cast.hpp>

CWebAudioMute::CWebAudioMute(string ip, int port, int timeOut)
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

bool CWebAudioMute::ComposeResult(cJSON *result)
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

string CWebAudioMute::ComposeResult()
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
bool CWebAudioMute::SetWebAudioMuteInner(CData& oResult, char* cResult,int CtrlType,int MuteVal, int ChnNam)
{
	mLogDebug("run SetWebAudioMuteInner(...)");

	 int SerialID = 2;//串口透传，串口1
	 int  cCmdLength   = 0;
	 char cCmdSend[1024]   = {0};
	 
	 sqlite3 *reg_db = NULL;
	 char DataBuf[1024];
	 char *buf;
	 char *register_db = "Audio_manage.db";
	 int ChnNum[5] = {1,16,28,1,4};
	 
	 STU stu;
	 time_t cur_time = time(NULL);
	 struct tm *tm_ptr;
	 tm_ptr = localtime(&cur_time);
	 reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	 create_db_table(reg_db);  //创建注册表
	 
	 //string Cmd[8] = {"<1,2,2,0,0>","<2,2,2,0,0>","<3,2,2,0,0>","<4,2,2,0,0>","<5,2,2,0,0>","<6,2,2,0,0>","<7,2,2,0,0>"};
	 //cCmdLength = strlen(Cmd.c_str());
	 //memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
	 char Mute[10];
	if(MuteVal == 1)
	{
		sprintf(Mute,"true"); 

	}else{
		sprintf(Mute,"false"); 

	}
	 //<指令类型,操作类型(1设置，2获取),2输入/1输出,通道,值>
	 sprintf(cCmdSend,"<10,1,%d,%d,%s>", CtrlType,ChnNam, Mute); 
	 cCmdLength = strlen(cCmdSend);
	 
	 MPSOperationRes opResCode = eMPSResultOK;
	 ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	 if(resCode != eResponseCodeSuccess){
		 mLogError("GetSerialNew(...) error:"<<resCode);
		 return false;
	 }
	//判断是否设置变声调节成功
	 char *arg[MAX_ARG_NUM] = {};
	 for (int i = 0; i < MAX_ARG_NUM; i ++)
	 {
		 arg[i] = (char *)calloc(1, strlen(cResult)+1);
		 if(NULL == arg[i]) 
		 {
		   mLogError("calloc is error\n");
		   return false;
		  }
	  }
	 sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);

	 //获取数据库数据修改并更新
	 if(!strcmp(arg[2], "2"))//2:输入
	 {
	 
		 if(!strcmp(arg[5], "ok>"))
		 {
	 
			 sprintf(stu.name,"Audio_ALLDefultMute2");
			 find_stu_db(reg_db, &stu, DataBuf);
			 //解析
			 char *carg[40] = {};
			 for (int i = 0; i < 40; i ++)
			 {
				  carg[i] = (char *)calloc(1, strlen(DataBuf)+1);
				  if(NULL == carg[i]) 
				  {
				   mLogError("calloc is error\n");
				   return eInterfaceResCodeError;
				 }
			  }
			 sscanf(DataBuf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
			 carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
			 carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
			 carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
			 carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);
			 int chn = atoi(arg[3]) + 3;//找到 <，，>数据中对应通道的数据的位置
			 carg[chn] = arg[4];//修改数据
			 //36个字符“，”除外
			 sprintf(DataBuf,"%s", carg[0]);
			 for(int i = 1; i <= 27; i++)
			 {					 
				 buf = DataBuf + strlen(DataBuf);
	 
				 sprintf(buf,",%s", carg[i]);
				 
			 }
			 mLogDebug("DataBuf = "<<DataBuf);
			 //更新数据
			 stu.id = 4;
			 sprintf(stu.val,DataBuf);
			 sprintf(stu.name,"Audio_ALLDefultMute2");
			 sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
			 update_stu_db(reg_db, &stu); //add 到数据库 
			 
			 mLogDebug("ConvertSTRING2CJSONParseSerialCmdParam Set gain Success !!!\n");
			 return true;
		 }
		 else
		 {
			 mLogError("SetWebAudioMuteInner failed");
			 return false;
		 }
	 }
	 else//1:输出
	 {
		 if(!strcmp(arg[5], "ok>"))
		 {
			 sprintf(stu.name,"Audio_ALLDefultMute1");
			 find_stu_db(reg_db, &stu, DataBuf);
			 //解析
			 char *carg[40] = {};
			 for (int i = 0; i < 40; i ++)
			 {
				  carg[i] = (char *)calloc(1, strlen(DataBuf)+1);
				  if(NULL == carg[i]) 
				  {
				   mLogError("calloc is error\n");
				   return eInterfaceResCodeError;
				 }
			  }
			 sscanf(DataBuf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
			 carg[0], carg[1], carg[2], carg[3], carg[4], carg[5], carg[6], carg[7], carg[8], carg[9],\
			 carg[10], carg[11], carg[12], carg[13], carg[14], carg[15], carg[16], carg[17], carg[18], carg[19],\
			 carg[20], carg[21], carg[22], carg[23], carg[24], carg[25], carg[26], carg[27], carg[28], carg[29], \
			 carg[30], carg[31], carg[32], carg[33], carg[34], carg[35], carg[36], carg[37], carg[38], carg[39]);
			 int chn = atoi(arg[3]) + 3;
			 carg[chn] = arg[4];
			 //26个
			 sprintf(DataBuf,"%s", carg[0]);
			 for(int i = 1; i <= 23; i++)
			 {					 
				 buf = DataBuf + strlen(DataBuf);
	 
				 sprintf(buf,",%s", carg[i]);
				 
			 }
			 mLogDebug("DataBuf = "<<DataBuf);
			 //更新数据
			 stu.id = 3;
			 sprintf(stu.val,DataBuf);
			 sprintf(stu.name,"Audio_ALLDefultMute1");
			 sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
			 update_stu_db(reg_db, &stu); //add 到数据库 
	 
			 mLogDebug("ConvertSTRING2CJSONParseSerialCmdParam Set gain Success !!!\n");
			 return true;
		 }
		 else
		 {
			 mLogError("SetWebAudioMuteInner failed");
			 return false;
		 }
	 }

	 return true;


}

InterfaceResCode CWebAudioMute::SetWebAudioMute(string &sResult,int MuteVal,int CtrlType, int ChnNam)
{
    //TODO
    mLogInfo("SetWebAudioMute...");
		int cmd,val;
		cJSON* temList[10];
		char cResult[RES_BUF_MAXLEN] = { 0 };
		CData oResult = SVSMAP();
		

		//1. 调用类内部方法进一步封装业务请求
		if (!SetWebAudioMuteInner(oResult, cResult, CtrlType, MuteVal, ChnNam))
		{
			mLogError("Failed to run  SetWebAudioMuteInner(...)");
			return eInterfaceResCodeError;
		}
		

#if 0
		if (!_AnalyParas.ConvertSTRING2CJSONParseSerialCmdParam(oResult, cResult, _result, _addressList, NULL)
		{
		 mLogError("Failed to run  ConvertSTRING2CJSONParseSerialCmdParam(...)");
		 return eInterfaceResCodeError;
		}
#endif
		//将响应格式化为Json字符串
		sResult = ComposeResult();
		cJSON_Delete(_result);
		return eInterfaceResCodeSuccess;

}
bool CWebAudioMute::GetWebAudioMuteInner(CData& oResult, char* cResult, int CtrlType,int ChnNam)
{
	mLogDebug("run GetWebAudioMuteInner(...)");
#if 0
	 int SerialID = 2;//串口透传，串口1
	 int  cCmdLength   = 0;
	 char cCmdSend[1024]   = {0};
	 //string Cmd[8] = {"<1,2,2,0,0>","<2,2,2,0,0>","<3,2,2,0,0>","<4,2,2,0,0>","<5,2,2,0,0>","<6,2,2,0,0>","<7,2,2,0,0>"};
	 //cCmdLength = strlen(Cmd.c_str());
	 //memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));

	 //<指令类型,操作类型,输入/输出,通道,值>
	 sprintf(cCmdSend,INTERFACE_GET_AUDIO_MUTE_SENDCMD, CtrlType, ChnNam); 
	 cCmdLength = strlen(cCmdSend);
	 
	 MPSOperationRes opResCode = eMPSResultOK;
	 ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	 if(resCode != eResponseCodeSuccess){
		 mLogError("GetSerialNew(...) error:"<<resCode);
		 return false;
	 }
	//判断是否设置变声调节成功
	 char *arg[MAX_ARG_NUM] = {};
	 for (int i = 0; i < MAX_ARG_NUM; i ++)
	 {
		 arg[i] = (char *)calloc(1, strlen(cResult)+1);
		 if(NULL == arg[i]) 
		 {
		   mLogError("calloc is error\n");
		   return false;
		  }
	  }
	 sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	 if (strcmp("ok>", arg[5]))
	 {
		 mLogError("GetWebAudioMuteInner failed");
		 return false;
	 }
	 
	int Mute;
	if(!strcmp(arg[4],"true"))
	{
		Mute = 1; 

	}
	else if(!strcmp(arg[4],"false"))
	{
		Mute = 0;
	}
	else{
	
		mLogError("GetWebAudioMuteInner arg[4] err");
		return false;
	}
	 cJSON_AddNumberToObject(_result, "ChnNum", ChnNam);
	 cJSON_AddNumberToObject(_result, "CtrlType", CtrlType);
//	 cJSON_AddNumberToObject(_result, "MuteVal", atoi(arg[4]));
	 cJSON_AddNumberToObject(_result, "MuteVal", Mute);
#else 
		 sqlite3 *reg_db = NULL;
		 char DataBuf[1024];
		 char *buf;
		 char *register_db = "Audio_manage.db";
		 int Mute;
		 
		 STU stu;
		 time_t cur_time = time(NULL);
		 struct tm *tm_ptr;
		 tm_ptr = localtime(&cur_time);
		 reg_db = (sqlite3 *)create_db(register_db); //创建数据库
		 create_db_table(reg_db);  //创建注册表
		 memset(DataBuf, 0, 1024);
		 
		 sprintf(stu.name,"Audio_ALLDefultMute%d",CtrlType);
		 find_stu_db(reg_db, &stu, DataBuf);
		 
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
	 		
		 if(CtrlType == 2)
		 {
		 
			 int chn = ChnNam + 3;
		
			 if(!strcmp(arg[chn],"true"))
			 {
				 Mute = 1; 
			 
			 }
			 else if(!strcmp(arg[chn],"false"))
			 {
				 Mute = 0;
			 }
			 else{
			 
				 mLogError("GetWebAudioMuteInner arg[chn] err");
				 return false;
			 }

	 // 	 sprintf(cResult,"<9,2,2,%d,%s,ok>", ChnNum, arg[chn]);
			 sprintf(cResult,"<9,2,2,%d,%d,ok>", ChnNam, Mute);
			 
			 mLogDebug("cResult = "<<cResult);
	 
		 }
		 else//1:输出
		 {
			 int chn = ChnNam + 3;
			 
			 if(!strcmp(arg[chn],"true"))
			 {
				 Mute = 1; 
			 
			 }
			 else if(!strcmp(arg[chn],"false"))
			 {
				 Mute = 0;
			 }
			 else{
			 
				 mLogError("GetWebAudioMuteInner arg[chn] err");
				 return false;
			 }

			 
	 // 	 sprintf(cResult,"<9,2,1,%d,%s,ok>", ChnNum, arg[chn]);
			 sprintf(cResult,"<9,2,1,%d,%d,ok>", ChnNam, Mute);
	 
	 
			 mLogDebug("cResult = "<<cResult);
	 
		 }
	 cJSON_AddNumberToObject(_result, "ChnNum", ChnNam);
	 cJSON_AddNumberToObject(_result, "CtrlType", CtrlType);
//	 cJSON_AddNumberToObject(_result, "MuteVal", atoi(arg[4]));
	 cJSON_AddNumberToObject(_result, "MuteVal", Mute);
#endif
	 return true;


}

InterfaceResCode CWebAudioMute::GetWebAudioMute(string &sResult,int CtrlType,int ChnNam)
{
    //TODO
    	mLogInfo("GetWebAudioMute...");
		int cmd,val;
		cJSON* temList[10];
		char cResult[RES_BUF_MAXLEN] = { 0 };
		CData oResult = SVSMAP();

		//1. 调用类内部方法进一步封装业务请求
		if (!GetWebAudioMuteInner(oResult, cResult, CtrlType, ChnNam))
		{
			mLogError("Failed to run  GetWebAudioMuteInner(...)");
			return eInterfaceResCodeError;
		}
#if 0
		if (!_AnalyParas.ConvertSTRING2CJSONParseSerialCmdParam(oResult, cResult, _result, _addressList, NULL)
		{
		 mLogError("Failed to run  ConvertSTRING2CJSONParseSerialCmdParam(...)");
		 return eInterfaceResCodeError;
		}
#endif
		//将响应格式化为Json字符串
		sResult = ComposeResult();
		cJSON_Delete(_result);
		return eInterfaceResCodeSuccess;

}


