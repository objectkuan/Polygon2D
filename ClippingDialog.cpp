// ClippingDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Artist2D.h"
#include "ClippingDialog.h"


// CClippingDialog �Ի���

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
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

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

// ע��: ������� IID_IClippingDialog ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {360E86CD-2A75-4F4C-AF8C-B9F26258651E}
static const IID IID_IClippingDialog =
{ 0x360E86CD, 0x2A75, 0x4F4C, { 0xAF, 0x8C, 0xB9, 0xF2, 0x62, 0x58, 0x65, 0x1E } };

BEGIN_INTERFACE_MAP(CClippingDialog, CDialog)
	INTERFACE_PART(CClippingDialog, IID_IClippingDialog, Dispatch)
END_INTERFACE_MAP()


// CClippingDialog ��Ϣ�������
