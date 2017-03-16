#include "Command.h"
#include "Shape.h"
#include "PaintModel.h"
#include <iostream>

Command::Command(const wxPoint& start, std::shared_ptr<Shape> shape)
	:mStartPoint(start)
	,mEndPoint(start)
	,mShape(shape)
{

}

Command::Command()
{
    
}

// Called when the command is still updating (such as in the process of drawing)
void Command::Update(const wxPoint& newPoint)
{
	mEndPoint = newPoint;
}

std::shared_ptr<Command> CommandFactory::Create(std::shared_ptr<PaintModel> model,
	CommandType type, const wxPoint& start)
{
	std::shared_ptr<Command> retVal;

    
    // create DrawCommand shared_ptr
    // Shape shared_ptr
    
    switch (type)
    {
        case CM_DrawRect:
        {
            std::shared_ptr<RectShape> rShape = std::make_shared<RectShape>(start);
            rShape->SetPen(model->GetPen());
            rShape->SetBrush(model->GetBrush());
            retVal = std::make_shared<DrawCommand>(start, rShape);
            model->AddShape(rShape);
            
            break;
        }
        case CM_DrawEllipse:
        {
            std::shared_ptr<EllipseShape> eShape = std::make_shared<EllipseShape>(start);
            eShape->SetPen(model->GetPen());
            eShape->SetBrush(model->GetBrush());
            retVal = std::make_shared<DrawCommand>(start, eShape);
            model->AddShape(eShape);
            break;
            
        }
        case CM_DrawLine:
        {
            std::shared_ptr<LineShape> lShape = std::make_shared<LineShape>(start);
            lShape->SetPen(model->GetPen());
            lShape->SetBrush(model->GetBrush());
            retVal = std::make_shared<DrawCommand>(start, lShape);
            model->AddShape(lShape);
            break;
            
        }
        case CM_DrawPencil:
        {
            std::shared_ptr<PencilShape> pShape = std::make_shared<PencilShape>(start);
            pShape->SetPen(model->GetPen());
            pShape->SetBrush(model->GetBrush());
            model->AddShape(pShape);
            retVal = std::make_shared<DrawCommand>(start, pShape);
            break;
            
        }
        case CM_SetPen:
        {
            auto selectShape = model->GetSelectedShape();
            auto selectComm = std::make_shared<SetPenCommand>(model);
            
            selectComm -> SetShape(selectShape);
            selectComm -> SetOldPen(selectShape->GetPen());
            selectComm -> SetNewPen(model->GetPen());
            retVal = selectComm;
            break;
        }
        case CM_SetBrush:
        {
            auto selectShape = model->GetSelectedShape();
            auto selectComm = std::make_shared<SetBrushCommand>(model);
            
            selectComm -> SetShape(selectShape);
            selectComm -> SetOldBrush(selectShape->GetBrush());
            selectComm -> SetNewBrush(model->GetBrush());
            retVal = selectComm;
            break;
        }
        case CM_Delete:
        {
            auto selectShape = model->GetSelectedShape();
            auto selectComm = std::make_shared<DeleteCommand>(model);
            selectComm -> SetDeletedShape(selectShape);
            retVal = selectComm;
            break;
            
        }
        case CM_Move:
        {
            auto selectShape = model->GetSelectedShape();
            auto selectComm = std::make_shared<MoveCommand>(model);
            
            wxPoint topLeft, botRight;
            selectShape->GetBounds(topLeft, botRight);
            
            // Undo/Redo stuff
            selectComm -> SetOffset(selectShape->GetOffset());
            selectComm -> SetOldPoints(topLeft, botRight);
            selectComm -> SetNewPoints(topLeft, botRight);
            selectComm -> SetMoveShape(selectShape);
            
            retVal = selectComm;
            break;
            
        }
    }

	return retVal;
}


// DrawCommand subclass
DrawCommand::DrawCommand(const wxPoint& start, std::shared_ptr<Shape> shape) : Command(start, shape)
{
    mStartPoint = start;
    mEndPoint = start;
    mShape = shape;
}

void DrawCommand::Update(const wxPoint& newPoint)
{
    Command::Update(newPoint);
    
    mShape->Update(newPoint);
}

void DrawCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    mShape->Finalize();
    }


