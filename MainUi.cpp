#include"Base.h"
#include "MainUi.h"
#include"CChartUi.h"
#include"USBCollect.h"

//全局变量
static int nCount;
int n = 2;
MainUi::MainUi()
{
}

MainUi::~MainUi()
{
}

LPCTSTR MainUi::GetWindowClassName() const
{
	return _T("DUIMainFrame");
}

CDuiString MainUi::GetSkinFile()
{
	return _T("MainUi.xml");
}

CDuiString MainUi::GetSkinFolder()
{
	return _T("");
}

void MainUi::Notify(TNotifyUI & msg)
{
	if (msg.sType == _T("click"))
	{
		CDuiString    strName = msg.pSender->GetName();
		CTabLayoutUI *pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("TabLayout")));
		if (strName == _T("Enter_Assessment"))
			pControl->SelectItem(1);
		if (strName == _T("Start_Stress_Assessment"))
		{
			SetTimer(m_hWnd, 1, 10, 0);
			SetTimer(m_hWnd, 2, 10, 0);
		}
		if (strName == _T("Stop_Stress_Assessment"))
		{
			KillTimer(m_hWnd, 1);
			KillTimer(m_hWnd, 2);
		}
		if (strName == _T("Gravity_Assessment"))
		{
			pControl->SelectItem(2);
			KillTimer(m_hWnd, 1);
			KillTimer(m_hWnd, 2);
		}
		if (strName == _T("Start_Gravity_Assessment"))
			SetTimer(m_hWnd, 3, 800, 0);

		if (strName == _T("Stop_Gravity_Assessment"))
			KillTimer(m_hWnd, 3);
		if (strName == _T("Stress_Assessment"))
		{
			pControl->SelectItem(1);
			KillTimer(m_hWnd, 3);
		}




	}
	__super::Notify(msg);
}


LRESULT MainUi::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT lRes = 0;
	switch (uMsg)
	{
	case WM_TIMER: 
		lRes = OnTimer(uMsg, wParam, lParam, bHandled); 
	break;
	}

	bHandled = FALSE;
	return 0;
}

LRESULT MainUi::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	switch (wParam)
	{
		case 1:
		{
			double Pi = 3.1415926536;
			double x, y1;
			nCount++;
			x = nCount / 10.0;
			y1 = 20.0 * sin(2.0*Pi / 400.0*nCount) + 40.0 + (2.0*rand() / RAND_MAX - 1.0);
			ChartL->AddPoint2D(x, y1, 0);
			ChartL->AddPoint2D(x, y1*0.32, 1);
			ChartL->SetDataTitle(_T("左脚前部压力"), 0);
			ChartL->SetDataTitle(_T("左脚后部压力"), 1);
			ChartL->OnDraw();
			break;
		}
		case 2:
		{
			double Pi = 5.1415926536;
			double x, y1;
			nCount++;
			x = nCount / 10.0;
			y1 = 20.0 * sin(2.0*Pi / 400.0*nCount) + 40.0 + (2.0*rand() / RAND_MAX - 1.0);
			ChartR->AddPoint2D(x, y1, 0);
			ChartR->AddPoint2D(x, y1*0.32, 1);
			ChartR->SetDataTitle(_T("右脚前部压力"), 0);
			ChartR->SetDataTitle(_T("右脚后部压力"), 1);
			ChartR->OnDraw();
			break;
		}
		case 3:
		{
			int Pi = 5;
			int x, L, R;

			x = nCount / 10;
			L = rand() % 100 + 1;
			R = rand() % 100 + 1;
			ChartG->SetPlotRange(-3.0, 3.0, -3.0, 3.0);
			ChartG->OnDraw();
			ChartG->SetPlotRange(-5.0, 5.0, -5.0 , 5.0);
			ChartG->OnDraw();

			CProgressUI* pProgressL = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("StressL")));
			pProgressL->SetValue(L);
			CProgressUI* pProgressR = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("StressR")));
			pProgressR->SetValue(R);
			break;
		}


	default:
		break;
	}
	return 0;
}

CControlUI * MainUi::CreateControl(LPCTSTR pstrClass)
{
	CChartUi *pUI = new CChartUi;
	//绘制左脚压力曲线
	if (_tcscmp(pstrClass, _T("ChartL")) == 0)
	{
		HWND hWnd = CreateWindow(_T("ChartCtrl"), _T("win32"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 180, 100, 820, 350, m_PaintManager.GetPaintWindow(), NULL, NULL, NULL);
		pUI->Attach(hWnd);
		ChartL = GetChart(hWnd);
		ChartL->SetType(kTypeXY);
		ChartL->SetDrawBasicAnyway(TRUE);
		ChartL->SetDefScreen(NULL, NULL);
		ChartL->SetGridLine(TRUE, FALSE, FALSE, FALSE);
		ChartL->DeleteAxisByLocation(1);
		ChartL->SetYRange(0, 100);
		ChartL->SetStaticYRange(TRUE, 0);
		ChartL->SetYTickCount(9);
		ChartL->SetAxisLabelFormat(_T("%gkg"), 0);
		ChartL->SetBkgndColor(RGB(255, 255, 255));
		return pUI;
	}
	//绘制右脚压力曲线
	if (_tcscmp(pstrClass, _T("ChartR")) == 0) 
	{
		HWND hWnd = CreateWindow(_T("ChartCtrl"), _T("win32"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 180, 100, 820, 350, m_PaintManager.GetPaintWindow(), NULL, NULL, NULL);
		pUI->Attach(hWnd);
		ChartR = GetChart(hWnd);
		ChartR->SetType(kTypeXY);
		ChartR->SetDrawBasicAnyway(TRUE);
		ChartR->DeleteAxisByLocation(1);
		ChartR->SetDefScreen(NULL, NULL);
		ChartR->SetAxisLabelFormat(_T("%gkg"), 0);
		ChartR->SetGridLine(TRUE, FALSE, FALSE, FALSE);
		ChartR->SetYRange(0, 100);
		ChartR->SetStaticYRange(TRUE, 0);
		ChartR->SetYTickCount(9);
		ChartR->SetBkgndColor(RGB(255, 255, 255));
		return pUI;
	}

	//绘制二维重心图
	if (_tcscmp(pstrClass, _T("ChartG")) == 0)
	{
		HWND hWnd = CreateWindow(_T("ChartCtrl"), _T("win32"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 180, 100, 820, 350, m_PaintManager.GetPaintWindow(), NULL, NULL, NULL);
		pUI->Attach(hWnd);
		ChartG = GetChart(hWnd);
		ChartG->SetType(kTypeContourMap);
		ChartG->SetDrawBasicAnyway(TRUE);
		ChartG->SetDefScreen(NULL, NULL);
		ChartG->IsTitleShow(FALSE);
		ChartG->SetBkgndColor(RGB(255, 255, 255));
		return pUI;
	}

	return WindowImplBase::CreateControl(pstrClass);
}

