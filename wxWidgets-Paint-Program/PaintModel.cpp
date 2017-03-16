#include "PaintModel.h"
#include <algorithm>
#include <wx/dcmemory.h>
#include <iostream>

PaintModel::PaintModel()
{
    mPen = *wxBLACK_PEN;
    mBrush = *wxWHITE_BRUSH;
    mSelectedShape = nullptr;
    loadedMap = false;
	
}

// Draws any shapes in the model to the provided DC (draw context)
void PaintModel::DrawShapes(wxDC& dc, bool showSelection)
{
    if (importMap.IsOk())
        dc.DrawBitmap(importMap, 0, 0);

    
    for (auto& myShape : mShapes)
    {
        myShape->Draw(dc);  // call draw
    }
    
    if (HasSelectedShape())
    {
        mSelectedShape->DrawSelection(dc);
    }
    
}

// Clear the current paint model and start fresh
void PaintModel::New()
{

    // Clear vector
    mShapes.clear();
    
    // Clearing redo and undo Stacks
    auto emptyStack =  std::stack<std::shared_ptr<Command>>();
    mUndoStack.swap(emptyStack);
    emptyStack =  std::stack<std::shared_ptr<Command>>();
    mRedoStack.swap(emptyStack);
    
    
    // Reset pointers
    mActiveCommand = nullptr;
    
    // Reset pen and brush
    mPen = *wxBLACK_PEN;
    mBrush = *wxWHITE_BRUSH;
    
    // Reset Selection
    mSelectedShape = nullptr;

    // Reset importMap
    importMap = wxBitmap();
    
}

// Add a shape to the paint model
void PaintModel::AddShape(std::shared_ptr<Shape> shape)
{
	mShapes.emplace_back(shape);

}

// Remove a shape from the paint model
void PaintModel::RemoveShape(std::shared_ptr<Shape> shape)
{
	auto iter = std::find(mShapes.begin(), mShapes.end(), shape);
	if (iter != mShapes.end())
	{
		mShapes.erase(iter);
	}
}

// Command Functions
bool PaintModel::HasActiveCommand()
{
    return (mActiveCommand != nullptr) ? true : false;
}

void PaintModel::CreateCommand(CommandType commandType, const wxPoint & start)
{
	mActiveCommand = CommandFactory::Create(shared_from_this(), commandType, start);
    
    ClearRedo();
}

void PaintModel::UpdateCommand(const wxPoint & wPoint)  // const causes error?
{
    
	mActiveCommand->Update(wPoint);
}

void PaintModel::Finalize(std::shared_ptr<PaintModel> pModel)
{
    mActiveCommand->Finalize(pModel);
    mUndoStack.push(mActiveCommand);
    mActiveCommand.reset();     // reset the shared pointer
    ClearRedo();    // clear redo
    
}


// Undo and Redo
void PaintModel::Undo()
{
    auto stackTop = mUndoStack.top();
    stackTop->Undo(shared_from_this());
    mRedoStack.push(stackTop);
    mUndoStack.pop();
    
}

void PaintModel::Redo()
{
    auto stackTop = mRedoStack.top();
    stackTop->Redo(shared_from_this());
    mUndoStack.push(stackTop);
    mRedoStack.pop();
}

bool PaintModel::CanUndo()
{
    if (mUndoStack.empty())
        return false;
    else
        return true;
}

bool PaintModel::CanRedo()
{
    if (mRedoStack.empty())
        return false;
    else
        return true;
}

void PaintModel::ClearRedo()
{
    // Clear redo stack
    for (int i = 0; i < int(mRedoStack.size()); i++)
    {
        mRedoStack.pop();
    }
}

// Pen Functions
wxPen PaintModel::GetPen()
{
    return mPen;
}

void PaintModel::SetPen(wxPen pen)
{
    mPen = pen;
}

wxColour PaintModel::GetPenColor()
{
    return mPen.GetColour();
}

void PaintModel::SetPenColor(wxColour colour)
{
    
    mPen.SetColour(colour);
}

int PaintModel::GetPenWidth()
{
    return mPen.GetWidth();
}

void PaintModel::SetPenWidth (int width)
{
    mPen.SetWidth(width);
}


// Brush Functions
wxBrush PaintModel::GetBrush()
{
    return mBrush;
}

void PaintModel::SetBrush(wxBrush brush)
{
    mBrush = brush;
}

wxColour PaintModel::GetBrushColor()
{
    return mBrush.GetColour();
}

void PaintModel::SetBrushColor(wxColour colour)
{
    mBrush.SetColour(colour);
}

// Selection Functions
void PaintModel::SelectShape (const wxPoint& selectionPoint)
{
    // Reset selected shape pointer
    ResetSelectedShape();
    
    for (int i = int(mShapes.size())-1; i >= 0; i--)
    {
        if (mShapes[i] -> Intersects(selectionPoint))
        {
            mSelectedShape = mShapes[i];
            break;
        }
    }
}

void PaintModel::ResetSelectedShape()
{
    mSelectedShape = nullptr;
}

bool PaintModel::HasSelectedShape()
{
    return mSelectedShape != nullptr;
}

bool PaintModel::IntersectsSelectedShape(const wxPoint& mousePoint)
{
    if (mSelectedShape -> Intersects(mousePoint))
        return true;
    else
        return false;
}

std::shared_ptr<Shape> PaintModel::GetSelectedShape()
{
    return mSelectedShape;
}

void PaintModel::FileExport(const std::string& fileName, const wxSize& size)
{
    wxBitmap bitmap;
    // Create the bitmap of the specified wxSize
    bitmap.Create(size);
    
    // Create a memory DC to draw to the bitmap
    wxMemoryDC dc(bitmap);
    // Clear the background color
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    // Draw all the shapes (make sure not the selection!)
    ResetSelectedShape();
    DrawShapes(dc);
    
    // Determine Type
    wxBitmapType fileType;
    std::string typeName = fileName.substr(fileName.size() - 3);
    
    if (typeName.compare("png") == 0)
        fileType = wxBITMAP_TYPE_PNG;
    else if (typeName.compare("bmp") == 0)
        fileType = wxBITMAP_TYPE_BMP;
    else if (typeName.compare("jpg") == 0)
        fileType = wxBITMAP_TYPE_JPEG;
    
    // Write the bitmap with the specified file name and wxBitmapType
    bitmap.SaveFile(fileName, fileType);
    
}
void PaintModel::FileImport(const wxString& fileName)
{
    // Clear out paint model
    New();
    
    // Determine File Type
    wxBitmapType fileType;
    std::string typeName = fileName.ToStdString().substr(fileName.ToStdString().size() - 3);
    if (typeName.compare("png") == 0)
        fileType = wxBITMAP_TYPE_PNG;
    else if (typeName.compare("bmp") == 0)
        fileType = wxBITMAP_TYPE_BMP;
    else if (typeName.compare("jpg") == 0)
        fileType = wxBITMAP_TYPE_JPEG;
    
    // Load File
    importMap.LoadFile(fileName,fileType);
    wxMemoryDC dc(importMap);
    
    // Draw Shapes
    if (importMap.IsOk())
        DrawShapes(dc);

}

