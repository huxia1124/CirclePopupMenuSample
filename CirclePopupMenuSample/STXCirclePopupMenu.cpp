
#include "STXCirclePopupMenu.h"
#include <windowsx.h>
#include <math.h>
#include "STXGraphics.h"
#include <commctrl.h>

#define STXCPM_TIMER_ID_ANIMATION			1243

//////////////////////////////////////////////////////////////////////////

CSTXCircleMenuItem::CSTXCircleMenuItem()
{
	_itemData = 0;
}


void CSTXCircleMenuItem::OnQueryToolTipsText(LPTSTR pszBuffer, int cchBufferSize, POINT ptLocation, LPCTSTR *ppszToolTips)
{
	if (_toolTips.size() == 0)
		*ppszToolTips = _T("");
	else
		*ppszToolTips = _toolTips.c_str();
}

//////////////////////////////////////////////////////////////////////////

LONG CSTXCirclePopupMenu::_instanceCount = 0;

CSTXCirclePopupMenu::CSTXCirclePopupMenu()
{
	m_AnimationManager.CoCreateInstance(CLSID_UIAnimationManager);
	m_AnimationTimer.CoCreateInstance(CLSID_UIAnimationTimer);
	m_AnimationTransitionLibrary.CoCreateInstance(CLSID_UIAnimationTransitionLibrary);
	m_AnimationManager->SetManagerEventHandler(this);
	_graphicsCacheId = 19999902;
	_mouseLButtonDown = FALSE;
	_windowId = 0;
	_autoDismiss = TRUE;
	_dismissed = FALSE;
	_hwndToolTips = NULL;
	_mouseEnter = FALSE;
}


CSTXCirclePopupMenu::~CSTXCirclePopupMenu()
{
}

