#include "SetWebVideoMatrix.h"
#include "MPSClient.h"


CSetWebVideoMatrix::CSetWebVideoMatrix(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebVideoMatrix::~CSetWebVideoMatrix()
{
	
}

bool CSetWebVideoMatrix::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebVideoMatrix::ComposeResult()
{
// 	cJSON* result = cJSON_CreateObject();
// 	if (ComposeResult(result) == false) {
// 		cJSON_Delete(result);
// 		return "";
// 	}
// 	else {
// 		char* cRes = cJSON_Print(_result);
// 		std::string res = cRes;
// 		cJSON_Delete(result);
// 		return res;
// 	}

	//assume the setmethod's response code is always okay. -_-!
	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	return res;
}


InterfaceResCode CSetWebVideoMatrix::SetWebVideoMatrix(std::string& sResult, int ChnnelID, string ChnnelName)
{
	mLogInfo("SetWebVideoMatrix...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebVideoMatrixInner( oResult, cResult, ChnnelID, ChnnelName))
	{
		mLogError("SetWebVideoMatrixInner failed !!!");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}


bool CSetWebVideoMatrix::SetWebVideoMatrixInner( CData& oResult, char* cResult, int ChnnelID, string ChnnelName)
{
	mLogDebug("run SetWebVideoMatrixInner(...)");

	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	
	sprintf(szReqBody,"vid_dis_hd%d.win1.channel=%s\n",ChnnelID, ChnnelName.c_str());

	int realBodySize = strlen(szReqBody);

	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;
	szReqCmd[1] = 3;
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize & 0xff;

	memcpy(&szReqCmd[4], szReqBody, realBodySize);
#if 1
	MPSOperationRes opResCode = eMPSResultOK;
	ResponseCode resCode = _mpsClient->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
	if (resCode != eResponseCodeSuccess)
	{
		mLogError("SetWebVideoMatrixInner Run Failed !!!");
		return false;
	}
#endif

	return true;

}
InterfaceResCode CSetWebVideoMatrix::SetWebVideoMatrixName(std::string& sResult, string InPutChnnelName1, string InPutChnnelName2, string InPutChnnelName3, string InPutChnnelName4, string InPutChnnelName5, string InPutChnnelName6, string InPutChnnelName7, string InPutChnnelName8, string InPutChnnelName9, string OutPutChnnelName1, string OutPutChnnelName2)
{
	mLogInfo("SetWebVideoMatrixName...");

	sqlite3 *reg_db = NULL;
	char cmdbuf[32];
	char *register_db = "Audio_manage.db";
	
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	char *table = "create table MatrixName (id INTEGER, name TEXT, val TEXT, time TEXT)";
	char deltab[1024];
	char instab[1024];
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库,如已创建则忽略
	create_self_db_table(reg_db, table);  //创建注册表,如已创建则忽略
	
	//初始化数据结构
	for(int i = 1;i <= 11; i++)
	{
		memset(stu.val, 0, 256);
		stu.id = i;
		sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
		switch(i)
		{
			case 1:
				sprintf(stu.val,"%s",InPutChnnelName1.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 2:
				sprintf(stu.val,"%s",InPutChnnelName2.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;				
			case 3:
				sprintf(stu.val,"%s",InPutChnnelName3.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 4:
				sprintf(stu.val,"%s",InPutChnnelName4.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 5:
				sprintf(stu.val,"%s",InPutChnnelName5.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 6:
				sprintf(stu.val,"%s",InPutChnnelName6.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;				
			case 7:
				sprintf(stu.val,"%s",InPutChnnelName7.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 8:
				sprintf(stu.val,"%s",InPutChnnelName8.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 9:
				sprintf(stu.val,"%s",InPutChnnelName9.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 10:
				sprintf(stu.val,"%s",OutPutChnnelName1.c_str());
				sprintf(stu.name,"OutPutChnnelName%d",i-9);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;			
			case 11:
				sprintf(stu.val,"%s",OutPutChnnelName2.c_str());
				sprintf(stu.name,"OutPutChnnelName%d",i-9);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;

		}

		delete_self_stu_db(reg_db, deltab);
		insert_self_stu_db(reg_db, instab); //add 到数据库 
		
	}
	sqlite3_close(reg_db);
	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}

