#include "Shape.h"
#include <iostream>

// Abstract Base Class

Shape::Shape(const wxPoint& start)
:mStartPoint(start)
,mEndPoint(start)
,mTopLeft(start)
,mBotRight(start)
{
    mPen = *wxBLACK_PEN;
    mBrush = *wxWHITE_BRUSH;
    mOffset = wxPoint(0, 0);
}

// Tests whether the provided point intersects
// with this shape
bool Shape::Intersects(const wxPoint& point) const
{
    wxPoint topleft;
    wxPoint botright;
    GetBounds(topleft, botright);
    if (point.x >= topleft.x && point.x  <= botright.x &&
        point.y >= topleft.y && point.y <= botright.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Update shape with new provided point
void Shape::Update(const wxPoint& newPoint)
{
    mEndPoint = newPoint;
    
    // For most shapes, we only have two points - start and end
    // So we can figure out the top left/bottom right bounds
    // based on this.
    mTopLeft.x = std::min(mStartPoint.x, mEndPoint.x);
    mTopLeft.y = std::min(mStartPoint.y, mEndPoint.y);
    mBotRight.x = std::max(mStartPoint.x, mEndPoint.x);
    mBotRight.y = std::max(mStartPoint.y, mEndPoint.y);
}

void Shape::Finalize()
{
    // Default finalize doesn't do anything
}

void Shape::GetBounds(wxPoint& topLeft, wxPoint& botRight) const
{
    
    // Normal
    if (mOffset.x == 0 && mOffset.y == 0)
    {
        topLeft = mTopLeft;
        botRight = mBotRight;
    }
    // Handle case of offset (move)
    else
    {
        topLeft = mOffset - topLeft;
        botRight = mOffset + botRight;
    }
    
}

void Shape::SetTopLeft(wxPoint & topLeft)
{
    mTopLeft = topLeft;
}

void Shape::SetBotRight(wxPoint & botRight)
{
    mBotRight = botRight;
}

// Pen and Brush Colors
wxPen Shape::GetPen()
{
    return mPen;
}

void Shape::SetPen(wxPen pen)
{
    mPen = pen;
}

wxColour Shape::GetPenColor()
{
    return mPen.GetColour();
}

void Shape::SetPenColor(wxColour colour)
{
    mPen.SetColour(colour);
}

int Shape::GetPenWidth()
{
    return mPen.GetWidth();
}

void Shape::SetPenWidth (int width)
{
    mPen.SetWidth(width);
}


wxBrush Shape::GetBrush()
{
    return mBrush;
}

void Shape::SetBrush(wxBrush brush)
{
    mBrush = brush;
}

wxColour Shape::GetBrushColor()
{
    return mBrush.GetColour();
}

void Shape::SetBrushColor(wxColour colour)
{
    mBrush.SetColour(colour);
}

// Move Shape
void Shape::SetOffset(const wxPoint& point)
{
    mOffset = point;
    topLeftOffset.x = mOffset.x - mTopLeft.x;
    topLeftOffset.y = mOffset.y - mTopLeft.y;
    botRightOffset.x = mBotRight.x - mOffset.x;
    botRightOffset.y = mBotRight.y - mOffset.y;
    
    // 1) Calculate the offset vector, create some offset vector member variables
    // 2) Handle case of offset in Shape::GetBounds
    // 3) Modify Draw for each shape to handle case of offset and draw selection
    // 4) DrawLines takes an x,y as 3rd and 4th parameters

}

void Shape::ResetOffset()
{
    //mOffset = wxPoint(0,0);
}

const wxPoint& Shape::GetOffset()
{
    return mOffset;
}


// Selection
void Shape::DrawSelection(wxDC& dc)
{
    dc.SetPen(*wxBLACK_DASHED_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    
    wxPoint mSelectTopLeft (mTopLeft.x - 5, mTopLeft.y - 5);
    wxPoint mSelectBotRight(mBotRight.x + 5, mBotRight.y + 5);
    
    // Draw Selection
    dc.DrawRectangle(wxRect(mSelectTopLeft + mOffset, mSelectBotRight + mOffset));
    
}


// RectShape
RectShape::RectShape(const wxPoint& start) : Shape(start)
{
    mStartPoint = start;
}
void RectShape::Draw(wxDC& dc) const
{
    dc.SetPen(mPen);
    dc.SetBrush(mBrush);
    dc.DrawRectangle(wxRect(mTopLeft + mOffset, mBotRight + mOffset));
}

// EllipseShape subclass
EllipseShape::EllipseShape(const wxPoint& start) : Shape(start)
{
    mStartPoint = start;
}
void EllipseShape::Draw(wxDC& dc) const
{
    dc.SetPen(mPen);
    dc.SetBrush(mBrush);
    dc.DrawEllipse(wxRect(mTopLeft + mOffset, mBotRight + mOffset));
}

// LineShape subclass
LineShape::LineShape(const wxPoint& start) : Shape(start)
{
    mStartPoint = start;
}
void LineShape::Draw(wxDC& dc) const
{
    dc.SetPen(mPen);
    dc.SetBrush(mBrush);
    dc.DrawLine(mStartPoint + mOffset, mEndPoint + mOffset);
}

// PencilShape subclass
PencilShape::PencilShape(const wxPoint& start) : Shape(start)
{
    mStartPoint = start;
    pencilVector.push_back(start);
}

void PencilShape::Draw(wxDC& dc) const
{
    if (pencilVector.size() == 1)
        dc.DrawPoint(pencilVector[0]);
    else
        dc.DrawLines(int(pencilVector.size()), pencilVector.data(), mOffset.x, mOffset.y);
}

void PencilShape::Update(const wxPoint& newPoint)
{
    Shape::Update(newPoint);
    pencilVector.push_back(newPoint);
}

void PencilShape::Finalize()
{
    int minX(pencilVector[0].x), minY(pencilVector[0].y);
    int maxX(pencilVector[0].x), maxY(pencilVector[0].y);
    
    for (int i = 1; i < pencilVector.size(); i++)
    {
        if (pencilVector[i].x > maxX)
            maxX = pencilVector[i].x;
        
        if (pencilVector[i].x < minX)
            minX = pencilVector[i].x;
        
        if (pencilVector[i].y > maxY)
            maxY = pencilVector[i].y;
        
        if (pencilVector[i].y < minY)
            minY = pencilVector[i].y;

    }
    
    mTopLeft = wxPoint(minX, minY);
    mBotRight = wxPoint(maxX, maxY);

}


