
// UngDungTuDienDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UngDungTuDien.h"
#include "UngDungTuDienDlg.h"
#include "afxdialogex.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#define MAX 500



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//Định nghĩa cấu trúc
//================================================================================================================
struct TuDien
{
	string Anh, Viet;
};

struct HashNode
{
	TuDien key;
	HashNode* next;
};

int hashFunction(string key, int KEYSIZE);
void initBucket(HashNode**& bucket, int KEYSIZE);
void push(HashNode**& bucket, int b, TuDien x);
int insertAfter(HashNode**& bucket, HashNode* p, TuDien x);
int place(HashNode**& bucket, int b, TuDien x);
int pop(HashNode**& bucket, int b);
int deleteAfter(HashNode* p);
void resize(HashNode**& bucket, int& KEYSIZE);

int docFileTuDien(HashNode**& bucket, string fileName, int& KEYSIZE);
string timKiemNghia(HashNode** bucket, int KEYSIZE, string Anh);
bool kiemTraTuCoTrongTuDien(HashNode** bucket, int KEYSIZE, string Anh);
int demSoTuTrongDVKhongCoTrongTuDien(HashNode** bucket, int KEYSIZE, string doanVan);
void capNhatNghiaCuaCacTuTrongDoanVan(HashNode**& bucket, int KEYSIZE, string doanVan);
int xoa1Tu(HashNode**& bucket, int KEYSIZE, string Anh);
int dieuChinhNghiaCua1Tu(HashNode**& bucket, int KEYSIZE, string Anh, string Viet);
int demTuDienTrong1Bucket(HashNode* q);
int demTuDien(HashNode** bucket);

///Hỗ trợ
void chuanHoa(string& s);
int soSanhChuoi(string s1, string s2);
string chuyenSoVeChuoi(int n);

//================================================================================================================

//Khai báo biến toàn cục
const float threshol = 0.6;		//Ngưỡng
int KEYSIZE = 100;
HashNode** bucket = (HashNode**)malloc(KEYSIZE* sizeof(HashNode*));

//================================================================================================================

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUngDungTuDienDlg dialog



CUngDungTuDienDlg::CUngDungTuDienDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUngDungTuDienDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUngDungTuDienDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LuaChon, LuaChon);
	DDX_Control(pDX, IDC_DauVaoAnh, DauVaoAnh);
	DDX_Control(pDX, IDC_DauVaoViet, DauVaoViet);
	DDX_Control(pDX, IDC_DuongDan, DuongDan);
	DDX_Control(pDX, IDC_Open, Open);
	DDX_Control(pDX, IDC_Chon, Chon);
	DDX_Control(pDX, IDC_KetQua, KetQua);
}

BEGIN_MESSAGE_MAP(CUngDungTuDienDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Open, &CUngDungTuDienDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_Chon, &CUngDungTuDienDlg::OnBnClickedChon)
END_MESSAGE_MAP()


// CUngDungTuDienDlg message handlers

BOOL CUngDungTuDienDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	initBucket(bucket, KEYSIZE);

	LuaChon.AddString(_T("Tim kiem nghia cua 1 tu"));
	LuaChon.AddString(_T("Dem so tu khong co trong danh sach tu dien"));
	LuaChon.AddString(_T("Them tu khong co trong danh sach tu dien"));
	LuaChon.AddString(_T("Them"));
	LuaChon.AddString(_T("Xoa"));
	LuaChon.AddString(_T("Dieu chinh lai nghia"));
	LuaChon.AddString(_T("Xuat ra danh sach tu dien"));
	LuaChon.SetCurSel(6);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUngDungTuDienDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUngDungTuDienDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUngDungTuDienDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




//Thân hàm con
//================================================================================================================
int hashFunction(string key, int KEYSIZE)
{
	int t = 0;
	for each (char c in key)
		t += (int)c;
	return t % KEYSIZE;
}


void initBucket(HashNode**& bucket, int KEYSIZE)
{
	for (int b = 0; b < KEYSIZE; b++)
		bucket[b] = NULL;
}

void push(HashNode**& bucket, int b, TuDien x)
{
	if (demTuDien(bucket) >= threshol * KEYSIZE)
		resize(bucket, KEYSIZE);
	HashNode* p = new HashNode;
	p->key = x;
	p->next = bucket[b];
	bucket[b] = p;
}

int insertAfter(HashNode**& bucket, HashNode* p, TuDien x)
{
	if (demTuDien(bucket) >= threshol * KEYSIZE)
		resize(bucket, KEYSIZE);
	if (p == NULL)
		return 1;
	HashNode* q = new HashNode;
	q->key = x;
	q->next = p->next;
	p->next = q;
}

