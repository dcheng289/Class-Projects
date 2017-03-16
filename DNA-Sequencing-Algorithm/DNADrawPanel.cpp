//
//  DNADrawPanel.cpp
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "DNADrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <array>
#include <sstream>
#include <iomanip>

BEGIN_EVENT_TABLE(DNADrawPanel, wxPanel)
	EVT_PAINT(DNADrawPanel::PaintEvent)
END_EVENT_TABLE()

DNADrawPanel::DNADrawPanel(wxFrame* parent)
: wxPanel(parent)
{
	
}

void DNADrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void DNADrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
}

void DNADrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	// Draw Histogram
    if (mCount > 0)
    {
        // Header
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawText(wxString(mHeader), 30, 30);
        
        // Histogram
        dc.SetPen(*wxRED_PEN);
        dc.SetTextForeground(*wxRED);
        dc.SetBrush(*wxRED_BRUSH);
        DrawBar(dc, 'A', "Alanine", 60);
        
        dc.SetPen(*wxBLUE_PEN);
        dc.SetTextForeground(*wxBLUE);
        dc.SetBrush(*wxBLUE_BRUSH);
        DrawBar(dc, 'C', "Cysteine", 90);
        
        dc.SetPen(*wxGREEN_PEN);
        dc.SetTextForeground(*wxGREEN);
        dc.SetBrush(*wxGREEN_BRUSH);
        DrawBar(dc, 'D', "Aspartic Acid", 120);
        
        dc.SetPen(*wxRED_PEN);
        dc.SetTextForeground(*wxRED);
        dc.SetBrush(*wxRED_BRUSH);
        DrawBar(dc, 'E', "Glutamic Acid", 150);
        
        dc.SetPen(*wxBLUE_PEN);
        dc.SetTextForeground(*wxBLUE);
        dc.SetBrush(*wxBLUE_BRUSH);
        DrawBar(dc, 'F', "Phenylalanine", 180);
        
        dc.SetPen(*wxGREEN_PEN);
        dc.SetTextForeground(*wxGREEN);
        dc.SetBrush(*wxGREEN_BRUSH);
        DrawBar(dc, 'G', "Glycine", 210);
        
        dc.SetPen(*wxRED_PEN);
        dc.SetTextForeground(*wxRED);
        dc.SetBrush(*wxRED_BRUSH);
        DrawBar(dc, 'H', "Histidine", 240);
        
        dc.SetPen(*wxBLUE_PEN);
        dc.SetTextForeground(*wxBLUE);
        dc.SetBrush(*wxBLUE_BRUSH);
        DrawBar(dc, 'I', "Isoleucine", 270);
        
        dc.SetPen(*wxGREEN_PEN);
        dc.SetTextForeground(*wxGREEN);
        dc.SetBrush(*wxGREEN_BRUSH);
        DrawBar(dc, 'K', "Lysine", 300);
        
        dc.SetPen(*wxRED_PEN);
        dc.SetTextForeground(*wxRED);
        dc.SetBrush(*wxRED_BRUSH);
        DrawBar(dc, 'L', "Leucine", 330);
        
        dc.SetPen(*wxBLUE_PEN);
        dc.SetTextForeground(*wxBLUE);
        dc.SetBrush(*wxBLUE_BRUSH);
        DrawBar(dc, 'M', "Methionine", 360);
        
        dc.SetPen(*wxGREEN_PEN);
        dc.SetTextForeground(*wxGREEN);
        dc.SetBrush(*wxGREEN_BRUSH);
        DrawBar(dc, 'N', "Asparagine", 390);
        
        dc.SetPen(*wxRED_PEN);
        dc.SetTextForeground(*wxRED);
        dc.SetBrush(*wxRED_BRUSH);
        DrawBar(dc, 'P', "Proline", 420);
        
        dc.SetPen(*wxBLUE_PEN);
        dc.SetTextForeground(*wxBLUE);
        dc.SetBrush(*wxBLUE_BRUSH);
        DrawBar(dc, 'Q', "Glutamine", 450);
        
        dc.SetPen(*wxGREEN_PEN);
        dc.SetTextForeground(*wxGREEN);
        dc.SetBrush(*wxGREEN_BRUSH);
        DrawBar(dc, 'A', "Arginine", 480);
        
        dc.SetPen(*wxRED_PEN);
        dc.SetTextForeground(*wxRED);
        dc.SetBrush(*wxRED_BRUSH);
        DrawBar(dc, 'S', "Serine", 510);
        
        dc.SetPen(*wxBLUE_PEN);
        dc.SetTextForeground(*wxBLUE);
        dc.SetBrush(*wxBLUE_BRUSH);
        DrawBar(dc, 'T', "Threonine", 540);
        
        dc.SetPen(*wxGREEN_PEN);
        dc.SetTextForeground(*wxGREEN);
        dc.SetBrush(*wxGREEN_BRUSH);
        DrawBar(dc, 'V', "Valine", 570);
        
        dc.SetPen(*wxRED_PEN);
        dc.SetTextForeground(*wxRED);
        dc.SetBrush(*wxRED_BRUSH);
        DrawBar(dc, 'W', "Tryptophan", 600);
        
        dc.SetPen(*wxBLUE_PEN);
        dc.SetTextForeground(*wxBLUE);
        dc.SetBrush(*wxBLUE_BRUSH);
        DrawBar(dc, 'Y', "Tyrosine", 630);
    }
    
}

void DNADrawPanel::SetAminoData(const std::string& header, std::map<char, int> map, const int count, const int max)
{
    mAminoCounter = map;
    mHeader = header;
    mCount = count;
    mMax = max;
}

void DNADrawPanel::DrawBar(wxDC& dc, const char& letter, const std::string& amino, const int height)
{

    int count = mAminoCounter[letter];
    float percentage = static_cast<float>(count)/mCount*100;
    
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << percentage;
    std::string text = amino + ": " + ss.str() + "% (" + std::to_string(count) + ")";
    
    wxString wText = wxString(text);
    dc.DrawText(wText, 30, height);
    dc.DrawRectangle(250, height, mBarLength*count/mMax, 30);
    
}
