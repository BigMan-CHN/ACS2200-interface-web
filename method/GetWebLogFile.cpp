#include "GetWebLogFile.h"
#include "MPSClient.h"
#define MAX_LOGFILE_NUM 1000

CGetWebLogFile::CGetWebLogFile(const std::string& ip, unsigned short port, int timeOut)
	:_result(cJSON_CreateObject())
	,_logList(cJSON_CreateArray())
	, _mpsClient(new MPSClient(ip, port, timeOut))
{

}

CGetWebLogFile::~CGetWebLogFile()
{
}

bool CGetWebLogFile::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}
// 
// 	if (_nbTaskList == NULL) {
// 		mLogWarn("cJSON _nbTaskList == NULL.");
// 		return false;
// 	}
// 
// 	//result中添加节点
// 
// 	cJSON_AddItemToObject(_result, GET_HOST_NBINFO_TASKLIST, _nbTaskList);
	return true;
}

std::string CGetWebLogFile::ComposeResult()
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

	if (_logList == NULL)
	{
		mLogError("_logList is NULL !!!");
		return "";
	}

	cJSON_AddItemToObject(_result, GET_WEB_LOG_FILE_RES_LOGLIST, _logList);

	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	//cJSON_Delete(result);
	return res;
}
void CGetWebLogFile::getFiles(std::string path, std::vector<std::string> &files, std::vector<std::string> &filesPath)
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

InterfaceResCode CGetWebLogFile::GetWebLogFile(string& sResult)
{
	mLogInfo("GetWebLogFile...");

	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();

	//1. 调用类内部方法进一步封装业务请求
	if (!GetWebLogFileInner(oResult, cResult))
	{
		mLogError("Failed to run  GetWebLogFileInner(...)");
		return eInterfaceResCodeError;
	}
	
	//2. 解析响应,存入类型为cJson对象的成员变量中
	if (!_analyPara.ConvertSTRING2CJSONGetWebLogFile(oResult, cResult, _result, _logList)) {
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebLogFile(...)");
		return eInterfaceResCodeError;
	}

	vector<string> filename,filepath;
	struct stat tbuf;
	struct tm *info;
	char buffer[80];
	int filsesize;
	string ftp_addr,creat_time;
	string ftpaddr = "ftp://192.168.1.245";

	//cJSON*	lognode[MAX_LOGFILE_NUM];
	cJSON*	lognode = NULL;
	getFiles("/mnt/HD0/log/", filename, filepath);
	
	for (int i = 0; i < filename.size(); i++)
	{
	  //cout << filename[i] << endl;
	  if(MAX_LOGFILE_NUM < filename.size())
	  {
		mLogError("MAX_LOGFILE_NUM is less strVec.size():"<<filename.size());
		return eInterfaceResCodeError;
	  }
	  
	  stat(filepath[i].c_str(), &tbuf);
	  
	  //ftp_addr = ftpaddr + filepath[i];
	  ftp_addr = filepath[i];

	  filsesize = (int)tbuf.st_size/1024;
	  
	  //creat_time = ctime(&tbuf.st_mtime);
	  //获得的tm结构体的时间
	  info = localtime(&tbuf.st_mtime);
	  //将时间格式化为想要的格式
	  strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);
	  //mLogDebug("st_mtime="<<buffer);
	  lognode = cJSON_CreateObject();
	  cJSON_AddItemToObject(lognode, GET_WEB_LOG_FILE_RES_LOGNAME ,cJSON_CreateString(filename[i].c_str()));
	  cJSON_AddItemToObject(lognode, GET_WEB_LOG_FILE_RES_LOGNUM  ,cJSON_CreateNumber(i+1));
	  cJSON_AddItemToObject(lognode, GET_WEB_LOG_FILE_RES_LOGSIZE ,cJSON_CreateNumber(filsesize));
	  //cJSON_AddItemToObject(lognode, GET_WEB_LOG_FILE_RES_LOGTIME ,cJSON_CreateString(creat_time.c_str()));
	  cJSON_AddItemToObject(lognode, GET_WEB_LOG_FILE_RES_LOGTIME ,cJSON_CreateString(buffer));
	  cJSON_AddItemToObject(lognode, GET_WEB_LOG_FILE_RES_LOGADDR ,cJSON_CreateString("/mnt/HD0/log/"));
	  cJSON_AddItemToArray(_logList,lognode);
	  
	}
	cJSON_AddNumberToObject(_result, "Total", filename.size());


	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();

	return eInterfaceResCodeSuccess;
}

bool CGetWebLogFile::GetWebLogFileInner(CData& oResult, char* cResult)
{
	mLogDebug("run GetWebLogFileInner(...)");

	//1. 封装业务请求
	char szReqBody[REQ_BODY_MAXLEN] = { 0 };
	sprintf(szReqBody,"syslog.all\n");
//	sprintf(szReqBody,"syslog.partition_size\nsyslog.time_limit\nsyslog.option\n");
	int realBodySize = strlen(szReqBody);

	char szReqCmd[REQ_CMD_MAXLEN] = { 0 };
	szReqCmd[0] = 0;  //不加密
	szReqCmd[1] = 2;  //Get请求, 3是SET, 4是SYS
	szReqCmd[2] = realBodySize >> 8;
	szReqCmd[3] = realBodySize;
	memcpy(&szReqCmd[4], szReqBody, realBodySize);

#if 1
	//2. 发送socket请求
	MPSOperationRes opResCode = eMPSResultOK; 
	ResponseCode resCode = _mpsClient->GetConfigNew(szReqCmd, 4 + realBodySize, opResCode, oResult, cResult);
	if (resCode != eResponseCodeSuccess) {
		mLogError("GetConfig(...) error:" << resCode);
		return false;
	}
#else
	//dummy response
#endif

	return true;
}
