#include "SetWebInfraredCreate.h"
#include "MPSClient.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <iconv.h>
#include <string.h>


CSetWebInfraredCreate::CSetWebInfraredCreate(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	, _result(cJSON_CreateObject())
{
}

CSetWebInfraredCreate::~CSetWebInfraredCreate()
{
}


bool CSetWebInfraredCreate::ComposeResult(cJSON* result)
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

std::string CSetWebInfraredCreate::ComposeResult()
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

	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	return res;
}

InterfaceResCode CSetWebInfraredCreate::SetWebInfraredCreate(string& sResult, 
	int port,
	const std::string& fileName, string DevID, string DevType, string Manufacturer)
{
	mLogInfo("设置 WebInfraredCreate...");

	char cResult[RES_BUF_MAXLEN] = { 0 };
	CData oResult = SVSMAP();

	//1. 调用类内部方法进一步封装业务请求
	if (!SetWebInfraredCreateInner(port, fileName, oResult, cResult, DevID, DevType, Manufacturer))
	{
		mLogError("Failed to run  SetWebInfraredCreateInner(...)");
		return eInterfaceResCodeError;
	}

	//2. 解析响应,存入类型为cJson对象的成员变量中
	if (!_analyPara.ConvertSTRING2CJSONSetWebInfraredCreate(oResult, cResult)) {
		mLogError("Failed to run  ConvertSTRING2CJSONSetWebInfraredCreate(...)");
		return eInterfaceResCodeError;
	}

	//3. 将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();

	return eInterfaceResCodeSuccess;
}
//代码转换:从一种编码转为另一种编码   
int CSetWebInfraredCreate::code_convert(char* from_charset, char* to_charset, char* inbuf,
				 int inlen, char* outbuf, int outlen)
{
	iconv_t cd;
	char** pin = &inbuf;   
	char** pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);   
	if(cd == 0)
		return -1;
	memset(outbuf,0,outlen);   
	if(iconv(cd,(char**)pin,(size_t*)&inlen,pout,(size_t*)&outlen)== -1)
		return -1;   
	iconv_close(cd);
	return 0;   
}

//UNICODE码转为GB2312码   
//成功则返回一个动态分配的char*变量，需要在使用完毕后手动free，失败返回NULL
char* CSetWebInfraredCreate::utf8_to_gb2312(char *inbuf)   
{
	int nOutLen = 2 * strlen(inbuf) - 1;
	char* szOut = (char*)malloc(nOutLen);
	
	if (-1 == code_convert("utf-8","gb2312",inbuf,strlen(inbuf),szOut,nOutLen))
	{
		free(szOut);
		szOut = NULL;
	}
	return szOut;
}   

//GB2312码转为UNICODE码   
//成功则返回一个动态分配的char*变量，需要在使用完毕后手动free，失败返回NULL
char* CSetWebInfraredCreate::gb2312_to_utf8(char *inbuf)   
{
	int nOutLen = 2 * strlen(inbuf) - 1;
	char* szOut = (char*)malloc(nOutLen);
	
	if (-1 == code_convert("gb2312","utf-8",inbuf,strlen(inbuf),szOut,nOutLen))
	{
		free(szOut);
		szOut = NULL;
	}
	return szOut;
}   

bool CSetWebInfraredCreate::SetWebInfraredCreateInner(int port, 
	const std::string& fileName, CData& oResult, char* cResult, string DevID, string DevType, string Manufacturer)
{
	mLogDebug("run SetWebInfraredCreateInner(...)");

	//定义文档和节点指针
	char PathFile[1024]  = {0};
	xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
	xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"root");
	
	//设置根节点
	xmlDocSetRootElement(doc,root_node);
#if 0	
	//一个中文字符串转换为UTF-8字符串，然后写入
	char* DevID_utf8 = gb2312_to_utf8((char*)DevID.c_str());
	if(DevID_utf8 == NULL)
		printf("gb2312_to_utf8 DevID_utf8 failed");
	char* DevType_utf8 = gb2312_to_utf8((char*)DevType.c_str());
	if(DevType_utf8 == NULL)
		printf("gb2312_to_utf8 DevType_utf8 failed");
	char* Manufacturer_utf8 = gb2312_to_utf8((char*)Manufacturer.c_str());
	if(Manufacturer_utf8 == NULL)
		printf("gb2312_to_utf8 Manufacturer_utf8 failed");
#endif
	//在根节点中直接创建节点

	xmlNewTextChild(root_node, NULL, BAD_CAST "DevID", BAD_CAST DevID.c_str());
	xmlNewTextChild(root_node, NULL, BAD_CAST "DevType", BAD_CAST DevType.c_str());
	xmlNewTextChild(root_node, NULL, BAD_CAST "Manufacturer", BAD_CAST Manufacturer.c_str());
#if 0
	//xmlNewChild(root_node, NULL, BAD_CAST "DevID",BAD_CAST DevID_utf8);
	//xmlNewChild(root_node, NULL, BAD_CAST "DevType",BAD_CAST DevType_utf8);
	//xmlNewChild(root_node, NULL, BAD_CAST "Manufacturer",BAD_CAST Manufacturer_utf8);
	//free(DevID_utf8);
	//free(DevType_utf8);
	//free(Manufacturer_utf8);
#endif

	//存储xml文档
	sprintf( PathFile, "/root/Communicate_Schedule_exe/config/hongwai/%s",fileName.c_str()); 
	//int nRel = xmlSaveFile(PathFile,doc);
	int nRel = xmlSaveFormatFileEnc(PathFile,doc,"UTF-8",1);
	if (nRel == -1)
	{
		mLogError("creat xml file failed："<<fileName.c_str());
		return false;
	}
	mLogDebug("creat xml file success："<<fileName.c_str());

	//释放文档内节点动态申请的内存
	xmlFreeDoc(doc);

#if 0
	//2. 发送socket请求
	//MPSOperationRes opResCode = eMPSResultOK; //??
	ResponseCode resCode = = _mpsClient->GetConfigNew(szReqCmd, 4 + realBodySize, eMPSResultOK, oResult, cResult);
	if (resCode != eResponseCodeSuccess) {
		mLogError("GetConfig(...) error:" << resCode);
		return false;
	}
#else
	//dummy response
#endif

	return true;
}