int place(HashNode**& bucket, int b, TuDien x)
{
	if (demTuDien(bucket) >= threshol * KEYSIZE)
		resize(bucket, KEYSIZE);
	for (HashNode* p = bucket[b]; p != NULL; p = p->next)
	{
		if (soSanhChuoi(x.Anh, p->key.Anh) == 0)
			return 0;
	}
	HashNode* q = NULL;
	for (HashNode* p = bucket[b]; p != NULL && x.Anh > p->key.Anh; p = p->next)
		q = p;
	if (q == NULL)
		push(bucket, b, x);
	else
		insertAfter(bucket, q, x);
	return 1;
}

int pop(HashNode**& bucket, int b)
{
	if (bucket[b] == NULL)
		return 0;
	HashNode* p = bucket[b];
	bucket[b] = p->next;
	delete p;
	return 1;
}

int deleteAfter(HashNode* p)
{
	if (p == NULL || p->next == NULL)
		return 0;
	HashNode* q = p->next;
	p->next = q->next;
	delete q;
	return 1;
}

void resize(HashNode**& bucket, int& KEYSIZE)
{
	int oldKEYSIZE = KEYSIZE;
	KEYSIZE *= 2;
	HashNode** newBucket = (HashNode**)malloc(KEYSIZE* sizeof(HashNode*));
	initBucket(newBucket, KEYSIZE);

	for (int i = 0; i < oldKEYSIZE; i++)
	{
		for (HashNode* p = bucket[i]; p != NULL; p = p->next)
		{
			int b = hashFunction(p->key.Anh, KEYSIZE);
			place(newBucket, b, p->key);
		}
	}
	bucket = newBucket;	
}

int docFileTuDien(HashNode**& bucket, string fileName, int& KEYSIZE)
{
	fstream f(fileName);
	if (f.fail())
		return 0;
	while (!f.eof())
	{
		TuDien x;
		getline(f, x.Anh, ':');
		chuanHoa(x.Anh);
		getline(f, x.Viet);
		chuanHoa(x.Viet);
		int b = hashFunction(x.Anh, KEYSIZE);
		place(bucket, b, x);
	}
	f.close();
	return 1;
}

string timKiemNghia(HashNode** bucket, int KEYSIZE, string Anh)
{
	chuanHoa(Anh);
	int b = hashFunction(Anh, KEYSIZE);
	for (HashNode* p = bucket[b]; p != NULL; p = p->next)
		if (soSanhChuoi(Anh, p->key.Anh) == 0)
			return p->key.Viet;
	return "";
}

bool kiemTraTuCoTrongTuDien(HashNode** bucket, int KEYSIZE, string Anh)
{
	chuanHoa(Anh);
	int b = hashFunction(Anh, KEYSIZE);
	for (HashNode* p = bucket[b]; p != NULL; p = p->next)
		if (soSanhChuoi(Anh, p->key.Anh) == 0)
			return true;
	return false;
}

int demSoTuTrongDVKhongCoTrongTuDien(HashNode** bucket, int KEYSIZE, string doanVan)
{
	int dem = 0;
	string tu = "";
	for each (char c in doanVan)
	{
		if (c != ' ')
		{
			tu += c;
			continue;
		}
		else
		{
			chuanHoa(tu);
			if (kiemTraTuCoTrongTuDien(bucket, KEYSIZE, tu) == false)
				dem++;
		}
		tu = "";
	}
	if (tu != "")
	{
		chuanHoa(tu);
		if (kiemTraTuCoTrongTuDien(bucket, KEYSIZE, tu) == false)
			dem++;
	}	
	return dem;
}

void capNhatNghiaCuaCacTuTrongDoanVan(HashNode** bucket, int KEYSIZE, string doanVan)
{
	string tu = "";
	for each (char c in doanVan)
	{
		if (c != ' ')
		{
			tu += c;
			continue;
		}
		else
		{
			chuanHoa(tu);
			if (kiemTraTuCoTrongTuDien(bucket, KEYSIZE, tu) == false)
			{
				TuDien x;
				x.Anh = tu;
				cout << "Nghia cua tu \"" << x.Anh << "\" la: ";
				getchar();
				getline(cin, x.Viet);
				chuanHoa(tu);
				place(bucket, hashFunction(tu, KEYSIZE), x);
			}
		}
		tu = "";
	}
	chuanHoa(tu);
	if (kiemTraTuCoTrongTuDien(bucket, KEYSIZE, tu) == false)
	{
		TuDien x;
		x.Anh = tu;
		cout << "Nghia cua tu \"" << x.Anh << "\" la: ";
		getchar();
		getline(cin, x.Viet);
		chuanHoa(tu);
		place(bucket, hashFunction(tu, KEYSIZE), x);
	}
}