LRESULT CALLBACK CSTXCirclePopupMenu::STXCircleMenuWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CSTXCirclePopupMenu *pThis = (CSTXCirclePopupMenu*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (pThis == NULL)
		return DefWindowProc(hwnd, uMsg, wParam, lParam);

	LPARAM pos = GetMessagePos();
	POINT ptScreen = { GET_X_LPARAM(pos), GET_Y_LPARAM(pos) };
	ScreenToClient(hwnd, &ptScreen);
	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_CHAR:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MOUSEMOVE:
	case WM_SETFOCUS:
	case WM_SETCURSOR:
	case WM_KILLFOCUS:
		//case WM_IME_STARTCOMPOSITION:
		//case WM_IME_COMPOSITION:
		//case WM_IME_ENDCOMPOSITION:
		//case WM_IME_CHAR:
		//pThis->OnPreWndProc(ptScreen.x, ptScreen.y, hwnd, uMsg, wParam, lParam);
		break;
	default:
		break;
	}
	//LRESULT res = pThis->OnPreWndProc(GET_X_LPARAM(pos), GET_Y_LPARAM(pos), hwnd, uMsg, wParam, lParam);

	//if (res != S_FALSE)
	//	return res;

	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		//pThis->OnPaint(hdc);
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_TIMER:
		pThis->OnTimer(static_cast<UINT>(wParam));
		break;

	case WM_DESTROY:
		pThis->OnDestroy();
		break;

	case WM_MOUSEMOVE:
		pThis->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
		break;
	case WM_MOUSELEAVE:
		pThis->OnMouseLeave();
		break;
	case WM_LBUTTONDOWN:
		pThis->OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
		break;
	case WM_LBUTTONUP:
		pThis->OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
		break;
	case WM_NOTIFY:
		pThis->OnNotify((UINT)wParam, (NMHDR*)lParam);
		break;

	case WM_MOUSEACTIVATE:
		return MA_NOACTIVATE;

	/*
	case WM_RBUTTONDOWN:
		pThis->OnRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
		break;
	case WM_RBUTTONUP:
		pThis->OnRButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
		break;
	case WM_LBUTTONDBLCLK:
		pThis->OnLButtonDblClk(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
		break;

	case WM_SIZE:
		pThis->OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_HSCROLL:
		pThis->OnHScroll(LOWORD(wParam), HIWORD(wParam), (HWND)lParam);
		break;
	case WM_VSCROLL:
		pThis->OnVScroll(LOWORD(wParam), HIWORD(wParam), (HWND)lParam);
		break;
	case WM_SETCURSOR:
		if (pThis->OnSetCursor((HWND)wParam, LOWORD(lParam), HIWORD(lParam)))
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	case WM_CTLCOLOREDIT:
		//SetBkMode((HDC)wParam, TRANSPARENT);
		//SetTextColor((HDC)wParam, RGB(255, 0, 0));



		//{
		//	HDC hdc = (HDC)wParam;
		//	SetBkMode(hdc, TRANSPARENT); // Ensure that "static" text doesn't use a solid fill
		//	POINT pt;
		//	pt.x = 0; pt.y = 0;
		//	MapWindowPoints((HWND)lParam, hwnd, &pt, 1);
		//	SetBrushOrgEx(hdc, -pt.x, -pt.y, NULL);
		//}

		////return (LRESULT)GetSysColorBrush(COLOR_HIGHLIGHT);
		//return (LRESULT)GetStockObject(HOLLOW_BRUSH);
		//OutputDebugString(_T("ASDFF\n"));
		break;
	case WM_ERASEBKGND:
		return pThis->OnEraseBackground((HDC)wParam);
		break;
		//case WM_NCPAINT:
		//	{
		//		HDC hdc;
		//		hdc = GetDCEx(hwnd, (HRGN)wParam, DCX_WINDOW | DCX_INTERSECTRGN);
		//		// Paint into this DC 
		//		ReleaseDC(hwnd, hdc);
		//		return NULL;
		//	}
		//	break;

		*/
	}



	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CSTXCirclePopupMenu::CalculateMenuItemLocation(int nMenuItemIndex, CSTXCircleMenuItem *pMenuItem)
{
	RECT rcControl;
	::GetClientRect(_hwndControl, &rcControl);

	int nCount = (int)_menuItems.size();

	int w = rcControl.right - rcControl.left;
	int h = rcControl.bottom - rcControl.top;
	int d = min(w, h);
	int r = (d - _itemWidth - _margins * 2) / 2 ;

	const float pi = 3.14159265358979323846;
	float a = 2 * pi * nMenuItemIndex / nCount;

	int dy = (int)(-cos(a) * r);
	int dx = (int)(sin(a) * r);

	int centerX = w / 2;
	int centerY = h / 2;

	CComPtr<IUIAnimationStoryboard> story;
	m_AnimationManager->CreateStoryboard(&story);

	CComPtr<IUIAnimationTransition> transX;
	//m_AnimationTransitionLibrary->CreateCubicBezierLinearTransition(m_nDefaultAnimationDuration, centerX + dx, 0, 0, 0.5, 1.9, &transX);
	m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration, centerX + dx, &transX);

	story->AddTransition(pMenuItem->_x, transX);

	CComPtr<IUIAnimationTransition> transY;
	//m_AnimationTransitionLibrary->CreateCubicBezierLinearTransition(m_nDefaultAnimationDuration, centerY + dy, 0, 0, 0.5, 1.9, &transY);
	m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration, centerY + dy, &transY);

	story->AddTransition(pMenuItem->_y, transY);

	CComPtr<IUIAnimationTransition> transOpacity;
	m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration, 1, &transOpacity);
	story->AddTransition(pMenuItem->_opacity, transOpacity);

	CComPtr<IUIAnimationTransition> transScale;
	//m_AnimationTransitionLibrary->CreateCubicBezierLinearTransition(m_nDefaultAnimationDuration, 1, 0, 0, 0.5, 1.9, &transScale);
	m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration, 1, &transScale);
	story->AddTransition(pMenuItem->_scale, transScale);

	story->Schedule(GetCurrentTime(), NULL);
}

HWND CSTXCirclePopupMenu::GetSafeHwnd()
{
	return _hwndControl;
}

HRESULT CSTXCirclePopupMenu::OnManagerStatusChanged(UI_ANIMATION_MANAGER_STATUS newStatus, UI_ANIMATION_MANAGER_STATUS previousStatus)
{
	if (newStatus == UI_ANIMATION_MANAGER_BUSY)
	{
		::SetTimer(_hwndControl, STXCPM_TIMER_ID_ANIMATION, 10, NULL);
	}
	else
	{
		::KillTimer(_hwndControl, STXCPM_TIMER_ID_ANIMATION);

		if (_dismissed)
		{
			DestroyWindow();
		}
	}

	return S_OK;
}

