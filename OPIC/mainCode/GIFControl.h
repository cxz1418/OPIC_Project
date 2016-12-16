#ifndef __LINA_BOT_GIF_CONTROL_H__

#include <vector>
#include<Windows.h>
#include<GdiPlus.h>
using namespace Gdiplus;


//The Control
class CGIFControl : public CStatic
{
public:
	//CTOR & DTOR
	CGIFControl();
	virtual ~CGIFControl();

	//
	BOOL Create(CRect& rc,CWnd* parent ,UINT nID);

	//Load gif image
	void Load(LPCTSTR sFileName);

	//Start play
	void Play();

	//Stop play
	void Stop();

	//Set border prop
	void SetBorderRound(Color& c,int iLineWidth = 1);

	//Set if show border
	void EnableBorder(BOOL bEnable = TRUE);

	//Url going when click
	void SetURL(LPCTSTR sURL);

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();

	void OnTimer(UINT_PTR nIDEvent);
	void OnLButtonDown(UINT nFlags,Point point);
	void OnMouseMove(UINT nFlags,Point point);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	bool OnEraseBkgnd(CDC* pDC);

protected:
	void DrawBorder(Graphics& g);


protected:
	Image* m_pImage;
	GUID* m_pDimensionIDs;
	UINT m_FrameCount;
	PropertyItem* m_pItem;
	UINT m_iCurrentFrame;

	String m_URL;

	Color m_cBorder;
	int m_iBorderLineWidth;
	bool m_bBorderEnable;

	HCURSOR m_hHandCursor;
	HCURSOR m_hOldCursor;
	bool m_bTrackOnce;

	//
	bool m_bIsPlaying;

protected:
};

#endif
