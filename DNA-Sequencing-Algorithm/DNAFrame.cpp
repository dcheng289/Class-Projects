//
//  DNAFrame.cpp
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "DNAFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/busyinfo.h>
#include "DNADrawPanel.h"
#include "Exceptions.h"
#include "DNAAlignDlg.h"

#include "FileParser.hpp"
#include <iostream>
#include "DNATranslator.hpp"
#include "Timer.h"
#include "NeedlemanWunsch.hpp"

enum
{
	ID_AMINO_HIST=1000,
    ID_PAIRWISE_ALIGN=2000
};

wxBEGIN_EVENT_TABLE(DNAFrame, wxFrame)
	EVT_MENU(wxID_EXIT, DNAFrame::OnExit)
	EVT_MENU(wxID_NEW, DNAFrame::OnNew)
	EVT_MENU(ID_AMINO_HIST, DNAFrame::OnAminoHist)
    EVT_MENU(ID_PAIRWISE_ALIGN, DNAFrame::OnPairwiseAlign)
wxEND_EVENT_TABLE()

DNAFrame::DNAFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(ID_AMINO_HIST, "Amino Acid Histogram...",
					 "Create a histogram from a FASTA file.");
    menuFile->Append(ID_PAIRWISE_ALIGN, "Pairwise Alignment...",
                     "Calculate pairwise alignment between two DNA sequences.");
	menuFile->Append(wxID_EXIT);
	
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	SetMenuBar(menuBar);
	CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new DNADrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	SetSizer(sizer);
	SetAutoLayout(true);
	Show(true);
	
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}

void DNAFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void DNAFrame::OnNew(wxCommandEvent& event)
{
	// Doesn't do anything...
}

void DNAFrame::OnAminoHist(wxCommandEvent& event) 	// (File>Amino Acid Histogram...)
{
    // Create Dialog Box
     wxFileDialog openDialog(this, _("Open FASTA File"), "./data", "",
                "FASTA files (*.fasta)|*.fasta", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    
    if (openDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }
    
    std::string filePath = openDialog.GetPath().ToStdString();
    try
    {
        FileParser fp (filePath);
        
        DNATranslator translator(fp.GetSequence());
        mPanel->SetAminoData(fp.GetHeader(), translator.GetAminoCounter(),
                        translator.GetCount(), translator.GetMaxAminoAcidCount());
        mPanel->PaintNow();
    }
    catch (FileLoadExcept fle)
    {
        wxMessageBox("Invalid FASTA File", "Error", wxOK | wxICON_ERROR);
    }
    
}

void DNAFrame::OnPairwiseAlign(wxCommandEvent& event)
{
    DNAAlignDlg dnaAlign;
    if(dnaAlign.ShowModal() == wxID_CANCEL)
    {
        return;
    }
    
    wxBusyInfo wait("Calculating pairwise match...", this);
    
    Timer t;
    t.Start();
    
    NeedlemanWunsch nw(dnaAlign.GetInputAPath(), dnaAlign.GetInputBPath());
    nw.RunAlgorithm();
    nw.SaveFile(dnaAlign.GetOutputPath());
    
    wxMessageBox("Needleman-Wunsch Algorithm took " + std::to_string(t.GetElapsed()) + "s!", "Time", wxOK | wxICON_INFORMATION);
    
}
