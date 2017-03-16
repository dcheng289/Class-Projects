#pragma once
#include <memory>
#include <vector>
#include "Shape.h"
#include "Command.h"
#include <wx/bitmap.h>
#include <stack>

class PaintModel : public std::enable_shared_from_this<PaintModel>
{
public:
	PaintModel();
	
	// Draws any shapes in the model to the provided DC (draw context)
	void DrawShapes(wxDC& dc, bool showSelection = true);

	// Clear the current paint model and start fresh
	void New();

	// Add a shape to the paint model
	void AddShape(std::shared_ptr<Shape> shape);
	// Remove a shape from the paint model
	void RemoveShape(std::shared_ptr<Shape> shape);
    
    // Command functions
    bool HasActiveCommand();
    void CreateCommand(CommandType commandType, const wxPoint & start);
    void UpdateCommand(const wxPoint & wPoint);
    void Finalize(std::shared_ptr<PaintModel> pModel);
    
    // Undo and Redo
    void Undo();
    void Redo();
    bool CanUndo();
    bool CanRedo();
    void ClearRedo();
    
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
    
    // Selection Functions
    void SelectShape (const wxPoint& selectionPoint);
    void ResetSelectedShape();
    bool HasSelectedShape();
	bool IntersectsSelectedShape(const wxPoint& mousePoint);
    std::shared_ptr<Shape> GetSelectedShape();
    
    // Export/Import
    void FileExport(const std::string &fileName, const wxSize& size);
    void FileImport(const wxString& fileName);
    
    
private:
	// Vector of all the shapes in the model
	std::vector<std::shared_ptr<Shape>> mShapes;
    std::shared_ptr<Command> mActiveCommand;
    
    // Undo and Redo Stacks
    std::stack<std::shared_ptr<Command>> mUndoStack;
    std::stack<std::shared_ptr<Command>> mRedoStack;
    
    // Pen and Brush
    wxPen mPen;
    wxBrush mBrush;
    
    // Selections
    std::shared_ptr<Shape> mSelectedShape;
    
    // Import map
    wxBitmap importMap;
    bool loadedMap;
};
