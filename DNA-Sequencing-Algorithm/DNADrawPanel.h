//
//  DNADrawPanel.h
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <string>
#include <memory>
#include <map>

class DNADrawPanel : public wxPanel
{
public:
	DNADrawPanel(wxFrame* parent);
	void PaintNow();
    void SetAminoData(const std::string& header, std::map<char, int> map, const int count, const int max);
    void SetColor(wxDC& dc, std::string color);
    void DrawBar(wxDC&, const char& letter, const std::string& amino, const int height);

protected:
	void PaintEvent(wxPaintEvent & evt);
	void Render(wxDC& dc);
 
	DECLARE_EVENT_TABLE()
	
public:
	// Variables here
    std::map<char, int> mAminoCounter;
    std::string mHeader;
    int mCount;
    int mMax;
    const int mBarHeight = 30;
    const int mBarLength = 500;
};