int xoa1Tu(HashNode**& bucket, int KEYSIZE, string Anh)
{
	chuanHoa(Anh);
	int b = hashFunction(Anh, KEYSIZE);
	HashNode* q = NULL;
	for (HashNode* p = bucket[b]; p != NULL; p = p->next)
	{
		if (soSanhChuoi(Anh, p->key.Anh) == 0)
		{
			if (q == NULL)
				pop(bucket, b);
			else
				deleteAfter(q);
			return 1;
		}
		q = p;
	}
	return 0;
}

int dieuChinhNghiaCua1Tu(HashNode**& bucket, int KEYSIZE, string Anh, string Viet)
{
	chuanHoa(Anh);
	int b = hashFunction(Anh, KEYSIZE);
	for (HashNode* p = bucket[b]; p != NULL; p = p->next)
	{
		if (soSanhChuoi(Anh, p->key.Anh) == 0)
		{
			p->key.Viet = Viet;
			return 1;
		}
	}
	return 0;
}

int demTuDienTrong1Bucket(HashNode* q)
{
	int dem = 0;
	for (HashNode* p = q; p != NULL; p = p->next)
		dem++;
	return dem;
}

int demTuDien(HashNode* bucket[])
{
	int dem = 0;
	for (int i = 0; i < KEYSIZE; i++)
		dem += demTuDienTrong1Bucket(bucket[i]);
	return dem;
}


/////Hỗ trợ
void chuanHoa(string& s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == ' ' && s[i + 1] == ' ' || s[i] == '\t')
		{
			s.erase(i, 1);
			i--;
		}
		else if (65 <= s[i] && s[i] <= 90)
			s[i] += 32;
	}
	if (97 <= s[0] && s[0] <= 122)
		s[0] -= 32;
}

int soSanhChuoi(string s1, string s2)
{
	char* ss1 = new char[MAX];
	char* ss2 = new char[MAX];
	int i;
	for (i = 0; i < s1.length(); i++)
		ss1[i] = s1[i];
	ss1[i] = '\t';
	for (i = 0; i < s2.length(); i++)
		ss2[i] = s2[i];
	ss2[i] = '\t';
	if (_strcmpi(ss1, ss2) == 0)
	{
		delete ss1;
		delete ss2;
		return 0;
	}
	else if (_strcmpi(ss1, ss2) > 0)
	{
		delete ss1;
		delete ss2;
		return 1;
	}
	delete ss1;
	delete ss2;
	return -1;
}

string chuyenSoVeChuoi(int n)
{
	string s = "";
	if (n == 0)
		s += 48;
	else
	{
		while (n != 0)
		{
			s += (n % 10) + 48;
			n /= 10;
		}
	}
	if (s.length() < 3)
		s += 48;
	reverse(s.begin(), s.end());
	return s;
}


////================================================================================================================


void CUngDungTuDienDlg::OnBnClickedOpen()
{
	// TODO: Add your control notification handler code here
	CString file;
	DuongDan.GetWindowText(file);
	if (file == "")
		file = _T("..//Data//TuDien.txt");
	string fileName = CStringA(file);

	if (docFileTuDien(bucket, fileName, KEYSIZE) == 0)
		AfxMessageBox(_T("Đọc File không thành công!"));
	else
		AfxMessageBox(_T("Đọc File thành công!"));
}


