#include "cgiAnalysis.h"

CgiAnalysis::CgiAnalysis()
    :_inputBuffer(NULL)
    ,_maxBufferSize(MAX_BUFFER_SIZE)
    ,_clientIp("")
{
    
}

char *CgiAnalysis:: GetCgiData()
{
    try{
        char *req_method = getenv("REQUEST_METHOD");
        return GetCgiData(stdin, req_method);
    }catch(exception& e){
        mLogError("Failed to try:"<<e.what());
        return NULL;
    }
}

char *CgiAnalysis::GetCgiData(FILE *fp, char *requestMethod)
{
    if (!strcmp(requestMethod, "GET")){
        mLogDebug("requestMethod == GET");
        return GetCgiData(fp,CgiAnalysis::eRequestGet);
    }
    else if (!strcmp(requestMethod, "POST")){
        mLogDebug("requestMethod == POST");
        return GetCgiData(fp,CgiAnalysis::eRequestPost);
    }else{
        mLogWarn("请求数据方式未知! requestMethod != GET && requestMethod != POST!requestMethod="<<requestMethod);
    }
    return NULL;
}

char *CgiAnalysis::GetCgiData(FILE *fp, CgiAnalysis::RequestMethod method)
{
    int len;
    int maxSize = _maxBufferSize;
    int i = 0;

    if (method == CgiAnalysis::eRequestGet){
        _inputBuffer = getenv("QUERY_STRING");
        return _inputBuffer;
    }
    else if (method == CgiAnalysis::eRequestPost){
        len = atoi(getenv("CONTENT_LENGTH"));
        if(maxSize > len){
            //申请长度+1，用于存储截止符'\0'
            maxSize = len+1;
        }else{
            mLogWarn("maxSize:"<<maxSize<<" <= len:"<<len<<",post发送数据太长，无法全部接收");
        }

        _inputBuffer = (char*)malloc(sizeof(char)*(maxSize));

        if(_inputBuffer == NULL){
            mLogError("申请内存失败，大小为 "<<maxSize);
            return NULL;
        }

        //接收到的数据长度为0
        if(len == 0){
            _inputBuffer[0] = '\0';
            return _inputBuffer;
        }

        while(1){
            //获取字符
            _inputBuffer[i] = (char)fgetc(fp);

            //接收长度达到最大存储空间
            if(i == (maxSize-1)){
                _inputBuffer[i+1] = '\0';
                return _inputBuffer;
            }

            --len;
            //已达到文件截止符 或者 接收到指定长度数据
            if(feof(fp) || (!(len))){
                i++;
                _inputBuffer[i] = '\0';
                return _inputBuffer;
            }
            i++;
        }
    }else{
        mLogWarn("请求数据方式未知! RequestMethod != GET && RequestMethod != POST!RequestMethod="<<method);
    }
    return NULL;
}

void CgiAnalysis::clearCgiData()
{
    if(_inputBuffer != NULL){
        free(_inputBuffer);
        _inputBuffer = NULL;
    }
}

string CgiAnalysis::getClientIp()
{
    string ip = "";
    try{
        char * cip = getenv("REMOTE_ADDR");
        ip = (string)(cip==NULL?"":cip)+"\0";
    }catch(exception& e){
        mLogError("获取ip失败:"<<e.what());
        return ip;
    }
    _clientIp = ip;
    return _clientIp;

    /*
    mLogInfo("REMOTE_ADDR:"<<getenv("REMOTE_ADDR"));
    mLogInfo("REMOTE_HOST:"<<getenv("REMOTE_HOST"));
    mLogInfo("ACCEPT:"<<getenv("ACCEPT"));
    mLogInfo("ACCEPT_ENCODING:"<<getenv("ACCEPT_ENCODING"));
    mLogInfo("ACCEPT_LANGUAGE:"<<getenv("ACCEPT_LANGUAGE"));
    mLogInfo("AUTORIZATION:"<<getenv("AUTORIZATION"));
    mLogInfo("FORM:"<<getenv("FORM"));
    mLogInfo("IF_MODIFIED_SINGCE:"<<getenv("IF_MODIFIED_SINGCE"));
    mLogInfo("PRAGMA:"<<getenv("PRAGMA"));
    mLogInfo("REFFERER:"<<getenv("REFFERER"));
    mLogInfo("USER_AGENT:"<<getenv("USER_AGENT"));
    //*/
}

