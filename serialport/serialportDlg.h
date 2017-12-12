
// serialportDlg.h : 头文件
//

#pragma once
#include"stdafx.h" //是一个预处理文件，有这个文件那么程序将自动调用#include<iostream>和<stdlib.h>
#include "afxwin.h"

#define MAXBUFSIZE 1024

/************用于串口通信的类************/
class ConSt
{
public:
	int state;//!当前状态
	int time; //!周期个数

	//! 未来考虑循环消息队列
	char bufS[MAXBUFSIZE];
	DWORD iSizeS;		//! 发送数据区

	char bufR[MAXBUFSIZE];
	DWORD iSizeR;		//! 收到的数据

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
	void setupdcb();//设置DCB
	void reconnect();
};



// CserialportDlg 对话框
class CserialportDlg : public CDialogEx
{
// 构造
public:
	CserialportDlg(CWnd* pParent = NULL);	// 标准构造函数

	bool m_bClose;
	bool m_bStart;

	CString FileName;

	void period();

// 对话框数据
	enum { IDD = IDD_SERIALPORT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	long lPeriod;
	CString strCom;
	bool m_bRun;
	//显示框
	WINDOWPLACEMENT m_wp;
	void OutPutWindow(CString strOutput);
	void OutPutWindowC(CString strOutput);
public:
	//按键
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void EnableChoose();
	afx_msg void UnableChoose();


public:
	// 生成的消息映射函数
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


