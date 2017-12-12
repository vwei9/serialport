
// serialportDlg.cpp : ʵ���ļ�
//
#include<iostream>
#include "stdafx.h"
#include "serialport.h"
#include "serialportDlg.h"
#include "afxdialogex.h"
#include "afxwin.h"
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



void ConSt::Read()
{
	if (time == 50 || hComm == NULL)  //�ж������Ӻ������봮�ڽ�������
	{
		Close();
		state = 0;
		time = 0;
		reconnect();
	}

	if (hComm)  //!  ������
	{
		DWORD irev = 0;
		do
		{
			irev = 0;
			ReadFile(hComm, bufR + iSizeR, MAXBUFSIZE - iSizeR, &irev, NULL);
			iSizeR += irev;
		} while (irev);

		if (iSizeR > 0)
		{
			state = 1;
			time = 0;
			return;
		}
		else
		{
			iSizeR = 0;
			time++;
		}
	}
}

void ConSt::reconnect()
{
	time = 0;
	hComm = CreateFile(port.GetBuffer(),        //�ļ����������Ǵ��ں�  
		GENERIC_READ | GENERIC_WRITE,			//�������д  
		0,										//��ռ��ʽ  
		NULL,
		OPEN_EXISTING,							//�򿪶����Ǵ���  
		0, //! ͬ��  
		NULL);

	if (hComm == INVALID_HANDLE_VALUE)       //�򿪴���ʧ��
	{
		CString str_error;
		str_error.Format(_T("open serialport failed"));
		::AfxMessageBox(str_error);
		hComm = NULL;
		return;
	}
	
	if (!SetupComm(hComm, MAXBUFSIZE, MAXBUFSIZE))
		::AfxMessageBox(_T("setuocomm error"));//���뻺����������������Ĵ�С����1024

	COMMTIMEOUTS TimeOuts;
	TimeOuts.ReadIntervalTimeout = 0;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 100;
	TimeOuts.WriteTotalTimeoutMultiplier = 5;
	TimeOuts.WriteTotalTimeoutConstant = 10;
	if (!SetCommTimeouts(hComm, &TimeOuts))
		::AfxMessageBox(_T("set comm timeout error"));//���ó�ʱ

	DCB dcb;
	GetCommState(hComm, &dcb);
	dcb.BaudRate = baud;
	dcb.ByteSize = byteSize;
	dcb.Parity = parity;
	dcb.StopBits = stopBits;

	if (!SetCommState(hComm, &dcb))
	{
		DWORD last_error = GetLastError();
		CString str_error;
		str_error.Format(_T("set comm state error: %d\n"), last_error);
		::AfxMessageBox(str_error);
	}
	if (!PurgeComm(hComm, PURGE_TXCLEAR | PURGE_RXCLEAR))  ::AfxMessageBox(_T("purge comm error"));
}

void ConSt::Send()
{
	if (hComm) //!	д����
	{
		DWORD ires = 0;
		bool flag = WriteFile(hComm, bufS, iSizeS, &ires, NULL);
		iSizeS = 0;
	}

}

void ConSt::Close()
{
	if (hComm)
		CloseHandle(hComm);
}

DWORD WINAPI ThreadFunc(void* Thread)
{
	CserialportDlg *dlg = (CserialportDlg*)Thread;
	while (!dlg->m_bClose)
	{
		dlg->period();
	}
	return 0;
}


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CserialportDlg �Ի���


CserialportDlg::CserialportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CserialportDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bRun = true;
}

void CserialportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INFO, m_Info);
	DDX_Control(pDX, IDC_EDIT_CIRC, m_Circ);
	DDX_Control(pDX, IDC_COM, m_com);
	DDX_Control(pDX, IDC_COM2, m_com2);
	DDX_Control(pDX, IDC_COM3, m_com3);
	DDX_Control(pDX, IDC_COM4, m_com4);
	DDX_Control(pDX, IDC_COM5, m_com5);
}

BEGIN_MESSAGE_MAP(CserialportDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_STOP, &CserialportDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_CANCEL, &CserialportDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_START, &CserialportDlg::OnBnClickedStart)

	ON_EN_CHANGE(IDC_EDIT_INFO, &CserialportDlg::OnEnChangeEditInfo)
	ON_EN_CHANGE(IDC_EDIT_CIRC, &CserialportDlg::OnEnChangeEditCircle)
