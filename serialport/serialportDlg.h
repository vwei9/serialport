
// serialportDlg.h : ͷ�ļ�
//

#pragma once
#include"stdafx.h" //��һ��Ԥ�����ļ���������ļ���ô�����Զ�����#include<iostream>��<stdlib.h>
#include "afxwin.h"

#define MAXBUFSIZE 1024

/************���ڴ���ͨ�ŵ���************/
class ConSt
{
public:
	int state;//!��ǰ״̬
	int time; //!���ڸ���

	//! δ������ѭ����Ϣ����
	char bufS[MAXBUFSIZE];
	DWORD iSizeS;		//! ����������

	char bufR[MAXBUFSIZE];
	DWORD iSizeR;		//! �յ�������

	int baud;
	int stopBits;
	int parity;
	int byteSize;

	CString port;

	HANDLE hComm;
	long lPeriod;

public:
	void Read();
	void Send();
	void Close();
	
	void AddString();

private:
	void setupdcb();//����DCB
	void reconnect();
};



// CserialportDlg �Ի���
class CserialportDlg : public CDialogEx
{
// ����
public:
	CserialportDlg(CWnd* pParent = NULL);	// ��׼���캯��

	bool m_bClose;
	bool m_bStart;

	CString FileName;

	void period();

// �Ի�������
	enum { IDD = IDD_SERIALPORT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	long lPeriod;
	CString strCom;
	bool m_bRun;
	//��ʾ��
	WINDOWPLACEMENT m_wp;
	void OutPutWindow(CString strOutput);
	void OutPutWindowC(CString strOutput);
public:
	//����
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void EnableChoose();
	afx_msg void UnableChoose();


public:
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void readCFG();
	void InitPort();

	void DisplayS();
	void DisplayR();
	void SaveR();
	void AnalyData();

	ConSt m_con;

	HANDLE m_Thread;
	DWORD m_dwThreadId;

public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnClose();
	CEdit m_Info;
	CEdit m_Circ;

	char read[9];
	int iReadSize;

	CString mCom;
	int mBaud;
	int mParity;
	int mByte;
	int mStop;
	
	afx_msg void choosedata();

	afx_msg void OnEnChangeEditInfo();
	afx_msg void OnEnChangeEditCircle();
//	afx_msg void OnCbnSelchangeCom();
	afx_msg void OnCbnSelchangeCom2();
	CComboBox m_com;
	CComboBox m_com2;
	CComboBox m_com3;
	CComboBox m_com4;
	CComboBox m_com5;
	/*afx_msg void OnCbnSelchangeCom();*/
};


