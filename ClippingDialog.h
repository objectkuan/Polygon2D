#pragma once
#include "afxwin.h"


// CClippingDialog �Ի���

class CClippingDialog : public CDialog
{
	DECLARE_DYNAMIC(CClippingDialog)

public:
	CClippingDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClippingDialog();

	virtual void OnFinalRelease();

// �Ի�������
	enum { IDD = IDD_DIALOG_CLIPPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CString m_StrClippedID;
	CString m_StrClippingID;
};
