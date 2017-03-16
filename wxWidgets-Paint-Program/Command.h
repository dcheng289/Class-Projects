#pragma once
#include <wx/gdicmn.h>
#include <wx/pen.h>
#include <wx/brush.h>
#include <memory>
#include "Shape.h"

// Abstract Base Class
enum CommandType
{
	CM_DrawLine,
	CM_DrawEllipse,
	CM_DrawRect,
	CM_DrawPencil,
	CM_Move,
	CM_Delete,
	CM_SetPen,
	CM_SetBrush,
};

// Forward declarations
class PaintModel;
class Shape;

// Abstract Base Command class
// All actions that change the drawing (drawing, deleting, etc., are commands)
class Command
{
public:
	Command(const wxPoint& start, std::shared_ptr<Shape> shape);
    Command();
	// Called when the command is still updating (such as in the process of drawing)
	virtual void Update(const wxPoint& newPoint);
	// Called when the command is completed
	virtual void Finalize(std::shared_ptr<PaintModel> model) = 0;
	// Used to "undo" the command
	virtual void Undo(std::shared_ptr<PaintModel> model) = 0;
	// Used to "redo" the command
	virtual void Redo(std::shared_ptr<PaintModel> model) = 0;
	virtual ~Command() { }
protected:
	wxPoint mStartPoint;
	wxPoint mEndPoint;
	std::shared_ptr<Shape> mShape;
};

// Factory method to help create a particular command
struct CommandFactory
{
	static std::shared_ptr<Command> Create(std::shared_ptr<PaintModel> model,
		CommandType type, const wxPoint& start);
    
    // create DrawCommand shared_ptr
    // Shape shared_ptr
};

// DrawCommand subclass
class DrawCommand : public Command
{
public:
    DrawCommand(const wxPoint& start, std::shared_ptr<Shape> shape);
    void Finalize(std::shared_ptr<PaintModel> model) override;
    void Undo(std::shared_ptr<PaintModel> model) override;
    void Redo(std::shared_ptr<PaintModel> model) override;
    void Update(const wxPoint& newPoint) override;
};


// SetPenCommand subclass
class SetPenCommand : public Command
{
public:
    SetPenCommand(std::shared_ptr<PaintModel> model);
    void Finalize(std::shared_ptr<PaintModel> model) override;
    void Undo(std::shared_ptr<PaintModel> model) override;
    void Redo(std::shared_ptr<PaintModel> model) override;
    
    void SetOldPen (const wxPen& pen);
    void SetNewPen (const wxPen& pen);
    void SetShape (std::shared_ptr<Shape> shape);
    
private:
    wxPen mOldPen;
    wxPen mNewPen;
    
};

// SetBrushCommand subclass
class SetBrushCommand : public Command
{
public:
    SetBrushCommand(std::shared_ptr<PaintModel> model);
    void Finalize(std::shared_ptr<PaintModel> model) override;
    void Undo(std::shared_ptr<PaintModel> model) override;
    void Redo(std::shared_ptr<PaintModel> model) override;
    
    void SetOldBrush (const wxBrush& brush);
    void SetNewBrush (const wxBrush& brush);
    void SetShape (std::shared_ptr<Shape> shape);
    
private:
    wxBrush mOldBrush;
    wxBrush mNewBrush;
    
};

// DeleteCommand subclass
class DeleteCommand : public Command
{
public:
    DeleteCommand(std::shared_ptr<PaintModel> model);
    void Finalize(std::shared_ptr<PaintModel> model) override;
    void Undo(std::shared_ptr<PaintModel> model) override;
    void Redo(std::shared_ptr<PaintModel> model) override;
    
    void SetDeletedShape (std::shared_ptr<Shape> shape);
    
private:
    std::shared_ptr<Shape> mDeletedShape;
};

// MoveCommand subclass
class MoveCommand : public Command
{
public:
    MoveCommand(std::shared_ptr<PaintModel> model);
    void Finalize(std::shared_ptr<PaintModel> model) override;
    void Undo(std::shared_ptr<PaintModel> model) override;
    void Redo(std::shared_ptr<PaintModel> model) override;
    void Update(const wxPoint& newPoint) override;
    
    void SetMoveShape(std::shared_ptr<Shape>& shape);
    void SetOldPoints(wxPoint& topLeft, wxPoint& botRight);
    void SetNewPoints(wxPoint& topLeft, wxPoint& botRight);
    void SetOffset(const wxPoint& point);

private:
    std::shared_ptr<Shape> mMoveShape;
    wxPoint mOldTopLeft, mOldBotRight, mNewTopLeft, mNewBotRight;
    wxPoint mOffset;
};
