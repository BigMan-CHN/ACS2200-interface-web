#include "GetWebVideoMatrix.h"
#include "MPSClient.h"


CGetWebVideoMatrix::CGetWebVideoMatrix(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
	, _ParamList(cJSON_CreateArray())
{

}

CGetWebVideoMatrix::~CGetWebVideoMatrix()
{
	
}

bool CGetWebVideoMatrix::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CGetWebVideoMatrix::ComposeResult()
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


InterfaceResCode CGetWebVideoMatrix::GetWebVideoMatrix(std::string& sResult)
{
	mLogInfo("GetWebVideoMatrix...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };
	string Method = "GetWebVideoMatrix";
	string ip;
	cJSON* temList[5];

	for(int i = 1; i <= 2; i++)
	{
		//用于生成节点，存储解析后的参数
		temList[i] = cJSON_CreateObject();
	}

	if (!GetWebVideoMatrixInner( oResult, cResult))
	{
		mLogError("GetWebVideoMatrixInner failed !!!");
		return eInterfaceResCodeError;
	}
	if(!_analyzePara.ConvertSTRING2CJSONGetWebParam(oResult, cResult, _result, _ParamList, temList, Method, ip, 0))
    {
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebParam(...)");
		return eInterfaceResCodeError;
    }
	for(int i = 1; i <= 2; i++)
	{
		cJSON_AddItemToArray(_ParamList, temList[i]);					
	} 
	cJSON_AddItemToObject(_result,"List",_ParamList);	

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}


bool CGetWebVideoMatrix::GetWebVideoMatrixInner( CData& oResult, char* cResult)
{
	mLogDebug("run GetWebVideoMatrixInner(...)");


	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	
	sprintf(szReqBody,"vid_dis_hd1.win1.channel\nvid_dis_hd2.win1.channel\n");

	int realBodySize = strlen(szReqBody);

	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;
	szReqCmd[1] = 2;
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
InterfaceResCode CGetWebVideoMatrix::GetWebVideoMatrixName(std::string& sResult)
{
	mLogInfo("GetWebVideoMatrixName...");
	sqlite3 *reg_db = NULL;
	char buf[1024];
	char *register_db = "Audio_manage.db";
	
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	char find_table[1024];
	char *table = "create table MatrixName (id INTEGER, name TEXT, val TEXT, time TEXT)";
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库,如已创建则忽略
	create_self_db_table(reg_db, table);  //创建注册表,如已创建则忽略
	
	for(int i = 1;i <= 11; i++)
	{
		memset(buf, 0, 1024);
		switch(i)
		{
			case 1:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 2:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;				
			case 3:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 4:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 5:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 6:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;				
			case 7:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 8:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 9:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 10:
				
				sprintf(stu.name,"OutPutChnnelName%d",i-9);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;			
			case 11:
				
				sprintf(stu.name,"OutPutChnnelName%d",i-9);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
		}
		
		find_self_stu_db(reg_db, find_table, buf);
		cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(buf));

	}
	sqlite3_close(reg_db);
	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}