UI_ANIMATION_SECONDS CSTXCirclePopupMenu::GetCurrentTime()
{
	UI_ANIMATION_SECONDS secTime;
	m_AnimationTimer->GetTime(&secTime);
	return secTime;
}

void CSTXCirclePopupMenu::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == STXCPM_TIMER_ID_ANIMATION)
	{
		UpdateAnimationManager();
		if (_hwndControl)
		{
			RedrawControl();
		}
	}

}

void CSTXCirclePopupMenu::UpdateAnimationManager()
{
	UI_ANIMATION_UPDATE_RESULT result;
	UI_ANIMATION_SECONDS secTime;
	m_AnimationTimer->GetTime(&secTime);
	m_AnimationManager->Update(secTime, &result);
}

void CSTXCirclePopupMenu::RedrawControl()
{
	RECT rcControl;
	::GetClientRect(_hwndControl, &rcControl);

	int w = rcControl.right - rcControl.left;
	int h = rcControl.bottom - rcControl.top;
	int d = min(w, h);
	int r = (d - _itemWidth - _margins * 2) / 2;
	const float pi = 3.14159265358979323846;

	int centerX = w / 2;
	int centerY = h / 2;

	CSTXLayeredGraphics *pLayeredGraphics = CSTXGraphics::CreateAutoLayeredGraphics(_hwndControl, NULL, w, h, _graphicsCacheId);

	CSTXGraphics *pGraphics = dynamic_cast<CSTXGraphics*>(pLayeredGraphics);
	if (pGraphics)
	{
		pGraphics->BeginDraw();
		CSTXGraphicsBrush *pBrush = pGraphics->CreateSolidBrush(32, 32, 32, 192);
		int nItemIndex = 0;
		for (std::shared_ptr<CSTXCircleMenuItem> item : _menuItems)
		{
			double x;
			item->_x->GetValue(&x);
			double y;
			item->_y->GetValue(&y);
			double opacity;
			item->_opacity->GetValue(&opacity);
			double scale;
			item->_scale->GetValue(&scale);
			double scaleClick;
			item->_scaleClick->GetValue(&scaleClick);
			double directionOpacity;
			item->_directionOpacity->GetValue(&directionOpacity);

			CSTXGraphicsImage *pImage = nullptr;
			
			if (item->_imageSp)
			{
				pImage = pGraphics->CreateDrawingImage(item->_imageSp, _graphicsCacheId + (long long)(IStream*)item->_imageSp);
			}
			else
			{
				pImage = pGraphics->CreateDrawingImage(item->_image.c_str(), _graphicsCacheId + std::hash<std::wstring>()(item->_image));
			}


			//if (_hoverItem == item)
			{
				size_t itemCount = _menuItems.size();
				float a = 2 * pi * nItemIndex / itemCount;

				CSTXGraphicsMatrix *pMatrixOld = pGraphics->GetTransform();

				CSTXGraphicsMatrix *pMatrixNew = pMatrixOld->Clone();
				pMatrixNew->Reset();
				pMatrixNew->Rotate(nItemIndex * 360 / itemCount , Gdiplus::MatrixOrderAppend);
				pMatrixNew->Translate(centerX, centerY, Gdiplus::MatrixOrderAppend);

				pGraphics->SetTransform(pMatrixNew);

				float anglePatch = _itemWidth * 2 * directionOpacity * pi / (itemCount * r);

				a = 0;
				int dy = (int)(-cos(a - anglePatch) * r);
				int dx = (int)(sin(a - anglePatch) * r);
				int dy2 = (int)(-cos(a + anglePatch) * r);
				int dx2 = (int)(sin(a + anglePatch) * r);

				POINT pt[3] = { { 0,0 },{ dx ,dy },{ dx2 , dy2 } };

				CSTXGraphicsBrush *pBrushDirection = pGraphics->CreateSimpleLinearGradientBrush(0, 0, 32, 32, 32, 24, 0 + (dx + dx2) / 2, 0 + (dy + dy2) / 2, 32, 32, 255, 128, 255, 0);

				CSTXGraphicsPolygon *pPolygon = pGraphics->CreatePolygon(pt, 3, TRUE);
				pBrushDirection->SetOpacityFloat(directionOpacity);
				pGraphics->FillPolygon(pPolygon, pBrushDirection);
				pPolygon->Release();

				pBrushDirection->Release();

				pGraphics->SetTransform(pMatrixOld);

				delete pMatrixOld;
				delete pMatrixNew;

			}

			CSTXGraphicsMatrix *pMatrixOld = pGraphics->GetTransform();

			CSTXGraphicsMatrix *pMatrixNew = pMatrixOld->Clone();

			pMatrixNew->Reset();
			pMatrixNew->Translate( -_itemWidth / 2, -_itemWidth / 2, Gdiplus::MatrixOrderAppend);
			pMatrixNew->Scale(scale * scaleClick, scale * scaleClick, Gdiplus::MatrixOrderAppend);
			pMatrixNew->Translate(x, y, Gdiplus::MatrixOrderAppend);
			pMatrixNew->Multiply(pMatrixOld, Gdiplus::MatrixOrderAppend);

			pGraphics->SetTransform(pMatrixNew);


			pBrush->SetOpacityFloat((float)opacity);

			//pGraphics->FillRectangle(0, 0 , static_cast<int>(_itemWidth), _itemWidth, pBrush);
			if (pImage)
			{
				CSTXGraphicsImageFormat imageFormat;
				imageFormat._opacity = opacity;
				pGraphics->DrawImage(0, 0, _itemWidth, _itemWidth, pImage, &imageFormat);
				pImage->Release();
			}



			pGraphics->SetTransform(pMatrixOld);



			delete pMatrixOld;
			delete pMatrixNew;

			nItemIndex++;
		}

		pBrush->Release();
		HRESULT hr = pGraphics->EndDraw();

		pLayeredGraphics->ApplyToLayeredWindow(_hwndControl);

#if _WIN32_WINNT >= 0x0601
		if (hr == D2DERR_RECREATE_TARGET)
		{
			pGraphics->DiscardDeviceDependentResource();
		}
#endif
	}

	delete pGraphics;
}

