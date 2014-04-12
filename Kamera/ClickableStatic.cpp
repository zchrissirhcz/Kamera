// ClickableStatic.cpp : implementation file
//

#include "stdafx.h"
#include "Kamera.h"
#include "ClickableStatic.h"


// ClickableStatic

IMPLEMENT_DYNAMIC(ClickableStatic, CStatic)

ClickableStatic::ClickableStatic()
{
	last_rect_serviced = true;
	last_rect.SetRect(-1, -1, -1, -1);
	start_pos.SetPoint(-1, -1);
	actual_rect.SetRect(-1, -1, -1, -1);

}

ClickableStatic::~ClickableStatic()
{
}


BEGIN_MESSAGE_MAP(ClickableStatic, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// ClickableStatic message handlers




void ClickableStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CStatic::OnMouseMove(nFlags, point);

	if (nFlags & MK_LBUTTON)
	{
		actual_rect.SetRect(start_pos, point);
		actual_rect.NormalizeRect();
	}
}


void ClickableStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CStatic::OnLButtonDown(nFlags, point);
	actual_rect.SetRect(-1, -1, -1, -1);
	start_pos = point;
}


void ClickableStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CStatic::OnLButtonUp(nFlags, point);
	last_rect.SetRect(start_pos, point);
	last_rect.NormalizeRect();
	actual_rect.SetRect(-1, -1, -1, -1);
	last_rect_serviced = false;
}