void CUngDungTuDienDlg::OnBnClickedChon()
{
	// TODO: Add your control notification handler code here

	LVITEM x;
	::ZeroMemory(&x, sizeof(x));

	CString LuaChonX;
	LuaChon.GetWindowText(LuaChonX);
	string luaChonThuX = CStringA(LuaChonX);
	//=========================================
	if (luaChonThuX == "Xuat ra danh sach tu dien")
	{
		KetQua.DeleteAllItems();
		for (int i = 0; i < 3; i++)
			KetQua.DeleteColumn(0);

		KetQua.InsertColumn(0, _T("SBK"), LVCFMT_LEFT, 50);
		KetQua.InsertColumn(1, _T("Nghia tieng Anh"), LVCFMT_LEFT, 225);
		KetQua.InsertColumn(2, _T("Nghia tieng Viet"), LVCFMT_LEFT, 225);
		KetQua.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
		for (int b = KEYSIZE - 1; b >= 0; b--)
		{
			for (HashNode* p = bucket[b]; p != NULL; p = p->next)
			{
				KetQua.InsertItem(&x);
				string STT = chuyenSoVeChuoi(b);
				CString STTC = _T("");
				STTC = STT.c_str();
				KetQua.SetItemText(0, 0, STTC);

				string Anh = p->key.Anh;
				CString AnhC = _T("");
				AnhC = Anh.c_str();
				KetQua.SetItemText(0, 1, AnhC);

				string Viet = p->key.Viet;
				CString VietC = _T("");
				VietC = Viet.c_str();
				KetQua.SetItemText(0, 2, VietC);
			}
		}
	}
	//=========================================
	else if (luaChonThuX == "Xoa")
	{
		CString dauVaoAnh;
		DauVaoAnh.GetWindowText(dauVaoAnh);
		string Anh = CStringA(dauVaoAnh);
		if (xoa1Tu(bucket, KEYSIZE, Anh) == 1)
			AfxMessageBox(_T("Xóa từ điển thành công!!"));
		else
			AfxMessageBox(_T("Xóa từ điển không thành công!!"));
	}
	//=========================================
	else if (luaChonThuX == "Them")
	{
		CString dauVaoAnh;
		DauVaoAnh.GetWindowText(dauVaoAnh);
		string Anh = CStringA(dauVaoAnh);

		CString dauVaoViet;
		DauVaoViet.GetWindowText(dauVaoViet);
		string Viet = CStringA(dauVaoViet);

		chuanHoa(Anh);
		chuanHoa(Viet);

		if (Viet == "" || Anh == "")
			AfxMessageBox(_T("Bạn cần nhập đầy đủ nghĩa của từ vựng!!"));
		else
		{
			TuDien x;
			x.Anh = Anh;
			x.Viet = Viet;
			chuanHoa(Anh);
			chuanHoa(Viet);
			int b = hashFunction(Anh, KEYSIZE);
			if (place(bucket, b, x) == 1)
				AfxMessageBox(_T("Thêm thành công!!"));
			else
				AfxMessageBox(_T("Từ bạn cần thêm đã có trong danh sách từ điển!!"));
		}
	}
	//=========================================
	else if (luaChonThuX == "Tim kiem nghia cua 1 tu")
	{
		//Xóa dữ liệu cũ trong bảng
		KetQua.DeleteAllItems();
		for (int i = 0; i < 3; i++)
			KetQua.DeleteColumn(0);

		CString dauVaoAnh;
		DauVaoAnh.GetWindowText(dauVaoAnh);
		string Anh = CStringA(dauVaoAnh);
		chuanHoa(Anh);

		string Viet = timKiemNghia(bucket, KEYSIZE, Anh);

		if (Anh == "")
			AfxMessageBox(_T("Bạn cần nhập nghĩa tiếng Anh của từ cần tìm!!"));
		else if (Viet == "")
			AfxMessageBox(_T("Từ bạn cần tìm hiện chưa có trong từ điển!!"));
		else
		{

			//Xóa dữ liệu cũ trong bảng
			KetQua.DeleteAllItems();
			for (int i = 0; i < 3; i++)
				KetQua.DeleteColumn(0);

			KetQua.InsertColumn(0, _T("SBK"), LVCFMT_LEFT, 50);
			KetQua.InsertColumn(1, _T("Nghia tieng Anh"), LVCFMT_LEFT, 225);
			KetQua.InsertColumn(2, _T("Nghia tieng Viet"), LVCFMT_LEFT, 225);
			KetQua.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

			KetQua.InsertItem(&x);
			string STT = chuyenSoVeChuoi(hashFunction(Anh, KEYSIZE));
			CString STTC = _T("");
			STTC = STT.c_str();
			KetQua.SetItemText(0, 0, STTC);

			CString AnhC = _T("");
			AnhC = Anh.c_str();
			KetQua.SetItemText(0, 1, AnhC);

			CString VietC = _T("");
			VietC = Viet.c_str();
			KetQua.SetItemText(0, 2, VietC);
		}
	}
	//=========================================
	else if (luaChonThuX == "Dieu chinh lai nghia")
	{
		CString dauVaoAnh;
		DauVaoAnh.GetWindowText(dauVaoAnh);
		string Anh = CStringA(dauVaoAnh);
		chuanHoa(Anh);

		CString dauVaoViet;
		DauVaoViet.GetWindowText(dauVaoViet);
		string Viet = CStringA(dauVaoViet);
		chuanHoa(Viet);

		if (Anh == "")
			AfxMessageBox(_T("Bạn cần nhập nghĩa tiếng Anh của từ cần điều chỉnh!!"));
		else if (Viet == "")
			AfxMessageBox(_T("Bạn cần nhập nghĩa mới cho từ điều chỉnh!!"));
		else
		{
			string KQViet = timKiemNghia(bucket, KEYSIZE, Anh);
			if (KQViet == "")
				AfxMessageBox(_T("Từ bạn cần điều chỉnh không có trong danh sách!!"));
			else if (dieuChinhNghiaCua1Tu(bucket, KEYSIZE, Anh, Viet) == 0)
				AfxMessageBox(_T("Điều chỉnh thất bại!!"));
			else
				AfxMessageBox(_T("Điều chỉnh thành công!!"));
		}
	}
	//=========================================
	else if (luaChonThuX == "Dem so tu khong co trong danh sach tu dien")
	{
		CString dauVaoAnh;
		DauVaoAnh.GetWindowText(dauVaoAnh);
		string doanVan = CStringA(dauVaoAnh);

		int dem = demSoTuTrongDVKhongCoTrongTuDien(bucket, KEYSIZE, doanVan);
		string so = "";
		if (dem == 0)
			so += dem + 48;
		while (dem != 0)
		{
			so += (dem % 10 + 48);
			dem /= 10;
		}
		CString demSo = _T("");
		demSo = so.c_str();

		KetQua.DeleteAllItems();
		for (int i = 0; i < 3; i++)
			KetQua.DeleteColumn(0);

		KetQua.InsertColumn(0, _T("So tu trong cau van khong co trong bang bam"), LVCFMT_LEFT, 400);
		KetQua.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
		KetQua.InsertItem(&x);
		KetQua.SetItemText(0, 0, demSo);
	}
	//=========================================
	else if (luaChonThuX == "Them tu khong co trong danh sach tu dien")
	{
		//Xóa dữ liệu cũ trong bảng
		KetQua.DeleteAllItems();
		for (int i = 0; i < 3; i++)
			KetQua.DeleteColumn(0);

		KetQua.InsertColumn(0, _T("SBK"), LVCFMT_LEFT, 50);
		KetQua.InsertColumn(1, _T("Nghia tieng Anh"), LVCFMT_LEFT, 225);
		KetQua.InsertColumn(2, _T("Nghia tieng Viet"), LVCFMT_LEFT, 225);
		KetQua.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

		CString dauVaoAnh;
		DauVaoAnh.GetWindowText(dauVaoAnh);
		string doanVan = CStringA(dauVaoAnh);


		string tu = "";
		int dem = 0;
		for each (char c in doanVan)
		{
			if (c != ' ')
			{
				tu += c;
				continue;
			}
			else
			{
				chuanHoa(tu);
				if (kiemTraTuCoTrongTuDien(bucket, KEYSIZE, tu) == false)
				{
					TuDien tuDien;
					tuDien.Anh = tu;
					tuDien.Viet = "-";
					place(bucket, hashFunction(tu, KEYSIZE), tuDien);

					KetQua.InsertItem(&x);
					string STT = chuyenSoVeChuoi(hashFunction(tuDien.Anh, KEYSIZE));
					CString STTC = _T("");
					STTC = STT.c_str();
					KetQua.SetItemText(0, 0, STTC);

					CString AnhC = _T("");
					AnhC = tuDien.Anh.c_str();
					KetQua.SetItemText(0, 1, AnhC);

					CString VietC = _T("");
					VietC = tuDien.Viet.c_str();
					KetQua.SetItemText(0, 2, VietC);
					dem++;
				}
			}
			tu = "";
		}
		chuanHoa(tu);
		if (kiemTraTuCoTrongTuDien(bucket, KEYSIZE, tu) == false)
		{
			TuDien tuDien;
			tuDien.Anh = tu;
			tuDien.Viet = "-";
			place(bucket, hashFunction(tu, KEYSIZE), tuDien);

			KetQua.InsertItem(&x);
			string STT = chuyenSoVeChuoi(hashFunction(tuDien.Anh, KEYSIZE));
			CString STTC = _T("");
			STTC = STT.c_str();
			KetQua.SetItemText(0, 0, STTC);

			CString AnhC = _T("");
			AnhC = tuDien.Anh.c_str();
			KetQua.SetItemText(0, 1, AnhC);

			CString VietC = _T("");
			VietC = tuDien.Viet.c_str();
			KetQua.SetItemText(0, 2, VietC);

			dem++;
		}
		if (dem != 0)
			AfxMessageBox(_T("Hãy cập nhật nghĩa cho những từ vừa thêm vào!!"));
	}
}

