#include "CAnalyzeParaProc.h"
#include <boost/lexical_cast.hpp>
int sipID = 0; // 暂时使用，sipid号。手动填写，后续修改自动

/*分割字符串到一个字符串数组中*/  
StringTab CAnalyzeParaProc::StringSplit(char* string,const char* split)  
{  
	StringTab result;  
	/*首先分配一个char*的内存，然后再动态分配剩下的内存*/  
	result.string = (char * * )malloc(sizeof(char *)*1);  
	memset(result.string,0,sizeof(char *)*1);  
	/*无论是否存在该分割串，绝对都会分割到一个字符串*/  
	result.number = 0;  
	/*定义一个遍历用的指针和一个寻找位置用的指针*/  
	char* p = string;  
	char* pos = string;  
	while(*p != '\0')  
	{  
		if (result.number > 8192)
			break;
		char* temp;  
		char* tt;  
		/*查找该字符串*/  
		pos = strstr(p,split);  
		/*结果为0说明剩下的字符串中没有该字符了*/  
		if(pos == 0)  
		{  
			result.string = (char **)realloc(result.string,sizeof(char *)*(result.number+1));  
			result.string[result.number] = p;  

			return result;  
		}  
		/*分配临时字符串空间*/  
		temp = (char *)malloc(sizeof(char)*(pos - p+1));  
		memset(temp,0,sizeof(char)*(pos - p+1));  
		/*设置头指针，以便赋值时使用*/  
		tt = temp;  
		while(p<=pos)  
		{  
			*temp++ = *p++;  
		}  
		/*将字符串结尾置零*/  
		*--temp = '\0';  
		result.string = (char **)realloc(result.string,sizeof(char *)*(result.number+1));  
		result.string[result.number] = tt;  
		/*计数器加一*/  
		result.number++;  
		/*设置下一次遍历时的指针（重要）。当split长度大于1时，不这样设置会多赋值不必要的字符串*/  
		p +=strlen(split)-1;  
	}  
	return result;  
} 

void CAnalyzeParaProc::AnalyParaCmdSplitMessage(char *string, char **top, char **low)
{
	if(NULL == string || NULL == top || NULL == low) {
		printf("string ||  top || low is NULL\n");
		return;
	}
	int i;
	for(i = 0; i < strlen(string); i++)
	{
		if (string[i] == ' ')
		{
			string[i] = '^'; //scanf 遇到空白字符结束
		}
	}
	sscanf(string, "%[^=]=%s", *top, *low);
}

void CAnalyzeParaProc::AnalyParaCmdCommandPartSplit(char *top, char **str)
{
	sscanf(top, "%[^.].%[^.].%[^.].%[^.].%s", str[0], str[1], str[2], str[3], str[4]);
}

void CAnalyzeParaProc::AnalyParaCmdArgumentsPartSplit(char *low, char **arg)
{
	sscanf(low, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
}

 void CAnalyzeParaProc::IncBuffer(char **pp)
{
	(*pp)++;
}

 void CAnalyzeParaProc::SubBuffer(unsigned char **pp)
{
	(*pp)--;
}

char* CAnalyzeParaProc::StringCmp( char *str1, const char *str2)
{
	char str_length; 
	char i;

	str_length = strlen(str2);
	for(i=0; i<str_length; i++)
	{   
		if(*str1 == *str2)
		{   
			IncBuffer(&str1);
			str2++;
		}   
		else
		{   
			return NULL;    
		}   
	}   
	return str1;
}


/*********************************************************
 *Function: ipaddr available check
 *Return  : 1(right) 0(fault)
 **********************************************************/
int CAnalyzeParaProc::IpAvalbCheck(char *ascii_buf)
{
	int i;
	int point = 0;

	if (INADDR_NONE == inet_addr(ascii_buf))     
		return 0;//不合法
	else    
	{
		if (!strncmp(ascii_buf, "0.0.0.0", strlen("0.0.0.0")))
			return 0;//不合法
		for(i=0; i<strlen(ascii_buf); i++)
		{
			if (ascii_buf[i] == ' ')
				return 0;//不合法
			else if (ascii_buf[i] == '.')
				point++;
		}
		if (point != 3)
			return 0;//不合法
	}

	return 1;//合法；
}

int CAnalyzeParaProc::AnalyNetRecvPara(int Id, char *NetParaStr, char *NetParaArg)
{
#if 0

	if (StringCmp(NetParaStr,"url"))
	{
		//printf("str %s arg %s\n", NetParaStr,NetParaArg);
		cJSON_AddItemToObject(_rtspdec,HOST_DEC_STATUS_RES_DEC_STATUS,cJSON_CreateBool((strlen(NetParaArg) > 9)?true:false));
	}
#if 0

	//printf("str %s arg %s\n", NetParaStr,NetParaArg);
	if (StringCmp(NetParaStr,"url"))
	{
		cArray = cJSON_CreateObject();
		cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_DE_DECID,cJSON_CreateNumber(Id-1));
		cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_DE_DECURL,cJSON_CreateString(NetParaArg));
	}
	else if (StringCmp(NetParaStr,"setup_mode"))
	{
		cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_DE_DECSTA,cJSON_CreateNumber(atoi(NetParaArg)));
	}
	else if (StringCmp(NetParaStr,"trans_protol"))
	{
		cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_DE_DECMODE,cJSON_CreateString(NetParaArg));
		cJSON_AddItemToArray(_rtspdec, cArray);
	}
#endif
#endif 
		if (StringCmp(NetParaStr,"url"))
		{
			cArray = cJSON_CreateObject();
			cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_DE_DECID,cJSON_CreateNumber(Id));
			cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_DE_DECURL,cJSON_CreateString(NetParaArg));
			if(strlen(NetParaArg) > 0){
				cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_DE_DECSTA,cJSON_CreateNumber(1));
			}
			else{
				cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_DE_DECSTA,cJSON_CreateNumber(0));
			}
		}
	/*	else if (StringCmp(NetParaStr,"setup_mode"))
		{
			cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_DE_DECSTA,cJSON_CreateNumber(atoi(NetParaArg)));
		}*/
		else if (StringCmp(NetParaStr,"trans_protol"))
		{
			cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_DE_DECMODE,cJSON_CreateString(NetParaArg));
			cJSON_AddItemToArray(_rtspdec, cArray);
		}

	return 1;//合法；
}

int CAnalyzeParaProc::AnalyNetH323Para(int Id, char *NetParaStr, char **NetParaArg)
{
	//printf("str %s arg %s\n", NetParaStr,NetParaArg);
#if 0
	if (StringCmp(NetParaStr,"call"))
	{
		cArray = cJSON_CreateObject();
		cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_DE_H323ID,cJSON_CreateNumber(Id));
		if (strlen(NetParaArg) < 9)
		{
			cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_DE_H323STA,cJSON_CreateNumber(0));
		}
		else 
		{
			cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_DE_H323STA,cJSON_CreateNumber(1));
		}
		cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_DE_H323CALLIP,cJSON_CreateString(NetParaArg));
		cJSON_AddItemToArray(_h323dec, cArray);
	}
#else
	//printf("str %s arg %s\n", NetParaStr,NetParaArg);
	if (StringCmp(NetParaStr,"call"))
	{
		h323cArray = cJSON_CreateObject();
		cJSON_AddItemToObject(h323cArray,GET_HOST_CONFIG_RES_DE_H323ID,cJSON_CreateNumber(1));
		//cJSON_AddItemToObject(h323cArray,GET_HOST_CONFIG_RES_DE_H323STA,cJSON_CreateNumber(atoi(NetParaArg[1])));

		cJSON_AddItemToObject(h323cArray,GET_HOST_CONFIG_RES_DE_H323STA,cJSON_CreateNumber(atoi(NetParaArg[1])));
 
		cJSON_AddItemToObject(h323cArray,GET_HOST_CONFIG_RES_DE_H323CALLIP,cJSON_CreateString(NetParaArg[0]));
		cJSON_AddItemToArray(_h323dec, h323cArray);
	}

#endif
	return 1;//合法；
}

int CAnalyzeParaProc::AnalyNetChnSendPara(int Id, char *NetParaStr, char *NetParaArg)
{
	//printf("str %d %s arg %s\n", Id, NetParaStr,NetParaArg);
#if 1
	if (StringCmp(NetParaStr,"name"))
	{
		cArray = cJSON_CreateObject();
		cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_CH_NCHNID,cJSON_CreateNumber(Id));
		cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_CH_NCHNNAME,cJSON_CreateString(NetParaArg));
		switch(Id)
		{
			case 1:
				cJSON_AddItemToObject(cArray, GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString("http://192.168.1.245:9696/live/vl1-1.flv"));
				break;
			case 2:
				cJSON_AddItemToObject(cArray, GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString("http://192.168.1.245:9696/live/vl2-1.flv"));
				break;
			case 3:
				cJSON_AddItemToObject(cArray, GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString("http://192.168.1.245:9696/live/vl3-1.flv"));
				break;
			case 4:
				cJSON_AddItemToObject(cArray, GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString("http://192.168.1.245:9696/live/vl4-1.flv"));
				break;
			case 5:
				cJSON_AddItemToObject(cArray, GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString("http://192.168.1.245:9696/live/vl5-1.flv"));
				break;
			case 6:
				cJSON_AddItemToObject(cArray, GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString("http://192.168.1.245:9696/live/vl6-1.flv"));
				break;
			case 7:
				cJSON_AddItemToObject(cArray, GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString("http://192.168.1.245:9696/live/vl7-1.flv"));
				break;
			case 8:
				cJSON_AddItemToObject(cArray, GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString("http://192.168.1.245:9696/live/vl8-1.flv"));
				break;
			case 9:
				cJSON_AddItemToObject(cArray, GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString("http://192.168.1.245:9696/live/comp1.flv"));
				break;
			case 10:
				cJSON_AddItemToObject(cArray, GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString("http://192.168.1.245:9696/live/comp2.flv"));
				break;
			case 11:
				cJSON_AddItemToObject(cArray, GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString("http://192.168.1.245:9696/live/vn1-1.flv"));
				break;
			case 12:
				cJSON_AddItemToObject(cArray, GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString("http://192.168.1.245:9696/live/vn2-1.flv"));
				break;
			case 13:
				cJSON_AddItemToObject(cArray, GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString("http://192.168.1.245:9696/live/vn3-1.flv"));
				break;
		}
	}
	else if (StringCmp(NetParaStr,"on"))
	{
		cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_CH_NCHNSTA,cJSON_CreateNumber(atoi(NetParaArg)));
	}
	else if (StringCmp(NetParaStr,"url"))
	{

		//cJSON_AddItemToObject(cArray, GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString("http://192.168.1.245:9696/live/ch1.flv"));	
		cJSON_AddItemToObject(cArray,"RtspUrl",cJSON_CreateString(NetParaArg));
	}
	else if (StringCmp(NetParaStr,"recording"))
	{
		cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_CH_NCHNRECSTA,cJSON_CreateNumber(atoi(NetParaArg)));
	}
	else if (StringCmp(NetParaStr,"rec_path"))
	{
		cJSON_AddItemToObject(cArray,GET_HOST_CONFIG_RES_CH_NCHNRECTASKID,cJSON_CreateString(NetParaArg));
		cJSON_AddItemToArray(_channal, cArray);
	}
#endif 
	return 1;//合法；
}

int CAnalyzeParaProc::AnalyGetRecParaSearch(char *cParaBuf, char *cDealBuf, char *cStartXml, char *cEndXml, int *iOffsetLen)
{
	char *ptr1 = NULL, *ptr2 = NULL;
	int offset1 = 0;
	int offset2 = 0;

	ptr1 = strstr(cParaBuf, cEndXml);
	if (NULL == ptr1) {
		//printf("doesn't contain %s\n", cEndXml);
		mLogError("Failed to run  AnalyGetRecParaSearch1()"<<cStartXml);
		mLogError("Failed to run  AnalyGetRecParaSearch1()"<<cParaBuf);
		return 0;
	}

	offset1 = ptr1 - cParaBuf;
	ptr2 = strstr(cParaBuf, cStartXml);
	if (NULL == ptr2) {
		//printf("doesn't contain %s\n", cStartXml);
		mLogError("Failed to run  AnalyGetRecParaSearch2()"<<cStartXml);
		mLogError("Failed to run  AnalyGetRecParaSearch2()"<<cParaBuf);
		return 0;
	}
	offset2 = ptr2 - cParaBuf;
	if (offset1 - offset2 - strlen(cStartXml) > 4096) {
		//printf("out of range  %s\n", cStartXml);
		mLogError("Failed to run  AnalyGetRecParaSearch3()"<<cStartXml);
		mLogError("Failed to run  AnalyGetRecParaSearch3()"<<cParaBuf);
		return 0;
	}

	*iOffsetLen = offset1 + strlen(cEndXml);
	strncpy(cDealBuf, cParaBuf+offset2+strlen(cStartXml), offset1 - offset2 - strlen(cStartXml));
	return 1;
}

int CAnalyzeParaProc::AnalyGetRecPara(char *cParaBuf, int iLen)
{
	int ret = 0, paranum = 0;
	char dealbuf[128] = {0};
	char Taskid[128] = {0};
	char tmpbuf[128] = {0};
	char *curPos = NULL, *ret_floder = NULL, *ret_file = NULL;
	int  curOffsetLen = 0;
	int  offsetLen = 0;

	if((_videofiles == NULL) && (_recresult == NULL))
	{
		printf("cJSON _videoviles or _recresult == NULL.");
		return false;
	}

#if 0 
	for (int i = 0; i < iLen; i++)
    {
		printf("%c", cParaBuf[i]);
    }
	printf("\n");
#endif 
    cParaBuf[iLen] = '\0';
	//printf("AnalyGetRecPara %s\n", cParaBuf);

	//allowDelete
	memset(dealbuf, 0, sizeof(dealbuf));
	curPos = cParaBuf;
	//搜索文件返回数据：<allowDelete>false</allowDelete><total>224</total><videoFiles><fileSort>1</fileSort><TaskID>15635713</TaskID><fileName>192.168.1.236_00_19700102003008_19700102003017.mp4</fileName><size>35</size><filePath>/mnt/HD0/15635713/</filePath><startTime>1970-01-02 00:30:08</startTime><endTime>1970-01-02 00:30:17</endTime><channelNum>01</channelNum><channelDesc>test</channelDesc><ftpPath>ftp://192.168.1.236/HD0/15635713/192.168.1.236_00_19700102003008_19700102003017.mp4</ftpPath><ftpusr>ftp</ftpusr><ftppasswd>ftp</ftppasswd><ftpport>21</ftpport><vodurlRtsp>rtsp://192.168.1.236:5566/mp4:/mnt/HD0/15635713/192.168.1.236_00_19700102003008_19700102003017.mp4</vodurlRtsp><fileHttpPath>http://192.168.1.236:8888/skyvis_file/15635713/192.168.1.236_00_19700102003008_19700102003017.mp4</fileHttpPath><fileUploadStatus>1</fileUploadStatus><vodurlRtmp></vodurlRtmp><osd_name>画面合成1-字幕1</osd_name></videoFiles>
	//搜索文件夹返回数据：<total>6</total><videoFiles><fileSort>1</fileSort><TaskID>15635713</TaskID></videoFiles>
	//按顺序解析数据个字段，顺序不能乱。
//	memcpy(tmpbuf, curPos, 20);//将数据的前20字节拷贝到tmpbuf，用来比较返回数据的字符串的头判断按那种方式解析数据
//	ret_file = strstr(curPos, "<allowDelete>");
//	ret_floder = strstr(curPos, "<total>");
//	if ((ret_file == NULL)&&(ret_floder == NULL))
//	{
//		//printf("AnalyGetRecParaSearch false %s\n", "<allowDelete>");
//		mLogError("Failed to run  AnalyGetRecParaSearch <allowDelete> or </total> !!!");
//		return false;
//	}

	//通过TaskID查询文件
	if( _Param == 1)
	{
		ret = AnalyGetRecParaSearch(curPos, dealbuf, "<allowDelete>", "</allowDelete>", &offsetLen);
		if (ret == 0)
		{
			//printf("AnalyGetRecParaSearch false %s\n", "<allowDelete>");
			mLogError("Failed to run  AnalyGetRecParaSearch <allowDelete>!!!");
			return false;
		}

		curOffsetLen = offsetLen;
		//printf("buf %s %d\n", dealbuf, curOffsetLen);
		cJSON_AddItemToObject(_recresult,GET_INFOLIST_RES_ALLOWDELETE,cJSON_CreateBool((strlen(dealbuf)==4)?true:false));
		
		//total
		memset(dealbuf, 0, sizeof(dealbuf));
		curPos = cParaBuf + curOffsetLen;
		ret = AnalyGetRecParaSearch(curPos, dealbuf, "<total>", "</total>", &offsetLen);
		if (ret == 0)
		{
			//printf("AnalyGetRecParaSearch false %s\n", "<total>");
			mLogError("Failed to run  AnalyGetRecParaSearch(<total>)");
			return false;
		}
		curOffsetLen += offsetLen; 
		//printf("buf %s %d\n", dealbuf, curOffsetLen);
		cJSON_AddItemToObject(_recresult,GET_INFOLIST_RES_TOTAL,cJSON_CreateNumber(atoi(dealbuf)));

		paranum = atoi(dealbuf);
		//循环次数
		FileEndIndex = FileEndIndex > paranum ? paranum : FileEndIndex;
		int filenum = (FileEndIndex - FileStartIndex + 1) < paranum ? (FileEndIndex - FileStartIndex + 1) : paranum;
		//printf("paranum %d\n", paranum);
		for (int i = 0; i < filenum; i++)
		{
			//mLogDebug("################ AnalyGetRecPara: paranum ############# "<<i);
			//</fileSort>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<fileSort>", "</fileSort>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<fileSort>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<fileSort>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cArray = cJSON_CreateObject();
			cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_FILESORT,cJSON_CreateNumber(atoi(dealbuf)));
			
			//</TaskID>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<TaskID>", "</TaskID>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<TaskID>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<TaskID>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_TASKID,cJSON_CreateString(dealbuf));
			
			//</fileName>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<fileName>", "</fileName>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<fileName>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<fileName>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_FILENAME,cJSON_CreateString(dealbuf));

			//</size>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<size>", "</size>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<size>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<size>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_SIZE,cJSON_CreateNumber(atoi(dealbuf)));
			
			//</filePath>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<filePath>", "</filePath>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<filePath>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<filePath>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_FILEPATH,cJSON_CreateString(dealbuf));

			//</startTime>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<startTime>", "</startTime>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<startTime>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<startTime>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_STARTTIME,cJSON_CreateString(dealbuf));

			//</endTime>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<endTime>", "</endTime>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<endTime>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<endTime>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_ENDTIME,cJSON_CreateString(dealbuf));
			
			//</channelNum>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<channelNum>", "</channelNum>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<channelNum>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<channelNum>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_CHANNELNUM,cJSON_CreateNumber(atoi(dealbuf) - 1));
		
			//</channelDesc>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<channelDesc>", "</channelDesc>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<channelDesc>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<channelDesc>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_CHANNELDESC,cJSON_CreateString(dealbuf));
			
			//</ftpPath>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<ftpPath>", "</ftpPath>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<ftpPath>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<ftpPath>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_FTPPATH,cJSON_CreateString(dealbuf));
			
			//</ftpusr>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<ftpusr>", "</ftpusr>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<ftpusr>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<ftpusr>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_FTPUSR,cJSON_CreateString(dealbuf));
			
			//</ftppasswd>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<ftppasswd>", "</ftppasswd>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<ftppasswd>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<ftppasswd>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_FTPPASSWD,cJSON_CreateString(dealbuf));
			
			//</vodurlRtsp>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<vodurlRtsp>", "</vodurlRtsp>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<vodurlRtsp>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<vodurlRtsp>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_VODURLRTSP,cJSON_CreateString(dealbuf));

			//</fileHttpPath>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<fileHttpPath>", "</fileHttpPath>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<vodurlRtmp>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<fileHttpPath>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,"HttpPath",cJSON_CreateString(dealbuf));
			
			//</fileUploadStatus>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<fileUploadStatus>", "</fileUploadStatus>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<vodurlRtmp>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<fileUploadStatus>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,"fileUploadStatus",cJSON_CreateString(dealbuf));
			
			//</vodurlRtmp>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<vodurlRtmp>", "</vodurlRtmp>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<vodurlRtmp>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<vodurlRtmp>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_VODURLRTMP,cJSON_CreateString(dealbuf));
			
			//</osd_name>
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<osd_name>", "</osd_name>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<osdname>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<osd_name>)");
				return false;
			}
			curOffsetLen += offsetLen;
			//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
			cJSON_AddItemToObject(cArray,"OsdName",cJSON_CreateString(dealbuf));
			
			//将对象添加到数组中
			cJSON_AddItemToArray(_videofiles, cArray);
		}
		
	}
	else if(_Param == 7) //查询目录
	{	
			//total
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<total>", "</total>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<total>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<total>)");
				return false;
			}
			curOffsetLen += offsetLen; 
			//printf("buf %s %d\n", dealbuf, curOffsetLen);
			cJSON_AddItemToObject(_recresult,GET_INFOLIST_RES_TOTAL,cJSON_CreateNumber(atoi(dealbuf)));
			
			paranum = atoi(dealbuf);
			//循环次数
			FileEndIndex = FileEndIndex > paranum ? paranum : FileEndIndex;
			int filenum = (FileEndIndex - FileStartIndex + 1) < paranum ? (FileEndIndex - FileStartIndex + 1) : paranum;
			//printf("paranum %d\n", paranum);
			for (int i = 0; i < filenum; i++)
			{
				//mLogDebug("################ AnalyGetRecPara: paranum ############# "<<i);
				//</fileSort>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<fileSort>", "</fileSort>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<fileSort>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<fileSort>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cArray = cJSON_CreateObject();
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_FILESORT,cJSON_CreateNumber(atoi(dealbuf)));
				
				//</TaskID>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<TaskID>", "</TaskID>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<TaskID>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<TaskID>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,"TaskID",cJSON_CreateString(dealbuf));
				
				//</channelDesc>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<channelDesc>", "</channelDesc>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<TaskID>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<channelDesc>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,"ChannelDesc",cJSON_CreateString(dealbuf));
				
				//</startTime>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<startTime>", "</startTime>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<TaskID>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<startTime>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,"StartTime",cJSON_CreateString(dealbuf));
				
				//</endTime>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<endTime>", "</endTime>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<TaskID>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<endTime>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,"EndTime",cJSON_CreateString(dealbuf));
				
				//</size> 单位：M
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<size>", "</size>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<TaskID>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<size>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				memset(tmpbuf,0,128);
				memcpy(tmpbuf,dealbuf,strlen(dealbuf));
				int buflen = strlen(dealbuf);
				//格式化字符串
				if(buflen > 3)
				{
					string strtmp;
					strtmp = dealbuf;
					for(int i = 1; i <= (buflen/3); i++)
					{
						if((buflen-3*i) != 0)
						{
							strtmp.insert(buflen-3*i,",");
						}
					}
					memset(tmpbuf,0,128);
					memcpy(tmpbuf,strtmp.c_str(),strtmp.size());
				}
				sprintf(dealbuf,"%s MB", tmpbuf);
				cJSON_AddItemToObject(cArray,"TotalFileSize",cJSON_CreateString(dealbuf));

				//将对象添加到数组中
				cJSON_AddItemToArray(_videofiles, cArray);
			}
	}
	else if(_Param == 6)//通过时间和目录查询文件
	{
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<allowDelete>", "</allowDelete>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<allowDelete>");
				mLogError("Failed to run  AnalyGetRecParaSearch <allowDelete>!!!");
				return false;
			}
	
			curOffsetLen = offsetLen;
			//printf("buf %s %d\n", dealbuf, curOffsetLen);
			cJSON_AddItemToObject(_recresult,GET_INFOLIST_RES_ALLOWDELETE,cJSON_CreateBool((strlen(dealbuf)==4)?true:false));
			
			//total
			memset(dealbuf, 0, sizeof(dealbuf));
			curPos = cParaBuf + curOffsetLen;
			ret = AnalyGetRecParaSearch(curPos, dealbuf, "<total>", "</total>", &offsetLen);
			if (ret == 0)
			{
				//printf("AnalyGetRecParaSearch false %s\n", "<total>");
				mLogError("Failed to run  AnalyGetRecParaSearch(<total>)");
				return false;
			}
			curOffsetLen += offsetLen; 
			//printf("buf %s %d\n", dealbuf, curOffsetLen);
			cJSON_AddItemToObject(_recresult,GET_INFOLIST_RES_TOTAL,cJSON_CreateNumber(atoi(dealbuf)));
	
			paranum = atoi(dealbuf);
			//循环次数
			FileEndIndex = FileEndIndex > paranum ? paranum : FileEndIndex;
			int filenum = (FileEndIndex - FileStartIndex + 1) < paranum ? (FileEndIndex - FileStartIndex + 1) : paranum;
			//printf("paranum %d\n", paranum);
			_ParamList = cJSON_CreateArray();
			cJSON* ChildrenFile = cJSON_CreateObject();
			for (int i = 0; i < filenum; i++)
			{
				//mLogDebug("################ AnalyGetRecPara: paranum ############# "<<i);
				
				cArray     = cJSON_CreateObject();
				_ParamKnod = cJSON_CreateObject();
				//</fileSort>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<fileSort>", "</fileSort>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<fileSort>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<fileSort>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_FILESORT,cJSON_CreateNumber(atoi(dealbuf)));
				
				//</TaskID>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<TaskID>", "</TaskID>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<TaskID>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<TaskID>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				//Taskid = dealbuf; 
				memcpy(Taskid,dealbuf,sizeof(dealbuf));
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_TASKID,cJSON_CreateString(dealbuf));
				
				//</fileName>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<fileName>", "</fileName>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<fileName>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<fileName>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_FILENAME,cJSON_CreateString(dealbuf));
	
				//</size>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<size>", "</size>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<size>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<size>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_SIZE,cJSON_CreateNumber(atoi(dealbuf)));
				//文件夹总大小
				_port += atoi(dealbuf);
				
				//</filePath>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<filePath>", "</filePath>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<filePath>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<filePath>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_FILEPATH,cJSON_CreateString(dealbuf));
	
				//</startTime>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<startTime>", "</startTime>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<startTime>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<startTime>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_STARTTIME,cJSON_CreateString(dealbuf));
				if(i == 0)
				{
					cJSON_AddItemToObject(ChildrenFile,"FileStartTime",cJSON_CreateString(dealbuf));
				}
				//</endTime>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<endTime>", "</endTime>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<endTime>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<endTime>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_ENDTIME,cJSON_CreateString(dealbuf));
				if(i == (filenum-1))
				{
					cJSON_AddItemToObject(ChildrenFile,"FileEndTime",cJSON_CreateString(dealbuf));
				}
				//</channelNum>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<channelNum>", "</channelNum>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<channelNum>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<channelNum>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_CHANNELNUM,cJSON_CreateNumber(atoi(dealbuf) - 1));
			
				//</channelDesc>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<channelDesc>", "</channelDesc>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<channelDesc>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<channelDesc>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_CHANNELDESC,cJSON_CreateString(dealbuf));
				if(i == 0)
				{
					cJSON_AddItemToObject(ChildrenFile,"ChannelDesc",cJSON_CreateString(dealbuf));
				}
				
				//</ftpPath>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<ftpPath>", "</ftpPath>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<ftpPath>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<ftpPath>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_FTPPATH,cJSON_CreateString(dealbuf));
				
				//</ftpusr>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<ftpusr>", "</ftpusr>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<ftpusr>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<ftpusr>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_FTPUSR,cJSON_CreateString(dealbuf));
				
				//</ftppasswd>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<ftppasswd>", "</ftppasswd>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<ftppasswd>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<ftppasswd>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_FTPPASSWD,cJSON_CreateString(dealbuf));
				
				//</vodurlRtsp>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<vodurlRtsp>", "</vodurlRtsp>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<vodurlRtsp>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<vodurlRtsp>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_VODURLRTSP,cJSON_CreateString(dealbuf));
	
				//</fileHttpPath>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<fileHttpPath>", "</fileHttpPath>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<vodurlRtmp>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<fileHttpPath>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,"HttpPath",cJSON_CreateString(dealbuf));
				
				//</fileUploadStatus>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<fileUploadStatus>", "</fileUploadStatus>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<vodurlRtmp>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<fileUploadStatus>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,"fileUploadStatus",cJSON_CreateString(dealbuf));
				
				//</vodurlRtmp>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<vodurlRtmp>", "</vodurlRtmp>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<vodurlRtmp>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<vodurlRtmp>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,GET_INFOLIST_RES_VODURLRTMP,cJSON_CreateString(dealbuf));
				
				//</osd_name>
				memset(dealbuf, 0, sizeof(dealbuf));
				curPos = cParaBuf + curOffsetLen;
				ret = AnalyGetRecParaSearch(curPos, dealbuf, "<osd_name>", "</osd_name>", &offsetLen);
				if (ret == 0)
				{
					//printf("AnalyGetRecParaSearch false %s\n", "<osdname>");
					mLogError("Failed to run  AnalyGetRecParaSearch(<osd_name>)");
					return false;
				}
				curOffsetLen += offsetLen;
				//printf("buf %s %d\n", dealbuf, strlen(dealbuf));
				cJSON_AddItemToObject(cArray,"OsdName",cJSON_CreateString(dealbuf));
				
				//将对象添加到数组中
				cJSON_AddItemToArray(_ParamList, cArray);
				
			}
		memset(tmpbuf,0,128);
		sprintf(tmpbuf,"%d", _port);
		int buflen = strlen(tmpbuf);
		//格式化字符串
		if(buflen > 3)
		{
			string strtmp;
			strtmp = tmpbuf;
			for(int i = 1; i <= (buflen/3); i++)
			{
				if((buflen-3*i) != 0)
				{
					strtmp.insert(buflen-3*i,",");
				}
			}
			memset(tmpbuf,0,128);
			memcpy(tmpbuf,strtmp.c_str(),strtmp.size());
		}
		sprintf(tmpbuf,"%s MB", tmpbuf);
		cJSON_AddItemToObject(ChildrenFile,"TotalFileSize",cJSON_CreateString(tmpbuf));
		cJSON_AddItemToObject(ChildrenFile,GET_INFOLIST_RES_TASKID, cJSON_CreateString(Taskid));
		cJSON_AddItemToObject(ChildrenFile,"ChildrenFile", _ParamList);
		cJSON_AddItemToArray(_videofiles, ChildrenFile);
		//cJSON_AddItemToArray(_videofiles, _ParamKnod);
		//cJSON_AddItemReferenceToObject(_videofiles,"ChildrenFile", _ParamList);

	}
	
	return 1;//合法；
}

