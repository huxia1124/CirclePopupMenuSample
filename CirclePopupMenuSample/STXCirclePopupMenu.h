#pragma once

#include "framework.h"
#include <atlbase.h>
#include <memory>
#include <vector>
#include <string>

#define STXCMN_ITEMCLICK				1
#define STXCMN_ITEMDBLCLICK				2
#define STXCMN_HOVERITEMCHANGED			5
#define STXCMN_SELECTEDITEMCHANGED		6
#define STXCMN_PREDELETEITEM			7
#define STXCMN_POSTDELETEITEM			8
#define STXCMN_ITEMMOUSEFLOAT			9

class CSTXCircleMenuItem;


typedef struct tagSTXCMNITEM
{
	NMHDR hdr;
	DWORD_PTR dwNotifySpec;
	CSTXCircleMenuItem *pNode;
	DWORD_PTR dwItemData;
}STXCMNITEM, *LPSTXCMNITEM;

class CSTXCircleMenuItem
{
public:
	CSTXCircleMenuItem();

	friend class CSTXCirclePopupMenu;
protected:
	CComPtr<IUIAnimationVariable> _x;
	CComPtr<IUIAnimationVariable> _y;
	CComPtr<IUIAnimationVariable> _opacity;
	CComPtr<IUIAnimationVariable> _scale;
	CComPtr<IUIAnimationVariable> _scaleClick;
	CComPtr<IUIAnimationVariable> _directionOpacity;

	std::wstring _image;
	std::wstring _toolTips;
	CComPtr<IStream> _imageSp;
	DWORD_PTR _itemData;

protected:
	virtual void OnQueryToolTipsText(LPTSTR pszBuffer, int cchBufferSize, POINT ptLocation, LPCTSTR *ppszToolTips);

};


class CSTXCirclePopupMenu : public IUIAnimationManagerEventHandler
{
public:
	CSTXCirclePopupMenu();
	virtual ~CSTXCirclePopupMenu();

public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) { return E_NOTIMPL; }
	virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 1; }
	virtual ULONG STDMETHODCALLTYPE Release(void) { return 1; }

protected:
	CComPtr<IUIAnimationManager> m_AnimationManager;
	CComPtr<IUIAnimationTimer> m_AnimationTimer;
	CComPtr<IUIAnimationTransitionLibrary> m_AnimationTransitionLibrary;

	UI_ANIMATION_SECONDS m_nDefaultAnimationDuration = 0.3;
	int _itemWidth = 28;
	int _margins = 16;

protected:
	static LONG _instanceCount;
	HWND _hwndControl;
	HWND _hwndToolTips;
	std::vector<std::shared_ptr<CSTXCircleMenuItem>> _menuItems;
	UINT _graphicsCacheId;
	std::shared_ptr<CSTXCircleMenuItem> _hoverItem;
	std::shared_ptr<CSTXCircleMenuItem> _itemLButtonDown;
	BOOL _mouseLButtonDown;
	POINT _ptLButtonDown;
	UINT _windowId;			//used to send notification message like a child control
	BOOL _autoDismiss;
	BOOL _dismissed;
	BOOL _mouseEnter;

protected:
	static LRESULT CALLBACK STXCircleMenuWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual HRESULT OnManagerStatusChanged(UI_ANIMATION_MANAGER_STATUS newStatus, UI_ANIMATION_MANAGER_STATUS previousStatus);

	void CalculateMenuItemLocation(int nMenuItemIndex, CSTXCircleMenuItem *pMenuItem);
	UI_ANIMATION_SECONDS GetCurrentTime();
	void UpdateAnimationManager();
	void RedrawControl();
	void ApplyHoverItemEffect();
	void CleanHoverItemEffect();
	std::shared_ptr<CSTXCircleMenuItem> MenuItemHitTestInternal(int x, int y);
	void ApplyClickDownEffect();
	void ApplyClickUpEffect();
	LRESULT SendCommonNotifyMessage(UINT nNotifyCode, CSTXCircleMenuItem *pNode, DWORD_PTR dwNotifySpec);
	void DismissMenuItems(std::shared_ptr<CSTXCircleMenuItem> pHighlightItem);

	void OnTimer(UINT nIDEvent);
	void OnDestroy();
	void OnMouseMove(int x, int y, UINT nFlags);
	void OnMouseLeave();
	void OnLButtonDown(int x, int y, UINT nFlags, BOOL bForRButton = FALSE);
	void OnLButtonUp(int x, int y, UINT nFlags, BOOL bForRButton = FALSE);
	virtual LRESULT OnNotify(UINT nID, NMHDR *pNMHDR);
	virtual void OnQueryToolTipsText(LPTSTR pszBuffer, int cchBufferSize, POINT ptLocation, LPCTSTR *ppszToolTips);

public:
	static void RegisterWindowClass();
	BOOL Create(LPCTSTR lpszWindowText, DWORD dwStyle, int x, int y, int cx, int cy, HWND hWndParent, UINT nID);

	HWND GetSafeHwnd();
	void AddMenuItem(IStream *pImage, LPCTSTR lpszToolTips);
	void DestroyWindow();
	int MenuItemHitTest(int x, int y);
	void SetMenuItemWidthHeight(int widthHeight);
	BOOL IsDestroyed();
	void Show();
};

