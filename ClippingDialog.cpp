// ClippingDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "Artist2D.h"
#include "ClippingDialog.h"


// CClippingDialog 对话框

IMPLEMENT_DYNAMIC(CClippingDialog, CDialog)

CClippingDialog::CClippingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CClippingDialog::IDD, pParent)
{

	EnableAutomation();

}

CClippingDialog::~CClippingDialog()
{
}

void CClippingDialog::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CDialog::OnFinalRelease();
}

void CClippingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CLIPPED, m_StrClippedID);
	DDX_Text(pDX, IDC_EDIT_CLIPPING, m_StrClippingID);
}


BEGIN_MESSAGE_MAP(CClippingDialog, CDialog)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CClippingDialog, CDialog)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_IClippingDialog 支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {360E86CD-2A75-4F4C-AF8C-B9F26258651E}
static const IID IID_IClippingDialog =
{ 0x360E86CD, 0x2A75, 0x4F4C, { 0xAF, 0x8C, 0xB9, 0xF2, 0x62, 0x58, 0x65, 0x1E } };

BEGIN_INTERFACE_MAP(CClippingDialog, CDialog)
	INTERFACE_PART(CClippingDialog, IID_IClippingDialog, Dispatch)
END_INTERFACE_MAP()


// CClippingDialog 消息处理程序
