#pragma once
#include "afxwin.h"


// CClippingDialog 对话框

class CClippingDialog : public CDialog
{
	DECLARE_DYNAMIC(CClippingDialog)

public:
	CClippingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClippingDialog();

	virtual void OnFinalRelease();

// 对话框数据
	enum { IDD = IDD_DIALOG_CLIPPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CString m_StrClippedID;
	CString m_StrClippingID;
};
