#include "CSetWebRecCtrl.h"
#include <boost/lexical_cast.hpp>

CSetWebRecCtrl::CSetWebRecCtrl(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_sipdec(cJSON_CreateObject())
    ,_VideoFiles(cJSON_CreateArray())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}
void CSetWebRecCtrl::RecgetFiles(std::string path, std::vector<std::string> &files, std::vector<std::string> &filesPath)
{
  DIR *dir;
  struct dirent *ptr;
 
  if ((dir = opendir(path.c_str())) == NULL)
  {
    perror("Open dir error...");
    exit(1);
  }

  while ((ptr = readdir(dir)) != NULL)
  {
    if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
      continue;
    else if (ptr->d_type == 8) // 普通文件
    {
      filesPath.push_back(path + ptr->d_name);
      files.push_back(ptr->d_name);
    }
    else if (ptr->d_type == 10) // 符号链接
      continue;
    else if (ptr->d_type == 4) // 目录文件
    {
      //files.push_back(ptr->d_name);
      //getFiles(path + ptr->d_name + "/", files);
    }
  }
  closedir(dir);
}

bool CSetWebRecCtrl::SetWebRec(int ChnID, int OnOff, string TaskID, int CurrentState, CData &oResult, char *cResult)
{
    mLogDebug("run SetWebRec(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
	
	vector<string> filename,filepath;
	struct stat tbuf;
	struct tm *info;
	char buffer[80];
	int filsesize;
	string ftp_addr,creat_time;
	char *ReqBody = NULL;

	//cJSON*	lognode[MAX_LOGFILE_NUM];
	cJSON*	lognode = NULL;
	char taskid[2048];
//	sprintf(taskid,"/mnt/HD0/%s/", TaskID.c_str());	
//	mLogDebug("taskid = "<<taskid);


	if(TaskID.size()<=1)
	return false;
#if 1
    if (OnOff == 1)//开始录制（开庭）和恢复
    {		
		sprintf(cChnCmd,"rec.app_start=%d,%s,%s\nnet_send%d.rec_current_status=%d\nnet_send%d.rec_task_id=%s\n", ChnID, TaskID.c_str(), TaskID.c_str(), ChnID, CurrentState, ChnID, TaskID.c_str());
		//sprintf(cChnCmd,INTERFACE_SETWEBRECORDSTART_SENDCMD, ChnID, TaskID.c_str(), TaskID.c_str());	
    }
    else if(OnOff == 0)//闭庭
    {
		sprintf(cChnCmd,"rec.app_stop=%s\nnet_send%d.rec_current_status=%d\nnet_send%d.rec_task_id=\n", TaskID.c_str(), ChnID, CurrentState, ChnID);  
		//sprintf(cChnCmd,INTERFACE_SETWEBRECORDSTOP_SENDCMD, TaskID.c_str());
    }
	else if(OnOff == 2)//暂停（休庭）
    {
		sprintf(cChnCmd,"rec.app_stop=%s\nnet_send%d.rec_current_status=%d\nnet_send%d.rec_task_id=%s\n", TaskID.c_str(), ChnID, CurrentState , ChnID, TaskID.c_str());  
		//sprintf(cChnCmd,INTERFACE_SETWEBRECORDSTOP_SENDCMD, TaskID.c_str());
    }
#endif 

    //add fwb 20201110
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSetWebRecCtrl::GetWebRecInfoList(int *GetMode,string TaskId, string StartTime, string EndTime, int StartIndex, int EndIndex, CData &oResult, char *cResult)
{
    mLogDebug("run GetWebRecInfoList(...)");
    //获取状态

    
    char cChnCmd[4096] = {0};
	//int GetMode;
	//char len[50] = {0};
	time_t timep;
	struct tm *LocalTime;
	char SysTime[80];
	time(&timep);
	LocalTime = localtime(&timep);
	LocalTime->tm_hour = LocalTime->tm_hour;//UTC时间转换为中国时间
	strftime(SysTime, 80, "%Y-%m-%d %H:%M:%S", LocalTime);
	
#if 0
	//TaskId、StartTime、EndTime都为空，按时间搜索查询文件夹
	if((!strcmp(TaskId.c_str(),""))&&(!strcmp(StartTime.c_str(),""))&&(!strcmp(EndTime.c_str(),"")))
	{		
		//EndTime = SysTime;
		EndTime = "2099-01-01 00:00:00";
		StartTime = "1970-01-01 00:00:00";
		*GetMode = 7;
		sprintf(cChnCmd,"<LFS_head><search>%d</search><dirname>%s</dirname><starttime>%s</starttime><endtime>%s</endtime><startindex>%d</startindex><endindex>%d</endindex></LFS_head>\n", *GetMode, TaskId.c_str(), StartTime.c_str(), EndTime.c_str(), StartIndex, EndIndex);
	}//TaskId、StartTime为空、EndTime不为空，按时间搜索查询文件夹
	else if((!strcmp(TaskId.c_str(),""))&&(!strcmp(StartTime.c_str(),""))&&(strcmp(EndTime.c_str(),"")))
	{		
		//EndTime = SysTime;
		StartTime = "1970-01-01 00:00:00";
		*GetMode = 7;
		sprintf(cChnCmd,"<LFS_head><search>%d</search><dirname>%s</dirname><starttime>%s</starttime><endtime>%s</endtime><startindex>%d</startindex><endindex>%d</endindex></LFS_head>\n", *GetMode, TaskId.c_str(), StartTime.c_str(), EndTime.c_str(), StartIndex, EndIndex);
	}//TaskId、EndTime为空，StartTime不为空，按时间搜索查询文件夹
	else if((!strcmp(TaskId.c_str(),""))&&(strcmp(StartTime.c_str(),""))&&(!strcmp(EndTime.c_str(),"")))
	{		
		//EndTime = SysTime;
		EndTime = "2099-01-01 00:00:00";
		*GetMode = 7;
		sprintf(cChnCmd,"<LFS_head><search>%d</search><dirname>%s</dirname><starttime>%s</starttime><endtime>%s</endtime><startindex>%d</startindex><endindex>%d</endindex></LFS_head>\n", *GetMode, TaskId.c_str(), StartTime.c_str(), EndTime.c_str(), StartIndex, EndIndex);
	}//TaskId为空，StartTime、EndTime不为空，按时间搜索查询文件夹
	else if((!strcmp(TaskId.c_str(),""))&&(strcmp(StartTime.c_str(),""))&&(strcmp(EndTime.c_str(),"")))
	{		
		*GetMode = 7;
		sprintf(cChnCmd,"<LFS_head><search>%d</search><dirname>%s</dirname><starttime>%s</starttime><endtime>%s</endtime><startindex>%d</startindex><endindex>%d</endindex></LFS_head>\n", *GetMode, TaskId.c_str(), StartTime.c_str(), EndTime.c_str(), StartIndex, EndIndex);
	}//EndTime、StartTime为空、TaskId不为空，依据GetMode来选择查询方式
	else if((strcmp(TaskId.c_str(),""))&&(!strcmp(StartTime.c_str(),""))&&(!strcmp(EndTime.c_str(),"")))
	{		

		if(*GetMode == 6)//web端按目录搜索，没有时间默认加入时间
		{
			//EndTime = SysTime;
			EndTime = "2099-01-01 00:00:00";
			StartTime = "1970-01-01 00:00:00";
			sprintf(cChnCmd,"<LFS_head><search>%d</search><dirname>%s</dirname><starttime>%s</starttime><endtime>%s</endtime><startindex>%d</startindex><endindex>%d</endindex></LFS_head>\n", *GetMode, TaskId.c_str(), StartTime.c_str(), EndTime.c_str(), StartIndex, EndIndex);
		}
		else//按文件夹查询，第2次查询所选择目录下的所有文件
		{
			*GetMode = 1;
			sprintf(cChnCmd,"<LFS_head><search>%d</search><dirname>%s</dirname><starttime>%s</starttime><endtime>%s</endtime><startindex>%d</startindex><endindex>%d</endindex></LFS_head>\n", *GetMode, TaskId.c_str(), StartTime.c_str(), EndTime.c_str(), StartIndex, EndIndex);
		}
	}//通过时间和目录查询文件
	else if((strcmp(TaskId.c_str(),""))&&(!strcmp(StartTime.c_str(),""))&&(strcmp(EndTime.c_str(),"")))
	{		
		StartTime = "1970-01-01 00:00:00";
		*GetMode = 6;
		sprintf(cChnCmd,"<LFS_head><search>%d</search><dirname>%s</dirname><starttime>%s</starttime><endtime>%s</endtime><startindex>%d</startindex><endindex>%d</endindex></LFS_head>\n", *GetMode, TaskId.c_str(), StartTime.c_str(), EndTime.c_str(), StartIndex, EndIndex);
	}//通过时间和目录查询文件
	else if((strcmp(TaskId.c_str(),""))&&(strcmp(StartTime.c_str(),""))&&(!strcmp(EndTime.c_str(),"")))
	{		
		//EndTime = SysTime;
		EndTime = "2099-01-01 00:00:00";
		*GetMode = 6;
		sprintf(cChnCmd,"<LFS_head><search>%d</search><dirname>%s</dirname><starttime>%s</starttime><endtime>%s</endtime><startindex>%d</startindex><endindex>%d</endindex></LFS_head>\n", *GetMode, TaskId.c_str(), StartTime.c_str(), EndTime.c_str(), StartIndex, EndIndex);
	}//通过时间和目录查询文件
	else if((strcmp(TaskId.c_str(),""))&&(strcmp(StartTime.c_str(),""))&&(strcmp(EndTime.c_str(),"")))
	{		
		*GetMode = 6;
		sprintf(cChnCmd,"<LFS_head><search>%d</search><dirname>%s</dirname><starttime>%s</starttime><endtime>%s</endtime><startindex>%d</startindex><endindex>%d</endindex></LFS_head>\n", *GetMode, TaskId.c_str(), StartTime.c_str(), EndTime.c_str(), StartIndex, EndIndex);
	}
	//sprintf(cChnCmd,"<LFS_head><search>%d</search><dirname>%s</dirname><starttime>%s</starttime><endtime>%s</endtime><startindex>%d</startindex><endindex>%d</endindex></LFS_head>\n", *GetMode, TaskId.c_str(), StartTime.c_str(), EndTime.c_str(), StartIndex, EndIndex);
//	mLogDebug("GetWebRecInfoList cChnCmd="<<cChnCmd);
	//sprintf(cChnCmd,INTERFACE_GETRECINFOLIST_SENDCMD, GetMode, TaskId.c_str(),StartTime.c_str(),EndTime.c_str());
#endif 
	switch (*GetMode)
	{
		case 1:
			//按时间搜索查询时间段内的文件夹,若StartTime、EndTime都为空,则默认时间为1970-2099年
			*GetMode = 7;
			if((!strcmp(StartTime.c_str(),""))&&(!strcmp(EndTime.c_str(),"")))
			{		
				EndTime = "2099-01-01 00:00:00";
				StartTime = "1970-01-01 00:00:00";
				sprintf(cChnCmd,"<LFS_head><search>%d</search><dirname>%s</dirname><starttime>%s</starttime><endtime>%s</endtime><startindex>%d</startindex><endindex>%d</endindex></LFS_head>\n", *GetMode, TaskId.c_str(), StartTime.c_str(), EndTime.c_str(), StartIndex, EndIndex);
			}
			else
			{
				sprintf(cChnCmd,"<LFS_head><search>%d</search><dirname>%s</dirname><starttime>%s</starttime><endtime>%s</endtime><startindex>%d</startindex><endindex>%d</endindex></LFS_head>\n", *GetMode, TaskId.c_str(), StartTime.c_str(), EndTime.c_str(), StartIndex, EndIndex);
			}
			break;
		case 2:
			//第2次查询选中的目录下的文件,若StartTime、EndTime都为空,则默认时间为1970-2099年
			*GetMode = 6;
			if((!strcmp(StartTime.c_str(),""))&&(!strcmp(EndTime.c_str(),"")))
			{		
				EndTime = "2099-01-01 00:00:00";
				StartTime = "1970-01-01 00:00:00";
				sprintf(cChnCmd,"<LFS_head><search>%d</search><dirname>%s</dirname><starttime>%s</starttime><endtime>%s</endtime><startindex>%d</startindex><endindex>%d</endindex></LFS_head>\n", *GetMode, TaskId.c_str(), StartTime.c_str(), EndTime.c_str(), StartIndex, EndIndex);
			}
			else
			{
				sprintf(cChnCmd,"<LFS_head><search>%d</search><dirname>%s</dirname><starttime>%s</starttime><endtime>%s</endtime><startindex>%d</startindex><endindex>%d</endindex></LFS_head>\n", *GetMode, TaskId.c_str(), StartTime.c_str(), EndTime.c_str(), StartIndex, EndIndex);
			}
			break;	
	}

    //add fwb 20201110
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 7; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
	
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSetWebRecCtrl::SetWebSegModel(int ChnId, string SegModel, CData &oResult, char *cResult)
{
    mLogDebug("run SetWebRecSegModel(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    sprintf(cChnCmd,INTERFACE_SETWEBRECORDSEGMODE_SENDCMD, ChnId, SegModel.c_str());    
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSetWebRecCtrl::SetWebMatrix(int OnOff, int WinID, int VideoID, CData &oResult, char *cResult)
{
    mLogDebug("run SetWebRecSegModel(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    sprintf(cChnCmd,INTERFACE_SETWEBVIDEOMATRIX_SENDCMD, WinID, VideoID, OnOff);    
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSetWebRecCtrl::SetWebSegModelSelect(int ChnId, int WinID, int VideoID, CData &oResult, char *cResult)
{
    mLogDebug("run SetWebRecSegModel(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
	char strtmp[64];
	string arrystr[5];
	int decNum = 0;
#if 0
    if (VideoID <= 8)//16
    {
        sprintf(cChnCmd,INTERFACE_SETWEBRECORDSEGMODESELECT_VLX_SENDCMD, ChnId, WinID, VideoID); 
    }
    else if ((VideoID > 8) && (VideoID <= 11))//16,32
    {
        sprintf(cChnCmd,INTERFACE_SETWEBRECORDSEGMODESELECT_VNX_SENDCMD, ChnId, WinID, VideoID-8); 
    }
    else if (VideoID > 11)//32
    {
        sprintf(cChnCmd,INTERFACE_SETWEBRECORDSEGMODESELECT_VDX_SENDCMD, ChnId, WinID, VideoID); 
    }
   
#endif 
		//解码通道1-6对应VN1-1~VN6-1，解码1-6对应的源rtsp1,rtsp2,h323,sip1,sip2,sip3
		string arry[6] = {"VN1-1","VN2-1","VN3-1","VN4-1","VN5-1","VN6-1"};

		if (VideoID <= 8)//16
		{
			sprintf(cChnCmd,"comp%d.win%d.channel=VL%d-1\n", ChnId, WinID, VideoID); 
		}
		else if ((VideoID >= 9) && (VideoID <= 11))
		{
			if(StrParam[4] != "new_audio_board")
			{
				mLogDebug("Audio board type: old_audio_board");
				//StrParam[5]~trParam[10]存储的为rtsp1,rtsp2,h323,sip1,sip2,sip3的url
				for(int i = 0; i <= 5; i++)
				{
					if(StrParam[i+5] != "")
					{
						arrystr[decNum] = arry[i];
						mLogDebug("arrystr"<<decNum<<":"<<arrystr[decNum].c_str());
						decNum++;
					}
				}
				switch(VideoID)
				{
					case 9:
						if(decNum > 0)
						{
							strcpy(strtmp,arrystr[0].c_str());
						}else{
							strcpy(strtmp,"VN8-1");
						}
						break;
					case 10:
						if(decNum > 1)
						{
							strcpy(strtmp,arrystr[1].c_str());
						}else{
							strcpy(strtmp,"VN8-1");
						}
						break;
					case 11:
						if(decNum > 2)
						{
							strcpy(strtmp,arrystr[2].c_str());
						}else{
							strcpy(strtmp,"VN8-1");
						}

						break;
				}
			}
			else
			{
				mLogDebug("Audio board type: new_audio_board");
				switch(VideoID)
				{
					case 9:
						if(StrParam[1] != "")
						{
							strcpy(strtmp,StrParam[1].c_str());
						}else{
							strcpy(strtmp,"VN2-1");
						}
						break;
					case 10:
						if(StrParam[2] != "")
						{
							strcpy(strtmp,StrParam[2].c_str());
						}else{
							strcpy(strtmp,"VN2-1");
						}
						break;
					case 11:
						if(StrParam[3] != "")
						{
							strcpy(strtmp,StrParam[3].c_str());
						}else{
							strcpy(strtmp,"VN2-1");
						}
						break;
				}
			}
			sprintf(cChnCmd,"comp%d.win%d.channel=%s\n", ChnId, WinID, strtmp); 
			mLogDebug("SetWebRecSegModel cChnCmd: "<<cChnCmd<<"  Audio board type: "<<StrParam[4].c_str());
	//		sprintf(cChnCmd,"comp%d.win%d.channel=VN%d-1\n", ChnId, WinID, VideoID); 
		}
		else if (VideoID > 11)
		{
			sprintf(cChnCmd,"comp%d.win%d.channel=VD%d-1\n", ChnId, WinID, VideoID); 
		}

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSetWebRecCtrl::SetWebRecTaskDel(string TaskId, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    sprintf(cChnCmd,INTERFACE_SETRECODELETE_SENDCMD, TaskId.c_str());    
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSetWebRecCtrl::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    return true;
}

string CSetWebRecCtrl::ComposeResult()
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

bool CSetWebRecCtrl::GetComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    cJSON_AddItemToObject(_result, GET_INFOLIST_RES_VIDEOFILE, _VideoFiles);
    return true;
}

string CSetWebRecCtrl::GetComposeResult()
{
    cJSON * result = cJSON_CreateObject();
    if(GetComposeResult(result) == false){
        cJSON_Delete(result);
        return "";
    }else{
        char *cRes = cJSON_Print(_result);
        string res = cRes;
        cJSON_Delete(result);
        return res;
    }
}

InterfaceResCode CSetWebRecCtrl::SetWebRecCtrl(string &sResult,int ChnID, int OnOff, string TaskID, int CurrentState)
{
    //TODO
    mLogInfo("SetWebRecCtrl...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
#if 1
    if(SetWebRec(ChnID, OnOff, TaskID, CurrentState, oResult, cResult) == false){
        mLogError("Failed to run  SetWebRec(...)");
        return eInterfaceResCodeError;
    }
#endif 
    
    ;
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("SetRecCtrlStart\n");
    for (int i = 0; i < iRecvLength; i++)
    {
	    if (i < 4)
	    {
		    printf("%d", cResult[i]);
			mLogInfo("SetWebRec cResult[5]:"<<cResult[i]);
	    }
	    else 
	    {
		    printf("%c", cResult[i]);
			mLogInfo("111SetWebRec cResult[5]:"<<cResult[i]);
	    }
    }
#endif 
//	mLogInfo("SetWebRec cResult[4]:"<<cResult[4]);

	if (cResult[4] != 0)
	{
		mLogError("SetWebRec error！ cResult[5]:"<<cResult[4]);
		return eInterfaceResCodeError;
	}


#if 0
    if( _AnalyParas.ConvertSTRING2CJSONSetRecCtrl(oResult, cResult, _result) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetRecCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}
bool CSetWebRecCtrl::getDecConfig(int DecId,CData &oResult, char *cResult)
{
    mLogDebug("run getDecConfig(...)");
    //获取状态

    //add fwb 20201104
    int  i = 0;
    int  itmplen   = 0;
    char cDecTmpbuf[128] = {0};
    char cDecCmd[1024]   = {0};


    sprintf(cDecCmd,"yuan1.info\nyuan2.info\nyuan3.info\nsys.audio_type\nnet_recv100.app_info\nh323h1.call\nsip100.call\n");

#if 0
    for (i = 1; i < RtspNum + 1; i++)
    {
        sprintf(cDecTmpbuf,INTERFACE_GETHOSTDECRTSP_SENDCMD,i,i,i);
        //printf("%d RtspNum %d ctmpbuff %s", i, RtspNum, cDecTmpbuf);
        memcpy(cDecCmd + itmplen, cDecTmpbuf, strlen(cDecTmpbuf));
        itmplen += strlen(cDecTmpbuf);
    }
    for (i = 1; i < H323Num + 1; i++)
    {
        sprintf(cDecTmpbuf,INTERFACE_GETHOSTDECH323_SENDCMD,i);
        //printf("%d RtspNum %d ctmpbuff %s", i, H323Num, cDecTmpbuf);
        memcpy(cDecCmd + itmplen, cDecTmpbuf, strlen(cDecTmpbuf));
        itmplen += strlen(cDecTmpbuf);
    }
    //printf("len %d \n%s", strlen(cDecCmd), cDecCmd);
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cDecCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cDecCmd) >> 8;  
    cHeadCmd[3] = strlen(cDecCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cDecCmd, strlen(cDecCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

InterfaceResCode CSetWebRecCtrl::SetWebRecSegModelSelect(string &sResult,int ChnID,int WinID,int VideoID)
{
    //TODO
    mLogInfo("SetWebRecSegModelSelect...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
	string Method = "SetWebRecSegModelSelect";
    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
#if 1
	if(getDecConfig(100, oResult, cResult) == false){
        mLogError("Failed to run  getMPSConfig(...)");
        return eInterfaceResCodeError;
    }
	if( _AnalyParas.ConvertSTRING2CJSONGetParam(oResult, cResult, _result, NULL,NULL, Method, StrParam, 0) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetParam(...)");
        return eInterfaceResCodeError;
    }

    if(SetWebSegModelSelect(ChnID, WinID, VideoID,oResult,cResult) == false){
        mLogError("Failed to run  SetWebSegModelSelect(...)");
        return eInterfaceResCodeError;
    }
#endif 
    
    ;
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("SetRecCtrlStart\n");
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

#if 1
    if( _AnalyParas.ConvertSTRING2CJSONSetRecCtrl(oResult, cResult, _result) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetRecCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}


InterfaceResCode CSetWebRecCtrl::SetWebRecSegModel(string &sResult,int ChnID,string SegModel)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
#if 1
    if(SetWebSegModel(ChnID, SegModel, oResult, cResult) == false){
        mLogError("Failed to run  SetWebRec(...)");
        return eInterfaceResCodeError;
    }
#endif 
    
    ;
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("SetRecCtrlStart\n");
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

#if 1
    if( _AnalyParas.ConvertSTRING2CJSONSetRecCtrl(oResult, cResult, _result) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetRecCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetWebRecCtrl::SetWebRecMatrix(string &sResult,int OnOff, int ChnID,int VideoID)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
#if 1
    if(SetWebMatrix(OnOff, ChnID, VideoID, oResult, cResult) == false){
        mLogError("Failed to run  SetWebRec(...)");
        return eInterfaceResCodeError;
    }
#endif 
    
    ;
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("SetRecCtrlStart\n");
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

#if 1
    if( _AnalyParas.ConvertSTRING2CJSONSetRecCtrl(oResult, cResult, _result) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetRecCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetWebRecCtrl::GetWebRecCtrlInfoList(string &sResult,string TaskId, string StartTime, string EndTime, int PageSize, int PageNum, int GetMode)
{
    //TODO
    mLogInfo("GetWebRecCtrlInfoList...");
    CData oResult = SVSMAP();
    //char cResult[INFO_LIST_MAX_BUFF] = {0};
	char *cResult = (char*)malloc(2*1024*1024);
	char FileStartTime[128],FileEndTime[128];
	int TotalFileSize;
    //memset(cResult, 0, INFO_LIST_MAX_BUFF);
    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());

	//int GetMode;
#if 1
    if(PageSize > 1000)
    {
		PageSize = 1000;
	}
	int StartIndex = (PageNum - 1) * PageSize +1;
	int EndIndex = PageNum * PageSize;
    if(GetWebRecInfoList(&GetMode, TaskId, StartTime, EndTime, StartIndex, EndIndex, oResult, cResult) == false){
        mLogError("Failed to run  GetWebRecInfoList(...)");
        return eInterfaceResCodeError;
    }
#endif 
#if 0
    unsigned int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("GetRecCtrlInfoList %d\n", iRecvLength);
    for (int i = 0; i < iRecvLength; i++)
    {
	    if (i < 4)
	    {
		    printf("%d", cResult[i]);
			mLogDebug("run GetWebRecSegModelInner(...)"<<cResult[i]);
	    }
	    else 
	    {
		    printf("%c", cResult[i]);
			mLogDebug("Result"<<cResult[i]);
	    }
    }
#endif 

#if 1


    if( _AnalyParas.ConvertSTRING2CJSONGetRecCtrl(oResult, cResult, GetMode, _VideoFiles, _result, StartIndex, EndIndex) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetRecCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = GetComposeResult();
	free(cResult);
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetWebRecCtrl::SetWebRecCtrlTaskDel(string &sResult, string TaskId)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetRecCtrlTaskDel TaskId %s\n", TaskId.c_str());
#if 1
    if(SetWebRecTaskDel(TaskId, oResult, cResult) == false){
        mLogError("Failed to run  SetWebRecTaskDel(...)");
        return eInterfaceResCodeError;
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

#if 0
    if( _AnalyParas.ConvertSTRING2CJSONSetRecCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetRecCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}
bool CSetWebRecCtrl::GetWebRecSegModelInner(int ChannelID, CData &oResult, char *cResult)
{
		mLogDebug("run GetWebRecSegModelInner(...)");
		//获取状态
	
		
		char cChnCmd[4096] = {0};
#if 1
		sprintf(cChnCmd,INTERFACE_GETRECSEG_MODEL, ChannelID);	 
#endif 
	
		
		char cCmdSend[1024] = {0};
		char cHeadCmd[4] = {0};
		//char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
		int  cCmdLength = 0;
		memset(cCmdSend, 0, sizeof(1024)); 
	
		cCmdLength = strlen(cChnCmd) + 4;
		cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
		cHeadCmd[1] = 2; //获取参数指令（GET）
		cHeadCmd[2] = strlen(cChnCmd) >> 8;  
		cHeadCmd[3] = strlen(cChnCmd) & 0xff;
	
		memcpy(cCmdSend, cHeadCmd, 4);
		memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));
	
		MPSOperationRes opResCode = eMPSResultOK;
		ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
		if(resCode != eResponseCodeSuccess){
			mLogError("GetConfig(...) error:"<<resCode);
			return false;
		}
	
		return true;

}

InterfaceResCode CSetWebRecCtrl::GetWebRecSegModel(string &sResult,int ChannelID)
{
		//TODO
		mLogInfo("GetWebRecSegModel...");
		CData oResult = SVSMAP();
		char cResult[4096] = {0};
		//printf("SetRecCtrlTaskDel TaskId %s\n", TaskId.c_str());
#if 1
		if(GetWebRecSegModelInner(ChannelID, oResult, cResult) == false){
			mLogError("Failed to run  GetWebRecSegModelInner(...)");
			return eInterfaceResCodeError;
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
	
#if 1
		if( _AnalyParas.ConvertSTRING2CJSONGetWebRecSegModel(oResult, cResult, _result) == false){
			mLogError("Failed to run  ConvertSTRING2CJSONSetRecCtrl(...)");
			return eInterfaceResCodeError;
		}
#endif 
	
		sResult = ComposeResult();
		cJSON_Delete(_result);
		return eInterfaceResCodeSuccess;

}
void CSetWebRecCtrl::getFiles(std::string path, std::vector<std::string> &files, std::vector<std::string> &filesPath)
{
  DIR *dir;
  struct dirent *ptr;
 
  if ((dir = opendir(path.c_str())) == NULL)
  {
    perror("Open dir error...");
    exit(1);
  }

  while ((ptr = readdir(dir)) != NULL)
  {
    if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
      continue;
    else if (ptr->d_type == 8) // 普通文件
    {
      //filesPath.push_back(path + ptr->d_name);
      files.push_back(ptr->d_name);
    }
    else if (ptr->d_type == 10) // 符号链接
      continue;
    else if (ptr->d_type == 4) // 目录文件
    {
      //files.push_back(ptr->d_name);
      //getFiles(path + ptr->d_name + "/", files, filesPath);
	  filesPath.push_back(ptr->d_name);
    }
  }
  closedir(dir);
}

InterfaceResCode CSetWebRecCtrl::GetWebRecTaskID(string &sResult)
{
#if 1
	mLogDebug("run GetWebRecTaskID(...)");
	char PathFile[1024]   = {0};
	char cmd_num[1024]   = {0};
	bool Flag = false;
	string file_path,file_name;
	vector<string> filename,filepath;
	struct stat tbuf;
	int filsesize;

	//cJSON*	lognode[MAX_LOGFILE_NUM];
	
	getFiles("/mnt/HD0/", filename, filepath);
	
	cJSON_AddItemToObject(_result, GET_WEB_INFRARED_STUDYLIST_RES_TOTAL, cJSON_CreateNumber(filepath.size()));
	for (int i = 0; i < filepath.size(); i++)
	{
	  //cout << filename[i] << endl;
	  	cJSON*	arrayItem = cJSON_CreateObject();
	  	//file_name = filename[i];
		file_path = filepath[i];
		
		cJSON_AddItemToObject(arrayItem, "NumBer", cJSON_CreateNumber(i));
		
		cJSON_AddItemToObject(arrayItem, "TaskID", cJSON_CreateString(file_path.c_str()));
		
		cJSON_AddItemToArray(_VideoFiles, arrayItem);	
		
	}
	cJSON_AddItemToObject(_result, GET_INFOLIST_RES_VIDEOFILE, _VideoFiles);
	sResult = ComposeResult();
	
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
#else	
	mLogInfo("GetWebRecTaskID...");
	CData oResult = SVSMAP();
	//char cResult[INFO_LIST_MAX_BUFF] = {0};
	char *cResult = (char*)malloc(2*1024*1024);
	//memset(cResult, 0, INFO_LIST_MAX_BUFF);
	//printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
	int StartIndex = 1;
	int EndIndex = 2000;
	int GetMode = 7;
#if 1
	if(GetWebRecInfoList(&GetMode, NULL, NULL, NULL, StartIndex, EndIndex, oResult, cResult) == false){
		mLogError("Failed to run  GetWebRecTaskID(...)");
		return eInterfaceResCodeError;
	}
#endif 
#if 0
	unsigned int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
	printf("GetRecCtrlInfoList %d\n", iRecvLength);
	for (int i = 0; i < iRecvLength; i++)
	{
		if (i < 4)
		{
			printf("%d", cResult[i]);
			mLogDebug("run GetWebRecSegModelInner(...)"<<cResult[i]);
		}
		else 
		{
			printf("%c", cResult[i]);
			mLogDebug("Result"<<cResult[i]);
		}
	}
#endif 

#if 1
	if( _AnalyParas.ConvertSTRING2CJSONGetRecCtrl(oResult, cResult, GetMode, _VideoFiles, _result, StartIndex, EndIndex) == false){
		mLogError("Failed to run  ConvertSTRING2CJSONGetRecCtrl(...)");
		return eInterfaceResCodeError;
	}
#endif 

	sResult = GetComposeResult();
	free(cResult);
	return eInterfaceResCodeSuccess;

#endif
}
bool CSetWebRecCtrl::SetWebRcrvRecordInner(CData &oResult, char *cResult, int RecOn)
{
			mLogDebug("run SetWebRcrvRecordInner(...)");
			//获取状态
		
			
			char cChnCmd[4096] = {0};
#if 1
			sprintf(cChnCmd,"rec.close_court_rec_on=%d\n", RecOn);	 
#endif 
		
			
			char cCmdSend[1024] = {0};
			char cHeadCmd[4] = {0};
			//char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
			int  cCmdLength = 0;
			memset(cCmdSend, 0, sizeof(1024)); 
		
			cCmdLength = strlen(cChnCmd) + 4;
			cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
			cHeadCmd[1] = 3; //获取参数指令（GET）
			cHeadCmd[2] = strlen(cChnCmd) >> 8;  
			cHeadCmd[3] = strlen(cChnCmd) & 0xff;
		
			memcpy(cCmdSend, cHeadCmd, 4);
			memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));
		
			MPSOperationRes opResCode = eMPSResultOK;
			ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
			if(resCode != eResponseCodeSuccess){
				mLogError("GetConfig(...) error:"<<resCode);
				return false;
			}
		
			return true;


}

InterfaceResCode CSetWebRecCtrl::SetWebRcrvRecord(string &sResult,int RecOn)
{
			//TODO
			mLogInfo("SetWebRcrvRecord...");
			CData oResult = SVSMAP();
			char cResult[4096] = {0};
			//printf("SetRecCtrlTaskDel TaskId %s\n", TaskId.c_str());
#if 1
			if(SetWebRcrvRecordInner(oResult, cResult, RecOn) == false){
				mLogError("Failed to run  GetWebRecSegModelInner(...)");
				return eInterfaceResCodeError;
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
		
#if 0
			if( _AnalyParas.ConvertSTRING2CJSONGetWebRecSegModel(oResult, cResult, _result) == false){
				mLogError("Failed to run  ConvertSTRING2CJSONSetRecCtrl(...)");
				return eInterfaceResCodeError;
			}
#endif 
		
			sResult = ComposeResult();
			cJSON_Delete(_result);
			return eInterfaceResCodeSuccess;

}

bool CSetWebRecCtrl::GetWebRcrvRecordInner(CData &oResult, char *cResult)
{
			mLogDebug("run GetWebRcrvRecordInner(...)");
			//获取状态
		
			
			char cChnCmd[4096] = {0};
#if 1
			sprintf(cChnCmd,"rec.close_court_rec_on\n");	 
#endif 
		
			
			char cCmdSend[1024] = {0};
			char cHeadCmd[4] = {0};
			//char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
			int  cCmdLength = 0;
			memset(cCmdSend, 0, sizeof(1024)); 
		
			cCmdLength = strlen(cChnCmd) + 4;
			cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
			cHeadCmd[1] = 2; //获取参数指令（GET）
			cHeadCmd[2] = strlen(cChnCmd) >> 8;  
			cHeadCmd[3] = strlen(cChnCmd) & 0xff;
		
			memcpy(cCmdSend, cHeadCmd, 4);
			memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));
		
			MPSOperationRes opResCode = eMPSResultOK;
			ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
			if(resCode != eResponseCodeSuccess){
				mLogError("GetConfig(...) error:"<<resCode);
				return false;
			}
		
			return true;


}

InterfaceResCode CSetWebRecCtrl::GetWebRcrvRecord(string &sResult)
{
			//TODO
			mLogInfo("SetWebRcrvRecord...");
			CData oResult = SVSMAP();
			char cResult[4096] = {0};
			//printf("SetRecCtrlTaskDel TaskId %s\n", TaskId.c_str());
#if 1
			if(GetWebRcrvRecordInner(oResult, cResult) == false){
				mLogError("Failed to run  GetWebRecSegModelInner(...)");
				return eInterfaceResCodeError;
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
		
#if 1
			if( _AnalyParas.ConvertSTRING2CJSONGetWebRecSegModel(oResult, cResult, _result) == false){
				mLogError("Failed to run  ConvertSTRING2CJSONSetRecCtrl(...)");
				return eInterfaceResCodeError;
			}
#endif 
		
			sResult = ComposeResult();
			cJSON_Delete(_result);
			return eInterfaceResCodeSuccess;

}
InterfaceResCode CSetWebRecCtrl::SetWebRecFileDel(string &sResult,string TaskId, int type, string filename)
{

			mLogInfo("SetWebRecFileDel...");
			CData oResult = SVSMAP();
			char cResult[4096] = {0};
			//printf("SetRecCtrlTaskDel TaskId %s\n", TaskId.c_str());
#if 1
			if(SetWebRecFileDelInner(TaskId, type, filename, oResult, cResult) == false){
				mLogError("Failed to run  SetWebRecFileDelInner(...)");
				return eInterfaceResCodeError130;
			}
#endif 
		char *ParaBuf = &cResult[5];
		mLogInfo("SetWebRecFileDel cResult="<<ParaBuf);
		if (!strcmp(ParaBuf, "<error>"))
		{
			mLogError("Del Recfile "<<TaskId<<"/"<<filename<<"failed!!");
			return eInterfaceResCodeError131;
		}
#if 0
			int  iParaLen = (cResult[2] << 16) + (cResult[3] << 8) + cResult[4] + 5 ;
    		char *ParaBuf = &cResult[5];

			for (int i = 0; i < iParaLen; i++)
			{
				if (i < 5)
				{
					printf("%d", cResult[i]);
				}
				else 
				{
					printf("%c", cResult[i]);
				}
			}
#endif 
		
#if 0
			if( _AnalyParas.ConvertSTRING2CJSONGetWebRecSegModel(oResult, cResult, _result) == false){
				mLogError("Failed to run  ConvertSTRING2CJSONSetRecCtrl(...)");
				return eInterfaceResCodeError;
			}
#endif 
		
			sResult = ComposeResult();
			cJSON_Delete(_result);
			return eInterfaceResCodeSuccess;

}

bool CSetWebRecCtrl::SetWebRecFileDelInner(string TaskId, int type, string filename, CData &oResult, char *cResult)
{
		mLogDebug("run SetWebRecFileDelInner(...)");
		//获取状态
	
		
		char cChnCmd[4096] = {0};
#if 1
		sprintf(cChnCmd,"<DELF_head><type>%d</type><dirname>%s</dirname><filename>%s</filename></DELF_head>\n",type, TaskId.c_str(), filename.c_str());	 
#endif 
	
		
		char cCmdSend[1024] = {0};
		char cHeadCmd[4] = {0};
		//char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
		int  cCmdLength = 0;
		memset(cCmdSend, 0, sizeof(1024)); 
	
		cCmdLength = strlen(cChnCmd) + 4;
		cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
		cHeadCmd[1] = 7; //获取参数指令（GET）
		cHeadCmd[2] = strlen(cChnCmd) >> 8;  
		cHeadCmd[3] = strlen(cChnCmd) & 0xff;
	
		memcpy(cCmdSend, cHeadCmd, 4);
		memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));
	
		MPSOperationRes opResCode = eMPSResultOK;
		ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
		if(resCode != eResponseCodeSuccess){
			mLogError("GetConfig(...) error:"<<resCode);
			return false;
		}

		return true;

}
InterfaceResCode CSetWebRecCtrl::SetWebTaskIDUpload(string &sResult, string TaskId)
{
    //TODO
    mLogInfo("SetWebTaskIDUpload...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetRecCtrlTaskDel TaskId %s\n", TaskId.c_str());
#if 1
    if(SetWebTaskIDUploadInner(TaskId, oResult, cResult) == false){
        mLogError("Failed to run  SetWebTaskIDUploadInner(...)");
        return eInterfaceResCodeError;
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

#if 0
    if( _AnalyParas.ConvertSTRING2CJSONSetRecCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetRecCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}
bool CSetWebRecCtrl::SetWebTaskIDUploadInner(string TaskId, CData &oResult, char *cResult)
{
		mLogDebug("run SetWebTaskIDUploadInner(...)");
		//获取状态
	
		
		char cChnCmd[4096] = {0};
#if 1
		sprintf(cChnCmd,"<UPFILE_head><dirname>%s</dirname></UPFILE_head>\n",TaskId.c_str());	 
#endif 
	
		
		char cCmdSend[1024] = {0};
		char cHeadCmd[4] = {0};
		//char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
		int  cCmdLength = 0;
		memset(cCmdSend, 0, sizeof(1024)); 
	
		cCmdLength = strlen(cChnCmd) + 4;
		cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
		cHeadCmd[1] = 7; //获取参数指令（GET）
		cHeadCmd[2] = strlen(cChnCmd) >> 8;  
		cHeadCmd[3] = strlen(cChnCmd) & 0xff;
	
		memcpy(cCmdSend, cHeadCmd, 4);
		memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));
	
		MPSOperationRes opResCode = eMPSResultOK;
		ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
		if(resCode != eResponseCodeSuccess){
			mLogError("GetConfig(...) error:"<<resCode);
			return false;
		}

		return true;

}