void DrawCommand::Undo(std::shared_ptr<PaintModel> model)
{
    model->RemoveShape(mShape);
    model->ResetSelectedShape();
}

void DrawCommand::Redo(std::shared_ptr<PaintModel> model)
{
    model->AddShape(mShape);
}


// SetPenCommand subclass
SetPenCommand::SetPenCommand(std::shared_ptr<PaintModel> model)
{
    
}

void SetPenCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    mShape->SetPen(mNewPen);
    mShape->Finalize();
}
void SetPenCommand::Undo(std::shared_ptr<PaintModel> model)
{
    mShape->SetPen(mOldPen);
}
void SetPenCommand::Redo(std::shared_ptr<PaintModel> model)
{
    mShape->SetPen(mNewPen);
}
void SetPenCommand::SetOldPen (const wxPen& pen)
{
    mOldPen = pen;
}
void SetPenCommand::SetNewPen(const wxPen& pen)
{
    mNewPen = pen;
}
void SetPenCommand::SetShape (std::shared_ptr<Shape> shape)
{
    mShape = shape;
}

// SetBrushCommand subclass
SetBrushCommand::SetBrushCommand(std::shared_ptr<PaintModel> model)
{
    
}

void SetBrushCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    mShape->SetBrush(mNewBrush);
    mShape->Finalize();
}
void SetBrushCommand::Undo(std::shared_ptr<PaintModel> model)
{
    mShape->SetBrush(mOldBrush);
}
void SetBrushCommand::Redo(std::shared_ptr<PaintModel> model)
{
    mShape->SetBrush(mNewBrush);
}
void SetBrushCommand::SetOldBrush(const wxBrush &brush)
{
    mOldBrush = brush;
}
void SetBrushCommand::SetNewBrush(const wxBrush &brush)
{
    mNewBrush = brush;
}
void SetBrushCommand::SetShape (std::shared_ptr<Shape> shape)
{
    mShape = shape;
}

// DeleteCommand subclass
DeleteCommand::DeleteCommand(std::shared_ptr<PaintModel> model)
{
    model->RemoveShape(mDeletedShape);
    model->ResetSelectedShape();
}
void DeleteCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    model->RemoveShape(mDeletedShape);
    model->ResetSelectedShape();
}
void DeleteCommand::Undo(std::shared_ptr<PaintModel> model)
{
	model->AddShape(mDeletedShape);
}
void DeleteCommand::Redo(std::shared_ptr<PaintModel> model)
{
 	model->RemoveShape(mDeletedShape);
}

void DeleteCommand::SetDeletedShape (std::shared_ptr<Shape> shape)
{
    mDeletedShape = shape;
}


// MoveCommand subclass
MoveCommand::MoveCommand(std::shared_ptr<PaintModel> model)
{
        
}
void MoveCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    mMoveShape->Finalize();
    mMoveShape->GetBounds(mNewTopLeft, mNewBotRight);
}
void MoveCommand::Undo(std::shared_ptr<PaintModel> model)
{
    mMoveShape -> SetOffset(mOffset);
    mMoveShape -> SetTopLeft(mOldTopLeft);
    mMoveShape -> SetBotRight(mOldBotRight);
}
void MoveCommand::Redo(std::shared_ptr<PaintModel> model)
{
    mMoveShape -> SetOffset(mOffset);
    mMoveShape -> SetTopLeft(mNewTopLeft);
    mMoveShape -> SetBotRight(mNewBotRight);
    
}
void MoveCommand::Update(const wxPoint& newPoint)
{
    mMoveShape -> SetOffset(newPoint - mOldTopLeft);
}
    
void MoveCommand::SetMoveShape(std::shared_ptr<Shape>& shape)
{
    mMoveShape = shape;
}
void MoveCommand::SetOldPoints(wxPoint& topLeft, wxPoint& botRight)
{
    mOldTopLeft = topLeft;
    mOldBotRight = botRight;
}
void MoveCommand::SetNewPoints(wxPoint& topLeft, wxPoint& botRight)
{
    mNewTopLeft = topLeft + mOffset;
    mNewBotRight = botRight + mOffset;
}

void MoveCommand::SetOffset(const wxPoint& point)
{
    mOffset = point;
}
