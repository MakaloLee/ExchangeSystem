#include <iostream>
#include <string>
#include <memory.h>
#include "CustomTradeSpi.h"
using namespace std;

extern TThostFtdcBrokerIDType BROKERID;//�����̴���
extern TThostFtdcInvestorIDType USERID;//Ͷ�����˺�
extern TThostFtdcPasswordType PASSWORD;//Ͷ��������
extern CThostFtdcTraderApi *g_pTradeUserApi;//���׽ӿ�ָ��
extern TThostFtdcInstrumentIDType g_pTradeInstrumentID;//�����׵ĺ�Լ����
extern int requestId;

TThostFtdcFrontIDType trade_front_id;//ǰ�ñ��
TThostFtdcSessionIDType session_id;//�Ự���
TThostFtdcOrderRefType max_order_ref;//�������

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CustomTradeSpi::OnFrontConnected() {
	cout << "�������ӳɹ������ڵ�¼..." << endl;
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, BROKERID);
	strcpy_s(req.UserID, USERID);
	strcpy_s(req.Password, PASSWORD);
	int ret = g_pTradeUserApi->ReqUserLogin(&req, requestId);
	if (!ret) {
		cout << "���͵�¼����ɹ���" << endl;
	}
	else {
		cout << "���͵�¼����ʧ��" << endl;
	}
};

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void CustomTradeSpi::OnFrontDisconnected(int nReason) {
	cout << "����ʧ�ܣ������룺" << nReason << endl;
};

///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void CustomTradeSpi::OnHeartBeatWarning(int nTimeLapse) {
	cout << "����������ʱ�����ϴ�����ʱ�䣺" << nTimeLapse << endl;
};

///��¼������Ӧ
void CustomTradeSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		trade_lock = true;
		cout << "��½�ɹ�" << endl;
		cout << "������:" << pRspUserLogin->TradingDay << endl;
		cout << "��¼ʱ��:" << pRspUserLogin->LoginTime << endl;
		cout << "�û���:" << pRspUserLogin->UserID << endl;
		cout << "������:" << pRspUserLogin->BrokerID << endl;

		//����Ự��Ϣ
		trade_front_id = pRspUserLogin->FrontID;
		session_id = pRspUserLogin->SessionID;
		strcpy_s(max_order_ref, pRspUserLogin->MaxOrderRef);

		//����Ͷ���߽�����ȷ������
		CThostFtdcSettlementInfoConfirmField settlementConfirmReq;
		memset(&settlementConfirmReq, 0, sizeof(settlementConfirmReq));
		strcpy_s(settlementConfirmReq.BrokerID, BROKERID);
		strcpy_s(settlementConfirmReq.InvestorID, USERID);
		static int requestID = 0;
		int ret = g_pTradeUserApi->ReqSettlementInfoConfirm(&settlementConfirmReq, requestID);
		if (!ret) {
			cout << "����Ͷ���߽�����ȷ������ɹ�" << endl;
		}
		else {
			cout << "����Ͷ���߽�����ȷ������ʧ��" << endl;
		}
	}
};

///�ǳ�������Ӧ
void CustomTradeSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		trade_lock = false;
		cout << "�˻��ǳ��ɹ�" << endl;
		cout << "�����̣�" << pUserLogout->BrokerID << endl;
		cout << "�˻�����" << pUserLogout->UserID << endl;
	}
	else {
		cout << "�ǳ�ʧ��" << endl;
	}
};

///����¼��������Ӧ
void CustomTradeSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "����¼��ɹ�" << endl;
		cout << "��Լ���룺" << pInputOrder->InstrumentID << endl;
		cout << "�۸�" << pInputOrder->LimitPrice << endl;
		cout << "������" << pInputOrder->VolumeTotalOriginal << endl;
		cout << "���ַ���" << pInputOrder->Direction << endl;
	}
};

///��������������Ӧ
void CustomTradeSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "���������ɹ�" << endl;
		cout << "��Լ���룺" << pInputOrderAction->InstrumentID << endl;
		cout << "������־��" << pInputOrderAction->ActionFlag << endl;
	}
	else {
		cout << "��������ʧ��" << endl;
	}
};

///Ͷ���߽�����ȷ����Ӧ
void CustomTradeSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "����������ɹ�" << endl;
		///���͹�˾����
		cout << "�����̣�" << pSettlementInfoConfirm->BrokerID << endl;
		///Ͷ���ߴ���
		cout << "�˻�����" << pSettlementInfoConfirm->InvestorID << endl;
		///ȷ������
		cout << "ȷ�����ڣ�" << pSettlementInfoConfirm->ConfirmDate << endl;
		///ȷ��ʱ��
		cout << "ȷ��ʱ�䣺" << pSettlementInfoConfirm->ConfirmTime << endl;

		//���Ͳ�ѯ��Լ����
		CThostFtdcQryInstrumentField instrumentReq;
		memset(&instrumentReq, 0, sizeof(instrumentReq));
		strcpy_s(instrumentReq.InstrumentID, g_pTradeInstrumentID);
		static int requestID = 0;
		int ret = g_pTradeUserApi->ReqQryInstrument(&instrumentReq, requestID);
		if (!ret) {
			cout << "���ͺ�Լ��ѯ����ɹ�" << endl;
		}
		else {
			cout << "���ͺ�Լ��ѯ����ʧ��" << endl;
		}
	}
};