/* SET 指令处理每一条函数 */
int CAnalyzeParaProc::AnalyParaCmdParseCommandHandle(char **str, char **arg, char *string)
{
	int  cmd_id = -1;
	char *n_ptr = NULL;
	//CRtspNode NetRecv;
    const char *CmdTab[] ={"vid_cap","vid_enc","vid_dec","vid_dis_hd","comp","net_recv",	/* 5 */
						 "net_send","pci_vid_in","pci_vid_out","pci_aud_in","pci_aud_out",	/* 10 */
						 "aud_cap","aud_enc","aud_dec","aud_out","hdmi_aud_out",	/* 15 */
						 "h323h","sip", "rec","demux","screenshot","eth",	/* 21 */
						 "mosaic","nm","mn","core","vm",	/* 26 */
						 "NO_FLAG","syslog","sys","tdc","del",	/* 31 */
						 "rzk","am","dvc","links","status",	/* 36 */
						 "encrypt","web","back_pic","ntp","audio_info",	/* 41 */
						 "ftp","yuan","","","END_FLAG"
						};


	int cmd_tab_number = sizeof(CmdTab)/sizeof(CmdTab[0]);

	for(int i = 0; i < cmd_tab_number; i++) {
		if((n_ptr = StringCmp(str[0], CmdTab[i]))) {
			cmd_id = i;
			break;
		}
	}
	
	//mLogDebug("##cmd_id ##"<<cmd_id );

	//模块名不存在
	if(-1 == cmd_id) {
		return false;
	}
    //printf("AnalyParaCmdParseCommandHandle %d %s %s %s\n", cmd_id, str[0], str[1], arg[0]);
	//mLogDebug("##str##"<<str[0]);

#if 1
	if (CHNSYS_ETH_OP == cmd_id)
	{
		if(!strcmp(CMethod.c_str(),"GetWebHostConfig"))
		{
			if (StringCmp(str[1], "conf"))
			{
				if (!IpAvalbCheck(arg[0])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, ip = "<<arg[0]);
					return ARG_STR_ERR;
				}
				if (!IpAvalbCheck(arg[1])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, netmask = "<<arg[1]);
					return ARG_STR_ERR;
				}
				if (!IpAvalbCheck(arg[2])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, gateway = "<<arg[2]);
					return ARG_STR_ERR;
				}		
				//printf("AnalyParaCmdParseCommandHandle %s %s %s\n", arg[0], arg[1], arg[2]);
				if(_network == NULL){
					mLogError("cJSON network == NULL.");
					return false;
				}
				cJSON_AddItemToObject(_network,GET_HOST_CONFIG_RES_NW_IP        ,cJSON_CreateString(arg[0]));
				cJSON_AddItemToObject(_network,GET_HOST_CONFIG_RES_NW_GATEWAY   ,cJSON_CreateString(arg[2]));
				cJSON_AddItemToObject(_network,GET_HOST_CONFIG_RES_NW_NETMASK   ,cJSON_CreateString(arg[1]));
			}
			else if (StringCmp(str[1], "macaddr"))
			{
				cJSON_AddItemToObject(_network,GET_HOST_CONFIG_RES_NW_MACADDR,cJSON_CreateString(arg[0]));
			}
		}
		if(!strcmp(CMethod.c_str(),"GetWebHostChnInfo"))
		{
			if (StringCmp(str[1], "conf"))
			{
				if (!IpAvalbCheck(arg[0])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, ip = "<<arg[0]);
					return ARG_STR_ERR;
				}
				if (!IpAvalbCheck(arg[1])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, netmask = "<<arg[1]);
					return ARG_STR_ERR;
				}
				if (!IpAvalbCheck(arg[2])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, gateway = "<<arg[2]);
					return ARG_STR_ERR;
				}		
				//printf("AnalyParaCmdParseCommandHandle %s %s %s\n", arg[0], arg[1], arg[2]);
				_ip = arg[0];

			}

		}
		if(!strcmp(CMethod.c_str(),"GetWebStartRec"))
		{
			if (StringCmp(str[1], "conf"))
			{
				if (!IpAvalbCheck(arg[0])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, ip = "<<arg[0]);
					return ARG_STR_ERR;
				}
				if (!IpAvalbCheck(arg[1])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, netmask = "<<arg[1]);
					return ARG_STR_ERR;
				}
				if (!IpAvalbCheck(arg[2])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, gateway = "<<arg[2]);
					return ARG_STR_ERR;
				}		
				//printf("AnalyParaCmdParseCommandHandle %s %s %s\n", arg[0], arg[1], arg[2]);
				_ip = arg[0];
			}

		}
		if(!strcmp(CMethod.c_str(),"GetWebRtmpParam"))
		{
			if (StringCmp(str[1], "conf"))
			{
				if (!IpAvalbCheck(arg[0])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, ip = "<<arg[0]);
					return ARG_STR_ERR;
				}
				if (!IpAvalbCheck(arg[1])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, netmask = "<<arg[1]);
					return ARG_STR_ERR;
				}
				if (!IpAvalbCheck(arg[2])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, gateway = "<<arg[2]);
					return ARG_STR_ERR;
				}		
				//printf("AnalyParaCmdParseCommandHandle %s %s %s\n", arg[0], arg[1], arg[2]);
				_ip = arg[0];
			}

		}

		if(!strcmp(CMethod.c_str(),"GetWebFtpConf"))
		{
			if (StringCmp(str[1], "conf"))
			{
				if (!IpAvalbCheck(arg[0])) {
					mLogError("GetWebFtpConf ip_avalb_check LogInfo, ip = "<<arg[0]);
					return ARG_STR_ERR;
				}
				if (!IpAvalbCheck(arg[1])) {
					mLogError("GetWebFtpConf ip_avalb_check LogInfo, netmask = "<<arg[1]);
					return ARG_STR_ERR;
				}
				if (!IpAvalbCheck(arg[2])) {
					mLogError("GetWebFtpConf ip_avalb_check LogInfo, gateway = "<<arg[2]);
					return ARG_STR_ERR;
				}		
				//printf("AnalyParaCmdParseCommandHandle %s %s %s\n", arg[0], arg[1], arg[2]);
				_ip = arg[0];
				//cJSON_AddItemToObject(_network,GET_HOST_CONFIG_RES_NW_IP        ,cJSON_CreateString(arg[0]));
				//cJSON_AddItemToObject(_network,GET_HOST_CONFIG_RES_NW_GATEWAY   ,cJSON_CreateString(arg[1]));
				//cJSON_AddItemToObject(_network,GET_HOST_CONFIG_RES_NW_NETMASK   ,cJSON_CreateString(arg[2]));
			}

		}
		if(!strcmp(CMethod.c_str(),"UartSearchCameraTableControlCmd"))
		{
			if (StringCmp(str[1], "conf"))
			{
				if (!IpAvalbCheck(arg[0])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, ip = "<<arg[0]);
					return ARG_STR_ERR;
				}
				if (!IpAvalbCheck(arg[1])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, netmask = "<<arg[1]);
					return ARG_STR_ERR;
				}
				if (!IpAvalbCheck(arg[2])) {
					mLogError("GetWebHostConfig ip_avalb_check LogInfo, gateway = "<<arg[2]);
					return ARG_STR_ERR;
				}		
				//printf("AnalyParaCmdParseCommandHandle %s %s %s\n", arg[0], arg[1], arg[2]);
				_ip = arg[0];

			}

		}
	}
	else if (CHNSYS_SYS_OP == cmd_id)
	{

		if(!strcmp(CMethod.c_str(),"GetWebHostConfig"))
		{
			if (StringCmp(str[1], "name"))
			{
				cJSON_AddItemToObject(_sysinfo,GET_HOST_CONFIG_RES_SYS_DEVTYPE ,cJSON_CreateString(arg[0]));
			}
			else if (StringCmp(str[1], "_swver"))
			{
				cJSON_AddItemToObject(_sysinfo,GET_HOST_CONFIG_RES_SYS_SVER,cJSON_CreateString(arg[0]));
			}
			else if (StringCmp(str[1], "_hwver"))
			{
				cJSON_AddItemToObject(_sysinfo,GET_HOST_CONFIG_RES_SYS_HVER,cJSON_CreateString(arg[0]));
			}
			else if (StringCmp(str[1], "sn"))
			{
				cJSON_AddItemToObject(_sysinfo,GET_HOST_CONFIG_RES_SYS_SN,cJSON_CreateString(arg[0]));
			}		
			else if (StringCmp(str[1], "dev_id"))
			{
				cJSON_AddItemToObject(_sysinfo,SET_WEB_EXTEND_PARAMS_DEVID,cJSON_CreateString(arg[0]));
			}
			else if (StringCmp(str[1], "buildtime"))
			{
				cJSON_AddItemToObject(_sysinfo,"buildtime",cJSON_CreateString(arg[0]));
			}

		}

		if(!strcmp(CMethod.c_str(),"GetWebDevID"))
		{
			if (StringCmp(str[1], "dev_id"))
	    	{
		    	cJSON_AddItemToObject(_result,SET_WEB_EXTEND_PARAMS_DEVID,cJSON_CreateString(arg[0]));
	    	}
		}
		if(!strcmp(CMethod.c_str(),"GetWebAudioBoardType"))
		{
			if (StringCmp(str[1], "audio_type"))
	    	{
		    	cJSON_AddItemToObject(_result,"AudioBoardType",cJSON_CreateString(arg[0]));
	    	}
			if (StringCmp(str[1], "audio_board_is_show"))
	    	{
		    	cJSON_AddItemToObject(_result,"ShowAudioBoard",cJSON_CreateString(arg[0]));
	    	}
		}
		
		if(!strcmp(CMethod.c_str(),"GetWebHostChnInfo"))
		{
			if (StringCmp(str[1], "audio_type"))
	    	{
				cJSON_AddItemToObject(_result,"AudioBoardType",cJSON_CreateString(arg[0]));
	    	}
		}
		if(!strcmp(CMethod.c_str(),"SetWebRecSegModelSelect"))
		{
			if (StringCmp(str[1], "audio_type"))
	    	{
				Params[4] = arg[0];
	    	}
		}
	}
	else if (CHNSYS_STATUS_GET == cmd_id)
	{
		if(_diskinfo != NULL)
		{
			if (StringCmp(str[1], "totalsize"))
			{
				cJSON_AddItemToObject(_diskinfo,GET_HOST_CONFIG_RES_DI_TOTALSIZE,cJSON_CreateNumber(atoi(arg[0])));
			}
			else if (StringCmp(str[1], "freedisk"))
			{
				cJSON_AddItemToObject(_diskinfo,GET_HOST_CONFIG_RES_DI_FREESIZE,cJSON_CreateNumber(atoi(arg[0])));
			}
			else if (StringCmp(str[1], "cpu"))
			{
				cJSON_AddItemToObject(_otherinfo,GET_HOST_CONFIG_RES_DI_CPURATE,cJSON_CreateNumber(atoi(arg[0])));
			}
		}
#if 1
		else if (_otherinfo != NULL)
		{
			if (StringCmp(str[1], "totalsize"))
			{
				cJSON_AddItemToObject(_otherinfo,GET_HOST_CONFIG_RES_DI_TOTALSIZE,cJSON_CreateNumber(atoi(arg[0])));
			}
			else if (StringCmp(str[1], "freedisk"))
			{
				cJSON_AddItemToObject(_otherinfo,GET_HOST_CONFIG_RES_DI_FREESIZE,cJSON_CreateNumber(atoi(arg[0])));
			}
			else if (StringCmp(str[1], "cpu"))
			{
				cJSON_AddItemToObject(_otherinfo,GET_HOST_CONFIG_RES_DI_CPURATE,cJSON_CreateNumber(atoi(arg[0])));
			}
			else if (StringCmp(str[1], "mem"))
			{
				cJSON_AddItemToObject(_otherinfo,GET_HOST_CONFIG_RES_DI_MEMRATE,cJSON_CreateNumber(atoi(arg[0])));
			}		
			else if (StringCmp(str[1], "temp"))
			{
				cJSON_AddItemToObject(_otherinfo,"Temperature",cJSON_CreateNumber(atoi(arg[0])));
			}
		}
#endif 

	}
	else if (CHNSYS_DVC_GET == cmd_id)
	{
		if(_decnum != NULL)
		{
			if (StringCmp(str[1], "net_recv_max_num"))
			{
				cJSON_AddItemToObject(_decnum,GET_HOST_CONFIG_RES_DE_RTSPDECNUM,cJSON_CreateNumber(atoi(arg[0])));
			}
			else if (StringCmp(str[1], "h323_num"))
			{
				cJSON_AddItemToObject(_decnum,GET_HOST_CONFIG_RES_DE_H323DECNUM,cJSON_CreateNumber(atoi(arg[0])));
			}
			else if (StringCmp(str[1], "sip_num"))
			{
				cJSON_AddItemToObject(_decnum,GET_HOST_CONFIG_RES_DE_SIPNUM,cJSON_CreateNumber(atoi(arg[0])));
			}
			else if (StringCmp(str[1], "net_send_max_num"))
			{
				cJSON_AddItemToObject(_decnum,GET_HOST_CONFIG_RES_CH_CHANNELNUM,cJSON_CreateNumber(atoi(arg[0])));
			}
			else if (StringCmp(str[1], "rtmp_client_send_num"))
			{
				cJSON_AddItemToObject(_decnum,GET_HOST_CONFIG_RES_CH_RTMPCLIPUSHNUM,cJSON_CreateNumber(atoi(arg[0])));
			}
			else if (StringCmp(str[1], "uart_num"))
			{
				cJSON_AddItemToObject(_decnum,GET_HOST_CONFIG_RES_CH_SERIALNUM,cJSON_CreateNumber(atoi(arg[0])));
			}
		}
		if(_sysinfo != NULL)
		{
			if (StringCmp(str[1], "ftp_port"))
			{
				cJSON_AddItemToObject(_sysinfo,GET_HOST_CONFIG_RES_SYS_FTPPORT,cJSON_CreateNumber(atoi(arg[0])));
			}
		}
		if(_status != NULL)
		{
			if (StringCmp(str[1], "allrecstatus"))
			{
				cJSON_AddItemToObject(_status,GET_HOST_CONFIG_RES_SYS_ALLRECSTATUS,cJSON_CreateNumber(atoi(arg[0])));
			}
		}

	}
	else if (CHNSYS_NET_RECV_OP == cmd_id)
	{
		//char *cCurPost = str[0] + 8;
		//printf("%s Num %d\n", cCurPost, atoi(cCurPost));
		int len = strlen(str[0]);
		memcpy(str[0],str[0]+8,len-8);//8:net_recv字符数量
		int num = atoi(str[0]);//net_recv%d的序号

		if(!strcmp(CMethod.c_str(),"GetWebHostChnInfo"))
		{
			if (StringCmp(str[1], "url"))
			{		
				int urllen = strlen(arg[0]);
				if(num <= 2 && urllen > 5)//目前支持2路
				{
					char tmpstr[64];
					sprintf(tmpstr,"RTSP%d:%s",num,arg[0]);
					Params[num] = tmpstr;
				}
			}
		 }
		
		if(!strcmp(CMethod.c_str(),"SetWebRecSegModelSelect"))
		{
			if (StringCmp(str[1], "url"))
			{		
				if(num <= 2)//目前支持2路
				{
					Params[num+4] = arg[0];
				}
			}
		 }
		
		if(!strcmp(CMethod.c_str(),"GetHostDecInfo"))
		{
			if(num <= 2)//目前支持2路
			{
				AnalyNetRecvPara(num, str[1], arg[0]);
			}

		}
		
	}
	else if (CHNSYS_H323H_OP == cmd_id)
	{
		int len = strlen(str[0]);
		memcpy(str[0],str[0]+5,len-5);//5:h323h字符数量
		int num = atoi(str[0]);//h323h%d的序号

		if(!strcmp(CMethod.c_str(),"GetHostDecInfo"))
		{
			AnalyNetH323Para(num, str[1], arg);
		}
		if(!strcmp(CMethod.c_str(),"SetWebRecSegModelSelect"))
		{
			if (StringCmp(str[1], "call"))
			{		
				Params[7] = arg[0];				
			}
		 }
		if(!strcmp(CMethod.c_str(),"GetWebHostChnInfo"))
		{
			if (StringCmp(str[1], "call"))
			{		
				int urllen = strlen(arg[0]);
				if(urllen > 5)//目前支持1路
				{
					char tmpstr[64];
					sprintf(tmpstr,"H323:%s",arg[0]);
					Params[num+2] = tmpstr;
				}
			}
		}

	}
	else if(CHNSYS_SIP_OP == cmd_id)
	{
		sipcArray = cJSON_CreateObject();
		int len = strlen(str[0]);
		memcpy(str[0],str[0]+3,len-3);//3:sip字符数量
		int num = atoi(str[0]);//sip%d的序号

		if(!strcmp(CMethod.c_str(),"GetHostDecInfo"))
		{
			if (StringCmp(str[1],"call"))
			{
				sipcArray = cJSON_CreateObject();

				if(num <= 3)//目前支持3路SIP
				{
					cJSON_AddItemToObject(sipcArray,GET_HOST_CONFIG_RES_DE_SIPID,cJSON_CreateNumber(num));

					cJSON_AddItemToObject(sipcArray,GET_HOST_CONFIG_RES_DE_SIPSTA,cJSON_CreateNumber(atoi(arg[1])));

					cJSON_AddItemToObject(sipcArray,GET_HOST_CONFIG_RES_DE_SIPCALLIP,cJSON_CreateString(arg[0]));
					cJSON_AddItemToArray(_sipdec, sipcArray);
					if(num == 2)//群呼通道号依次递增，所以只需判断第2通道即可
					{
						if(strlen(arg[0]) > 5)//判断是否群呼，有IP数据为群呼
						{
							_Param = 1;//群呼
						}else{
							_Param = 0;//单呼
						}
					}
				}
			}
		}

		if(!strcmp(CMethod.c_str(),"SetWebRecSegModelSelect"))
		{
			if (StringCmp(str[1], "call"))
			{		
				if(num <= 3)//目前支持3路
				{
					Params[num+7] = arg[0];
				}
			}
		 }
				
		if(!strcmp(CMethod.c_str(),"GetWebHostChnInfo"))
		{
			if (StringCmp(str[1], "call"))
			{			
				int urllen = strlen(arg[0]);
				if(num <= 3 && urllen > 5)//目前支持3路
				{
					char tmpstr[64];
					sprintf(tmpstr,"SIP%d:%s", num,arg[0]);
					Params[num+3] = tmpstr;
				}
			}
		 }

	}
	else if (CHNSYS_DEMUX_OP == cmd_id)
	{
		if(_statusinfo == NULL)
		{
			mLogDebug("_statusinfo == NULL.");
			return false;
		}

		if (StringCmp(str[1], "total_time"))
		{
    		cJSON_AddItemToObject(_statusinfo,GET_HOST_CONFIG_DS_DISCIPLINESTATUS,cJSON_CreateBool((atoi(arg[0])>0)?true:false));
		}

	}
	else if (CHNSYS_VID_ENC_OP == cmd_id)
	{
     	if(!strcmp(CMethod.c_str(),"GetWebVideoParam"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+7,len-7);//7:vid_enc字符数量
			int num = atoi(str[0]);//vid_enc%d,vid_enc的序号

			if (StringCmp(str[1], "channel"))
			{				  
			  cJSON_AddItemToObject(_ParamArry[num], SET_WEB_VIDEO_PARAMS_CHNNAME, cJSON_CreateString(arg[0]));
			  //添加通道号信息，对应发送命令中的通道号
			  cJSON_AddItemToObject(_ParamArry[num], GET_WEB_VIDEO_PARAM_RES_CHNNUM, cJSON_CreateNumber(num));
			}
			
			if (StringCmp(str[1], "vid_payload"))
			{
			
			  if((!strcmp(arg[0], "h264"))||(!strcmp(arg[0], "H264"))){
			  cJSON_AddItemToObject(_ParamArry[num], SET_WEB_VIDEO_PARAMS_CODEMODE, cJSON_CreateNumber(1));	   
			  }else{
			  cJSON_AddItemToObject(_ParamArry[num], SET_WEB_VIDEO_PARAMS_CODEMODE, cJSON_CreateNumber(2));
			  }
			}
			
			if (StringCmp(str[1], "resolution"))
			{
			  if(!strcmp(arg[0], "720P"))
			  {
				cJSON_AddItemToObject(_ParamArry[num], SET_WEB_VIDEO_PARAMS_RESOLUTIONSET, cJSON_CreateNumber(1));	   
			  }
			  else if(!strcmp(arg[0], "1080P"))
			  {
				cJSON_AddItemToObject(_ParamArry[num], SET_WEB_VIDEO_PARAMS_RESOLUTIONSET, cJSON_CreateNumber(2));
			  }
			  else if((!strcmp(arg[0], "4k"))||(!strcmp(arg[0], "4K")))
			  {
				cJSON_AddItemToObject(_ParamArry[num], SET_WEB_VIDEO_PARAMS_RESOLUTIONSET, cJSON_CreateNumber(3));
			  }
			  else
			  {
				mLogDebug("ResolutionSet not support !!!");
				//cJSON_AddItemToObject(_ParamArry[num], "CharResolution", cJSON_CreateString(arg[0]));
			  }
			  cJSON_AddItemToObject(_ParamArry[num], "CharResolution", cJSON_CreateString(arg[0]));
			}
			
			if (StringCmp(str[1], "bitrate_mode"))
			{
			  if((!strcmp(arg[0], "cbr"))||(!strcmp(arg[0], "CBR"))){
			  cJSON_AddItemToObject(_ParamArry[num], SET_WEB_VIDEO_PARAMS_RATETYPE, cJSON_CreateNumber(1));	   
			  }else{
			  cJSON_AddItemToObject(_ParamArry[num], SET_WEB_VIDEO_PARAMS_RATETYPE, cJSON_CreateNumber(2));
			  }
			}		
			//StringCmp（）会将"bitrate_mode"里的bitrate和"bitrate"进行比较
			if (!strcmp(str[1], "bitrate"))
			{
			  cJSON_AddItemToObject(_ParamArry[num], SET_WEB_VIDEO_PARAMS_RATESIZE, cJSON_CreateNumber(atoi(arg[0])));
			}
			
			if (StringCmp(str[1], "fps"))
			{
			  cJSON_AddItemToObject(_ParamArry[num], SET_WEB_VIDEO_PARAMS_FRAMESET, cJSON_CreateNumber(atoi(arg[0])));
			}
			
			if (StringCmp(str[1], "gop"))
			{
			  cJSON_AddItemToObject(_ParamArry[num], SET_WEB_VIDEO_PARAMS_GOPSET, cJSON_CreateNumber(atoi(arg[0])));
			}

		}
		if(!strcmp(CMethod.c_str(),"GetWebCaptionList"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+7,len-7);//7:vid_enc字符数量
			int comp_num = atoi(str[0]);//vid_enc%d,vid_enc的序号

			mLogDebug("GetWebCaptionList cmd_id="<<comp_num);

			int length = strlen(str[1]);
			memcpy(str[1],str[1]+3,length-3);//3:osd字符数量
			int osdnum = atoi(str[1]);//osd,osd的序号
	
			if (StringCmp(str[2], "s_display"))
			{			
			
				//cJSON_AddItemToObject(_ParamArry[net_send_num], "OverlayDisplay", cJSON_CreateString(arg[0]));
				cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayDisplay", cJSON_CreateNumber(atoi(arg[0])));
				switch(comp_num)
				{
			  		case 1:
				   		//cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYCHN, cJSON_CreateString("COMP1"));
						cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayChn", cJSON_CreateNumber(comp_num));
				   		break;
			  		case 2:
				  		//cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYCHN, cJSON_CreateString("COMP2"));
						cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayChn", cJSON_CreateNumber(comp_num));
				   		break;
			  		default:
				   		mLogError("please input the right comp_num!!!\n");
				   		return false;
				}	
				//cJSON_AddItemToObject(_ParamArry[net_send_num], "OverlayChn", cJSON_CreateNumber(net_send_num));
				cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayNum", cJSON_CreateNumber(osdnum));
			
				//mLogDebug("CHNSYS_N cmd_id="<<net_send_num);
			}
			if (StringCmp(str[2], "position"))
			{			
				cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayPost", cJSON_CreateNumber(atoi(arg[0])));
			}
			if (StringCmp(str[2], "type"))
			{			
				cJSON_AddItemToObject(_ParamArry[osdnum], "OSDType", cJSON_CreateNumber(atoi(arg[0])));
			}
			if (StringCmp(str[2], "content"))
			{			
				cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayContent", cJSON_CreateString(arg[0]));
			}
			if (StringCmp(str[2], "char_set"))
			{			

				cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayType", cJSON_CreateString(arg[0]));
			}
		
			if (StringCmp(str[2], "color"))
			{			
				cJSON_AddItemToObject(_ParamArry[osdnum], "Color", cJSON_CreateNumber(atoi(arg[0])));
			}
			if (StringCmp(str[2], "size"))
			{			
				cJSON_AddItemToObject(_ParamArry[osdnum], "FontSize", cJSON_CreateNumber(atoi(arg[0])));
			}

		}

	}
	else if (CHNSYS_NET_SEND_OP == cmd_id)
	{
		mLogDebug("CHNSYS_NET_SEND_OP cmd_id="<<cmd_id);
		int len = strlen(str[0]);
		memcpy(str[0],str[0]+8,len-8);//7:net_send字符数量
		int num = atoi(str[0]);//net_send%d,net_send的序号
		if((!strcmp(CMethod.c_str(),"GetWebHostChnInfo")) && (num <= 12))
		{
			if (StringCmp(str[1],"name"))
			{
				
				cJSON_AddItemToObject(_ParamArry[num],GET_HOST_CONFIG_RES_CH_NCHNNAME,cJSON_CreateString(arg[0]));
			}
			if (StringCmp(str[1],"rec_current_status"))
			{
				cJSON_AddItemToObject(_ParamArry[num],"CurrentState",cJSON_CreateNumber(atoi(arg[0])));
			}
			if (StringCmp(str[1],"rec_task_id"))
			{
				cJSON_AddItemToObject(_ParamArry[num],"CurrentTaskID",cJSON_CreateString(arg[0]));
			}
			else if (StringCmp(str[1],"on"))
			{

				cJSON_AddItemToObject(_ParamArry[num],GET_HOST_CONFIG_RES_CH_NCHNID,cJSON_CreateNumber(num));
				cJSON_AddItemToObject(_ParamArry[num],GET_HOST_CONFIG_RES_CH_NCHNSTA,cJSON_CreateNumber(atoi(arg[0])));
			}
			else if (StringCmp(str[1],"url"))
			{
				cJSON_AddItemToObject(_ParamArry[num],"RtspUrl",cJSON_CreateString(arg[0]));
			}
			else if (StringCmp(str[1],"recording"))
			{
				cJSON_AddItemToObject(_ParamArry[num],GET_HOST_CONFIG_RES_CH_NCHNRECSTA,cJSON_CreateNumber(atoi(arg[0])));
			}
			else if (StringCmp(str[1],"rec_path"))
			{
				cJSON_AddItemToObject(_ParamArry[num],GET_HOST_CONFIG_RES_CH_NCHNRECTASKID,cJSON_CreateString(arg[0]));
			}

		}
		if(!strcmp(CMethod.c_str(),"GetWebRtspParam"))
		{
			if(num <= 13)
			{
				if (StringCmp(str[1], "name"))
				{			
				 	cJSON_AddItemToObject(_ParamArry[num], SET_WEB_RTSP_PARAMS_RTSPSOURCE, cJSON_CreateString(arg[0]));
					cJSON_AddItemToObject(_ParamArry[num], SET_WEB_RTSP_PARAMS_RTSPNUM, cJSON_CreateNumber(num));
				}

				if (StringCmp(str[1], "url"))
				{
			 	 	cJSON_AddItemToObject(_ParamArry[num], SET_WEB_RTSP_PARAMS_RTSPURL, cJSON_CreateString(arg[0]));
				}
				
				if (StringCmp(str[1], "usr"))
				{			
					cJSON_AddItemToObject(_ParamArry[num], "UserName", cJSON_CreateString(arg[0]));
					cJSON_AddItemToObject(_ParamArry[num], "Password", cJSON_CreateString(arg[1]));
				}

				if (StringCmp(str[1], "encrypt"))
				{
					cJSON_AddItemToObject(_ParamArry[num], "Encrypt", cJSON_CreateNumber(atoi(arg[0])));
				}
			}
		}
		//GetWebStartRec接口
		if(!strcmp(CMethod.c_str(),"GetWebStartRec"))
		{
			if(num <= 12)
			{
				if (StringCmp(str[1], "rec_is_select"))
				{			
					cJSON_AddItemToObject(_ParamArry[num], "VideoEncChnnel", cJSON_CreateNumber(num));
				
					cJSON_AddItemToObject(_ParamArry[num], "IfRec", cJSON_CreateNumber(atoi(arg[0])));
				}
			}
		}
		if(!strcmp(CMethod.c_str(),"GetWebRtmpBroadConf"))
		{
			if (StringCmp(str[1], "on"))
			{			
				cJSON_AddItemToObject(_result, "IsEnabled", cJSON_CreateNumber(atoi(arg[0])));
			}
			if (StringCmp(str[1], "url"))
			{			
				cJSON_AddItemToObject(_result, "BroadAddr", cJSON_CreateString(arg[0]));
			}
		}
		if(!strcmp(CMethod.c_str(),"GetWebRecordTime"))
		{
			
			if (StringCmp(str[1], "rec_stime"))
			{	
				cJSON_AddItemToObject(_ParamArry[num], "ChannelID", cJSON_CreateNumber(num));
				if(atoi(arg[0]) == 0)
				{
					cJSON_AddItemToObject(_ParamArry[num], "RecTime", cJSON_CreateString("00:00:00:00"));
				}
				else
				{
					time_t timep;
					int dates;
					struct tm *LocalTime;
					char SysTime[80];
					char tmpSysTime[80];
					time(&timep);
					mLogDebug("GetWebRecordTime timep = "<<timep);
					timep = timep - atoi(arg[0]) - 60*60*8;//系统当前时间-开始时间-8小时
					dates = timep/60/60/24;//录制天数
					LocalTime = std::localtime(&timep);
					strftime(SysTime, 80, "%H:%M:%S", LocalTime);
					sprintf(tmpSysTime, "%02d:%s", dates,SysTime);
					cJSON_AddItemToObject(_ParamArry[num], "RecTime", cJSON_CreateString(tmpSysTime));
				}
			}
			if (StringCmp(str[1], "rec_task_id"))
			{			
				cJSON_AddItemToObject(_ParamArry[num], "TaskID", cJSON_CreateString(arg[0]));
			}
			if (StringCmp(str[1], "rec_current_status"))
			{			
				cJSON_AddItemToObject(_ParamArry[num], "RecStatus", cJSON_CreateNumber(atoi(arg[0])));
				switch(num)
				{
					case 1:
						cJSON_AddItemToObject(_ParamArry[num], "ChnName", cJSON_CreateString("合成1"));
						break;
					case 2:
						cJSON_AddItemToObject(_ParamArry[num], "ChnName", cJSON_CreateString("本地1"));
						break;
					case 3:
						cJSON_AddItemToObject(_ParamArry[num], "ChnName", cJSON_CreateString("本地2"));
						break;
					case 4:
						cJSON_AddItemToObject(_ParamArry[num], "ChnName", cJSON_CreateString("本地3"));
						break;
					case 5:
						cJSON_AddItemToObject(_ParamArry[num], "ChnName", cJSON_CreateString("本地4"));
						break;
					case 6:
						cJSON_AddItemToObject(_ParamArry[num], "ChnName", cJSON_CreateString("本地5"));
						break;
					case 7:
						cJSON_AddItemToObject(_ParamArry[num], "ChnName", cJSON_CreateString("本地6"));
						break;
					case 8:
						cJSON_AddItemToObject(_ParamArry[num], "ChnName", cJSON_CreateString("本地7"));
						break;
					case 9:
						cJSON_AddItemToObject(_ParamArry[num], "ChnName", cJSON_CreateString("本地8"));
						break;
				}
			}
			else if (StringCmp(str[1],"recording"))
			{
				cJSON_AddItemToObject(_ParamArry[num],"OldRecStatus",cJSON_CreateNumber(atoi(arg[0])));
			}
			else if (StringCmp(str[1],"rec_path"))
			{
				cJSON_AddItemToObject(_ParamArry[num],"OldRecTaskID",cJSON_CreateString(arg[0]));
			}
			
		}
	}
	else if (CHNSYS_AUD_ENC_OP == cmd_id)
	{
		mLogDebug("CHNSYS_AUD_ENC_OP cmd_id="<<cmd_id);
		if(!strcmp(CMethod.c_str(),"GetWebAudioParam"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+7,len-7);//7:aud_enc字符数量
			int num = atoi(str[0]);//aud_enc%d,aud_enc的序号

			if(num <= 8)
			{
				 if (StringCmp(str[1], "channel"))
				 {			 
				   cJSON_AddItemToObject(_ParamArry[num], SET_WEB_AUDIO_PARAMS_SOUNDSOURCE, cJSON_CreateString(arg[0]));
				   cJSON_AddItemToObject(_ParamArry[num], SET_WEB_AUDIO_PARAMS_SOUNDNUM, cJSON_CreateNumber(num));
				 }
				
				 if (StringCmp(str[1], "encode"))
				 {
					 int SamplingRate = atoi(arg[1]);
					 if ((!strcmp(arg[0], "AAC"))||(!strcmp(arg[0], "aac")))
					 {			 
					   cJSON_AddItemToObject(_ParamArry[num], SET_WEB_AUDIO_PARAMS_CODEMODE, cJSON_CreateNumber(1));
					 }
					 
					 if ((!strcmp(arg[0], "G711U"))||(!strcmp(arg[0], "g711u")))
					 {			 
					   cJSON_AddItemToObject(_ParamArry[num], SET_WEB_AUDIO_PARAMS_CODEMODE, cJSON_CreateNumber(2));
					 }
					 
					 if ((!strcmp(arg[0], "G711A"))||(!strcmp(arg[0], "g711a")))
					 {			 
					   cJSON_AddItemToObject(_ParamArry[num], SET_WEB_AUDIO_PARAMS_CODEMODE, cJSON_CreateNumber(3));
					 }

				
					 switch(SamplingRate)
					 {
					   case 8000:
							SamplingRate = 1;
							break;
					   case 11025:
							SamplingRate = 2;
							break;
					   case 22050:
							SamplingRate = 3;
							break;
					   case 24000:
							SamplingRate = 4;
							break;
					   case 44100:
							SamplingRate = 5;
							break;
					   case 48000:
							SamplingRate = 6;
							break;
					   default:
							mLogError("please input the right SamplingRate!!!\n");
							return false;
					 }
				
				   cJSON_AddItemToObject(_ParamArry[num], SET_WEB_AUDIO_PARAMS_SAMPLERATE, cJSON_CreateNumber(SamplingRate));
				 }
				 if (StringCmp(str[1], "bitrate"))
				{
				   cJSON_AddItemToObject(_ParamArry[num], SET_WEB_AUDIO_PARAMS_RATESET, cJSON_CreateNumber(atoi(arg[0])));
				}
			}
		}
	}
	else if (CHNSYS_WEB_GET == cmd_id)
	{
		mLogDebug("CHNSYS_WEB_GET cmd_id="<<cmd_id);
		
		if (StringCmp(str[1], "guest_passwd"))
		{			
		  oldpassword = arg[0];
		}
		if (StringCmp(str[1], "admin_username"))
		{			
		  UserName = arg[0];
		}
		if (StringCmp(str[1], "root_passwd"))
		{			
		  oldpassword = arg[0];
		}
		if (StringCmp(str[1], "root_username"))
		{			
		  UserName = arg[0];
		}
	}
	else if (CHNSYS_VID_CAP_OP == cmd_id)
	{
		mLogDebug("CHNSYS_VID_CAP_OP cmd_id="<<cmd_id);
		if(!strcmp(CMethod.c_str(),"GetWebVideoChnList"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+7,len-7);//7:vid_cap字符数量
			int num = atoi(str[0]);//vid_cap%d,vid_cap的序号
			
			int length = strlen(str[2]);
			memcpy(str[2],str[2]+3,length-3);//7:osd字符数量
			int osdnum = atoi(str[2]);//osd,osd的序号
			
			if(osdnum == 1)
			{
				if (StringCmp(str[3], "s_display"))
				{			
				  
				  cJSON_AddItemToObject(_ParamArry[num+1], SET_WEB_VIDEO_CHN_CONF_PARAMS_CHNPORT, cJSON_CreateNumber(num+1));

				  cJSON_AddItemToObject(_ParamArry[num+1], SET_WEB_VIDEO_CHN_CONF_PARAMS_ISSHOW, cJSON_CreateNumber(atoi(arg[0])));
				  
				}
				
				if (StringCmp(str[3], "content"))
				{
				  cJSON_AddItemToObject(_ParamArry[num+1], SET_WEB_VIDEO_CHN_CONF_PARAMS_CHNNAME, cJSON_CreateString(arg[0]));
				
				}
			}
		}
		if(!strcmp(CMethod.c_str(),"GetWebHostChnInfo"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+7,len-7);//7:vid_cap字符数量
			int num = atoi(str[0]);//vid_cap%d,vid_cap的序号

			mLogDebug("GetWebHostChnInfo");
			if (StringCmp(str[3], "content"))
			{
			  cJSON_AddItemToObject(_ParamArry[num+1], "OsdContent", cJSON_CreateString(arg[0]));
			
			}
		}
		if(!strcmp(CMethod.c_str(),"GetWebCaptionList"))
		{

			int len = strlen(str[0]);
			memcpy(str[0],str[0]+7,len-7);//7:vid_cap字符数量
			int num = atoi(str[0]);//vid_cap%d,vid_cap的序号

			int length = strlen(str[2]);
			memcpy(str[2],str[2]+3,length-3);//7:osd字符数量
			int osdnum = atoi(str[2]);//osd,osd的序号
			//mLogDebug("GetWebCaptionList=");
			if(osdnum > 1)
			{
				if (StringCmp(str[3], "position"))
				{		
				  cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYPOST, cJSON_CreateNumber(atoi(arg[0])));
				  cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYNUM, cJSON_CreateNumber(osdnum));
				  	switch(num)
					{
						case 1:
							cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYCHN, cJSON_CreateString("VL1-1"));
							break;
						case 2:
							cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYCHN, cJSON_CreateString("VL2-1"));
							break;
						case 3:
							cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYCHN, cJSON_CreateString("VL3-1"));
							break;
						case 4:
							cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYCHN, cJSON_CreateString("VL4-1"));
							break;
						case 5:
							cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYCHN, cJSON_CreateString("VL5-1"));
							break;
						case 6:
							cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYCHN, cJSON_CreateString("VL6-1"));
							break;
						case 7:
							cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYCHN, cJSON_CreateString("VL7-1"));
							break;
						case 8:
							cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYCHN, cJSON_CreateString("VL8-1"));
							break;		
					}
				  
				}
				
				if (StringCmp(str[3], "content"))
				{
				  cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYCONTENT, cJSON_CreateString(arg[0]));
				}
				if (StringCmp(str[3], "char_set"))
				{		
				  cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYTYPE, cJSON_CreateString(arg[0]));
				}
				if (StringCmp(str[3], "size"))
				{
				  cJSON_AddItemToObject(_ParamArry[osdnum], "FontSize", cJSON_CreateNumber(atoi(arg[0])));
				}
				if (StringCmp(str[3], "color"))
				{		
				  cJSON_AddItemToObject(_ParamArry[osdnum], "Color", cJSON_CreateNumber(atoi(arg[0])));
				}
				if (StringCmp(str[3], "type"))
				{		
				  cJSON_AddItemToObject(_ParamArry[osdnum], "TimeORText", cJSON_CreateNumber(atoi(arg[0])));
				}
				if (StringCmp(str[3], "s_display"))
				{
				  cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayDisplay", cJSON_CreateNumber(atoi(arg[0])));
				}
			}
		}
		//GetWebStartRec接口
		if(!strcmp(CMethod.c_str(),"GetWebStartRec"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+7,len-7);//7:vid_cap字符数量
			int net_send_num = atoi(str[0]);//vid_cap%d,vid_cap的序号
			mLogDebug("GetWebStartRec cmd_id="<<net_send_num);

			if (StringCmp(str[3], "content"))
			{			
				
				cJSON_AddItemToObject(_ParamArry[net_send_num+1], SET_WEB_EFFECT_PARAMS_VOERLAYCONTENT, cJSON_CreateString(arg[0]));
				//cJSON_AddItemToObject(_ParamArry[net_send_num], "IfRec", cJSON_CreateNumber(atoi(arg[0])));
				//cJSON_AddItemToArray(_ParamList, _ParamArry[net_send_num]);
				//mLogDebug("CHNSYS_N cmd_id="<<net_send_num);
			}

		}

	}
	else if (CHNSYS_TDC_OP == cmd_id)
	{
		mLogDebug("CHNSYS_TDC_OP cmd_id="<<cmd_id);
		if(_ParamList == NULL)
		{
			//printf("_ParamList == NULL.");
			mLogDebug("_ParamList == NULL.");
			return false;
		}

		if (StringCmp(str[1], "baudrate"))
		{			
			int BaudRate = atoi(arg[0]);

			switch(BaudRate)
		  {
			case 2400:
				 BaudRate = 1;
				 break;
			case 4800:
				 BaudRate = 2;
				 break;
			case 7200:
				 BaudRate = 3;
				 break;
			case 9600:
				 BaudRate = 4;
				 break;
			case 115200:
				 BaudRate = 5;
				 break;
			case 14400:
				 BaudRate = 6;
				 break;
			case 19200:
				 BaudRate = 7;
				 break;
			default:
				 mLogError("please input the right bitRate!!!\n");
				 return false;
			}

		  cJSON_AddItemToObject(_taskList, SET_WEB_UART_CONF_PARAMS_BAUDRATE, cJSON_CreateNumber(BaudRate));
		}

		if (StringCmp(str[1], "databits"))
		{
		  cJSON_AddItemToObject(_taskList, SET_WEB_UART_CONF_PARAMS_DATEBIT, cJSON_CreateNumber(atoi(arg[0])));
		}
	
	    if (StringCmp(str[1], "parity"))
	    {
	      cJSON_AddItemToObject(_taskList, SET_WEB_UART_CONF_PARAMS_PARITYBIT, cJSON_CreateNumber(atoi(arg[0])));
	    }
		
		if (StringCmp(str[1], "stopbits"))
		{
		  cJSON_AddItemToObject(_taskList, SET_WEB_UART_CONF_PARAMS_STOPBIT, cJSON_CreateNumber(atoi(arg[0])));
		}
	}
	else if (CHNSYS_VID_OUT_OP == cmd_id)
	{
		mLogDebug("CHNSYS_VID_OUT_OP cmd_id="<<cmd_id);
		if(!strcmp(CMethod.c_str(),"GetWebVideoMatrix"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+10,len-10);//4:vid_dis_hd字符数量
			int num = atoi(str[0]);//vid_dis_hd%d,vid_dis_hd的序号
			if (StringCmp(str[2], "channel"))
			{
			  cJSON_AddItemToObject(_ParamArry[num], "ChnnelID", cJSON_CreateNumber(num));
			  cJSON_AddItemToObject(_ParamArry[num], "ChnnelName", cJSON_CreateString(arg[0]));
			}
		}
		if(!strcmp(CMethod.c_str(),"GetWebVideoOutPutConf"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+10,len-10);//4:vid_dis_hd字符数量
			int num = atoi(str[0]);//vid_dis_hd%d,vid_dis_hd的序号
			
			if (StringCmp(str[1], "resolution"))
			{			
				if(!strcmp(str[0],"vid_dis_hd2"))
				{
					if(StringCmp(arg[0], "1080P60"))
					{
						cJSON_AddItemToObject(_ParamArry[num], "OutpuResolution", cJSON_CreateNumber(1));
					}
					else if((!strcmp(arg[0], "4k"))||(!strcmp(arg[0], "4K")))
					{
						cJSON_AddItemToObject(_ParamArry[num], "OutpuResolution", cJSON_CreateNumber(2));
					}
					
					cJSON_AddItemToObject(_ParamArry[num], "CharResoulation", cJSON_CreateString(arg[0]));
				}else{
					cJSON_AddItemToObject(_ParamArry[num], "OutpuResolution", cJSON_CreateNumber(1));
					cJSON_AddItemToObject(_ParamArry[num], "CharResoulation", cJSON_CreateString(arg[0]));
				}
			}
			if (StringCmp(str[1], "luminance"))
			{			
			  cJSON_AddItemToObject(_ParamArry[num], "Luminance", cJSON_CreateNumber(atoi(arg[0])));
			}
			if (StringCmp(str[1], "contrast"))
			{			
			  cJSON_AddItemToObject(_ParamArry[num], "Contrast", cJSON_CreateNumber(atoi(arg[0])));
			}
			if (StringCmp(str[1], "hue"))
			{			
			  cJSON_AddItemToObject(_ParamArry[num], "Hue", cJSON_CreateNumber(atoi(arg[0])));
			}
			if (StringCmp(str[1], "satuature"))
			{			
			  cJSON_AddItemToObject(_ParamArry[num], "Satuature", cJSON_CreateNumber(atoi(arg[0])));
			}	
		}
		//获取DSP切换接口
//		if (StringCmp(str[2], "channel"))
//		{			
//		  cJSON_AddItemToObject(_ParamKnod, "InPutChnnel", cJSON_CreateString(arg[0]));
//		}
	}
	else if (CHNSYS_FTP_INFO_OP == cmd_id)
	{
		mLogDebug("CHNSYS_FTP_INFO_OP cmd_id="<<cmd_id);

		if (StringCmp(str[1], "listen_port"))
		{			
		  cJSON_AddItemToObject(_result, SET_FTP_CONF_PARAMS_FTPPORT, cJSON_CreateNumber(atoi(arg[0])));
		}
/*
		if (StringCmp(str[1], "passwd"))
		{
		  cJSON_AddItemToObject(_result, SET_FTP_CONF_PARAMS_FTPPASSWORD, cJSON_CreateString(arg[0]));
		}
*/	
	    if (StringCmp(str[1], "local_root"))
	    {
	      cJSON_AddItemToObject(_result, SET_FTP_CONF_PARAMS_FTPPATH, cJSON_CreateString(arg[0]));
	    }
		
	}
	else if (CHNSYS_REC_OP == cmd_id)
	{
		mLogDebug("CHNSYS_REC_OP cmd_id="<<cmd_id);

		if (StringCmp(str[1], "rectype"))
		{			
		  cJSON_AddItemToObject(_result, GET_WEB_STORAGE_CONF_PARAMS_FILETYPE, cJSON_CreateNumber(atoi(arg[0])));
		}

		if (StringCmp(str[1], "minifreespace"))
		{
		  cJSON_AddItemToObject(_result, GET_WEB_STORAGE_CONF_PARAMS_STORAGETHRESOLD, cJSON_CreateNumber(atoi(arg[0])));
		}
	
	    if (StringCmp(str[1], "auto_del_file_on"))
	    {
	      cJSON_AddItemToObject(_result, GET_WEB_STORAGE_CONF_PARAMS_AUTODEL, cJSON_CreateNumber(atoi(arg[0])));
	    }
		if (StringCmp(str[1], "partition"))
		{
		  cJSON_AddItemToObject(_result, GET_WEB_STORAGE_CONF_PARAMS_SEGSIZE, cJSON_CreateNumber(atoi(arg[0])));
		}
	    if (StringCmp(str[1], "autorecon"))
	    {
	      cJSON_AddItemToObject(_result, "StartRec", cJSON_CreateNumber(atoi(arg[0])));
	    }
		
		//GetWebRcrvRecord
	    if (StringCmp(str[1], "close_court_rec_on"))
	    {
	      cJSON_AddItemToObject(_result,"RecOn", cJSON_CreateNumber(atoi(arg[0])));
	    }
		
	}
	else if (CHNSYS_SYSLOG_OP == cmd_id)
	{
		mLogDebug("CHNSYS_SYSLOG_OP cmd_id="<<cmd_id);

		if (StringCmp(str[1], "partition_size"))
		{			
		  cJSON_AddItemToObject(_result, "LogPartitionSize", cJSON_CreateNumber(atoi(arg[0])));
		  cJSON_AddItemToObject(_result, "CharLogPartitionSize", cJSON_CreateString(arg[0]));
		}

		if (StringCmp(str[1], "option"))
		{
		    if (StringCmp(arg[0], "debug"))
		    {
			  cJSON_AddItemToObject(_result, SET_WEB_LOG_FILE_CONF_PARAMS_LOGLEVEL, cJSON_CreateNumber(1));
		    }
		    else if (StringCmp(arg[0], "info"))
		    {
			  cJSON_AddItemToObject(_result, SET_WEB_LOG_FILE_CONF_PARAMS_LOGLEVEL, cJSON_CreateNumber(2));
		    }
		    else
		    {
			  cJSON_AddItemToObject(_result, SET_WEB_LOG_FILE_CONF_PARAMS_LOGLEVEL, cJSON_CreateNumber(3));
		    }
		}
	
	    if (StringCmp(str[1], "time_limit"))
	    {
	      cJSON_AddItemToObject(_result, SET_WEB_LOG_FILE_CONF_PARAMS_LOGSAVETIME, cJSON_CreateNumber(atoi(arg[0])));
	    }
		
	}
	else if (CHNSYS_VID_COMP_OP == cmd_id)
	{
		mLogDebug("CHNSYS_VID_COMP_OP cmd_id="<<cmd_id);
		
		if (!strcmp(str[0], "comp1"))
		{			
		  if (StringCmp(str[1], "mode"))
		  {
		  	cJSON_AddItemToObject(_result, "ChannelID", cJSON_CreateNumber(1));
		  	cJSON_AddItemToObject(_result, "Mode", cJSON_CreateString(arg[0]));
		  }
		}
		
		 if (!strcmp(str[0], "comp2"))
		 {			 
		   if (StringCmp(str[1], "mode"))
		   {
			 cJSON_AddItemToObject(_result, "ChannelID", cJSON_CreateNumber(2));
			 cJSON_AddItemToObject(_result, "Mode", cJSON_CreateString(arg[0]));
		   }
		 }
		 
		if(!strcmp(CMethod.c_str(),"GetWebHostChnInfo"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+4,len-4);//4:comp字符数量
			int num = atoi(str[0]);//comp%d,comp的序号
			if (StringCmp(str[2], "content"))
			{
			  cJSON_AddItemToObject(_ParamArry[num], "OsdContent", cJSON_CreateString(arg[0]));
			
			}
		}
		if(!strcmp(CMethod.c_str(),"GetWebStartRec"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+4,len-4);//4:comp字符数量
			int num = atoi(str[0]);//comp%d,comp的序号
			if (StringCmp(str[2], "content"))
			{
			  cJSON_AddItemToObject(_ParamArry[num], "OverlayContent", cJSON_CreateString(arg[0]));			
			}
		}
		if(!strcmp(CMethod.c_str(),"GetWebVideoChnList"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+4,len-4);//4:comp字符数量
			int num = atoi(str[0]);//comp%d,comp的序号

			int length = strlen(str[1]);
			memcpy(str[1],str[1]+3,length-3);//3:osd字符数量
			int osdnum = atoi(str[1]);//osd,osd的序号
			
			if(osdnum == 1)
			{
				if (StringCmp(str[2], "s_display"))
				{			
				  
				  cJSON_AddItemToObject(_ParamArry[num], SET_WEB_VIDEO_CHN_CONF_PARAMS_CHNPORT, cJSON_CreateNumber(num));

				  cJSON_AddItemToObject(_ParamArry[num], SET_WEB_VIDEO_CHN_CONF_PARAMS_ISSHOW, cJSON_CreateNumber(atoi(arg[0])));
				  
				}
				
				if (StringCmp(str[2], "content"))
				{
				  cJSON_AddItemToObject(_ParamArry[num], SET_WEB_VIDEO_CHN_CONF_PARAMS_CHNNAME, cJSON_CreateString(arg[0]));
				
				}
			}
		}
		if(!strcmp(CMethod.c_str(),"GetWebCaptionList"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+4,len-4);//4:comp字符数量
			int comp_num = atoi(str[0]);//comp%d,comp的序号
			mLogDebug("GetWebCaptionList cmd_id="<<comp_num);

			int length = strlen(str[1]);
			memcpy(str[1],str[1]+3,length-3);//3:osd字符数量
			int osdnum = atoi(str[1]);//osd,osd的序号
			
			if(osdnum > 1)//osd通道从2开始
			{
				if (StringCmp(str[2], "s_display"))
				{			
				
					//cJSON_AddItemToObject(_ParamArry[net_send_num], "OverlayDisplay", cJSON_CreateString(arg[0]));
					cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayDisplay", cJSON_CreateNumber(atoi(arg[0])));
					switch(comp_num)
					{
				  		case 1:
					   		//cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYCHN, cJSON_CreateString("COMP1"));
							cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayChn", cJSON_CreateString("COMP1"));
					   		break;
				  		case 2:
					  		//cJSON_AddItemToObject(_ParamArry[osdnum], SET_WEB_EFFECT_PARAMS_VOERLAYCHN, cJSON_CreateString("COMP2"));
							cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayChn", cJSON_CreateString("COMP2"));
					   		break;
				  		default:
					   		mLogError("please input the right comp_num!!!\n");
					   		return false;
					}	
					//cJSON_AddItemToObject(_ParamArry[net_send_num], "OverlayChn", cJSON_CreateNumber(net_send_num));
					cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayNum", cJSON_CreateNumber(osdnum));
				
					//mLogDebug("CHNSYS_N cmd_id="<<net_send_num);
				}
				if (StringCmp(str[2], "position"))
				{			
					cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayPost", cJSON_CreateNumber(atoi(arg[0])));
				}
				if (StringCmp(str[2], "type"))
				{			
					cJSON_AddItemToObject(_ParamArry[osdnum], "TimeORText", cJSON_CreateNumber(atoi(arg[0])));
				}
				if (StringCmp(str[2], "content"))
				{			
					cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayContent", cJSON_CreateString(arg[0]));
				}
				if (StringCmp(str[2], "char_set"))
				{			

					cJSON_AddItemToObject(_ParamArry[osdnum], "OverlayType", cJSON_CreateString(arg[0]));
				}
			
				if (StringCmp(str[2], "color"))
				{			
					cJSON_AddItemToObject(_ParamArry[osdnum], "Color", cJSON_CreateNumber(atoi(arg[0])));
				}
				if (StringCmp(str[2], "size"))
				{			
					cJSON_AddItemToObject(_ParamArry[osdnum], "FontSize", cJSON_CreateNumber(atoi(arg[0])));
				}
			}
		}

	}
	else if (CHNSYS_MOSAIC_OP == cmd_id)
	{
		mLogDebug("CHNSYS_MOSAIC_OP cmd_id="<<cmd_id);

//		if (StringCmp(str[1], "s_display"))
//		{			
//		  cJSON_AddItemToObject(_ParamKnod, "MosaicDisplay", cJSON_CreateNumber(atoi(arg[0])));
//		}
//
//		if (StringCmp(str[1], "channel"))
//		{
//		  cJSON_AddItemToObject(_ParamKnod, SET_WEB_NEW_MOSAIC_PARAMS_MOSAICCHN, cJSON_CreateString(arg[0]));
//		}
//	
//	    if (StringCmp(str[1], "mode"))
//	    {
//	      cJSON_AddItemToObject(_ParamKnod, SET_WEB_NEW_MOSAIC_PARAMS_MOSAICTYPE, cJSON_CreateNumber(atoi(arg[0])));
//	    }
//		if (StringCmp(str[1], "win"))
//		{//arg={win_x,win_y,win_w,win_h=1/3,1/3,1/3,1/3}
//		  char WinX[16]={0};
//		  sscanf(arg[3], "%[^=]=%s", WinX, arg[3]);
//		  cJSON_AddItemToObject(_ParamKnod, "MosaicWinX", cJSON_CreateString(arg[3]));
//		  cJSON_AddItemToObject(_ParamKnod, "MosaicWinY", cJSON_CreateString(arg[4]));
//		  cJSON_AddItemToObject(_ParamKnod, "MosaicWinW", cJSON_CreateString(arg[5]));
//		  cJSON_AddItemToObject(_ParamKnod, "MosaicWinH", cJSON_CreateString(arg[6]));
//		}
		int len = strlen(str[0]);
		memcpy(str[0],str[0]+6,len-6);//2:mosaic%d中mosaic字符个数
		int num = atoi(str[0]);//mosaic%d,mosaic的序号
		if(!strcmp(CMethod.c_str(),"GetWebMosaicList"))
		{
			if (StringCmp(str[1], "s_display"))
			{			
			  cJSON_AddItemToObject(_ParamArry[num], "MosaicDisplay", cJSON_CreateNumber(atoi(arg[0])));
			}
		
			if (StringCmp(str[1], "channel"))
			{
			  cJSON_AddItemToObject(_ParamArry[num], SET_WEB_NEW_MOSAIC_PARAMS_MOSAICCHN, cJSON_CreateString(arg[0]));
			  cJSON_AddItemToObject(_ParamArry[num], SET_WEB_NEW_MOSAIC_PARAMS_MOSAICNUM, cJSON_CreateNumber(num));
			}
		
			if (StringCmp(str[1], "mode"))
			{
			  cJSON_AddItemToObject(_ParamArry[num], SET_WEB_NEW_MOSAIC_PARAMS_MOSAICTYPE, cJSON_CreateNumber(atoi(arg[0])));
			}
			if (StringCmp(str[1], "win"))
			{
//			  //arg={win_x,win_y,win_w,win_h=1/3,1/3,1/3,1/3}
//			  char WinX[16]={0};
//			  sscanf(arg[3], "%[^=]=%s", WinX, arg[3]);
//			  cJSON_AddItemToObject(_ParamArry[num], "MosaicWinX", cJSON_CreateString(arg[3]));
//			  cJSON_AddItemToObject(_ParamArry[num], "MosaicWinY", cJSON_CreateString(arg[4]));
//			  cJSON_AddItemToObject(_ParamArry[num], "MosaicWinW", cJSON_CreateString(arg[5]));
//			  cJSON_AddItemToObject(_ParamArry[num], "MosaicWinH", cJSON_CreateString(arg[6]));
			  
			  cJSON_AddItemToObject(_ParamArry[num], "MosaicWinX", cJSON_CreateString(arg[0]));
			  cJSON_AddItemToObject(_ParamArry[num], "MosaicWinY", cJSON_CreateString(arg[1]));
			  cJSON_AddItemToObject(_ParamArry[num], "MosaicWinW", cJSON_CreateString(arg[2]));
			  cJSON_AddItemToObject(_ParamArry[num], "MosaicWinH", cJSON_CreateString(arg[3]));
			}
		
		}

	}
	else if (CHNSYS_AUD_OUT_OP == cmd_id)
	{
		mLogDebug("CHNSYS_AUD_OUT_OP cmd_id="<<cmd_id);
		if(!strcmp(CMethod.c_str(),METHOD_GET_WEB_NEW_WHINE))
		{
			int len = strlen(str[1]);
			memcpy(str[1],str[1]+2,len-2);//2:aud_out1.ch%d中ch字符个数
			int num = atoi(str[1]);//ch%d,ch的序号
			//char ChnName[64];
			mLogDebug("GetWebNewWhine"<<num);
			switch(num)
		  {
			case 1:
				 num = 1;
				 UserName = "通道1";
				 break;
			case 2:
				 num = 2;
				 UserName = "通道2";
				 break;
			case 5:
				 num = 3;
				 UserName = "通道3";
				 break;
			case 6:
				 num = 4;
				 UserName = "通道4";
				 break;
			default:
				 mLogError("GetWebNewWhine net_send_num error !!!\n"<<num);
				 return false;
			}
			if (StringCmp(str[2], "audio_crack_mode"))
			{			
			  cJSON_AddItemToObject(_ParamArry[num], "WhineChn", cJSON_CreateNumber(num));
			  cJSON_AddItemToObject(_ParamArry[num], "WhineType", cJSON_CreateNumber(atoi(arg[0])));
			  
			}
			if (!strcmp(str[2], "audio_crack"))
			{			
			  cJSON_AddItemToObject(_ParamArry[num], "WhineButton", cJSON_CreateNumber(atoi(arg[0])));
			  cJSON_AddItemToObject(_ParamArry[num], "ChnName", cJSON_CreateString(UserName.c_str()));
			  //mLogDebug("######GetWebNewWhineButton####"<<num);
			}

		}
		if(!strcmp(CMethod.c_str(),METHOD_GET_WEB_NEW_WHINE_BUTTON))
		{
			int len = strlen(str[1]);
			memcpy(str[1],str[1]+2,len-2);//2:aud_out1.ch%d中ch字符个数
			int num = atoi(str[1]);//ch%d,ch的序号
			mLogDebug("GetWebNewWhineButton"<<num);
			switch(num)
		  {
			case 1:
				 num = 1;
				 break;
			case 2:
				 num = 2;
				 break;
			case 5:
				 num = 3;
				 break;
			case 6:
				 num = 4;
				 break;
			default:
				 mLogError("GetWebNewWhineButton net_send_num error !!!\n"<<num);
				 return false;
			}
			if (StringCmp(str[2], "audio_crack"))
			{			
			  cJSON_AddItemToObject(_ParamArry[num], "WhineButton", cJSON_CreateNumber(atoi(arg[0])));
			  cJSON_AddItemToObject(_ParamArry[num], "WhineChn", cJSON_CreateNumber(num));
			  //mLogDebug("######GetWebNewWhineButton####"<<num);
			}

		}
	}
	else if (CHNSYS_NTP_OP == cmd_id)
	{
		mLogDebug("CHNSYS_NTP_OP cmd_id="<<cmd_id);

		if(!strcmp(CMethod.c_str(),"GetWebSyncTime"))
		{			
		 	if (StringCmp(str[1], "server"))
			{
		 		cJSON_AddItemToObject(_result, "NtpIP", cJSON_CreateString(arg[0]));
			}
		}	
	}
	else if (CHNSYS_YUAN_INFO_OP == cmd_id)
	{
		if(!strcmp(CMethod.c_str(),"GetWebHostChnInfo"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+4,len-4);//4:yuan字符数量
			int num = atoi(str[0]);//yuan%d,yuan的序号
			mLogDebug("CHNSYS_YUAN_INFO_OP cmd_id="<<cmd_id);
			if (StringCmp(str[1], "info"))
			{
				
				if(StringCmp(arg[1], "h323"))
				{
					char tmpstr[64];
					sprintf(tmpstr,"%s:%s", arg[1],arg[3]);
					Params[num] = tmpstr;
					
				}else{
					
					Params[num] = arg[3];
				}
			}
		}
		if(!strcmp(CMethod.c_str(),"SetWebRecSegModelSelect"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+4,len-4);//4:yuan字符数量
			int num = atoi(str[0]);//yuan%d,yuan的序号
			if (StringCmp(str[1], "info"))
			{
				Params[num] = arg[4];
			}
			
		}
		if(!strcmp(CMethod.c_str(),"GetWebDecInfo"))
		{
			int len = strlen(str[0]);
			memcpy(str[0],str[0]+4,len-4);//4:yuan字符数量
			int num = atoi(str[0]);//yuan%d,yuan的序号
			if (StringCmp(str[1], "info"))
			{
				char tmpstr[64];
				sprintf(tmpstr,"远端%d", num);
				cJSON_AddItemToObject(_ParamArry[num], "DecName", cJSON_CreateString(tmpstr));
				cJSON_AddItemToObject(_ParamArry[num], "DecChnID", cJSON_CreateString(arg[0]));
				cJSON_AddItemToObject(_ParamArry[num], "CallType", cJSON_CreateString(arg[1]));
				cJSON_AddItemToObject(_ParamArry[num], "CallChnNum", cJSON_CreateNumber(atoi(arg[2])+1));
				cJSON_AddItemToObject(_ParamArry[num], "CallUrl", cJSON_CreateString(arg[3]));
				
				if(strlen(arg[3]) > 5 )
				{
					cJSON_AddItemToObject(_ParamArry[num], "CallState", cJSON_CreateNumber(1));
				}else{
					cJSON_AddItemToObject(_ParamArry[num], "CallState", cJSON_CreateNumber(0));
				}
			}
		}
	}
	else
	{
		//printf("The CMD is not support ,cmd_id == %d\n",cmd_id);
		mLogError("The CMD is not support ,cmd_id == "<<cmd_id);
		return false;
	}

