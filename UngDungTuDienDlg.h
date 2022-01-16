
// UngDungTuDienDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CUngDungTuDienDlg dialog
class CUngDungTuDienDlg : public CDialogEx
{
// Construction
public:
	CUngDungTuDienDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_UNGDUNGTUDIEN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CComboBox LuaChon;
	CEdit DauVaoAnh;
	CEdit DauVaoViet;
	CEdit DuongDan;
	CButton OK;
	CButton Open;
	CButton Chon;
public:
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedChon();
protected:
	CListCtrl KetQua;
};