///�����ѯͶ���ֲ߳���Ӧ
void CustomTradeSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "��ѯͶ���ֲֳ߳ɹ�" << endl;
		if (pInvestorPosition != NULL) {
			cout << "��Լ���룺" << pInvestorPosition->InstrumentID << endl;
			cout << "���ּ۸�" << pInvestorPosition->OpenAmount << endl;
			cout << "��������" << pInvestorPosition->OpenVolume << endl;
			cout << "���ַ���" << pInvestorPosition->PosiDirection << endl;
			cout << "ռ�ñ�֤��" << pInvestorPosition->UseMargin << endl;

			//�µ�����
			if (trade_lock) {
				//to do
			}

			//���Խ���
			if (trade_lock) {
				//to do
			}
			
		}
		else {
			cout << "�ú�Լδ�ֲ�" << endl;
		}
	}
	else {
		cout << "��ѯͶ���ֲ߳�ʧ��" << endl;
	}
};

///�����ѯ�ʽ��˻���Ӧ
void CustomTradeSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "��ѯͶ�����ʽ��˻��ɹ�" << endl;
		cout << "�˻�����" << pTradingAccount->AccountID << endl;
		cout << "�����ʽ�" << pTradingAccount->Available << endl;
		cout << "��ȡ�ʽ�" << pTradingAccount->WithdrawQuota << endl;
		cout << "��ǰ��֤��" << pTradingAccount->CurrMargin << endl;
		cout << "ƽ��ӯ����" << pTradingAccount->CloseProfit << endl;

		//�����ѯͶ���ֲ߳�
		CThostFtdcQryInvestorPositionField positionReq;
		memset(&positionReq, 0, sizeof(positionReq));
		strcpy_s(positionReq.BrokerID, BROKERID);
		strcpy_s(positionReq.InvestorID, USERID);
		strcpy_s(positionReq.InstrumentID, g_pTradeInstrumentID);
		static int requestID = 0;
		int ret = g_pTradeUserApi->ReqQryInvestorPosition(&positionReq, requestID);
		if (!ret) {
			cout << "����Ͷ���ֲֲ߳�ѯ�ɹ�" << endl;
		}
		else {
			cout << "����Ͷ���ֲֲ߳�ѯʧ��" << endl;
		}
	}
};

///�����ѯ��Լ��Ӧ
void CustomTradeSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!pRspInfo->ErrorID) {
		cout << "��ѯ��Լ�ɹ�" << endl;
		cout << "���������룺" << pInstrument->ExchangeID << endl;
		cout << "��Լ���룺" << pInstrument->InstrumentID << endl;
		cout << "��Լ���ڽ��������룺" << pInstrument->ExchangeInstID << endl;
		cout << "ִ�мۣ�" << pInstrument->StrikePrice << endl;
		cout << "�����գ�" << pInstrument->EndDelivDate << endl;
		cout << "��ǰ����״̬��" << pInstrument->IsTrading << endl;

		//�����ѯͶ�����˻�
		CThostFtdcQryTradingAccountField tradingAccountReq;
		memset(&tradingAccountReq, 0, sizeof(tradingAccountReq));
		strcpy_s(tradingAccountReq.BrokerID, BROKERID);
		strcpy_s(tradingAccountReq.InvestorID, USERID);
		static int requestID = 0;
		int ret = g_pTradeUserApi->ReqQryTradingAccount(&tradingAccountReq, requestID);
		if (!ret) {
			cout << "�����ʽ��˻���ѯ�ɹ�" << endl;
		}
		else {
			cout << "�����ʽ��˻���ѯʧ��" << endl;
		}
	}
	else {
		cout << "��ѯ��Լʧ��" << endl;
	}
};

///����֪ͨ
void CustomTradeSpi::OnRtnOrder(CThostFtdcOrderField *pOrder) {
	char status[10];
	sprintf_s(status, "%d", pOrder->OrderSubmitStatus);
	int orderState = atoi(status) - 48;//0���ύ��3�ѽ���
	cout << "�յ�����Ӧ��" << endl;

	if ((pOrder->SessionID == session_id) && (pOrder->FrontID == trade_front_id) && (strcmp(pOrder->OrderRef, max_order_ref) == 0)) {
		if ((pOrder->OrderStatus != THOST_FTDC_OST_AllTraded) && (pOrder->OrderStatus != THOST_FTDC_OST_Canceled) && (pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing)){
			cout << "�ȴ��ɽ���" << endl;
		}
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled){
			cout << "�����ɹ�" << endl;
		}
	}
};

///�ɽ�֪ͨ
void CustomTradeSpi::OnRtnTrade(CThostFtdcTradeField *pTrade) {
	cout << "�����ɹ��ɽ�" << endl;
	cout << "�ɽ�ʱ�䣺" << pTrade->TradeTime << endl;
	cout << "��Լ���룺" << pTrade->InstrumentID << endl;
	cout << "�ɽ��۸�" << pTrade->Price << endl;
	cout << "�ɽ�����" << pTrade->Volume << endl;
	cout << "��ƽ�ַ���" << pTrade->Direction << endl;
};
