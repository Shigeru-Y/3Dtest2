
/**
 * @file 3Dtest2Dlg.cpp
 * @brief 3D coordinate drawing on 2D display.
 *
 * @author Shigeru Yamamoto
 * @date 28Oct2021
 * @details 3D graphics display on 2D display. 
 * @details Confirmation of combined matrix behavior will be equivalent as sequencial of independent matrix.
 */

#include "stdafx.h"
#include "3Dtest2.h"
#include "3Dtest2Dlg.h"
#include "afxdialogex.h"

#include "TransMatrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About
/**
 * @brief Behavior of dialog.
 */
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMy3Dtest2Dlg dialog

/**
 * @brief Global variables.
 */
// Transfer matrix.
TransMatrix preRotation;
TransMatrix postRotation;
TransMatrix mainRotation;

TransMatrix joinedRotation;

CMy3Dtest2Dlg::CMy3Dtest2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MY3DTEST2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy3Dtest2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMy3Dtest2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ROTATE, &CMy3Dtest2Dlg::OnBnClickedRotate)
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMy3Dtest2Dlg message handlers
/**
 * @fn BOOL CMy3Dtest2Dlg::OnInitDialog()
 * @brief Initialization of dialog.
 */
BOOL CMy3Dtest2Dlg::OnInitDialog()
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
	// Set first location.
	currentLocation[0] = 50;
	currentLocation[1] = 50;
	currentLocation[2] = 50;
	currentLocation[3] = 1;		// Unit

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMy3Dtest2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
/**
 * @fn void CMy3Dtest2Dlg::OnPaint()
 * @brief Handler of paint message.
 * @details Added 3D graphics drawing with axis.
 */
void CMy3Dtest2Dlg::OnPaint()
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
		// S.Yamamoto
		// Draw application graphics.
		RECT r;
		GetWindowRect(&r);
		auto center = CPoint((r.right - r.left) / 2, (r.bottom - r.top) / 2);
		int  axisLength = (r.bottom - r.top) / 2 * 80 / 100;
		double ratio = (double)axisLength / 100.;


		// Draw axis
		CPaintDC dc(this);
		CPen blackPen(PS_SOLID, 1, RGB(0, 0, 0)), 
			redPen(PS_SOLID, 1, RGB(255, 0, 0)), 
			redLightPen(PS_SOLID, 1, RGB(196, 128, 128)),
			*pOldPen;
		pOldPen = dc.SelectObject(&blackPen);
		// Z
		dc.MoveTo(center.x, center.y);
		dc.LineTo(center.x, center.y - axisLength);
		dc.TextOutW(center.x, center.y - axisLength - 10, CString("Z"));

		// Y
		dc.MoveTo(center.x, center.y);
		dc.LineTo(center.x + axisLength, center.y);
		dc.TextOutW(center.x + axisLength + 10, center.y, CString("Y"));

		// X
		dc.MoveTo(center.x, center.y);
		dc.LineTo(center.x - (int)((double)axisLength * 0.7 * 0.86), center.y + (int)((double)axisLength * 0.7 * 0.5));
		dc.TextOutW(center.x - (int)((double)axisLength * 0.7 * 0.86) - 10, 
			center.y + (int)((double)axisLength * 0.7 * 0.5) + 10, CString("X"));

		// Draw point
		// Calcurate location on the screen.
		int scY, scX;
		scY = (int)(center.y - ratio * currentLocation[2] + ratio * 0.7 * 0.5 * currentLocation[0]);
		scX = (int)(center.x + ratio * currentLocation[1] - ratio * 0.7 * 0.86 * currentLocation[0]);

		// Select light colored pen if any of coordinate are negative.
		if (currentLocation[0] < 0 || currentLocation[1] < 0 || currentLocation[2] < 0) {
			dc.SelectObject(&redLightPen);
		}
		else {
			dc.SelectObject(&redPen);
		}
		dc.MoveTo(scX, scY);
		dc.LineTo(scX + 1, scY);
		dc.LineTo(scX + 1, scY + 1);
		dc.LineTo(scX, scY + 1);
		dc.LineTo(scX, scY);

		dc.SelectObject(pOldPen);//ƒyƒ“‚ðŒ³‚É–ß‚·

		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMy3Dtest2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/**
 * @fn void CMy3Dtest2Dlg::OnBnClickedRotate()
 * @brief Message handler of Rotation button.
 * @details Two types of coordinate transfer with independent matrix with 3 step rotation and combination matrix.
 * @details Starting timer for each 0.1sec.
 */
void CMy3Dtest2Dlg::OnBnClickedRotate()
{
	// Start rotation.
	// Set rotation angle.
	rotationAngle = 5;
	preRotation.SetRotation(AXIS_X, 45.);
	postRotation.SetRotation(AXIS_X, -45.);
	mainRotation.SetRotation(AXIS_Z, rotationAngle);

	// 2021/Oct/29 S.Yamamoto
	// In order to confirm joined transfer be equivalent to independent sequential transfer.
	// Calculate joined rotation.
	joinedRotation = postRotation;	// First, set the last transfer.
	joinedRotation.Product(&mainRotation);	// Main transfer rotation.
	joinedRotation.Product(&preRotation);	// Lastly, calculate first pre-transfer to the joined matrix.

	// Set timer.
	UINT interval = 1 * 100;	// 100mS
	m_timerID = SetTimer(1, interval, NULL);	// Timer ID = 1.
	totalRotation = 0;
}


void CAboutDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnTimer(nIDEvent);
}

/**
 * @fn void CMy3Dtest2Dlg::OnTimer(UINT_PTR nIDEvent)
 * @brief Message handler of timer.
 * @details Invoked each 0.1sec.
 */
void CMy3Dtest2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// Transfer one step.
	//preRotation.TransferCoordinate(currentLocation);
	//mainRotation.TransferCoordinate(currentLocation);
	//postRotation.TransferCoordinate(currentLocation);

	// 2021/Oct/29 S.Yamamoto
	// In order to confirm joined transfer be equivalent to independent sequential transfer.
	joinedRotation.TransferCoordinate(currentLocation);

	InvalidateRect(NULL, FALSE);	// Redraw.
	totalRotation += rotationAngle;
	if (totalRotation >= 360) {
		// Reached one round. Stop timer.
		KillTimer(1);
	}
	CDialogEx::OnTimer(nIDEvent);
}

/**
 * @fn void CMy3Dtest2Dlg::OnSize(UINT nType, int cx, int cy)
 * @brief Message handler of window size change.
 * @details Erase old drawing.
 */
void CMy3Dtest2Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	InvalidateRect(NULL, TRUE);		// Erase and Redraw.
}
