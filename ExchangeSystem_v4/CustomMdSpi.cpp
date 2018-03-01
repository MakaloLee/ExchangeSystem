#include <iostream>
#include <memory.h>
#include <string>
#include "CustomMdSpi.h"
using namespace std;

//登录检测
void CustomMdSpi::OnFrontConnected() {
	cout << "连接成功，正在登录..." << endl;
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, BROKERID);
	strcpy_s(req.UserID, USERID);
	strcpy_s(req.Password, PASSWORD);
	int ret = pUserApi->ReqUserLogin(&req, requestId++);
	if (!ret) {
		cout << "发送登录请求成功！" << endl;
	}
	else {
		cout << "发送登录请求失败" << endl;
	}
};

void CustomMdSpi::OnFrontDisconnected(int nReason) {
	cout << "连接失败，错误码：" << nReason << endl;
};

///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void CustomMdSpi::OnHeartBeatWarning(int nTimeLapse) {
	cout << "超时，距上次连接时间：" << nTimeLapse << endl;
};


///登录请求响应,并发送行情请求
void CustomMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "登录成功" << endl;
		cout << "交易日:" << pRspUserLogin->TradingDay << endl;
		cout << "登录时间:" << pRspUserLogin->LoginTime << endl;
		cout << "用户名:" << pRspUserLogin->UserID << endl;
		cout << "经济商:" << pRspUserLogin->BrokerID << endl;

		//发送行情请求
		int ret = pUserApi->SubscribeMarketData(CONTRACTS, CONTRACTSNUM);
	}
	else {
		cout << "登录失败，错误信息：" << pRspInfo->ErrorMsg << endl;
	}
};

///登出请求响应
void CustomMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "账户登出成功" << endl;
		cout << "经纪商：" << pUserLogout->BrokerID << endl;
		cout << "账户名：" << pUserLogout->UserID << endl;
	}
	else {
		cout << "账户登出失败" << endl;
	}
};

///错误应答
void CustomMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (pRspInfo->ErrorID) {
		cout << "错误应答，错误信息：" << pRspInfo->ErrorMsg << endl;
	}
};

///订阅行情应答
void CustomMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "行情订阅成功" << endl;
		cout << "合约代码：" << pSpecificInstrument->InstrumentID << endl;
		/*此处可将数据存入数据库*/
		/*      to do       */
	}
	else {
		cout << "行情订阅失败" << endl;
	}
};

///取消订阅行情应答
void CustomMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID){
		cout << "取消订阅行情成功" << endl;
		cout << "合约代码：" << pSpecificInstrument->InstrumentID << endl;
	}
	else {
		cout << "取消行情订阅成功" << endl;
	}
};

///订阅询价应答
void CustomMdSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "询价订阅成功" << endl;
		cout << "合约代码：" << pSpecificInstrument->InstrumentID << endl;
	}
	else {
		cout << "询价订阅失败" << endl;
	}
};

///取消订阅询价应答
void CustomMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "取消询价订阅成功" << endl;
		cout << "合约代码：" << endl;
	}
	else {
		cout << "取消询价订阅失败" << endl;
	}
};

///深度行情通知
void CustomMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	cout << "获取深度行情" << endl;
	cout << "交易日：" << pDepthMarketData->TradingDay << endl;
	cout << "交易所代码：" << pDepthMarketData->ExchangeID << endl;
	cout << "合约代码：" << pDepthMarketData->InstrumentID << endl;
	cout << "合约所在交易所代码：" << pDepthMarketData->ExchangeInstID << endl;
	cout << "最新价：" << pDepthMarketData->LastPrice << endl;
	cout << "持仓量：" << pDepthMarketData->Volume << endl;
	/*此处可将数据存入数据库*/
	/*      to do       */
};

///询价通知
void CustomMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {
	cout << "询价结果" << endl;
	///交易日
	cout << pForQuoteRsp->TradingDay << endl;
	///合约代码
	cout << pForQuoteRsp->InstrumentID << endl;
	///询价编号
	cout << pForQuoteRsp->ForQuoteSysID << endl;
	///询价时间
	cout << pForQuoteRsp->ForQuoteTime << endl;
	///业务日期
	cout << pForQuoteRsp->ActionDay << endl;
	///交易所代码
	cout << pForQuoteRsp->ExchangeID << endl;
};