void CSTXCirclePopupMenu::ApplyHoverItemEffect()
{
	if (_hoverItem && !_dismissed)
	{
		CComPtr<IUIAnimationStoryboard> story;
		m_AnimationManager->CreateStoryboard(&story);

		CComPtr<IUIAnimationTransition> transScale;
		//m_AnimationTransitionLibrary->CreateCubicBezierLinearTransition(m_nDefaultAnimationDuration / 2, 1.3, 0, 0, 0.5, 1.9, &transScale);
		m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration / 2, 1.3, &transScale);
		story->AddTransition(_hoverItem->_scale, transScale);

		CComPtr<IUIAnimationTransition> transDirectionOpacity;
		m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration, 0.8, &transDirectionOpacity);
		story->AddTransition(_hoverItem->_directionOpacity, transDirectionOpacity);

		story->Schedule(GetCurrentTime(), NULL);
	}
}

void CSTXCirclePopupMenu::CleanHoverItemEffect()
{
	if (_hoverItem && !_dismissed)
	{
		CComPtr<IUIAnimationStoryboard> story;
		m_AnimationManager->CreateStoryboard(&story);

		CComPtr<IUIAnimationTransition> transScale;
		//m_AnimationTransitionLibrary->CreateCubicBezierLinearTransition(m_nDefaultAnimationDuration / 2, 1.0, 0, 0, 0.5, 1.9, &transScale);
		m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration / 2, 1.0, &transScale);
		story->AddTransition(_hoverItem->_scale, transScale);

		CComPtr<IUIAnimationTransition> transDirectionOpacity;
		m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration / 2, 0.0, &transDirectionOpacity);
		story->AddTransition(_hoverItem->_directionOpacity, transDirectionOpacity);

		story->Schedule(GetCurrentTime(), NULL);
	}
}