//	ON_CBN_SELCHANGE(IDC_COM, &CserialportDlg::OnCbnSelchangeCom)
	ON_CBN_SELCHANGE(IDC_COM2, &CserialportDlg::OnCbnSelchangeCom2)

	/*ON_CBN_SELCHANGE(IDC_COM, &CserialportDlg::OnCbnSelchangeCom)*/
END_MESSAGE_MAP()


// CserialportDlg ��Ϣ�������

BOOL CserialportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	

	//���ô��ڲ���
	readCFG();

	m_com.SetWindowTextA(mCom);

	CString temp,temp2,temp3,temp4,temp5;
	temp2.Format(_T("%d"), mBaud);
	m_com2.SetWindowTextA(temp2);

	switch (mParity)
	{
	case 0:
		temp3 = "NONE"; break;
	case 1:
		temp3 = "ODD"; break;
	case 2:
		temp3 = "EVEN"; break;
	}
    m_com3.SetWindowTextA(temp3);
	
	temp4.Format(_T("%d"), mByte);
	m_com4.SetWindowTextA(temp4);

	switch (mStop)
	{
	case 0:
		temp5 = "1"; break;
	case 1:
		temp5 = "1.5"; break;
	case 2:
		temp5 = "2"; break;
	}
	/*temp5.Format(_T("%d"), mStop);*/
	m_com5.SetWindowTextA(temp5);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CserialportDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CserialportDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CserialportDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CserialportDlg::choosedata()
{
	int nSelcom;
	int nSelcom2;
	int nSelcom3;
	int nSelcom4;
	int nSelcom5;

	//��ȡ��Ͽ�ؼ����б����ѡ���������
	nSelcom = m_com.GetCurSel();
	switch (nSelcom)
	{
	case 0:
		mCom = "com1"; break;
	case 1:
		mCom = "com2"; break;
	case 2:
		mCom = "com3"; break;
	case 3:
		mCom = "com4"; break;
	case 4:
		mCom = "com5"; break;
	case 5:
		mCom = "com6"; break;
	case 6:
		mCom = "com7"; break;
	case 7:
		mCom = "com8"; break;
	case 8:
		mCom = "com9"; break;
	case 9:
		mCom = "com10"; break;
	case 10:
		mCom = "com11"; break;
	case 11:
		mCom = "com12"; break;
	case 12:
		mCom = "com13"; break;
	case 13:
		mCom = "com14"; break;
	case 14:
		mCom = "com15"; break;
	case 15:
		mCom = "com16"; break;
	case 16:
		mCom = "com17"; break;
	case 17:
		mCom = "com18"; break;
	case 18:
		mCom = "com19"; break;
	case 19:
		mCom = "com20"; break;
	}
//������
	nSelcom2 = m_com2.GetCurSel();
	switch (nSelcom2)
	{
	case 0:
		mBaud = 9600; break;
	case 1:
		mBaud = 14400; break;
	case 2:
		mBaud = 19200; break;
	case 3:
		mBaud = 114200; break;
	}
//У��λ	
	nSelcom3 = m_com3.GetCurSel();
	switch (nSelcom3)
	{
	case 0:
		mParity = 0; break;
	case 1:
		mParity = 1; break;
	case 2:
		mParity = 2; break;
	}
//����λ	
	nSelcom4 = m_com4.GetCurSel();
	switch (nSelcom4)
	{
	case 0:
		mByte = 5; break;
	case 1:
		mByte = 6; break;
	case 2:
		mByte = 7; break;
	case 3:
		mByte = 8; break;
	}
//ֹͣλ
	nSelcom5 = m_com5.GetCurSel();
	switch (nSelcom5)
	{
	case 0:
		mStop = 0; break;
	case 1:
		mStop = 2; break;


	}

	return;

}
//���ڲ���ѡ��ʹ��
void CserialportDlg::UnableChoose()
{
	m_com.EnableWindow(FALSE);
	m_com2.EnableWindow(FALSE);
	m_com3.EnableWindow(FALSE);
	m_com4.EnableWindow(FALSE);
	m_com5.EnableWindow(FALSE);
}
//��ֹ���ڲ���ѡ��ʹ��
void CserialportDlg::EnableChoose()
{
	m_com.EnableWindow(TRUE);
	m_com2.EnableWindow(TRUE);
	m_com3.EnableWindow(TRUE);
	m_com4.EnableWindow(TRUE);
	m_com5.EnableWindow(TRUE);
}
void CserialportDlg::OnBnClickedStart()
{
	m_bStart = !m_bStart;
	if (m_bStart == true)
	{
		choosedata();
		//! ���������߳�
		InitPort();
		UnableChoose();
		m_bClose = false;
		m_Thread = ::CreateThread
			(NULL, 0, ThreadFunc, this, 0, &m_dwThreadId);
		SetDlgItemText(IDC_START, _T("�رմ���"));
	}
	else
	{
		m_bClose = true;
		Sleep(lPeriod * 3);
		::CloseHandle(m_Thread);
		m_con.Close();
		EnableChoose();
		m_Thread = 0;
		SetDlgItemText(IDC_START, _T("�򿪴���"));
	}
}
void CserialportDlg::OnBnClickedStop()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_bRun = !m_bRun;
	if (m_bRun == true)
	{
		SetDlgItemText(IDC_STOP, _T("��ͣ"));
	}
	else
	{
		SetDlgItemText(IDC_STOP, _T("����"));
	}
}

void CserialportDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_Thread)
	{
		m_bClose = true;
		Sleep(lPeriod * 3);
		::CloseHandle(m_Thread);
		m_con.Close();
	}

	CDialogEx::OnCancel();
}

/*void CserialportDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnTimer(nIDEvent);
}
*/
void CserialportDlg::period()
{
	if (m_bRun)
	{   /*******ÿ�δ򿪳��򶼻ᴴ��һ����ʱ���������ı��Լ�¼�շ�����*******/
		std::ofstream file;
		CString str; //��ȡϵͳʱ�� ����
		CTime tm; tm = CTime::GetCurrentTime();
		str = tm.Format("%Y-%m-%d ");
		FileName = str + ".txt";

        file.open(FileName, std::ofstream::app);
		char send[] = { 0x01, 0x04, 0x00, 0x01, 0x00, 0x02, 0x20, 0x0B };
		memcpy_s(m_con.bufS, MAXBUFSIZE, send, 8);
		m_con.iSizeS = 8;

        DisplayS();
		m_con.Send();
		m_con.Read();
		SaveR();
		if (iReadSize == 9)
		{
			DisplayR();
		}
		Sleep(lPeriod);
	}
}

void  CserialportDlg::DisplayS()  
{  
	    std::ofstream fileS;
		//CString FileName="test.txt";
	    char send[] = { 0x01, 0x04, 0x00, 0x01, 0x00, 0x02, 0x20, 0x0B };
		CString output = _T("send: ");

		for (int i = 0; i < m_con.iSizeS; i++)
		{
			CString temp;
			temp.Format(_T("%02x "), (unsigned char)send[i]);
			output += temp;
		}

		fileS.open(FileName, std::ofstream::app);
		fileS << output << std::endl;
		fileS.close();
		OutPutWindow(output);
	
	return;
}
void  CserialportDlg::SaveR()
{   
	if (m_con.iSizeR > 0)
	{   
		CString output = _T("������: ");
		//! ����У��
		if (m_con.iSizeR < 9)
		{
			output = _T("���Ȳ��㣬������һ�ν���");
			OutPutWindow(output);
			
		}
		else if (m_con.iSizeR == 9)
		{
			memcpy_s(read, 9, m_con.bufR, 9);
			iReadSize = 9;
		}
		else if (m_con.iSizeR > 9)
		{
			output = _T("����̫����������һ�ν���");
			OutPutWindow(output);
			
		}
		m_con.iSizeR = 0;
		
	}
	return;
}
void  CserialportDlg::DisplayR()
{   
	std::ofstream fileR;
	//CString FileName = "test.txt";
	CString output = _T("read: ");
	if (iReadSize > 0)
	{   
		for (int i = 0; i < iReadSize; i++)
		{
			CString temp;
			temp.Format(_T("%02x "), (unsigned char)read[i]);
			output += temp;
		}
		fileR.open(FileName,std::ofstream::app);
		fileR << output << std::endl;
		fileR.close();

		OutPutWindow(output);
		iReadSize = 0;
		AnalyData();
	}
	return;
}

void  CserialportDlg::AnalyData() //Э��ת��
{  
	

	int chan[4] = { 0 };
	long int circle = 0; 
	for (int i = 3,j=0; i < 7; i++,j++)
	{
		if (read[i]<0)
			chan[j] = read[i] + 256;
		else chan[j] = read[i];
	}
	circle = (chan[0] * pow((double)16, (int)6) + chan[1] * pow((double)16, (int)4) + chan[2] * pow((double)16, (int)2) + chan[3]) / 4096;
	
	CString output = _T("circles: ");
    CString temp;
	temp.Format(_T("%d"), circle);
	output += temp;
	OutPutWindowC(output);


    return;
}


