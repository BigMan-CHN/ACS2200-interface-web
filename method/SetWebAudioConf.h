#ifndef __SETWEBAUDIOCONF__H__
#define __SETWEBAUDIOCONF__H__

#include <string>

#include <cJSON.h>
#include <TransmitClient.h>
#include "CAnalyzeParaProc.h"

/*
	设置音频接口参数方法类
*/

class MPSClient;
class CSetWebAudioConf
{
public:
	CSetWebAudioConf(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CSetWebAudioConf();
	/**
	 * @brief SetWebAudioConf 内部调用SetWebAudioConfInner实现功能
	 * @param sResult
	 * @param isPower
	 * @param isIncentives
	 * @param isFeedBack
	 * @param isAEC
	 * @param isAGC
	 * @param spl
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode SetWebAudioConf(std::string& sResult,
		int isPower,
		int IsANS,
		int isIncentives,
		int isFeedBack,
		int isAEC,
		int isAGC,
		int spl);
	InterfaceResCode GetWebAudioSPL(std::string& sResult);
	InterfaceResCode GetWebAudioPower(std::string& sResult);
	InterfaceResCode SetWebAudioSPL(std::string& sResult,int ChnNum,int SPL);
	InterfaceResCode SetWebAudioPower(std::string& sResult,int ChnNum,int IsPower);
	InterfaceResCode SetWebAudioMotVal(std::string& sResult, int ChnNum, int MotVal);
	InterfaceResCode SetWebAudioMotTime(std::string& sResult,int ChnNum,int MotTime);
	InterfaceResCode SetWebAudioFeedback(std::string& sResult,int ChnNum,int IsFeedback);
	InterfaceResCode SetWebAudioAEC(std::string& sResult,int ChnNum,int IsAEC);
	InterfaceResCode SetWebAudioANS(std::string& sResult, int ChnNum, int IsANS);
	InterfaceResCode SetWebAudioAGC(std::string& sResult,int ChnNum,int IsAGC);
	InterfaceResCode SetWebAudioIncentives(std::string& sResult,int ChnNum,int IsIncentives);
	InterfaceResCode GetWebAudioBoardType(std::string& sResult);

private:
	/**
	 * @brief SetWebAudioConfInner 构造、发送业务请求并获取响应
	 * @param isPower
	 * @param isIncentives
	 * @param isFeedBack
	 * @param isAEC
	 * @param isAGC
	 * @param spl
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool SetWebAudioConfInner(int cmd,int val,int isPower,int IsANS,
		int isIncentives,
		int isFeedBack,
		int isAEC,
		int isAGC,
		int spl,
		CData& oResult,
		char* cResult);
	bool GetWebAudioSPLInner(CData& oResult, char* cResult, int ChnNum);
	bool GetWebAudioPowerInner(CData& oResult, char* cResult, int ChnNum);
	bool SetWebAudioSPLInner(int ChnNum, int SPL, CData& oResult, char* cResult);
	bool SetWebAudioPowerInner(int ChnNum, int IsPower, CData& oResult, char* cResult);
	bool GetWebAudioMotivationValInner(CData& oResult, char* cResult, int ChnNum);
	bool GetWebAudioMotivationTimeInner(CData& oResult, char* cResult, int ChnNum);
	bool SetWebAudioMotValInner(int ChnNum, int MotVal, CData& oResult, char* cResult);
	bool SetWebAudioMotTimeInner(int ChnNum, int MotTime, CData& oResult, char* cResult);
	bool GetWebAudioIncentivesInner(CData& oResult, char* cResult, int ChnNum);
	bool GetWebAudioAGCInner(CData& oResult, char* cResult, int ChnNum);
	bool SetWebAudioFeedbackInner(int ChnNum, int IsFeedback, CData& oResult, char* cResult);
	bool SetWebAudioAECInner(int ChnNum, int IsAEC, CData& oResult, char* cResult);
	bool SetWebAudioANSInner(int ChnNum, int IsANS, CData& oResult, char* cResult);
	bool SetWebAudioAGCInner(int ChnNum, int IsAGC, CData& oResult, char* cResult);
	bool SetWebAudioIncentivesInner(int ChnNum, int IsIncentives, CData& oResult, char* cResult);
	bool GetWebAudioBoardTypeInner(CData& oResult, char* cResult);


	bool ComposeResult(cJSON* result);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();

	MPSClient* _mpsClient;
	CAnalyzeParaProc _analyzePara;
	cJSON* _addressList;
	cJSON* _param[20];
	cJSON* _result;
};


#endif //__SETWEBAUDIOCONF__H__