void CSTXCirclePopupMenu::AddMenuItem(IStream *pImage, LPCTSTR lpszToolTips)
{
	std::shared_ptr<CSTXCircleMenuItem> pNewItem(new CSTXCircleMenuItem());

	pNewItem->_imageSp = pImage;
	pNewItem->_toolTips = lpszToolTips ? lpszToolTips : _T("");
	_menuItems.push_back(pNewItem);

	RECT rcControl;
	::GetClientRect(_hwndControl, &rcControl);

	int w = rcControl.right - rcControl.left;
	int h = rcControl.bottom - rcControl.top;

	int centerX = w / 2;
	int centerY = h / 2;

	m_AnimationManager->CreateAnimationVariable(centerX, &pNewItem->_x);
	m_AnimationManager->CreateAnimationVariable(centerY, &pNewItem->_y);
	m_AnimationManager->CreateAnimationVariable(0, &pNewItem->_scale);
	m_AnimationManager->CreateAnimationVariable(0, &pNewItem->_opacity);
	m_AnimationManager->CreateAnimationVariable(1, &pNewItem->_scaleClick);
	m_AnimationManager->CreateAnimationVariable(0, &pNewItem->_directionOpacity);

	int index = 0;
	for (std::shared_ptr<CSTXCircleMenuItem> item : _menuItems)
	{
		CalculateMenuItemLocation(index, item.get());
		index++;
	}
}

void CSTXCirclePopupMenu::RegisterWindowClass()
{
	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = STXCircleMenuWindowProc;
	wc.lpszClassName = _T("CSTXCirclePopupMenu");
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.hCursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));

	RegisterClass(&wc);
}

BOOL CSTXCirclePopupMenu::Create(LPCTSTR lpszWindowText, DWORD dwStyle, int x, int y, int cx, int cy, HWND hWndParent, UINT nID)
{
	HWND hWnd = CreateWindow(_T("CSTXCirclePopupMenu"), lpszWindowText, dwStyle, x, y, cx, cy, hWndParent, (HMENU)NULL, GetModuleHandle(NULL), NULL);
	if (hWnd == NULL)
		return FALSE;

	InterlockedIncrement(&_instanceCount);

	_windowId = nID;
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
	_hwndControl = hWnd;

	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW);

	_hwndToolTips = CreateWindowEx(WS_EX_TOPMOST,
		TOOLTIPS_CLASS,
		NULL,
		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hWnd,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = TTTOOLINFO_V1_SIZE;
	toolInfo.hwnd = hWnd;
	toolInfo.uFlags = TTF_SUBCLASS | TTF_IDISHWND;
	toolInfo.uId = (UINT_PTR)hWnd;
	toolInfo.lpszText = LPSTR_TEXTCALLBACK;
	GetClientRect(hWnd, &toolInfo.rect);
	SendMessage(_hwndToolTips, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);

	return TRUE;
}

void CSTXCirclePopupMenu::OnDestroy()
{
	LONG nInstance = InterlockedDecrement(&_instanceCount);

	if (nInstance == 0)
	{
		CSTXGraphics::ClearCachedGraphicsObjects(_graphicsCacheId);
#if _WIN32_WINNT >= 0x0601
		CSTXD2DGraphics::Clear(_graphicsCacheId);
#endif
	}

	_hwndControl = NULL;
}

void CSTXCirclePopupMenu::DestroyWindow()
{
	::DestroyWindow(_hwndControl);
}

int CSTXCirclePopupMenu::MenuItemHitTest(int x, int y)
{
	POINT pt = { x, y };
	int n = (int)_menuItems.size();
	for (int i = 0; i < n; i++)
	{
		std::shared_ptr<CSTXCircleMenuItem> item = _menuItems[i];
		double x;
		item->_x->GetValue(&x);
		double y;
		item->_y->GetValue(&y);

		RECT rcItem = { x - _itemWidth / 2, y - _itemWidth / 2, x + _itemWidth / 2, y + _itemWidth / 2 };
		if (PtInRect(&rcItem, pt))
		{
			return i;
		}
	}
	return -1;
}

