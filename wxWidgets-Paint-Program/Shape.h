#pragma once
#include <wx/dc.h>

// Abstract base class for all Shapes
class Shape
{
public:
	Shape(const wxPoint& start);
	// Tests whether the provided point intersects
	// with this shape
	bool Intersects(const wxPoint& point) const;
	// Update shape with new provided point
	virtual void Update(const wxPoint& newPoint);
	// Finalize the shape -- when the user has finished drawing the shape
	virtual void Finalize();
	// Returns the top left/bottom right points of the shape
	void GetBounds(wxPoint& topLeft, wxPoint& botRight) const;
    
    // set top left and bottom right
    void SetTopLeft(wxPoint& topLeft);
    void SetBotRight(wxPoint& botRight);
    
	// Draw the shape
	virtual void Draw(wxDC& dc) const = 0;
	virtual ~Shape() { }
    
    // Pen Functions
    wxPen GetPen();
    void SetPen(wxPen pen);
    wxColour GetPenColor();
    void SetPenColor(wxColour colour);
    int GetPenWidth();
    void SetPenWidth (int width);
    
    // Brush Functions
    wxBrush GetBrush();
    void SetBrush(wxBrush brush);
    wxColour GetBrushColor();
    void SetBrushColor(wxColour colour);
    
    // Selection
    void DrawSelection(wxDC& dc);
    
    // Move
    void SetOffset(const wxPoint& point);
    void ResetOffset();
    const wxPoint& GetOffset();
    
    
protected:
	// Starting point of shape
	wxPoint mStartPoint;
	// Ending point of shape
	wxPoint mEndPoint;
	// Top left point of shape
	wxPoint mTopLeft;
	// Bottom right point of shape
	wxPoint mBotRight;
    
    // Pen and Brush
    wxPen mPen;
    wxBrush mBrush;
    
    // Move
    wxPoint mOffset;
    wxPoint topLeftOffset, botRightOffset;
    
    
};

// RectShape subclass
class RectShape : public Shape
{
public:
    RectShape(const wxPoint& start);
    void Draw(wxDC& dc) const override; // should be an override
};


// EllipseShape subclass
class EllipseShape : public Shape
{
public:
    EllipseShape(const wxPoint& start);
    void Draw(wxDC& dc) const override; // should be an override
};

// LineShape subclass
class LineShape : public Shape
{
public:
    LineShape(const wxPoint& start);
    void Draw(wxDC& dc) const override; // should be an override
};

// PencilShape subclass
class PencilShape : public Shape
{
public:
    PencilShape(const wxPoint& start);
    void Draw(wxDC& dc) const override; // should be an override
    void Update(const wxPoint& newPoint) override;
    void Finalize() override;
protected:
    std::vector<wxPoint> pencilVector;
    
};
