
// 3Dtest2Dlg.h : header file
//

#pragma once


// CMy3Dtest2Dlg dialog
class CMy3Dtest2Dlg : public CDialogEx
{
// Construction
public:
	CMy3Dtest2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY3DTEST2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// coordinate data
	UINT m_timerID;
	int rotationAngle;
	int totalRotation;
	double currentLocation[4];

public:
	bool C3Dtransfer();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRotate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2