void CSTXCirclePopupMenu::OnMouseMove(int x, int y, UINT nFlags)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = _hwndControl;
	tme.dwFlags = TME_LEAVE;
	TrackMouseEvent(&tme);

	if (!_mouseEnter)
	{
		_mouseEnter = TRUE;
		::SendMessage(_hwndToolTips, TTM_ACTIVATE, FALSE, 0);
		::SendMessage(_hwndToolTips, TTM_ACTIVATE, TRUE, 0);
	}


	std::shared_ptr<CSTXCircleMenuItem> item = MenuItemHitTestInternal(x, y);

	if (item == nullptr)
	{
		if (_hoverItem)
		{
			CleanHoverItemEffect();
			::SendMessage(_hwndToolTips, TTM_ACTIVATE, FALSE, 0);
			::SendMessage(_hwndToolTips, TTM_ACTIVATE, TRUE, 0);
			_hoverItem = nullptr;
		}
	}
	else
	{
		if (_hoverItem != item)
		{
			if (_hoverItem)
			{
				CleanHoverItemEffect();
			}
			double opacity;
			item->_opacity->GetValue(&opacity);

			if (opacity > 0.5)
			{
				_hoverItem = item;
				ApplyHoverItemEffect();
				::SendMessage(_hwndToolTips, TTM_ACTIVATE, FALSE, 0);
				::SendMessage(_hwndToolTips, TTM_ACTIVATE, TRUE, 0);
			}
		}
	}
}

std::shared_ptr<CSTXCircleMenuItem> CSTXCirclePopupMenu::MenuItemHitTestInternal(int x, int y)
{
	int index = MenuItemHitTest(x, y);
	if (index < 0)
	{
		return nullptr;
	}
	else
	{
		return _menuItems[index];
	}
}

void CSTXCirclePopupMenu::OnMouseLeave()
{
	_mouseEnter = FALSE;
	::SendMessage(_hwndToolTips, TTM_ACTIVATE, FALSE, 0);
	::SendMessage(_hwndToolTips, TTM_ACTIVATE, TRUE, 0);
	if (_hoverItem)
	{
		CleanHoverItemEffect();
		_hoverItem = nullptr;
	}
}

void CSTXCirclePopupMenu::OnLButtonDown(int x, int y, UINT nFlags, BOOL bForRButton /*= FALSE*/)
{
	//SetFocus(_hwndControl);
	SetCapture(_hwndControl);
	_mouseLButtonDown = TRUE;
	_ptLButtonDown.x = x;
	_ptLButtonDown.y = y;

	UINT nHitFlags = 0;
	POINT pt = { x, y };
	int nIndex = -1;
	std::shared_ptr<CSTXCircleMenuItem> pHit = MenuItemHitTestInternal(pt.x, pt.y);
	_itemLButtonDown = pHit;

	ApplyClickDownEffect();
}

void CSTXCirclePopupMenu::OnLButtonUp(int x, int y, UINT nFlags, BOOL bForRButton /*= FALSE*/)
{
	ReleaseCapture();

	if (_mouseLButtonDown)
	{
		ApplyClickUpEffect();

		if (abs(_ptLButtonDown.y - y) < 6)
		{
			POINT pt = { x, y };
			std::shared_ptr<CSTXCircleMenuItem> pHit = MenuItemHitTestInternal(pt.x, pt.y);

			if (_autoDismiss)
			{
				DismissMenuItems(pHit);
			}

			SendCommonNotifyMessage(STXCMN_ITEMCLICK, pHit.get(), bForRButton ? 1 : 0);
		}
	}
	_mouseLButtonDown = FALSE;
	_itemLButtonDown = NULL;
}

void CSTXCirclePopupMenu::ApplyClickDownEffect()
{
	if (_itemLButtonDown == NULL || _dismissed)
		return;

	CComPtr<IUIAnimationStoryboard> pStory;
	m_AnimationManager->CreateStoryboard(&pStory);

	CComPtr<IUIAnimationTransition> pTrans;
	m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration / 8, 0.92, &pTrans);
	pStory->AddTransition(_itemLButtonDown->_scaleClick, pTrans);

	pStory->Schedule(GetCurrentTime(), NULL);
}

void CSTXCirclePopupMenu::ApplyClickUpEffect()
{
	if (_itemLButtonDown == NULL || _dismissed)
		return;

	CComPtr<IUIAnimationStoryboard> pStory;
	m_AnimationManager->CreateStoryboard(&pStory);

	CComPtr<IUIAnimationTransition> pTrans;
	m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration / 8, 1.0, &pTrans);
	pStory->AddTransition(_itemLButtonDown->_scaleClick, pTrans);

	pStory->Schedule(GetCurrentTime(), NULL);
}