void CserialportDlg::OutPutWindow(CString strOutput)//������ʾ��һ��listbox
{
		CString str; //��ȡϵͳʱ�� ����
		CTime tm; tm = CTime::GetCurrentTime();
		str = tm.Format("%Y-%m-%d %H:%M:%S ");

		str += strOutput;
		str += _T("\r\n");
		int  nLength = m_Info.SendMessage(WM_GETTEXTLENGTH);

		if (nLength > 1024 * 10)
		{
			m_Info.SetWindowText(_T(""));
			nLength = m_Info.SendMessage(WM_GETTEXTLENGTH);
		}

		m_Info.SetSel(nLength, nLength);
		m_Info.ReplaceSel(str);
	
}
void CserialportDlg::OutPutWindowC(CString strOutput)//�����ڵڶ���listbox��ʾ
{
	CString str; //��ȡϵͳʱ�� ����
	CTime tm; tm = CTime::GetCurrentTime();
	str = tm.Format("%Y-%m-%d %H:%M:%S ");

	str += strOutput;
	str += _T("\r\n");
	int  nLength = m_Circ.SendMessage(WM_GETTEXTLENGTH);

	if (nLength > 1024 * 10)
	{
		m_Circ.SetWindowText(_T(""));
		nLength = m_Circ.SendMessage(WM_GETTEXTLENGTH);
	}

	m_Circ.SetSel(nLength, nLength);
	m_Circ.ReplaceSel(str);

}
void CserialportDlg::readCFG()  
{   
	//��ȡ�����ļ����ں������ʵ���Ϣ
	lPeriod = GetPrivateProfileInt(_T("Init"), _T("Period"), 100, _T("./setting.ini"));
	TCHAR buf[10] = { 0 };
	GetPrivateProfileString(_T("Init"), _T("port"), _T("com10"), buf, 10, _T("./setting.ini"));
	mCom = buf;
	mBaud = GetPrivateProfileInt(_T("Init"), _T("baud"), 9600, _T("./setting.ini"));
	mParity = GetPrivateProfileInt(_T("Init"), _T("Parity"), 2, _T("./setting.ini"));
	mStop = GetPrivateProfileInt(_T("Init"), _T("StopBits"), 1, _T("./setting.ini"));
	mByte = GetPrivateProfileInt(_T("Init"), _T("ByteSize"), 8, _T("./setting.ini"));
}

void CserialportDlg::InitPort()
{
	m_con.port = _T("\\\\.\\");
	m_con.port += mCom;

	m_con.baud = mBaud;
	m_con.parity = mParity;
	m_con.byteSize = mByte;
	m_con.stopBits = mStop;

	m_con.iSizeR = 0;
	m_con.iSizeS = 0;
	m_con.state = 0;
	m_con.time = 0;
	m_con.hComm = NULL;
	m_con.lPeriod = lPeriod;
}



/*BOOL CserialportDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SetWindowPlacement(&m_wp);
	CenterWindow(GetDesktopWindow());
	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}
*/
void CserialportDlg::OnClose()
{
	m_bClose = true;
	Sleep(lPeriod * 3);
	::CloseHandle(m_Thread);

	m_con.Close();
	CDialogEx::OnClose();
}



void CserialportDlg::OnEnChangeEditInfo()//��ʾ��
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CserialportDlg::OnEnChangeEditCircle()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}



void CserialportDlg::OnCbnSelchangeCom2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

//����ת������
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}





//void CserialportDlg::OnCbnSelchangeCom()
//{
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	int nSelcom = m_com.GetCurSel();
//	switch (nSelcom)
//	{
//	case 0:
//		mCom = "com1"; break;
//	case 1:
//		mCom = "com2"; break;
//	case 2:
//		mCom = "com3"; break;
//	case 3:
//		mCom = "com2"; break;
//	case 4:
//		mCom = "com4"; break;
//	case 5:
//		mCom = "com5"; break;
//	case 6:
//		mCom = "com6"; break;
//	case 7:
//		mCom = "com7"; break;
//	case 8:
//		mCom = "com8"; break;
//	case 9:
//		mCom = "com9"; break;
//	}
//}
