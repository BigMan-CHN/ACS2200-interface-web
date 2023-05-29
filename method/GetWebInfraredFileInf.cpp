#include "GetWebInfraredFileInf.h"
#include "MPSClient.h"

CGetWebInfraredFileInf::CGetWebInfraredFileInf(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	, _result(cJSON_CreateObject())
	, _ParamList(cJSON_CreateArray())
{
}

CGetWebInfraredFileInf::~CGetWebInfraredFileInf()
{
}


bool CGetWebInfraredFileInf::ComposeResult(cJSON* result)
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

std::string CGetWebInfraredFileInf::ComposeResult()
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

	if (_ParamList == NULL)
	{
		mLogError("_videoParamList is NULL !!!");
		return "";
	}

	cJSON_AddItemToObject(_result, "ParamList", _ParamList);

	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	//cJSON_Delete(result);
	return res;
}

InterfaceResCode CGetWebInfraredFileInf::GetWebInfraredFileInf(string& sResult)
{
	mLogInfo("获取 GetWebInfraredFileInf...");

	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();

	//1. 调用类内部方法进一步封装业务请求
	if (!GetWebInfraredFileInfListInner(oResult, cResult))
	{
		mLogError("Failed to run  GetWebInfraredFileInfListInner(...)");
		return eInterfaceResCodeError;
	}
#if 0
	//2. 解析响应,存入类型为cJson对象的成员变量中
	if (!_analyPara.ConvertSTRING2CJSONGetWebInfraredStudyList(oResult, cResult, _result, _ParamList)) {
		mLogError("Failed to run  ConvertSTRING2CJSONGetWebInfraredStudyList(...)");
		return eInterfaceResCodeError;
	}
#endif
	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	cJSON_Delete(_result);

	return eInterfaceResCodeSuccess;
}
xmlXPathObjectPtr CGetWebInfraredFileInf::get_nodeset(xmlDocPtr doc, const xmlChar *szXpath) 
{
	xmlXPathContextPtr context;	    //XPATH上下文指针
	xmlXPathObjectPtr result;		//XPATH对象指针，用来存储查询结果
	
	context = xmlXPathNewContext(doc);		//创建一个XPath上下文指针
	if (context == NULL) 
	{	
		printf("context is NULL\n");
		return NULL; 
	}
	
	result = xmlXPathEvalExpression(szXpath, context); //查询XPath表达式，得到一个查询结果
	xmlXPathFreeContext(context);				//释放上下文指针
	if (result == NULL) 
	{
		printf("xmlXPathEvalExpression return NULL\n"); 
		return NULL;  
	}
	
	if (xmlXPathNodeSetIsEmpty(result->nodesetval))   //检查查询结果是否为空
	{
		xmlXPathFreeObject(result);
		printf("nodeset is empty\n");
		return NULL;
	}
	
	return result;	
}
void CGetWebInfraredFileInf::getFiles(std::string path, std::vector<std::string> &files, std::vector<std::string> &filesPath)
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

bool CGetWebInfraredFileInf::GetWebInfraredFileInfListInner(CData& oResult, char* cResult)
{
	mLogDebug("run GetWebInfraredFileInfListInner(...)");

	xmlDocPtr doc = NULL;			 //定义解析文档指针 
	xmlNodePtr RootNode= NULL;			
	xmlNodePtr curNode = NULL;		 //定义结点指针(你需要它为了在各个结点间移动) 
	char PathFile[1024]   = {0};
	char cmd_num[1024]   = {0};
	bool Flag = false;
	string file,file_name;
	vector<string> filename,filepath;
	struct stat tbuf;
	int filsesize;

	//cJSON*	lognode[MAX_LOGFILE_NUM];
	cJSON*	lognode = NULL;
	getFiles("/root/Communicate_Schedule_exe/config/hongwai/", filename, filepath);
	
	cJSON_AddItemToObject(_result, GET_WEB_INFRARED_STUDYLIST_RES_TOTAL, cJSON_CreateNumber(filename.size()));
	for (int i = 0; i < filename.size(); i++)
	{
	  //cout << filename[i] << endl;
	  	file_name = filename[i];
		file = filepath[i];
		doc = xmlReadFile(file.c_str(),"UTF-8",XML_PARSE_RECOVER);  //解析文件 
		if (NULL == doc) 
		{ 	
			//printf("Document not parsed successfully.\n"); 
			mLogError("Document not parsed successfully.\n");
			return false; 
		} 
		//获得根节点
		RootNode = xmlDocGetRootElement(doc); 
		/*检查确认当前文档中包含内容*/ 
		if (NULL == RootNode) 
		{ 
			mLogError("empty document. \n");
			xmlFreeDoc(doc); 
			return false; 
		} 
		//查询并得到root节点的所有子节点
		xmlXPathObjectPtr app_result = get_nodeset(doc,BAD_CAST ("/root/*"));  
		
		if (NULL == app_result) 
		{
			mLogError("app_result is NULL\n");
			xmlFreeDoc(doc);
			return false; 
		}
		xmlChar *szValue = NULL;
		xmlChar *attrValue = NULL;
	
		if(app_result) 
		{	//获得子节点集合
			xmlNodeSetPtr nodeset = app_result->nodesetval;
			cJSON* arrayItem = cJSON_CreateObject();
			cJSON_AddItemToObject(arrayItem, "FileName", cJSON_CreateString(file_name.c_str()));
			for (int i = 0; i < nodeset->nodeNr; i++) //nodeset->nodeNr：子节点的总个数
			{	//取出子节点
				curNode = nodeset->nodeTab[i];
				//sprintf("curNode->name:%s\n",curNode->name);
	
				if(curNode != NULL) 
				{				
					if (!xmlStrcmp(curNode->name, BAD_CAST "DevID"))
					{
						//cJSON_AddItemToObject(arrayItem, GET_WEB_INFRARED_STUDYLIST_RES_NUM, cJSON_CreateNumber(atoi((char*)(curNode->name))));
						szValue = xmlNodeGetContent(curNode);
						cJSON_AddItemToObject(arrayItem, "DevID", cJSON_CreateString((char*)szValue));
						xmlFree(szValue);
						continue;
					}
				
					if (!xmlStrcmp(curNode->name, BAD_CAST "DevType"))
					{
						//cJSON_AddItemToObject(arrayItem, GET_WEB_INFRARED_STUDYLIST_RES_NUM, cJSON_CreateNumber(atoi((char*)(curNode->name))));
						szValue = xmlNodeGetContent(curNode);
						cJSON_AddItemToObject(arrayItem, "DevType", cJSON_CreateString((char*)szValue));
						xmlFree(szValue);
						continue;
					}
				
					if (!xmlStrcmp(curNode->name, BAD_CAST "Manufacturer"))
					{
						//cJSON_AddItemToObject(arrayItem, GET_WEB_INFRARED_STUDYLIST_RES_NUM, cJSON_CreateNumber(atoi((char*)(curNode->name))));
						szValue = xmlNodeGetContent(curNode);
						cJSON_AddItemToObject(arrayItem, "Manufacturer", cJSON_CreateString((char*)szValue));
						xmlFree(szValue);
						continue;
					}	
					
				}
				
			}
			cJSON_AddItemToArray(_ParamList, arrayItem);
			xmlXPathFreeObject (app_result);
		}

	}
	
	return true;
}