#endif 
	return true;
}

int CAnalyzeParaProc::AnalyParaCmdExtracMessage(char *top, char *low, char *string)
{
	if(NULL == string || NULL == top || NULL == low) {
		printf("arg is NULL\n");
		return false;
	}
	int i, rt;

	/* 申请资源 */
	char *str[MAX_ARG_NUM] = {};
	char *arg[MAX_ARG_NUM] = {};
	for (i = 0; i < MAX_ARG_NUM; i ++){
		str[i] = (char *)calloc(1, strlen(top)+1);
		if(NULL == str[i]) {
			printf("calloc is error\n");
			goto clean;
		}
		arg[i] = (char *)calloc(1, strlen(low)+1);
		if(NULL == arg[i]) {
			printf("calloc is error\n");
			goto clean;
		}
	}

	//printf("top = \"%s\", low = \"%s\"\n", top, low);

	/* 按'.'分割指令内容部分 */
	AnalyParaCmdCommandPartSplit(top, str);
	//mLogDebug("##str##"<<str[0]);

	/* 根据需求。按‘，’分割参数内容部分 */
	AnalyParaCmdArgumentsPartSplit(low, arg);
	//mLogDebug("##arg##"<<arg[0]);

	/* 网管协议指令处理 */
	rt = AnalyParaCmdParseCommandHandle(str, arg, string);
	if(rt < 0){
		mLogError("set cmd "<<top<<"="<<low<<"error, rt="<<rt<<" :[-1 CMD_STR_ERR; -2 ARG_STR_ERR; -3 CMD_DISABLE; -4 XML_SAVE_ERR]\n");
	}


clean:
	/* 释放资源 */
	for (i = 0; i < MAX_ARG_NUM; i ++){		
		if( str[i] != NULL ) {
			free(str[i]);
			str[i] = NULL;
		}
		if( arg[i] != NULL ) {
			free(arg[i]);
			arg[i] = NULL;
		}
	}

	return rt;
}