LRESULT CSTXCirclePopupMenu::SendCommonNotifyMessage(UINT nNotifyCode, CSTXCircleMenuItem *pNode, DWORD_PTR dwNotifySpec)
{
	STXCMNITEM nm;
	nm.hdr.hwndFrom = _hwndControl;
	nm.hdr.idFrom = _windowId;
	nm.hdr.code = nNotifyCode;

	if (pNode)
		nm.dwItemData = pNode->_itemData;
	else
		nm.dwItemData = 0;

	nm.dwNotifySpec = dwNotifySpec;
	nm.pNode = pNode;

	return ::SendMessage(GetParent(_hwndControl), WM_NOTIFY, _windowId, (LPARAM)&nm);
}

void CSTXCirclePopupMenu::DismissMenuItems(std::shared_ptr<CSTXCircleMenuItem> pHighlightItem)
{
	CComPtr<IUIAnimationStoryboard> pStory;
	m_AnimationManager->CreateStoryboard(&pStory);

	for (std::shared_ptr<CSTXCircleMenuItem> item : _menuItems)
	{
		if (item == pHighlightItem)
		{
			CComPtr<IUIAnimationTransition> pTransScale;
			m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration / 2, 2.5, &pTransScale);
			pStory->AddTransition(item->_scale, pTransScale);

			CComPtr<IUIAnimationTransition> pTransOpacity;
			m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration / 2, 0, &pTransOpacity);
			pStory->AddTransition(item->_opacity, pTransOpacity);

			CComPtr<IUIAnimationTransition> pTransDirectionOpacity;
			m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration / 2, 0, &pTransDirectionOpacity);
			pStory->AddTransition(item->_directionOpacity, pTransDirectionOpacity);
		}
		else
		{
			CComPtr<IUIAnimationTransition> pTransScale;
			m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration / 2, 0, &pTransScale);
			pStory->AddTransition(item->_scale, pTransScale);

			CComPtr<IUIAnimationTransition> pTransOpacity;
			m_AnimationTransitionLibrary->CreateSmoothStopTransition(m_nDefaultAnimationDuration / 2, 0, &pTransOpacity);
			pStory->AddTransition(item->_opacity, pTransOpacity);
		}
	}

	_dismissed = TRUE;
	pStory->Schedule(GetCurrentTime(), NULL);

}

void CSTXCirclePopupMenu::SetMenuItemWidthHeight(int widthHeight)
{
	if (_hwndControl == NULL)
	{
		return;
	}

	_itemWidth = widthHeight;

	RECT rcControl;
	::GetClientRect(_hwndControl, &rcControl);

	int w = rcControl.right - rcControl.left;
	int h = rcControl.bottom - rcControl.top;
	int d = min(w, h);


	if (_itemWidth < 8)
		_itemWidth = 8;

	if (_itemWidth > d / 4)
		_itemWidth = d / 4;
}

void CSTXCirclePopupMenu::OnQueryToolTipsText(LPTSTR pszBuffer, int cchBufferSize, POINT ptLocation, LPCTSTR *ppszToolTips)
{
	if (_hoverItem)
	{
		*ppszToolTips = NULL;
		_hoverItem->OnQueryToolTipsText(pszBuffer, cchBufferSize, ptLocation, ppszToolTips);
	}
}

LRESULT CSTXCirclePopupMenu::OnNotify(UINT nID, NMHDR *pNMHDR)
{
	if (pNMHDR->code == TTN_GETDISPINFO)		//TTN_NEEDTEXT is equal to TTN_GETDISPINFO
	{
		LPNMTTDISPINFO pDispInfo = (LPNMTTDISPINFO)pNMHDR;

		POINT ptCursor;
		GetCursorPos(&ptCursor);
		::ScreenToClient(_hwndControl, &ptCursor);

		LPCTSTR lpszToolTips = NULL;
		OnQueryToolTipsText(pDispInfo->szText, 80, ptCursor, &lpszToolTips);
		pDispInfo->lpszText = pDispInfo->szText;

		if (lpszToolTips)
			pDispInfo->lpszText = (LPTSTR)lpszToolTips;

		return 1;
	}

	return 0;
}

BOOL CSTXCirclePopupMenu::IsDestroyed()
{
	return _hwndControl == NULL;
}

void CSTXCirclePopupMenu::Show()
{
	ShowWindow(_hwndControl, SW_SHOWNOACTIVATE);
}
