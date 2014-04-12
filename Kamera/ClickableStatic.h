#pragma once


// ClickableStatic

class ClickableStatic : public CStatic
{
	DECLARE_DYNAMIC(ClickableStatic)

public:
	ClickableStatic();
	virtual ~ClickableStatic();
	CPoint start_pos;
	CRect last_rect, actual_rect;
	bool last_rect_serviced;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