int CAnalyzeParaProc::AnalyParaMessage(char *string)
{
	if(NULL == string) {
		printf("string is NULL\n");
		return false;
	}
	int rt;
	//用于保存等号的前半部，代表命令字段
	char *top = (char *)calloc(1, strlen(string)+1);
	if(NULL == top) {
		printf("calloc is error\n");
		return false;
	}
	//用于保存等号的后半部，代表参数字段
	char *low = (char *)calloc(1, strlen(string)+1);
	if(NULL == low) {
		printf("calloc is error\n");
		if(NULL != top){
			free(top);
			top = NULL;
		}		
		return false;
	}

	/* 获取命令字段部分和参数字段部分 */
	AnalyParaCmdSplitMessage(string, &top, &low);
	//mLogDebug("##top##"<<top);
	//mLogDebug("##low##"<<low);

	/* 指令解析 */
	rt = AnalyParaCmdExtracMessage(top, low,string);

	/* 释放资源 */
	if(NULL != top){
		free(top);
		top = NULL;
	}
	if(NULL != low){
		free(low);
		low = NULL;
	}
	return rt;
}

bool CAnalyzeParaProc::ConvertParaProcessing(char *cParaBuf, int iLen)
{
    int i,count = 0;
	int ret = 0, result = 0;
    StringTab array = {0};
#if 0
    printf("ConvertParaProcessing\n");
    for (int i = 0; i < iLen; i++)
    {
		printf("%c", cParaBuf[i]);
    }
#endif 
    cParaBuf[iLen] = '\0';
    //指令拆分，以回车符分割指令
    array = StringSplit(cParaBuf,"\n");
	//for(int k = 0; k < array.number; k++){
	//	mLogDebug("array.string"<<array.string[k]);
	//	}
	for(i = 0; i < array.number; i ++){  
		if (strlen(array.string[i]) > 1){
			count++;
			//printf("array.string %d = %s", count, array.string[i]);
			mLogDebug("######array.string["<<i<<"]: "<<array.string[i]<<"######");
			/* 处理拆分后的每一条SET指令 */
			result = AnalyParaMessage(array.string[i]);
			if (result < 0) {
				ret = result;
			}
		}	
	}

	if(array.string != NULL){
		free(array.string);
		array.string = NULL;
	}	

    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebHostConfig(CData &oResult, char *cResult, cJSON *network, cJSON *sysinfo, cJSON *otherinfo,string Method)
{
    _network  = network;
    _sysinfo  = sysinfo;
    _otherinfo = otherinfo;
	CMethod = Method;

#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("ConvertSTRING2CJSONGetHostConfig\n");

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
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
    ConvertParaProcessing(ParaBuf, iParaLen);

    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetHostConfig(CData &oResult, char *cResult, cJSON *network, cJSON *sysinfo, cJSON *diskinfo, cJSON *decnum)
{
    _network  = network;
    _sysinfo  = sysinfo;
    _diskinfo = diskinfo;
	_decnum   = decnum;

#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("ConvertSTRING2CJSONGetHostConfig\n");

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
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
    ConvertParaProcessing(ParaBuf, iParaLen);

    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetHostDecInfo(CData &oResult, char *cResult, cJSON *rtspdec, cJSON *h323dec)
{
    _rtspdec  = rtspdec;
    _h323dec  = h323dec;
	

#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("ConvertSTRING2CJSONGetHostDecInfo\n");

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
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
    ConvertParaProcessing(ParaBuf, iParaLen);

    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebHostChnInfo(CData &oResult, char *cResult, cJSON* result, cJSON *node[], cJSON* _addressList, string Method, int ChnNum, string AudioBoard)
{
	CMethod = Method;
	_ParamArry = node;
	char httpAddr[128]={0};
	char strosd[128];
	string oldBoardOsd[10];
	
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("ConvertSTRING2CJSONGetHostDecInfo\n");

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
	
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	//printf("ConvertSTRING2CJSONGetHostDecInfo Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
	
	//mLogDebug("ConvertSTRING2CJSONGetWebHostChnInfo IP="<<_ip.c_str());
	//printf("IP = %s\n",_ip.c_str());
	int decNums = 0;
	if(AudioBoard != "new_audio_board")
	{
		for(int i = 1; i <= 6; i++)
		{
			if(Params[i] != "")
			{
				decNums++;
				oldBoardOsd[decNums] = Params[i];
			}
		}
	}
	if(ChnNum > 12)
	{			
		for(int i = 1; i <= 12; i++)
		{
				memset(httpAddr, 0, 128);
				memset(strosd, 0, 128);
				switch(i)
				{
				case 1:
					sprintf(httpAddr,"http://%s:9696/live/comp.flv", _ip.c_str());
//					cJSON_AddItemToObject(_ParamArry[i], "OsdContent", cJSON_CreateString("合成"));
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					break;
				case 2:
					sprintf(httpAddr,"http://%s:9696/live/vl1-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					break;
				case 3:
					sprintf(httpAddr,"http://%s:9696/live/vl2-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					break;
				case 4:
					sprintf(httpAddr,"http://%s:9696/live/vl3-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					break;
				case 5:
					sprintf(httpAddr,"http://%s:9696/live/vl4-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					break;
				case 6:
					sprintf(httpAddr,"http://%s:9696/live/vl5-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					break;
				case 7:
					sprintf(httpAddr,"http://%s:9696/live/vl6-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					break;
				case 8:
					sprintf(httpAddr,"http://%s:9696/live/vl7-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					break;
				case 9:
					sprintf(httpAddr,"http://%s:9696/live/vl8-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					break;
//				case 9:
//					sprintf(httpAddr,"http://%s:9696/live/comp.flv", _ip.c_str());
//					cJSON_AddItemToObject(_ParamArry[i], "OsdContent", cJSON_CreateString("合成"));
//					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
//					break;
				case 10:
					if(AudioBoard == "new_audio_board")
					{
						if(strlen(Params[1].c_str()) > 5)
						{
							sprintf(strosd,"远端1:%s", Params[1].c_str());
						}else{
							sprintf(strosd,"远端1");
						}
					}
					else
					{
						//Params[1-6],rtsp1-2,h323,sip1-3
						if(decNums > 0)
						{
							sprintf(strosd,"远端1:%s", oldBoardOsd[1].c_str());

						}else{
							sprintf(strosd,"远端1");
						}
						
					}

					sprintf(httpAddr,"http://%s:9696/live/vn1-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], "OsdContent", cJSON_CreateString(strosd));
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					break;
				case 11:

					if(AudioBoard == "new_audio_board")
					{
						if(strlen(Params[2].c_str()) > 5)
						{
							sprintf(strosd,"远端2:%s", Params[2].c_str());
						}else{
							sprintf(strosd,"远端2");
						}
					}
					else
					{
						//Params[1-7],rtsp1-2,h323,sip1-3
						if(decNums > 1)
						{
							sprintf(strosd,"远端2:%s", oldBoardOsd[2].c_str());

						}else{
							sprintf(strosd,"远端2");
						}
					}
					sprintf(httpAddr,"http://%s:9696/live/vn2-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], "OsdContent", cJSON_CreateString(strosd));
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					break;
				case 12:
					if(AudioBoard == "new_audio_board")
					{
						if(strlen(Params[3].c_str()) > 5)
						{
							sprintf(strosd,"远端3:%s", Params[3].c_str());
						}else{
							sprintf(strosd,"远端3");
						}
					}
					else
					{
						//Params[1-7],rtsp1-2,h323,sip1-3
						if(decNums > 2)
						{
							sprintf(strosd,"远端3:%s", oldBoardOsd[3].c_str());

						}else{
							sprintf(strosd,"远端3");
						}
					}
					sprintf(httpAddr,"http://%s:9696/live/vn3-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], "OsdContent", cJSON_CreateString(strosd));
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					break;
				}
				cJSON *CurrentState = cJSON_GetObjectItem(_ParamArry[i],"CurrentState");
				cJSON *CurrentTaskID = cJSON_GetObjectItem(_ParamArry[i],"CurrentTaskID");
				cJSON *RecStatus  = cJSON_GetObjectItem(_ParamArry[i],GET_HOST_CONFIG_RES_CH_NCHNRECSTA);
				cJSON *RecTaskID = cJSON_GetObjectItem(_ParamArry[i],GET_HOST_CONFIG_RES_CH_NCHNRECTASKID);
				//录制状态中的开始和停止状态依据RecStatus，暂停状态依据CurrentState
				if(RecStatus->valueint == 1)
				{
					cJSON_DeleteItemFromObject(_ParamArry[i],"CurrentState");
					cJSON_AddItemToObject(_ParamArry[i],"CurrentState",cJSON_CreateNumber(1));
					cJSON_DeleteItemFromObject(_ParamArry[i],"CurrentTaskID");
					cJSON_AddItemToObject(_ParamArry[i],"CurrentTaskID",cJSON_CreateString(RecTaskID->valuestring));
				}
				
				if((RecStatus->valueint == 0)&&(CurrentState->valueint != 2))
				{
					cJSON_DeleteItemFromObject(_ParamArry[i],"CurrentState");
					cJSON_AddItemToObject(_ParamArry[i],"CurrentState",cJSON_CreateNumber(0));
					cJSON_DeleteItemFromObject(_ParamArry[i],"CurrentTaskID");
					cJSON_AddItemToObject(_ParamArry[i],"CurrentTaskID",cJSON_CreateString(RecTaskID->valuestring));
				}
		}
	}
	else
	{
		switch(ChnNum)
		{
		case 1:
			sprintf(httpAddr,"http://%s:9696/live/comp.flv", _ip.c_str());
//			cJSON_AddItemToObject(_ParamArry[ChnNum], "OsdContent", cJSON_CreateString("合成"));
			cJSON_AddItemToObject(_ParamArry[ChnNum], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
			break;
		case 2:
			sprintf(httpAddr,"http://%s:9696/live/vl1-1.flv", _ip.c_str());//
			cJSON_AddItemToObject(_ParamArry[ChnNum], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
			break;
		case 3:
			sprintf(httpAddr,"http://%s:9696/live/vl2-1.flv", _ip.c_str());
			cJSON_AddItemToObject(_ParamArry[ChnNum], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
			break;
		case 4:
			sprintf(httpAddr,"http://%s:9696/live/vl3-1.flv", _ip.c_str());
			cJSON_AddItemToObject(_ParamArry[ChnNum], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
			break;
		case 5:
			sprintf(httpAddr,"http://%s:9696/live/vl4-1.flv", _ip.c_str());
			cJSON_AddItemToObject(_ParamArry[ChnNum], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
			break;
		case 6:
			sprintf(httpAddr,"http://%s:9696/live/vl5-1.flv", _ip.c_str());
			cJSON_AddItemToObject(_ParamArry[ChnNum], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
			break;
		case 7:
			sprintf(httpAddr,"http://%s:9696/live/vl6-1.flv", _ip.c_str());
			cJSON_AddItemToObject(_ParamArry[ChnNum], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
			break;
		case 8:
			sprintf(httpAddr,"http://%s:9696/live/vl7-1.flv", _ip.c_str());
			cJSON_AddItemToObject(_ParamArry[ChnNum], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
			break;
		case 9:
			sprintf(httpAddr,"http://%s:9696/live/vl8-1.flv", _ip.c_str());
			cJSON_AddItemToObject(_ParamArry[ChnNum], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
			break;
//		case 9:
//			sprintf(httpAddr,"http://%s:9696/live/comp.flv", _ip.c_str());
//			cJSON_AddItemToObject(_ParamArry[ChnNum], "OsdContent", cJSON_CreateString("合成"));
//			cJSON_AddItemToObject(_ParamArry[ChnNum], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
//			break;
		case 10:
			if(strlen(Params[1].c_str()) > 5)
			{
				sprintf(strosd,"远端1:%s", Params[1].c_str());
			}else{
				sprintf(strosd,"远端1");
			}
			sprintf(httpAddr,"http://%s:9696/live/vn1-1.flv", _ip.c_str());
			cJSON_AddItemToObject(_ParamArry[ChnNum], "OsdContent", cJSON_CreateString(strosd));
			cJSON_AddItemToObject(_ParamArry[ChnNum], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
			break;
		case 11:
			if(strlen(Params[2].c_str()) > 5)
			{
				sprintf(strosd,"远端2:%s", Params[2].c_str());
			}else{
				sprintf(strosd,"远端2");
			}
			sprintf(httpAddr,"http://%s:9696/live/vn2-1.flv", _ip.c_str());
			cJSON_AddItemToObject(_ParamArry[ChnNum], "OsdContent", cJSON_CreateString(strosd));
			cJSON_AddItemToObject(_ParamArry[ChnNum], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
			break;
		case 12:
			if(strlen(Params[3].c_str()) > 5)
			{
				sprintf(strosd,"远端3:%s", Params[3].c_str());
			}else{
				sprintf(strosd,"远端3");
			}
			sprintf(httpAddr,"http://%s:9696/live/vn3-1.flv", _ip.c_str());
			cJSON_AddItemToObject(_ParamArry[ChnNum], "OsdContent", cJSON_CreateString(strosd));
			cJSON_AddItemToObject(_ParamArry[ChnNum], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
			break;
		}
		cJSON *CurrentState = cJSON_GetObjectItem(_ParamArry[ChnNum],"CurrentState");
		cJSON *CurrentTaskID = cJSON_GetObjectItem(_ParamArry[ChnNum],"CurrentTaskID");
		cJSON *RecStatus  = cJSON_GetObjectItem(_ParamArry[ChnNum],GET_HOST_CONFIG_RES_CH_NCHNRECSTA);
		cJSON *RecTaskID = cJSON_GetObjectItem(_ParamArry[ChnNum],GET_HOST_CONFIG_RES_CH_NCHNRECTASKID);
		if(RecStatus->valueint == 1)
		{
			cJSON_DeleteItemFromObject(_ParamArry[ChnNum],"CurrentState");
			cJSON_AddItemToObject(_ParamArry[ChnNum],"CurrentState",cJSON_CreateNumber(1));
			cJSON_DeleteItemFromObject(_ParamArry[ChnNum],"CurrentTaskID");
			cJSON_AddItemToObject(_ParamArry[ChnNum],"CurrentTaskID",cJSON_CreateString(RecTaskID->valuestring));
		}
		
		if((RecStatus->valueint == 0)&&(CurrentState->valueint != 2))
		{
			cJSON_DeleteItemFromObject(_ParamArry[ChnNum],"CurrentState");
			cJSON_AddItemToObject(_ParamArry[ChnNum],"CurrentState",cJSON_CreateNumber(0));
			cJSON_DeleteItemFromObject(_ParamArry[ChnNum],"CurrentTaskID");
			cJSON_AddItemToObject(_ParamArry[ChnNum],"CurrentTaskID",cJSON_CreateString(RecTaskID->valuestring));
		}

	}
	
	if(ChnNum > 12)
    {
   		for(int i = 1; i <= 12; i++)
		{//用于生成节点，存储解析后的参数
			cJSON_AddItemToArray(_addressList, _ParamArry[i]);
		}
    }else{
		cJSON_AddItemToArray(_addressList, _ParamArry[ChnNum]);
	}

    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetHostChnInfo(CData &oResult, char *cResult, cJSON *status, cJSON *channal)
{
    _channal  = channal;
	_status   = status;

#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("ConvertSTRING2CJSONGetHostDecInfo\n");

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
	
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	//printf("ConvertSTRING2CJSONGetHostDecInfo Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    
	cJSON_AddItemToObject(_status, "method" ,cJSON_CreateString("sendHttpContent"));

	cJSON_AddItemToObject(_channal, GET_HOST_CONFIG_RES_RETCODE,cJSON_CreateNumber(0));
	cJSON_AddItemToObject(_channal, GET_HOST_CONFIG_RES_RETMES,cJSON_CreateString("ok"));
	cJSON_AddItemToObject(_channal, GET_HOST_CONFIG_RES_CONTENT,cJSON_CreateString(ParaBuf));
	// ConvertParaProcessing(ParaBuf, iParaLen);

    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetOsdCtrl(CData &oResult, char *cResult)
{
#if 1
    //printf("ConvertSTRING2CJSONSetOsdCtrl\n");
	/*
    for (int i = 0; i < 6; i++)
    {
		printf("%d", cResult[i]);
    }
	printf("\n");
	*/
	if (cResult[5] != 0)
	{
		printf("error:%d\n", cResult[5]);
		return false;
	}
#else 	
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	printf("ConvertSTRING2CJSONSetOsdCtrl Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
#endif 
    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetPutNoteFileCtrl(CData &oResult, char *cResult)
{
#if 1
    //printf("ConvertSTRING2CJSONSetPutNoteFileCtrl\n");
	/*
    for (int i = 0; i < 6; i++)
    {
		printf("%d", cResult[i]);
    }
	printf("\n");
	*/
	if (cResult[5] != 0)
	{
		//printf("error:%d\n", cResult[5]);
		return false;
	}
#else 	
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	printf("ConvertSTRING2CJSONSetPutNoteFileCtrl Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
#endif 
    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebMultiCtrl(CData &oResult, char *cResult)
{
#if 1
    //printf("ConvertSTRING2CJSONSetDecCtrl\n");
	/*
    for (int i = 0; i < 6; i++)
    {
		printf("%d", cResult[i]);
    }
	printf("\n");
	*/
	if (cResult[5] != 0)
	{
		printf("error:%d\n", cResult[5]);
		return false;
	}
#else 	
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	printf("ConvertSTRING2CJSONSetDecCtrl Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
#endif 
    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebAudioCtrl(CData &oResult, char *cResult)
{
#if 1
    //printf("ConvertSTRING2CJSONSetDecCtrl\n");
	/*
    for (int i = 0; i < 6; i++)
    {
		printf("%d", cResult[i]);
    }
	printf("\n");
	*/
	if (cResult[5] != 0)
	{
		printf("error:%d\n", cResult[5]);
		return false;
	}
#else 	
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	printf("ConvertSTRING2CJSONSetDecCtrl Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
#endif 
    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebExtendCtrl(CData &oResult, char *cResult)
{
#if 1
    //printf("ConvertSTRING2CJSONSetDecCtrl\n");
	/*
    for (int i = 0; i < 6; i++)
    {
		printf("%d", cResult[i]);
    }
	printf("\n");
	*/
	if (cResult[5] != 0)
	{
		printf("error:%d\n", cResult[5]);
		return false;
	}
#else 	
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	printf("ConvertSTRING2CJSONSetDecCtrl Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
#endif 
    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetDecCtrl(CData &oResult, char *cResult)
{
#if 1
    //printf("ConvertSTRING2CJSONSetDecCtrl\n");
	/*
    for (int i = 0; i < 6; i++)
    {
		printf("%d", cResult[i]);
    }
	printf("\n");
	*/
	if (cResult[5] != 0)
	{
		printf("error:%d\n", cResult[5]);
		return false;
	}
#else 	
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	printf("ConvertSTRING2CJSONSetDecCtrl Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
#endif 
    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebRecCtrl(CData &oResult, char *cResult, cJSON *cRecStatus)
{
	_cRecStatus = cRecStatus;
#if 0
    //printf("ConvertSTRING2CJSONSetRecCtrl\n");
	/*
    for (int i = 0; i < 6; i++)
    {
		printf("%d", cResult[i]);
    }
	printf("\n");
	*/
	if (cResult[5] != 0)
	{
		printf("error:%d\n", cResult[5]);
		return false;
	}
#else 	

/*
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("ConvertSTRING2CJSONSetRecCtrl\n");

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
*/
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	//printf("ConvertSTRING2CJSONSetRecCtrl Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
#endif 
    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetRecCtrl(CData &oResult, char *cResult, cJSON *cRecStatus)
{
	_cRecStatus = cRecStatus;
#if 0
    //printf("ConvertSTRING2CJSONSetRecCtrl\n");
	/*
    for (int i = 0; i < 6; i++)
    {
		printf("%d", cResult[i]);
    }
	printf("\n");
	*/
	if (cResult[5] != 0)
	{
		printf("error:%d\n", cResult[5]);
		return false;
	}
#else 	

/*
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("ConvertSTRING2CJSONSetRecCtrl\n");

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
*/
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	//printf("ConvertSTRING2CJSONSetRecCtrl Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
#endif 
    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebEffectCtrl(CData &oResult, char *cResult)
{
#if 1
    //printf("ConvertSTRING2CJSONSetPowerCtrl\n");
	/*
    for (int i = 0; i < 6; i++)
    {
		printf("%d", cResult[i]);
    }
	printf("\n");
	*/
	if (cResult[5] != 0)
	{
		printf("error:%d\n", cResult[5]);
		return false;
	}
#else 	
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	//printf("ConvertSTRING2CJSONSetPowerCtrl Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
#endif 
    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetPowerCtrl(CData &oResult, char *cResult)
{
#if 1
    //printf("ConvertSTRING2CJSONSetPowerCtrl\n");
	/*
    for (int i = 0; i < 6; i++)
    {
		printf("%d", cResult[i]);
    }
	printf("\n");
	*/
	if (cResult[5] != 0)
	{
		printf("error:%d\n", cResult[5]);
		return false;
	}
#else 	
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	//printf("ConvertSTRING2CJSONSetPowerCtrl Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
#endif 
    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetDiscipLineCtrl(CData &oResult, char *cResult)
{
#if 1
    //printf("ConvertSTRING2CJSONSetDiscipLineCtrl\n");
	/*
    for (int i = 0; i < 6; i++)
    {
		printf("%d", cResult[i]);
    }
	printf("\n");
	*/
#else 	
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	printf("ConvertSTRING2CJSONSetRecCtrl Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
#endif 
    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetDiscipLineCtrl(CData &oResult, char *cResult, cJSON *statusinfo)
{
	_statusinfo = statusinfo;
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("ConvertSTRING2CJSONSetDiscipLineCtrl\n");
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
	printf("\n");
#else 	
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	//printf("ConvertSTRING2CJSONGetDiscipLineCtrl Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
#endif 
    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetStreamCtrl(CData &oResult, char *cResult)
{
#if 1
    //printf("ConvertSTRING2CJSONSetStreamCtrl\n");
	/*
    for (int i = 0; i < 6; i++)
    {
		printf("%d", cResult[i]);
    }
	printf("\n");
	*/
	if (cResult[5] != 0)
	{
		printf("error:%d\n", cResult[5]);
		return false;
	}
#else 	
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	printf("ConvertSTRING2CJSONSetStreamCtrl Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
#endif 
    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSendConsoleCmd(CData &oResult, char *cResult)
{
#if 1
    //printf("ConvertSTRING2CJSONSendConsoleCmd\n");
	/*
    for (int i = 0; i < 6; i++)
    {
		printf("%d", cResult[i]);
    }
	printf("\n");
	*/
	if (cResult[5] != 0)
	{
		printf("error:%d\n", cResult[5]);
		return false;
	}
#else 	
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
	printf("ConvertSTRING2CJSONSendConsoleCmd Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ConvertParaProcessing(ParaBuf, iParaLen);
#endif 
    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetRecCtrl(CData &oResult, char *cResult, int GetMode, cJSON *videofiles, cJSON *recresult, int StartIndex, int EndIndex)
{
	int ret = true;
	_videofiles = videofiles;
	_recresult = recresult;
	FileStartIndex = StartIndex;
	FileEndIndex = EndIndex;
	_Param = GetMode;
#if 0
    //int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
	int  iRecvLength = (cResult[2] << 16) + (cResult[3] << 8) + cResult[4] + 4;

    printf("ConvertSTRING2CJSONGetRecCtrl\n");
	printf("ConvertSTRING2CJSONGetRecCtrl\n");
    for (int i = 0; i < 5; i++)
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
	printf("\n");
#else 	
    //int  iParaLen = (cResult[2] << 8) + cResult[3];
    //char *ParaBuf = &cResult[4];
	int  iParaLen = (cResult[2] << 16) + (cResult[3] << 8) + cResult[4] + 5 ;
    char *ParaBuf = &cResult[5];
	if(iParaLen == 24 )
	{
		char *Buff = &cResult[5];
		if(!strcmp(Buff,"The search is empty"))
		{
			mLogDebug("ConvertSTRING2CJSONGetRecCtrl cResult[5]="<<Buff<<"iParaLen"<<iParaLen);
			return ret;
		}
	}
	//printf("ConvertSTRING2CJSONGetRecCtrl Len %d bufLen %d\n", iParaLen, strlen(ParaBuf));
    ret = AnalyGetRecPara(ParaBuf, iParaLen);
#endif 
    return ret;
}



//add by libx in 2022/02/23
//--[BEGIN]--
//	pBuf 是xml格式
bool CAnalyzeParaProc::AnalyGetWebInfraredStudyList(char* pBuf, int nLen)
{
	int ret = 0, paranum = 0;
	char dealbuf[128] = { 0 };
	char* curPos = NULL;
	int  curOffsetLen = 0;
	int  offsetLen = 0;

	if (_videoParamList == NULL || _infraStudyListRes == NULL)
	{
		//printf("cJSON _videoviles or _recresult == NULL.");
		mLogError("cJSON _videoParamList or _infraStudyListRes is NULL.");
		return false;
	}

	pBuf[nLen] = '\0';

	//dummy
	cJSON_AddItemToObject(_infraStudyListRes, GET_WEB_INFRARED_STUDYLIST_RES_TOTAL, cJSON_CreateNumber(atoi("2")));
	for (int i = 0; i < 2; i++)
	{
		cJSON* arrayItem = cJSON_CreateObject();
		cJSON_AddItemToObject(arrayItem, GET_WEB_INFRARED_STUDYLIST_RES_NUM, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_INFRARED_STUDYLIST_RES_ORDER, cJSON_CreateString("power"));
		cJSON_AddItemToObject(arrayItem, GET_WEB_INFRARED_STUDYLIST_RES_DESCRIBE, cJSON_CreateString("播放"));

		cJSON_AddItemToArray(_videoParamList, arrayItem);
	}

	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebInfraredStudyList(CData& oResult, char* cResult, cJSON* result, cJSON* videoParamList)
{
	_infraStudyListRes = result;
	_videoParamList = videoParamList;

	int nRespLen = (cResult[2] << 8) + cResult[3];
	char* szRespBuf = &cResult[4];

	//进一步解析响应
#if 1 //XML
	return AnalyGetWebInfraredStudyList(szRespBuf, nRespLen);
#else	
	//内部扩展函数AnalyParaCmdParseCommandHandle
	ConvertParaProcessing(ParaBuf, iParaLen);
	return true;
#endif
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebInfraredDel(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebInfraredStudy(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebInfraredSend(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebInfraredCreate(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::AnalyGetWebAudioConf(char* pBuf, int nLen)
{
	int ret = 0, paranum = 0;
	char dealbuf[128] = { 0 };
	char* curPos = NULL;
	int  curOffsetLen = 0;
	int  offsetLen = 0;

	if (_audioParamList == NULL || _audioConfListRes == NULL)
	{
		//printf("cJSON _videoviles or _recresult == NULL.");
		mLogError("cJSON _audioParamList or _audioConfListRes is NULL.");
		return false;
	}

	pBuf[nLen] = '\0';

	//dummy
	cJSON_AddItemToObject(_audioConfListRes, GET_WEB_AUDIO_CONF_RES_TOTAL, cJSON_CreateNumber(atoi("2")));
	for (int i = 0; i < 2; i++)
	{
		cJSON* arrayItem = cJSON_CreateObject();
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_CONF_RES_ISPOWER, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_CONF_RES_ISINCENTIVES, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_CONF_RES_ISFEEDBACK, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_CONF_RES_ISAEC, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_CONF_RES_ISAGC, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_CONF_RES_ISANS, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_CONF_RES_SPL, cJSON_CreateNumber(50));

		cJSON_AddItemToArray(_audioParamList, arrayItem);
	}

	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebAudioConf(CData & oResult, char* cResult, cJSON * result, cJSON * audioParamList)
{
	_audioConfListRes = result;
	_audioParamList = audioParamList;

	int nRespLen = (cResult[2] << 8) + cResult[3];
	char* szRespBuf = &cResult[4];

	//进一步解析响应
#if 1 //XML
	return AnalyGetWebAudioConf(szRespBuf, nRespLen);
#else	
	//内部扩展函数AnalyParaCmdParseCommandHandle
	ConvertParaProcessing(ParaBuf, iParaLen);
	return true;
#endif
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebAudioConf(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::AnalyGetWebAudioGroupConf(char* pBuf, int nLen)
{
	int ret = 0, paranum = 0;
	char dealbuf[128] = { 0 };
	char* curPos = NULL;
	int  curOffsetLen = 0;
	int  offsetLen = 0;

	if (_audioGroupList == NULL || _audioGroupConfListRes == NULL)
	{
		//printf("cJSON _videoviles or _recresult == NULL.");
		mLogError("cJSON _audioGroupList or _audioGroupConfListRes is NULL.");
		return false;
	}

	pBuf[nLen] = '\0';

	//dummy
	cJSON_AddItemToObject(_audioGroupConfListRes, GET_WEB_AUDIO_GROUP_CONF_RES_TOTAL, cJSON_CreateNumber(atoi("2")));
	for (int i = 0; i < 2; i++)
	{
		cJSON* arrayItem = cJSON_CreateObject();
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_GROUP_CONF_RES_GROUPNAME, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_GROUP_CONF_RES_CHNNUM, cJSON_CreateString("1"));

		cJSON_AddItemToArray(_audioGroupList, arrayItem);
	}

	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebAudioGroupConf(CData& oResult, char* cResult, cJSON* result, cJSON* audioGroupList)
{
	_audioGroupConfListRes = result;
	_audioGroupList = audioGroupList;

	int nRespLen = (cResult[2] << 8) + cResult[3];
	char* szRespBuf = &cResult[4];

	//进一步解析响应
#if 1 //XML
	return AnalyGetWebAudioGroupConf(szRespBuf, nRespLen);
#else	
	//内部扩展函数AnalyParaCmdParseCommandHandle
	ConvertParaProcessing(ParaBuf, iParaLen);
	return true;
#endif
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebAudioGroupConf(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::AnalyGetWebAudioMatrixConf(char* pBuf, int nLen)
{
	int ret = 0, paranum = 0;
	char dealbuf[128] = { 0 };
	char* curPos = NULL;
	int  curOffsetLen = 0;
	int  offsetLen = 0;

	if (_audioMatrixList == NULL || _audioMatrixListRes == NULL)
	{
		//printf("cJSON _videoviles or _recresult == NULL.");
		mLogError("cJSON _audioMatrixList or _audioMatrixListRes is NULL.");
		return false;
	}

	pBuf[nLen] = '\0';

	//dummy
	cJSON_AddItemToObject(_audioMatrixListRes, GET_WEB_AUDIO_MATRIX_CONF_RES_TOTAL, cJSON_CreateNumber(atoi("2")));
	for (int i = 0; i < 2; i++)
	{
		cJSON* arrayItem = cJSON_CreateObject();
		//cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_MATRIX_CONF_RES_GROUPNAME, cJSON_CreateString("1"));
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_MATRIX_CONF_RES_CHNNUM, cJSON_CreateString("1"));

		cJSON_AddItemToArray(_audioMatrixList, arrayItem);
	}

	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebAudioMatrixConf(CData& oResult, char* cResult, cJSON* result, cJSON* audioMatrixList)
{
	_audioMatrixListRes = result;
	_audioMatrixList = audioMatrixList;

	int nRespLen = (cResult[2] << 8) + cResult[3];
	char* szRespBuf = &cResult[4];

	//进一步解析响应
#if 1 //XML
	return AnalyGetWebAudioMatrixConf(szRespBuf, nRespLen);
#else	
	//内部扩展函数AnalyParaCmdParseCommandHandle
	ConvertParaProcessing(ParaBuf, iParaLen);
	return true;
#endif
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebAudioMatrixConf(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::AnalyGetWebVideoParam(char* pBuf, int nLen)
{
	int ret = 0, paranum = 0;
	char dealbuf[128] = { 0 };
	char* curPos = NULL;
	int  curOffsetLen = 0;
	int  offsetLen = 0;

	if (_videoParamList == NULL || _videoParamResult == NULL)
	{
		//printf("cJSON _videoviles or _recresult == NULL.");
		mLogError("cJSON _videoParamList or _videoParamResult is NULL.");
		return false;
	}

	pBuf[nLen] = '\0';

	//dummy
	cJSON_AddItemToObject(_videoParamResult, GET_WEB_VIDEO_PARAM_RES_TOTAL, cJSON_CreateNumber(atoi("2")));
	for (int i = 0; i < 2; i++)
	{
		cJSON* arrayItem = cJSON_CreateObject();
		cJSON_AddItemToObject(arrayItem, GET_WEB_VIDEO_PARAM_RES_CHNNUM, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_VIDEO_PARAM_RES_CHNNAME, cJSON_CreateString("Hdmia"));
		cJSON_AddItemToObject(arrayItem, GET_WEB_VIDEO_PARAM_RES_RESOLUTIONSET, cJSON_CreateNumber(2));
		cJSON_AddItemToObject(arrayItem, GET_WEB_VIDEO_PARAM_RES_CODEMODE, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_VIDEO_PARAM_RES_RATETYPE, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_VIDEO_PARAM_RES_RATESIZE, cJSON_CreateNumber(4000));
		cJSON_AddItemToObject(arrayItem, GET_WEB_VIDEO_PARAM_RES_FRAMESET, cJSON_CreateNumber(30));
		cJSON_AddItemToObject(arrayItem, GET_WEB_VIDEO_PARAM_RES_GOPSET, cJSON_CreateNumber(30));

		cJSON_AddItemToArray(_videoParamList, arrayItem);
	}

	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebVideoParam(CData& oResult, char* cResult, cJSON* result, cJSON* videoParamList,cJSON* node[], string Method, string &ip, int ParamNum)
{
	_videoParamResult = result;
	_videoParamList = videoParamList;
	_ParamArry = node;
	CMethod = Method;
	int nRespLen = (cResult[2] << 8) + cResult[3];
	char* szRespBuf = &cResult[4];
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] +4;
    int leng = strlen(&cResult[4]);
    printf("ConvertSTRING2CJSONGetWebVideoParam\n");
    for (int i = 0; i < iRecvLength; i++)
    {
	    if (i < 4)
	    {
		    printf("%d\n", cResult[i]);
			mLogDebug("nRespLen:"<<nRespLen);
			mLogDebug("leng:"<<leng);
	    }
	    else 
	    {
		    printf("%c\n", cResult[i]);
			mLogDebug("STring:"<<cResult[i]);
	    }
    }
	printf("\n");
#endif	
	//进一步解析响应
#if 0 //XML
	return AnalyGetWebVideoParam(szRespBuf, nRespLen);
#else	

	//解析获得的一组通道的信息
	ConvertParaProcessing(szRespBuf, nRespLen);
	
	//cJSON_Delete(_taskList);
	return true;
#endif
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebVideoParam(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::AnalyGetWebAudioParam(char* pBuf, int nLen)
{
	int ret = 0, paranum = 0;
	char dealbuf[128] = { 0 };
	char* curPos = NULL;
	int  curOffsetLen = 0;
	int  offsetLen = 0;

	if (_audioParamList == NULL || _audioParamResult == NULL)
	{
		//printf("cJSON _videoviles or _recresult == NULL.");
		mLogError("cJSON _audioParamList or _audioParamResult is NULL.");
		return false;
	}

	pBuf[nLen] = '\0';

	//dummy
	cJSON_AddItemToObject(_audioParamResult, GET_WEB_AUDIO_PARAM_RES_TOTAL, cJSON_CreateNumber(atoi("2")));
	for (int i = 0; i < 2; i++)
	{
		cJSON* arrayItem = cJSON_CreateObject();
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_PARAM_RES_SOUNDNUM, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_PARAM_RES_SOUNDSOURCE, cJSON_CreateString("audio1"));
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_PARAM_RES_CODEMODE, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_PARAM_RES_GAIN, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_PARAM_RES_SAMPLERATE, cJSON_CreateNumber(6));
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_PARAM_RES_SOUNDCHN, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_AUDIO_PARAM_RES_RATESET, cJSON_CreateNumber(32000));

		cJSON_AddItemToArray(_audioParamList, arrayItem);
	}

	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebAudioParam(CData& oResult, char* cResult, cJSON* result, cJSON* audioParamList,cJSON* node[], string Method, string &ip, int ParamNum)
{
	_audioParamResult = result;
	_audioParamList = audioParamList;
	_ParamArry = node;
	CMethod = Method;

	int nRespLen = (cResult[2] << 8) + cResult[3];
	char* szRespBuf = &cResult[4];

	//进一步解析响应
#if 0 //XML
	return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	
	
	//解析获得的一组通道的信息
	ConvertParaProcessing(szRespBuf, nRespLen);

	return true;

#endif

}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebAudioParam(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::AnalyGetWebRtspParam(char* pBuf, int nLen)
{
	int ret = 0, paranum = 0;
	char dealbuf[128] = { 0 };
	char* curPos = NULL;
	int  curOffsetLen = 0;
	int  offsetLen = 0;

	if (_audioParamList == NULL || _rtspParamResult == NULL)
	{
		//printf("cJSON _videoviles or _recresult == NULL.");
		mLogError("cJSON _audioParamList or _rtspParamResult is NULL.");
		return false;
	}

	pBuf[nLen] = '\0';

	//dummy
	cJSON_AddItemToObject(_rtspParamResult, GET_WEB_RTSP_PARAM_RES_TOTAL, cJSON_CreateNumber(atoi("2")));
	for (int i = 0; i < 2; i++)
	{
		cJSON* arrayItem = cJSON_CreateObject();
		cJSON_AddItemToObject(arrayItem, GET_WEB_RTSP_PARAM_RES_SOUNDNUM, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_RTSP_PARAM_RES_RTSPSOURCE, cJSON_CreateString("net1"));
		cJSON_AddItemToObject(arrayItem, GET_WEB_RTSP_PARAM_RES_RTSPURL, cJSON_CreateString("rtsp://192.168.1.164/3"));

		cJSON_AddItemToArray(_audioParamList, arrayItem);
	}

	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebRtspParam(CData& oResult, char* cResult, cJSON* result, cJSON* ParamList,cJSON* node[], string Method, string &ip, int ParamNum)
{
	_rtspParamResult = result;
	_rtspParamList = ParamList;
	_ParamArry = node;
	CMethod = Method;
	int nRespLen = (cResult[2] << 8) + cResult[3];
	char* szRespBuf = &cResult[4];

	//进一步解析响应
#if 0 //XML
	return AnalyGetWebRtspParam(szRespBuf, nRespLen);
#else	

	//解析获得的一组通道的信息
	ConvertParaProcessing(szRespBuf, nRespLen);
	
	//cJSON_Delete(_taskList);
	return true;

#endif
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebRtspParam(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::AnalyGetWebRtmpParam(char* pBuf, int nLen)
{
	int ret = 0, paranum = 0;
	char dealbuf[128] = { 0 };
	char* curPos = NULL;
	int  curOffsetLen = 0;
	int  offsetLen = 0;
	char ReqBody[REQ_BODY_MAXLEN] = { 0 };
	if (_rtmpParamList == NULL || _rtmpParamResult == NULL)
	{
		//printf("cJSON _videoviles or _recresult == NULL.");
		mLogError("cJSON _rtmpParamList or _rtmpParamResult is NULL.");
		return false;
	}

	pBuf[nLen] = '\0';

	//dummy
	cJSON_AddItemToObject(_rtmpParamResult, GET_WEB_RTMP_PARAM_RES_TOTAL, cJSON_CreateNumber(atoi("13")));
	for (int i = 0; i < 13; i++)
	{
		memset(ReqBody,0,REQ_BODY_MAXLEN);
		cJSON* arrayItem = cJSON_CreateObject();
		switch(i)
		{
			case 0:
				sprintf(ReqBody, "rtmp://127.0.0.1:1935/live/%s","vl1-1");
				cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl1-1"));
				break;
			case 1:
				sprintf(ReqBody, "rtmp://127.0.0.1:1935/live/%s","vl2-1");
				cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl2-1"));
				break;
			case 2:
				sprintf(ReqBody, "rtmp://127.0.0.1:1935/live/%s","vl3-1");
				cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl3-1"));
				break;
			case 3:
				sprintf(ReqBody, "rtmp://127.0.0.1:1935/live/%s","vl4-1");
				cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl4-1"));
				break;
			case 4:
				sprintf(ReqBody, "rtmp://127.0.0.1:1935/live/%s","vl5-1");
				cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl5-1"));
				break;
			case 5:
				sprintf(ReqBody, "rtmp://127.0.0.1:1935/live/%s","vl6-1");
				cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl6-1"));
				break;
			case 6:
				sprintf(ReqBody, "rtmp://127.0.0.1:1935/live/%s","vl7-1");
				cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl7-1"));
				break;
			case 7:
				sprintf(ReqBody, "rtmp://127.0.0.1:1935/live/%s","vl8-1");
				cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vl8-1"));
				break;
			case 8:
				sprintf(ReqBody, "rtmp://127.0.0.1:1935/live/%s","comp1");
				cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("comp1"));
				break;
			case 9:
				sprintf(ReqBody, "rtmp://127.0.0.1:1935/live/%s","comp2");
				cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("comp2"));
				break;
			case 10:
				sprintf(ReqBody, "rtmp://127.0.0.1:1935/live/%s","vn1-1");
				cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vn1-1"));
				break;
			case 11:
				sprintf(ReqBody, "rtmp://127.0.0.1:1935/live/%s","vn2-1");
				cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vn2-1"));
				break;
			case 12:
				sprintf(ReqBody, "rtmp://127.0.0.1:1935/live/%s","vn3-1");
				cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("vn3-1"));
				break;
		}
		
		cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_NUM, cJSON_CreateNumber(i+1));
		//cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_RTMPNAME, cJSON_CreateString("hdmia"));
		cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_ISHTTP, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_ISHLS, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_ISRTMP, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_ISRTSP, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_ISBROADCAST, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_BROADCASTADDR, cJSON_CreateString("233.233.2.1:2000"));
		cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_ISPUSHSTREAM, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_RTMP_PARAM_RES_PUSHADDR, cJSON_CreateString(ReqBody));


		cJSON_AddItemToArray(_rtmpParamList, arrayItem);
	}

	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebRtmpParam(CData& oResult, char* cResult, cJSON* result, cJSON* ParamList,cJSON* node[], string Method, string &ip, int ParamNum)
{
	_rtmpParamResult = result;
	_rtmpParamList = ParamList;
	_ParamArry = node;
	CMethod = Method;
	

	int nRespLen = (cResult[2] << 8) + cResult[3];
	char* szRespBuf = &cResult[4];

	//进一步解析响应
#if 0 //XML
	return AnalyGetWebRtmpParam(szRespBuf, nRespLen);
#else	
	//内部扩展函数AnalyParaCmdParseCommandHandle
	ConvertParaProcessing(szRespBuf, nRespLen);
	ip = _ip;
	return true;
#endif
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebRtmpParam(CData& oResult, char* cResult)
{
	return true;
}



bool CAnalyzeParaProc::AnalyGetWebLogFile(char* pBuf, int nLen)
{
	int ret = 0, paranum = 0;
	char dealbuf[128] = { 0 };
	char* curPos = NULL;
	int  curOffsetLen = 0;
	int  offsetLen = 0;

	if (_logList == NULL || _logResult == NULL)
	{
		//printf("cJSON _videoviles or _recresult == NULL.");
		mLogError("cJSON _logList or _logResult is NULL.");
		return false;
	}

	pBuf[nLen] = '\0';

	//dummy
	cJSON_AddItemToObject(_logResult, GET_WEB_LOG_FILE_RES_TOTAL, cJSON_CreateNumber(atoi("2")));
	for (int i = 0; i < 2; i++)
	{
		cJSON* arrayItem = cJSON_CreateObject();
		cJSON_AddItemToObject(arrayItem, GET_WEB_LOG_FILE_RES_LOGNUM, cJSON_CreateNumber(1));
		cJSON_AddItemToObject(arrayItem, GET_WEB_LOG_FILE_RES_LOGNAME, cJSON_CreateString("mps.log"));
		cJSON_AddItemToObject(arrayItem, GET_WEB_LOG_FILE_RES_LOGSIZE, cJSON_CreateNumber(1024));
		cJSON_AddItemToObject(arrayItem, GET_WEB_LOG_FILE_RES_LOGTIME, cJSON_CreateString("2021-12-08 11:22:33"));
		cJSON_AddItemToObject(arrayItem, GET_WEB_LOG_FILE_RES_LOGADDR, cJSON_CreateString("ftp://192.168.1.1"));


		cJSON_AddItemToArray(_logList, arrayItem);
	}

	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebLogFile(CData& oResult, char* cResult, cJSON* result, cJSON* logList)
{
	_result = result;
	_logList = logList;

	int nRespLen = (cResult[2] << 8) + cResult[3];
	char* szRespBuf = &cResult[4];

	//进一步解析响应
#if 0 //XML
	return AnalyGetWebLogFile(szRespBuf, nRespLen);
#else	
	//内部扩展函数AnalyParaCmdParseCommandHandle
	ConvertParaProcessing(szRespBuf, nRespLen);
	return true;
#endif
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebLogFileConf(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebDevID(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebRtmpBroadConf(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebFirmUpdate(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebZKTUpdate(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebZKTDownload(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebConfigUpdateInner(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebDiscipFileUpdate(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebBackupCfgFile(CData& oResult, char* cResult)
{
	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONSetWebRecoverCFGFile(CData& oResult, char* cResult)
{
	return true;
}

//--[END]--
//add by ltz 2022-03-09
bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebUserPasswordParam(CData& oResult, char* cResult, string &Oldpassword)
{	
	int nRespLen = (cResult[2] << 8) + cResult[3];
	char* szRespBuf = &cResult[4];

	//解析获得的一组通道的信息
	ConvertParaProcessing(szRespBuf, nRespLen);
	Oldpassword = oldpassword;
	mLogDebug("##ConvertSTRING2CJSONGetWebUserPasswordParam get oldpassword=##"<<oldpassword);
	return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebVideoChnListParam(CData& oResult, char* cResult,string Method, cJSON* result, cJSON* _addressList, cJSON* node[], int chn_num)
{
		_result = result;
		_videoParamList = _addressList;
		_ParamArry = node;
		CMethod = Method;
		int nRespLen = (cResult[2] << 8) + cResult[3];
		char* szRespBuf = &cResult[4];
	
		//进一步解析响应
#if 0 //XML
		return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	

		//解析获得的一组通道的信息
		ConvertParaProcessing(szRespBuf, nRespLen);
		return true;
	
#endif

}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebDevIDParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList)
{
	_result = result;
		
	int nRespLen = (cResult[2] << 8) + cResult[3];
	char* szRespBuf = &cResult[4];
		
			//进一步解析响应
#if 0 //XML
			return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	
			//解析获得的一组通道的信息
			ConvertParaProcessing(szRespBuf, nRespLen);
			return true;
		
#endif

}
bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebUartConfListParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList, cJSON* node, int chn_num)
{
			_result = result;
			_ParamList = _addressList;
			_taskList = node;
		
			int nRespLen = (cResult[2] << 8) + cResult[3];
			char* szRespBuf = &cResult[4];
		
			//进一步解析响应
#if 0 //XML
			return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	
			//内部扩展函数AnalyParaCmdParseCommandHandle
			if(_taskList == NULL)
			{
				cJSON* _taskList = cJSON_CreateObject();
				mLogError("_taskList == NULL.");
				return false;
			} 
			//添加通道号信息，对应发送命令中的通道号
			cJSON_AddItemToObject(_taskList, SET_WEB_UART_CONF_PARAMS_UARTPORT, cJSON_CreateNumber(chn_num));
			//解析获得的一组通道的信息
			ConvertParaProcessing(szRespBuf, nRespLen);
			//将获得的一组通道信息添加到_videoParamList节点
			cJSON_AddItemToArray(_ParamList, _taskList);
			
			//cJSON_Delete(_taskList);
			return true;
		
#endif

}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebVideoOutPutConfListParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList, cJSON* node, int chn_num)
{
				_result = result;
				_ParamList = _addressList;
				_taskList = node;
			
				int nRespLen = (cResult[2] << 8) + cResult[3];
				char* szRespBuf = &cResult[4];
			     string ChnName;
				//进一步解析响应
#if 0 //XML
				return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	
				//内部扩展函数AnalyParaCmdParseCommandHandle
				if(_taskList == NULL)
				{
					cJSON* _taskList = cJSON_CreateObject();
					mLogError("_taskList == NULL.");
					return false;
				} 
			switch(chn_num)
			{
			  case 1:
				   ChnName = "VGA";
				   break;
			  case 2:
				   ChnName = "HDMI";
				   break;
			  default:
				   mLogError("please input the right Resolution!!!\n");
				   return false;
			}
				//添加通道号信息，对应发送命令中的通道号
				cJSON_AddItemToObject(_taskList, SET_WEB_VIDEO_CHN_CONF_PARAMS_CHNNAME, cJSON_CreateString(ChnName.c_str()));
				//解析获得的一组通道的信息
				ConvertParaProcessing(szRespBuf, nRespLen);
				//将获得的一组通道信息添加到_videoParamList节点
				cJSON_AddItemToArray(_ParamList, _taskList);
				
				//cJSON_Delete(_taskList);
				return true;
			
#endif

}
bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebFtpConfListParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList,string Method,string &LocallIP)
{
		_result = result;
		_ParamList = _addressList;
	    CMethod =  Method;	
			
		int nRespLen = (cResult[2] << 8) + cResult[3];
		char* szRespBuf = &cResult[4];

		//进一步解析响应
#if 0 //XML
		return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	
		//解析获得的一组通道的信息
		ConvertParaProcessing(szRespBuf, nRespLen);
		LocallIP = _ip;	
		return true;
				
#endif

}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebStorageConfListParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList)
{
		_result = result;
		_ParamList = _addressList;
					
					
		int nRespLen = (cResult[2] << 8) + cResult[3];
		char* szRespBuf = &cResult[4];
	
		//进一步解析响应
#if 0 //XML
		return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	
		//解析获得的一组通道的信息
		ConvertParaProcessing(szRespBuf, nRespLen);
	
		return true;
					
#endif

}

bool CAnalyzeParaProc::ConvertSTRING2CJSONParseSerialCmdParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList, cJSON* node)
{
	_result = result;
	_ParamList = node;
	int chn;
	char *arg[MAX_ARG_NUM] = {};

	sqlite3 *reg_db = NULL;
	char DataBuf[1024];
	char *buf;
	char *register_db = "Audio_manage.db";
	
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
	memset(DataBuf, 0, 1024);

	for (int i = 0; i < MAX_ARG_NUM; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		  mLogError("calloc is error\n");
		  return false;
		 }
	 }
	mLogDebug("cResult:"<<cResult);
	//命令格式cResult: "<9（指令类型）,2（操作类型，1:SET，2:GET）,2（输入2/输出1）,1（通道0~31）,80（值）,ok（状态）>"
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	//printf("cResult:%s\n",cResult);
	//mLogDebug("arg:"<<arg[0]<<arg[1]<<arg[2]<<arg[3]<<arg[4]<<arg[5]);
   /*********************************
	*
	*1灵敏度
	*
	*********************************/
	if(!strcmp(arg[0], "<1"))
	{	//操作类型为1,SET，若状态为OK直接返回
		if(!strcmp(arg[1], "1"))
		{
			if(!strcmp(arg[5], "ok>"))
			{
				arg[1] = "2";//设置为获取类型，以便获取操作直接读取
				sprintf(DataBuf,"%s", arg[0]);
				
				for(int i = 1; i <= 5; i++)
				{					
					buf = DataBuf + strlen(DataBuf);
					sprintf(buf,",%s", arg[i]);					
				}

				stu.id = 200;
				sprintf(stu.val,DataBuf);
				sprintf(stu.name,"Audio_SPL");
				sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
				update_stu_db(reg_db, &stu); //add 到数据库 
				mLogDebug("ConvertSTRING2CJSONParseSerialCmdParam Set SPL Success !!!\n");
				return true;
			}else{
				mLogError("ConvertSTRING2CJSONParseSerialCmdParam Set SPL failed !!!\n");
				return false;
			}
		}

		//操作类型为2,GET
		//2:输入
		if(!strcmp(arg[2], "2"))
		{	//判断是否出错
			if(!strcmp(arg[5], "error>"))
		   {
			  mLogError("ConvertSTRING2CJSONParseSerialCmdParam Get SPL failed !!!\n");;
			  return false;
		   }

		cJSON_AddItemToObject(_result, SET_WEB_AUDIO_CONF_PARAMS_SPL, cJSON_CreateNumber(atoi(arg[4])));
		}
	}
	
	/*********************************
	 *
	 *2幻象电源开关
	 *
	 *********************************/

	if(!strcmp(arg[0], "<2"))
	{	//操作类型为1,SET，若状态为OK直接返回
		if(!strcmp(arg[1], "1"))
		{
			if(!strcmp(arg[5], "ok>"))
			{
				arg[1] = "2";//设置为获取类型，以便获取操作直接读取
				sprintf(DataBuf,"%s", arg[0]);
				
				for(int i = 1; i <= 5; i++)
				{					
					buf = DataBuf + strlen(DataBuf);
					sprintf(buf,",%s", arg[i]);					
				}

				stu.id = 201;
				sprintf(stu.val,DataBuf);
				sprintf(stu.name,"Audio_IsPower");
				sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
				update_stu_db(reg_db, &stu); //add 到数据库 

				mLogDebug("ConvertSTRING2CJSONParseSerialCmdParam Set IsPower Success !!!\n");
				return true;
			}else{
				mLogError("ConvertSTRING2CJSONParseSerialCmdParam Set IsPower failed !!!\n");
				return false;

			}
		}

		//操作类型为2,GET
		//2:输入
		if(!strcmp(arg[2], "2"))
		{	//判断是否出错
			if(!strcmp(arg[5], "error>"))
		   {
			  mLogError("ConvertSTRING2CJSONParseSerialCmdParam Get IsPower failed !!!\n");
			  return false;
		   }
			cJSON_AddItemToObject(_result, SET_WEB_AUDIO_CONF_PARAMS_ISPOWER, cJSON_CreateNumber(atoi(arg[4])));
		}
	}
	/*********************************
	 *
	 *3自动增益开关
	 *
	 *********************************/

	if(!strcmp(arg[0], "<3"))
	{	//操作类型为1,SET，若状态为OK直接返回
		if(!strcmp(arg[1], "1"))
		{
			if(!strcmp(arg[5], "ok>"))
			{
					arg[1] = "2";//设置为获取类型，以便获取操作直接读取
					sprintf(DataBuf,"%s", arg[0]);
					
					for(int i = 1; i <= 5; i++)
					{					
						buf = DataBuf + strlen(DataBuf);
						sprintf(buf,",%s", arg[i]); 				
					}

					stu.id = 5;
					sprintf(stu.val,DataBuf);
					sprintf(stu.name,"Audio_IsAGC");
					sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
					update_stu_db(reg_db, &stu); //add 到数据库 

					mLogDebug("ConvertSTRING2CJSONParseSerialCmdParam Set IsAGC Success !!!\n");
					return true;
			}else{
					mLogError("ConvertSTRING2CJSONParseSerialCmdParam Set IsAGC failed !!!\n");
					return false;

			}
		}

		//操作类型为2,GET
		//2:输入
		if(!strcmp(arg[2], "2"))
		{	//判断是否出错
			if(!strcmp(arg[5], "error>"))
		   {
			  mLogError("ConvertSTRING2CJSONParseSerialCmdParam Get IsAGC failed !!!\n");
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
			 
				 mLogError("ConvertSTRING2CJSONParseSerialCmdParam arg[chn] err");
				 return false;
			 }
			cJSON_AddItemToObject(_result, SET_WEB_AUDIO_CONF_PARAMS_ISAGC, cJSON_CreateNumber(Mute));
		}
	}
	/*********************************
	 *
	 *4反馈抑制开关
	 *
	 *********************************/

	if(!strcmp(arg[0], "<4"))
	{	//操作类型为1,SET，若状态为OK直接返回
		if(!strcmp(arg[1], "1"))
		{
			if(!strcmp(arg[5], "ok>"))
			{
				arg[1] = "2";//设置为获取类型，以便获取操作直接读取
				sprintf(DataBuf,"%s", arg[0]);
				
				for(int i = 1; i <= 5; i++)
				{					
					buf = DataBuf + strlen(DataBuf);
					sprintf(buf,",%s", arg[i]);					
				}

				stu.id = 6;
				sprintf(stu.val,DataBuf);
				sprintf(stu.name,"Audio_IsFeedback");
				sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
				update_stu_db(reg_db, &stu); //add 到数据库 

				mLogDebug("ConvertSTRING2CJSONParseSerialCmdParam Set IsFeedback Success !!!\n");
				return true;
			}else{
				mLogError("ConvertSTRING2CJSONParseSerialCmdParam Set IsFeedback failed !!!\n");
				return false;

			}
		}

		//操作类型为2,GET
		//2:输入
		if(!strcmp(arg[2], "2"))
		{	//判断是否出错
			if(!strcmp(arg[5], "error>"))
		   {
			  mLogError("ConvertSTRING2CJSONParseSerialCmdParam Get IsFeedback failed !!!\n");
			  return false;
		   }
			int Mute;
			if(!strcmp(arg[4],"true"))
			 {
				 Mute = 0; 
			 
			 }
			 else if(!strcmp(arg[4],"false"))
			 {
				 Mute = 1;
			 }
			 else{
			 
				 mLogError("ConvertSTRING2CJSONParseSerialCmdParam arg[chn] err");
				 return false;
			 }			
			cJSON_AddItemToObject(_result, SET_WEB_AUDIO_CONF_PARAMS_ISFEEDBACK, cJSON_CreateNumber(Mute));
		}
	}
	/*********************************
	 *
	 *5回声消除开关
	 *
	 *********************************/

	if(!strcmp(arg[0], "<5"))
	{	//操作类型为1,SET，若状态为OK直接返回
		if(!strcmp(arg[1], "1"))
		{
			if(!strcmp(arg[5], "ok>"))
			{
				arg[1] = "2";//设置为获取类型，以便获取操作直接读取
				sprintf(DataBuf,"%s", arg[0]);
				
				for(int i = 1; i <= 5; i++)
				{					
					buf = DataBuf + strlen(DataBuf);
					sprintf(buf,",%s", arg[i]);					
				}

				stu.id = 7;
				sprintf(stu.val,DataBuf);
				sprintf(stu.name,"Audio_IsAEC");
				sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
				update_stu_db(reg_db, &stu); //add 到数据库 

				mLogDebug("ConvertSTRING2CJSONParseSerialCmdParam Set IsAEC Success !!!\n");
				return true;
			}else{
				mLogError("ConvertSTRING2CJSONParseSerialCmdParam Set IsAEC failed !!!\n");
				return false;

			}
		}

		//操作类型为2,GET
		//2:输入
		if(!strcmp(arg[2], "2"))
		{	//判断是否出错
			if(!strcmp(arg[5], "error>"))
		   {
			  mLogError("ConvertSTRING2CJSONParseSerialCmdParam Get IsAEC failed !!!\n");
			  return false;
		   }
			int Mute;
			if(!strcmp(arg[4],"true"))
			 {
				 Mute = 0; 
			 
			 }
			 else if(!strcmp(arg[4],"false"))
			 {
				 Mute = 1;
			 }
			 else{
			 
				 mLogError("ConvertSTRING2CJSONParseSerialCmdParam arg[chn] err");
				 return false;
			 }				
			cJSON_AddItemToObject(_result, SET_WEB_AUDIO_CONF_PARAMS_ISAEC, cJSON_CreateNumber(Mute));
		}
	}
	/*********************************
	 *
	 *6噪声抑制开关
	 *
	 *********************************/

	if(!strcmp(arg[0], "<6"))
	{	//操作类型为1,SET，若状态为OK直接返回
		if(!strcmp(arg[1], "1"))
		{
			if(!strcmp(arg[5], "ok>"))
			{
				arg[1] = "2";//设置为获取类型，以便获取操作直接读取
				sprintf(DataBuf,"%s", arg[0]);
				
				for(int i = 1; i <= 5; i++)
				{					
					buf = DataBuf + strlen(DataBuf);
					sprintf(buf,",%s", arg[i]);					
				}

				stu.id = 8;
				sprintf(stu.val,DataBuf);
				sprintf(stu.name,"Audio_IsANS");
				sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
				update_stu_db(reg_db, &stu); //add 到数据库 

				mLogDebug("ConvertSTRING2CJSONParseSerialCmdParam Set IsANS Success !!!\n");
				return true;
			}else{
				mLogError("ConvertSTRING2CJSONParseSerialCmdParam Set IsANS failed !!!\n");
				return false;

			}
		}

		//操作类型为2,GET
		//2:输入
		if(!strcmp(arg[2], "2"))
		{	//判断是否出错
			if(!strcmp(arg[5], "error>"))
		   {
			  mLogError("ConvertSTRING2CJSONParseSerialCmdParam Get IsANS failed !!!\n");
			  return false;
		   }
			int Mute;
			if(!strcmp(arg[4],"true"))
			 {
				 Mute = 0; 
			 
			 }
			 else if(!strcmp(arg[4],"false"))
			 {
				 Mute = 1;
			 }
			 else{
			 
				 mLogError("ConvertSTRING2CJSONParseSerialCmdParam arg[chn] err");
				 return false;
			 }				
			cJSON_AddItemToObject(_result, SET_WEB_AUDIO_CONF_PARAMS_ISANS, cJSON_CreateNumber(Mute));
		}
	}
	/*********************************
	 *
	 *7语音激励开关
	 *
	 *********************************/
#if 1
	if(!strcmp(arg[0], "<11"))
	{	//操作类型为1,SET，若状态为OK直接返回
		if(!strcmp(arg[1], "1"))
		{
			if(!strcmp(arg[5], "ok>"))
			{
				arg[1] = "2";//设置为获取类型，以便获取操作直接读取
				sprintf(DataBuf,"%s", arg[0]);
				
				for(int i = 1; i <= 5; i++)
				{					
					buf = DataBuf + strlen(DataBuf);
					sprintf(buf,",%s", arg[i]);					
				}

				stu.id = 9;
				sprintf(stu.val,DataBuf);
				sprintf(stu.name,"Audio_IsIncentives");
				sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
				update_stu_db(reg_db, &stu); //add 到数据库 

				mLogDebug("ConvertSTRING2CJSONParseSerialCmdParam Set IsIncentives Success !!!\n");
				return true;
			}else{
				mLogError("ConvertSTRING2CJSONParseSerialCmdParam Set IsIncentives failed !!!\n");
				return false;

			}
		}

		//操作类型为2,GET
		//2:输入
		if(!strcmp(arg[2], "2"))
		{	//判断是否出错
			if(!strcmp(arg[5], "error>"))
		   {
			  mLogError("ConvertSTRING2CJSONParseSerialCmdParam Get IsIncentives failed\n");
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
			 
				 mLogError("ConvertSTRING2CJSONParseSerialCmdParam arg[chn] err");
				 return false;
			 }				
			cJSON_AddItemToObject(_result, SET_WEB_AUDIO_CONF_PARAMS_ISINCENTIVES, cJSON_CreateNumber(Mute));
		}
	}
#endif

	/*********************************
	 *
	 *9:增益调节
	 *
	 *********************************/
	if(!strcmp(arg[0], "<9"))
	{    //操作类型为SET，若状态为OK直接返回
		if(!strcmp(arg[1], "1"))
		{//2:输入
			if(!strcmp(arg[2], "2"))
			{

				if(!strcmp(arg[5], "ok>"))
				{

					sprintf(stu.name,"Audio_AllDefultGain2");
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
					//修改数据库相应通道的值
					int chn = atoi(arg[3]) + 3;
					carg[chn] = arg[4];
					//36个
					sprintf(DataBuf,"%s", carg[0]);
					for(int i = 1; i <= 27; i++)
					{					
						buf = DataBuf + strlen(DataBuf);

						sprintf(buf,",%s", carg[i]);
						
					}
					mLogDebug("DataBuf = "<<DataBuf);
					//更新数据
					stu.id = 2;
					sprintf(stu.val,DataBuf);
					sprintf(stu.name,"Audio_AllDefultGain2");
					sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
					update_stu_db(reg_db, &stu); //add 到数据库 
					
					mLogDebug("ConvertSTRING2CJSONParseSerialCmdParam Set gain Success !!!\n");
					return true;
				}
				else
				{
					mLogError("ConvertSTRING2CJSONParseSerialCmdParam Set gain failed !!!\n");
					return false;
				}
			}
			else//1:输出
			{
				if(!strcmp(arg[5], "ok>"))
				{
					sprintf(stu.name,"Audio_AllDefultGain1");
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
					int chn = atoi(arg[3]) + 3;//对应通道值的位置
					//mLogDebug("arg[4]"<<arg[4]);
					carg[chn] = arg[4];
					//26个
					sprintf(DataBuf,"%s", carg[0]);
//					float Valtmp = Val;
//					float GainVal = Valtmp/10;
					for(int i = 1; i <= 23; i++)
					{					
						buf = DataBuf + strlen(DataBuf);

						sprintf(buf,",%s", carg[i]);
						
					}
					mLogDebug("DataBuf = "<<DataBuf);
					//更新数据
					stu.id = 1;
					sprintf(stu.val,DataBuf);
					sprintf(stu.name,"Audio_AllDefultGain1");
					sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
					update_stu_db(reg_db, &stu); //add 到数据库 

					mLogDebug("ConvertSTRING2CJSONParseSerialCmdParam Set gain Success !!!\n");
					return true;
				}
				else
				{
					mLogError("ConvertSTRING2CJSONParseSerialCmdParam Set gain failed !!!\n");
					return false;
				}
			}
		}
		//操作类型为GET
		//2:输入
		if(!strcmp(arg[2], "2"))
		{	//判断是否出错
			if(!strcmp(arg[5], "error>"))
		   {
			  mLogError("ConvertSTRING2CJSONParseSerialCmdParam Get gain failed !!!\n");
			  return false;
		   }
			//输入通道类型及返回值
			chn = atoi(arg[3]);
			//字符串转换成double
			char ctmp[10];
			float Valtmp;
			sprintf(ctmp,"%s",arg[4]);
			sscanf(ctmp,"%f",&Valtmp);
			
//			mLogDebug("ctmp = "<<ctmp);
//			mLogDebug("Valtmp = "<<Valtmp);

			cJSON_AddItemToObject(_result, AUDIO_CHN_CONF_PARAMS_CTRLTYPE, cJSON_CreateNumber(2));
			cJSON_AddItemToObject(_result, AUDIO_CHN_CONF_PARAMS_CHNNUM, cJSON_CreateNumber(chn));
//			cJSON_AddItemToObject(_result, AUDIO_CHN_CONF_PARAMS_VAL, cJSON_CreateNumber(atoi(arg[4])));
			cJSON_AddItemToObject(_result, AUDIO_CHN_CONF_PARAMS_VAL, cJSON_CreateNumber(Valtmp));

		} 
		else
		{  //1:输出
		   if(!strcmp(arg[5], "error>"))
		   {
			  mLogError("audio output set gain failed\n");
			  return false;
		   }
		    chn = atoi(arg[3]);
		   	//字符串转换成double
			char ctmp[10];
			float Valtmp;
			sprintf(ctmp,"%s",arg[4]);
			sscanf(ctmp,"%f",&Valtmp);
			
//			mLogDebug("ctmp = "<<ctmp);
//			mLogDebug("Valtmp = "<<Valtmp);
		    //输出通道类型及返回值
			cJSON_AddItemToObject(_result, AUDIO_CHN_CONF_PARAMS_CTRLTYPE, cJSON_CreateNumber(1));
			cJSON_AddItemToObject(_result, AUDIO_CHN_CONF_PARAMS_CHNNUM, cJSON_CreateNumber(chn));
//			cJSON_AddItemToObject(_result, AUDIO_CHN_CONF_PARAMS_VAL, cJSON_CreateNumber(atoi(arg[4])));
			cJSON_AddItemToObject(_result, AUDIO_CHN_CONF_PARAMS_VAL, cJSON_CreateNumber(Valtmp));
		}
	}

	return true;
	
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONLoginCheckParam(CData& oResult, char* cResult, string &Oldpassword, string &username)
{
	
		int nRespLen = (cResult[2] << 8) + cResult[3];
		char* szRespBuf = &cResult[4];
		
		//进一步解析响应
#if 0 //XML
		return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	
		//解析获得的一组通道的信息
		ConvertParaProcessing(szRespBuf, nRespLen);
		Oldpassword = oldpassword;	
		username = UserName;
		mLogDebug("##ConvertSTRING2CJSONLoginCheckParam get oldpassword=##"<<oldpassword);
		return true;
						
#endif

}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebRecSegModel(CData& oResult, char* cResult, cJSON* result)
{
			int nRespLen = (cResult[2] << 8) + cResult[3];
			char* szRespBuf = &cResult[4];
			_result = result;
			//进一步解析响应
#if 0 //XML
			return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	
			//解析获得的一组通道的信息
			ConvertParaProcessing(szRespBuf, nRespLen);
			return true;
							
#endif

}
bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebCaptionListParam(CData& oResult, char* cResult,string Method, cJSON* result, cJSON* _addressList, cJSON* node[], int osd_num)
{
	_result = result;
	_ParamList = _addressList;
	_ParamArry = node;
	CMethod = Method;
	int nRespLen = (cResult[2] << 8) + cResult[3];
	char* szRespBuf = &cResult[4];
			
	//进一步解析响应
#if 0 //XML
		return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	
	//内部扩展函数AnalyParaCmdParseCommandHandle

	//添加通道号信息，对应发送命令中的通道号
	//cJSON_AddItemToObject(_ParamArry, SET_WEB_EFFECT_PARAMS_VOERLAYNUM, cJSON_CreateNumber(osd_num));
	//解析获得的一组通道的信息
	ConvertParaProcessing(szRespBuf, nRespLen);
	//将获得的一组通道信息添加到_videoParamList节点
	//cJSON_AddItemToArray(_ParamList, _ParamArry);
				
	//cJSON_Delete(_taskList);
	return true;
			
#endif

}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebMosaicListParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList, cJSON* node, int masic_num)
{
		_result = result;
		_ParamList = _addressList;
		_ParamKnod = node;
				
		int nRespLen = (cResult[2] << 8) + cResult[3];
		char* szRespBuf = &cResult[4];
				
		//进一步解析响应
#if 0 //XML
			return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	
		//内部扩展函数AnalyParaCmdParseCommandHandle
		if(_ParamKnod == NULL)
		{
			cJSON* _taskList = cJSON_CreateObject();
			mLogError("_ParamKnod == NULL.");
			return false;
		} 
		//添加通道号信息，对应发送命令中的通道号
		cJSON_AddItemToObject(_ParamKnod, SET_WEB_NEW_MOSAIC_PARAMS_MOSAICNUM, cJSON_CreateNumber(masic_num));
		//解析获得的一组通道的信息
		ConvertParaProcessing(szRespBuf, nRespLen);
		//将获得的一组通道信息添加到_videoParamList节点
		cJSON_AddItemToArray(_ParamList, _ParamKnod);
					
		//cJSON_Delete(_taskList);
		return true;
				
#endif

}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebDSPMatrixParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList,cJSON* node, int OutputType)
{
			_result = result;
			_ParamList = _addressList;
			_ParamKnod = node;
					
			int nRespLen = (cResult[2] << 8) + cResult[3];
			char* szRespBuf = &cResult[4];
					
			//进一步解析响应
#if 0 //XML
				return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	
			//内部扩展函数AnalyParaCmdParseCommandHandle
			if(_ParamKnod == NULL)
			{
				cJSON* _taskList = cJSON_CreateObject();
				mLogError("_ParamKnod == NULL.");
				return false;
			} 
			cJSON_AddItemToObject(_ParamKnod, "OutputType", cJSON_CreateNumber(OutputType));
			//解析获得的一组通道的信息
			ConvertParaProcessing(szRespBuf, nRespLen);
			//将获得的一组通道信息添加到_videoParamList节点
			cJSON_AddItemToArray(_ParamList, _ParamKnod);
						
			//cJSON_Delete(_taskList);
			return true;
					
#endif

}
bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebStartAutoRecParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList)
{
				_result = result;
				_ParamList = _addressList;
				
						
				int nRespLen = (cResult[2] << 8) + cResult[3];
				char* szRespBuf = &cResult[4];
						
				//进一步解析响应
#if 0 //XML
					return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	


				//解析获得的一组通道的信息
				ConvertParaProcessing(szRespBuf, nRespLen);
				return true;
						
#endif

}
bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebStartRecParam(CData& oResult, char* cResult, string Method, cJSON* result, cJSON* _addressList,cJSON* node[], int VideoEncChnnel,string &ip)
{
				_result = result;
				_ParamList = _addressList;
				_ip = ip;
				CMethod = Method;
				//_ParamKnod = node;
			
				_ParamArry = node;	
				char httpAddr[128] = {0};
				int nRespLen = (cResult[2] << 8) + cResult[3];
				char* szRespBuf = &cResult[4];
						
				//进一步解析响应
#if 0 //XML
					return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	
				//内部扩展函数AnalyParaCmdParseCommandHandle

				//cJSON_AddItemToObject(_ParamKnod, "VideoEncChnnel", cJSON_CreateNumber(VideoEncChnnel));
				//解析获得的一组通道的信息
				ConvertParaProcessing(szRespBuf, nRespLen);
				//将获得的一组通道信息添加到_videoParamList节点
			for(int i = 1; i <= VideoEncChnnel; i++)
			{
				memset(httpAddr, 0, 128);
				switch(i)
				{
				case 1:
					sprintf(httpAddr,"http://%s:9696/live/comp.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					cJSON_AddItemToObject(_ParamArry[i], "VideoChn", cJSON_CreateString("合成画面"));
//					cJSON_AddItemToObject(_ParamArry[i], SET_WEB_EFFECT_PARAMS_VOERLAYCONTENT, cJSON_CreateString("合成"));
					cJSON_AddItemToObject(_ParamArry[i], "VideoSource", cJSON_CreateString("COMP"));
					break;
				case 2:
					sprintf(httpAddr,"http://%s:9696/live/vl1-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					cJSON_AddItemToObject(_ParamArry[i], "VideoChn", cJSON_CreateString("本地单画面1"));
					cJSON_AddItemToObject(_ParamArry[i], "VideoSource", cJSON_CreateString("卡槽1-1"));
					break;
				case 3:
					sprintf(httpAddr,"http://%s:9696/live/vl2-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					cJSON_AddItemToObject(_ParamArry[i], "VideoChn", cJSON_CreateString("本地单画面2"));
					cJSON_AddItemToObject(_ParamArry[i], "VideoSource", cJSON_CreateString("卡槽1-2"));
					break;
				case 4:
					sprintf(httpAddr,"http://%s:9696/live/vl3-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					cJSON_AddItemToObject(_ParamArry[i], "VideoChn", cJSON_CreateString("本地单画面3"));
					cJSON_AddItemToObject(_ParamArry[i], "VideoSource", cJSON_CreateString("卡槽1-3"));
					break;
				case 5:
					sprintf(httpAddr,"http://%s:9696/live/vl4-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					cJSON_AddItemToObject(_ParamArry[i], "VideoChn", cJSON_CreateString("本地单画面4"));
					cJSON_AddItemToObject(_ParamArry[i], "VideoSource", cJSON_CreateString("卡槽1-4"));
					break;
				case 6:
					sprintf(httpAddr,"http://%s:9696/live/vl5-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					cJSON_AddItemToObject(_ParamArry[i], "VideoChn", cJSON_CreateString("本地单画面5"));
					cJSON_AddItemToObject(_ParamArry[i], "VideoSource", cJSON_CreateString("卡槽2-1"));
					break;
				case 7:
					sprintf(httpAddr,"http://%s:9696/live/vl6-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					cJSON_AddItemToObject(_ParamArry[i], "VideoChn", cJSON_CreateString("本地单画面6"));
					cJSON_AddItemToObject(_ParamArry[i], "VideoSource", cJSON_CreateString("卡槽2-2"));
					break;
				case 8:
					sprintf(httpAddr,"http://%s:9696/live/vl7-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					cJSON_AddItemToObject(_ParamArry[i], "VideoChn", cJSON_CreateString("本地单画面7"));
					cJSON_AddItemToObject(_ParamArry[i], "VideoSource", cJSON_CreateString("卡槽2-3"));
					break;
				case 9:
					sprintf(httpAddr,"http://%s:9696/live/vl8-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					cJSON_AddItemToObject(_ParamArry[i], "VideoChn", cJSON_CreateString("本地单画面8"));
					cJSON_AddItemToObject(_ParamArry[i], "VideoSource", cJSON_CreateString("卡槽2-4"));
					break;
//				case 9:
//					sprintf(httpAddr,"http://%s:9696/live/comp.flv", _ip.c_str());
//					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
//					cJSON_AddItemToObject(_ParamArry[i], "VideoChn", cJSON_CreateString("合成画面"));
//					cJSON_AddItemToObject(_ParamArry[i], SET_WEB_EFFECT_PARAMS_VOERLAYCONTENT, cJSON_CreateString("合成"));
//					cJSON_AddItemToObject(_ParamArry[i], "VideoSource", cJSON_CreateString("COMP"));
//					break;
				case 10:
					sprintf(httpAddr,"http://%s:9696/live/vn1-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					cJSON_AddItemToObject(_ParamArry[i], "VideoChn", cJSON_CreateString("远端画面1"));
					cJSON_AddItemToObject(_ParamArry[i], SET_WEB_EFFECT_PARAMS_VOERLAYCONTENT, cJSON_CreateString("远端1"));
					cJSON_AddItemToObject(_ParamArry[i], "VideoSource", cJSON_CreateString("VN1-1"));
					break;
				case 11:
					sprintf(httpAddr,"http://%s:9696/live/vn2-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					cJSON_AddItemToObject(_ParamArry[i], "VideoChn", cJSON_CreateString("远端画面2"));
					cJSON_AddItemToObject(_ParamArry[i], SET_WEB_EFFECT_PARAMS_VOERLAYCONTENT, cJSON_CreateString("远端2"));
					cJSON_AddItemToObject(_ParamArry[i], "VideoSource", cJSON_CreateString("VN2-1"));					
					break;
				case 12:
					sprintf(httpAddr,"http://%s:9696/live/vn3-1.flv", _ip.c_str());
					cJSON_AddItemToObject(_ParamArry[i], GET_HOST_CONFIG_RES_CH_NCHNURL, cJSON_CreateString(httpAddr));
					cJSON_AddItemToObject(_ParamArry[i], "VideoChn", cJSON_CreateString("远端画面3"));
					cJSON_AddItemToObject(_ParamArry[i], SET_WEB_EFFECT_PARAMS_VOERLAYCONTENT, cJSON_CreateString("远端3"));
					cJSON_AddItemToObject(_ParamArry[i], "VideoSource", cJSON_CreateString("VN3-1"));
					break;
				}
			}
			
				for(int i = 1; i <= VideoEncChnnel; i++)
				{
					//mLogDebug("##ConvertSTRING2CJSONGetWebStartRecParam get i=##"<<i);
					//sprintf(ReqBody, "http://192.168.1.245:9696/live/vl1-1.flv", VideoChnnel);
					//sprintf(ReqBody, "http://%s:9696/live/vl1-1.flv", VideoChnnel);
					//cJSON_AddItemToObject(_ParamArry[i], "url", cJSON_CreateString(("")));
					cJSON_AddItemToArray(_addressList, _ParamArry[i]);	
					//_ParamArry[i] = NULL;
				}
							
				//cJSON_Delete(_taskList);
				return true;
						
#endif

}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetHostInfo(CData &oResult, char *cResult, string Method, cJSON *rtspdec, cJSON *h323dec,cJSON *sipdec, int &GopCallFlag)
{
    _rtspdec  = rtspdec;

    _rtspdec  = rtspdec;
    _h323dec  = h323dec;
	_sipdec   = sipdec;
	CMethod = Method;
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("ConvertSTRING2CJSONGetHostDecInfo\n");

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
    int  iParaLen = (cResult[2] << 8) + cResult[3];
    char *ParaBuf = &cResult[4];
    ConvertParaProcessing(ParaBuf, iParaLen);
	GopCallFlag = _Param;

    return true;
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebNewWhine(CData& oResult, char* cResult, string Method,cJSON* result, cJSON* _addressList,cJSON* node[], int ParamNumber)
{
				_result = result;
				_ParamList = _addressList;
				_ParamArry = node;
				CMethod = Method;
				int nRespLen = (cResult[2] << 8) + cResult[3];
				char* szRespBuf = &cResult[4];
						
				//进一步解析响应
#if 0 //XML
					return AnalyGetWebAudioParam(szRespBuf, nRespLen);
#else	

				//cJSON_AddItemToObject(_ParamKnod, "OutputType", cJSON_CreateNumber(OutputType));
				//解析获得的一组通道的信息
				ConvertParaProcessing(szRespBuf, nRespLen);
				//将获得的一组通道信息添加到_videoParamList节点
				for(int i = 1;i  <= ParamNumber; i++)
				{
					cJSON_AddItemToArray(_ParamList, _ParamArry[i]);
					//mLogDebug("##############"<<i);
				}
				//cJSON_Delete(_ParamArry);
				return true;

#endif
}

bool CAnalyzeParaProc::ConvertSTRING2CJSONGetWebParam(CData& oResult, char* cResult, cJSON* result, cJSON* ParamList,cJSON* node[], string Method, string &ip, int ParamNum)
{
	_result = result;
	_ParamList = ParamList;
	_ParamArry = node;
	CMethod = Method;
	int nRespLen = (cResult[2] << 8) + cResult[3];
	char* szRespBuf = &cResult[4];

	//进一步解析响应
#if 0 //XML
	return AnalyGetWebRtspParam(szRespBuf, nRespLen);
#else	

	//解析获得的一组通道的信息
	ConvertParaProcessing(szRespBuf, nRespLen);
	ip = _ip;
	//cJSON_Delete(_taskList);
	return true;

#endif
}
bool CAnalyzeParaProc::ConvertSTRING2CJSONGetParam(CData& oResult, char* cResult, cJSON* result, cJSON* ParamList,cJSON* node[], string Method, string *StrParam, int ParamNum)
{
	_result = result;
	_ParamList = ParamList;
	_ParamArry = node;
	CMethod = Method;
	int nRespLen = (cResult[2] << 8) + cResult[3];
	char* szRespBuf = &cResult[4];

	//进一步解析响应
#if 0 //XML
	return AnalyGetWebRtspParam(szRespBuf, nRespLen);
#else	

	//解析获得的一组通道的信息
	ConvertParaProcessing(szRespBuf, nRespLen);
	for(int i = 1; i <= 10; i++)
	{
		StrParam[i] = Params[i];
	}
	
	//cJSON_Delete(_taskList);
	return true;

#endif
}










