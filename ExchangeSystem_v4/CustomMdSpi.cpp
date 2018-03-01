#include <iostream>
#include <memory.h>
#include <string>
#include "CustomMdSpi.h"
using namespace std;

//��¼���
void CustomMdSpi::OnFrontConnected() {
	cout << "���ӳɹ������ڵ�¼..." << endl;
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, BROKERID);
	strcpy_s(req.UserID, USERID);
	strcpy_s(req.Password, PASSWORD);
	int ret = pUserApi->ReqUserLogin(&req, requestId++);
	if (!ret) {
		cout << "���͵�¼����ɹ���" << endl;
	}
	else {
		cout << "���͵�¼����ʧ��" << endl;
	}
};

void CustomMdSpi::OnFrontDisconnected(int nReason) {
	cout << "����ʧ�ܣ������룺" << nReason << endl;
};

///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void CustomMdSpi::OnHeartBeatWarning(int nTimeLapse) {
	cout << "��ʱ�����ϴ�����ʱ�䣺" << nTimeLapse << endl;
};


///��¼������Ӧ,��������������
void CustomMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "��¼�ɹ�" << endl;
		cout << "������:" << pRspUserLogin->TradingDay << endl;
		cout << "��¼ʱ��:" << pRspUserLogin->LoginTime << endl;
		cout << "�û���:" << pRspUserLogin->UserID << endl;
		cout << "������:" << pRspUserLogin->BrokerID << endl;

		//������������
		int ret = pUserApi->SubscribeMarketData(CONTRACTS, CONTRACTSNUM);
	}
	else {
		cout << "��¼ʧ�ܣ�������Ϣ��" << pRspInfo->ErrorMsg << endl;
	}
};

///�ǳ�������Ӧ
void CustomMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "�˻��ǳ��ɹ�" << endl;
		cout << "�����̣�" << pUserLogout->BrokerID << endl;
		cout << "�˻�����" << pUserLogout->UserID << endl;
	}
	else {
		cout << "�˻��ǳ�ʧ��" << endl;
	}
};

///����Ӧ��
void CustomMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (pRspInfo->ErrorID) {
		cout << "����Ӧ�𣬴�����Ϣ��" << pRspInfo->ErrorMsg << endl;
	}
};

///��������Ӧ��
void CustomMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "���鶩�ĳɹ�" << endl;
		cout << "��Լ���룺" << pSpecificInstrument->InstrumentID << endl;
		/*�˴��ɽ����ݴ������ݿ�*/
		/*      to do       */
	}
	else {
		cout << "���鶩��ʧ��" << endl;
	}
};

///ȡ����������Ӧ��
void CustomMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID){
		cout << "ȡ����������ɹ�" << endl;
		cout << "��Լ���룺" << pSpecificInstrument->InstrumentID << endl;
	}
	else {
		cout << "ȡ�����鶩�ĳɹ�" << endl;
	}
};

///����ѯ��Ӧ��
void CustomMdSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "ѯ�۶��ĳɹ�" << endl;
		cout << "��Լ���룺" << pSpecificInstrument->InstrumentID << endl;
	}
	else {
		cout << "ѯ�۶���ʧ��" << endl;
	}
};

///ȡ������ѯ��Ӧ��
void CustomMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "ȡ��ѯ�۶��ĳɹ�" << endl;
		cout << "��Լ���룺" << endl;
	}
	else {
		cout << "ȡ��ѯ�۶���ʧ��" << endl;
	}
};

///�������֪ͨ
void CustomMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	cout << "��ȡ�������" << endl;
	cout << "�����գ�" << pDepthMarketData->TradingDay << endl;
	cout << "���������룺" << pDepthMarketData->ExchangeID << endl;
	cout << "��Լ���룺" << pDepthMarketData->InstrumentID << endl;
	cout << "��Լ���ڽ��������룺" << pDepthMarketData->ExchangeInstID << endl;
	cout << "���¼ۣ�" << pDepthMarketData->LastPrice << endl;
	cout << "�ֲ�����" << pDepthMarketData->Volume << endl;
	/*�˴��ɽ����ݴ������ݿ�*/
	/*      to do       */
};

///ѯ��֪ͨ
void CustomMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {
	cout << "ѯ�۽��" << endl;
	///������
	cout << pForQuoteRsp->TradingDay << endl;
	///��Լ����
	cout << pForQuoteRsp->InstrumentID << endl;
	///ѯ�۱��
	cout << pForQuoteRsp->ForQuoteSysID << endl;
	///ѯ��ʱ��
	cout << pForQuoteRsp->ForQuoteTime << endl;
	///ҵ������
	cout << pForQuoteRsp->ActionDay << endl;
	///����������
	cout << pForQuoteRsp->ExchangeID << endl;
};