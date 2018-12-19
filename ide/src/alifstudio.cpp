// ==============================
//  Alif Studio
// (C)2018 DRAGA Hassan.
// www.aliflang.org
// ------------------------------
// The Alif Arabic Programming Language
// ------------------------------
// ==============================

#define UNICODE
#define _UNICODE
#define wxDEBUG_LEVEL 0
#define wxUSE_UNICODE_MSLU 1

#if  __APPLE__

	#define _WCHAR_H_CPLUSPLUS_98_CONFORMANCE_ // Fix : /include/wx/wxcrt.h:576:14: error: call to 'wcsstr' is ambiguous
	#define ALIF_STUDIO_NO_STC // Use wxTextCtrl instead of wxStyledTextCtrl

#endif

// ------------------------------------------------
// C++ Include
// ------------------------------------------------

#include <sstream> // ASSCI, Write batch file
#include <fstream> // ASSCI, Write batch file
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>

#include <utf8.h>

// ------------------------------------------------
// WX Include
// ------------------------------------------------

#ifdef WX_PRECOMP
    #include <wx/wxprec.h>
#else
    #include <wx/wx.h>
#endif

#include <wx/aui/aui.h>
//#include <wx/string.h> // maybe not needed.
#include <wx/event.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
//#include <wx/textentry.h> // class for txt ctrl, maybe not needed
//#include <wx/textcompleter.h> // Auto Complete
#include <wx/splitter.h>
#include <wx/toolbar.h>
#include <wx/artprov.h> //Get Default Icon from System for Toolbar
#include <wx/busyinfo.h>
#include <wx/wfstream.h> // Unicode, Read file line by line.
#include <wx/txtstrm.h> // Unicode, Read file line by line.
#include <wx/textfile.h>
#include <wx/dirctrl.h> // wxGenericDirCtrl
#include <wx/treectrl.h> // wxTreeCtrl
#include <wx/dir.h>
#include <wx/dialog.h>
#include <wx/utils.h>
//#include <wx/tokenzr.h>
#include <wx/propgrid/propgrid.h>
#include <wx/splash.h> // Splash Alif Studio Logo
#include <wx/utils.h>
#include <wx/textdlg.h> // Get Text from user, to search
#include <wx/fontdlg.h> // Font Dialog
#include <wx/stdpaths.h>	// Get Path
#include <wx/filename.h>	// Path tools

#include <wx/msgdlg.h> // About Dialog
#include <wx/aboutdlg.h> // About Dialog

// IMG
//#include "img.h" // PNG to Byte Array

// XPM
#include <wx/image.h>
#include <wx/imaglist.h>
#include "xpm/button_21.xpm"
#include "xpm/text_ctrl_21.xpm"
#include "xpm/static_text_21.xpm"
#include "xpm/folder_open_bleu_32.xpm"
#include "xpm/file_bleu_32.xpm"
#include "xpm/file_red_32.xpm"
#include "xpm/multi_window_21.xpm"
#include "xpm/window_21.xpm"

// ------------------------------------------------
// Version
// ------------------------------------------------

using namespace std;

const static wxString ALIF_STUDIO_VERSION = wxT("1.24");
wxString ALIF_VERSION = wxT("0.0");

// TODO: when click on STC, last find_position must be updated !

// ------------------------------------------------
// Custum Include
// ------------------------------------------------

//#include "alifstudio_path.h"
#include "alifstudio_tools_convert.h"

// ------------------------------------------------
// Scintilla Define / include
// ------------------------------------------------

// For Windows and Linux
#define SCI_WORDRIGHTEND 2441
#define SCI_CHARRIGHT 2306
#define SCI_CHARRIGHTEXTEND 2307
#define SCI_WORDRIGHTENDEXTEND 2442
#define SCI_SETMARGINRIGHT 2157
#define SCI_WORDRIGHT 2310
#define SCI_WORDRIGHTEXTEND 2311
#define SCI_DELWORDRIGHT 2336
#define SCI_DELWORDRIGHTEND 2518
#define SCI_WORDPARTRIGHT 2392
#define SCI_WORDPARTRIGHTEXTEND 2393

/*
// Tray RTL for macOS by STC (not working right now)

//#define EM_SETBIDIOPTIONS
//#define EM_SETEDITSTYLE

//#define SC_TECHNOLOGY_DIRECTWRITE
//#define SC_TECHNOLOGY_DIRECTWRITERETAIN
//#define SC_TECHNOLOGY_DIRECTWRITEDC

//#define SCI_SETBIDIRECTIONAL(SC_BIDIRECTIONAL_R2L)
//#define SCI_SETBIDIRECTIONAL 1
//#define SC_BIDIRECTIONAL_R2L
*/

#include <wx/stc/stc.h> // Scintilla

// ****************************************************************************
// ** GLOBAL *******************************************************************
// ****************************************************************************

// ------------------------------------------------
// Rand
// ------------------------------------------------

// WX Reserved : 4999 et 5999.
int IDG = 5901;
int GET_ID(){IDG++; return IDG;}

// ------------------------------------------------
// GLOBAL ID
// ------------------------------------------------

enum {
	ID_WINDOW_MAIN, 

	ID_MENU_REPLACE_ALL, 
	ID_MENU_LTR, 
	ID_MENU_RTL, 
	ID_MENU_SETTING_IDE, 
	ID_MENU_SETTING_FONT, 
	ID_MENU_SETTING_COMPILER, 
	ID_MENU_UPDATE, 
	//ID_MENU_UPDATE_BY_FILE, 
	ID_MENU_LEARN_PDF, 
	ID_MENU_LEARN_HTML, 
	ID_MENU_ABOUT_IDE, 
	ID_MENU_ABOUT_BUG, 
	ID_MENU_FIND_NEXT,
	ID_MENU_BUILD,
	ID_MENU_COMPILE,
	ID_MENU_RUN,
	ID_MENU_LEARN_ONLINE,

	ID_CODE, 
	ID_TOOLBAR_NEW, 
	ID_TOOLBAR_OPEN, 
	ID_TOOLBAR_SAVE, 
	ID_TOOLBAR_BUILD, 
	ID_TOOLBAR_UPDATE, 
	ID_TOOLBAR_HELP, 
	ID_TREE_FILES, 
	ID_TREE_WINDOWS, 
	ID_TREE_CONTROLS, 
	ID_PROPERTIES
};

// ------------------------------------------------
// GLOBAL Var
// ------------------------------------------------

int ID_CODE_ERROR_INDICATOR = 8;				// or any number between 8 and 32
//int CODE_ERROR_INDICATOR_START;				// to clear error when new compile
//int CODE_ERROR_INDICATOR_LEN;					// to clear error when new compile
//int CODE_ERROR_INDICATOR_LINE_NUMBER = -1;	// to clear error when new compile
bool CODE_ERROR_INDICATOR = false;				// to clear error when new compile

wxString FIND_WORD;
bool IS_FOUND = false;
int FIND_LAST_POS = 0;
size_t FIND_LEN;

bool UILOG_ERROR = false;

// ------------------------------------------------
// GLOBAL VAR
// ------------------------------------------------

std::string INSTALL_PATH;
bool PATH_CONTAIN_SPACE = false;

wxString PATH_TEMP;
wxString PATH_FULL_ALIF_COMPILER;
wxString PATH_FULL_SOURCE;
wxString PATH_FULL_EXECUTABLE;
wxString PATH_FULL_LOG;
wxString PATH_FULL_UPDATE;
wxString PATH_DIRECTORY;
wxString CURRENT_FILE_EXTENSION;
wxString PATH_FULL_PDF;

wxString SOURCE_FILE_NAME;

#ifdef ALIF_STUDIO_NO_STC
	bool CODE_MODIFIED = false;
#endif

bool IGNORE_UPDATE_SETTINGS_EVENT = false;
bool WINDOW_RESIZING_BY_SYSTEM = false;

wxTreeItemId TREE_ID_FILES_ROOT;
wxTreeItemId TREE_ID_FILES_ITEM[64];
wxTreeItemId TREE_ID_FILES_LAST_BOLD;
wxString TREE_FILES_PATH[64];
wxString TREE_FILES_TYPE[64];
unsigned int TREE_FILES_TOTAL_ITEM;
bool TREE_FILES_HAVE_ITEM;

wxTreeItemId TREE_ID_CONTROL_ROOT;
wxTreeItemId TREE_ID_CONTROL_ITEM_BUTTON;
wxTreeItemId TREE_ID_CONTROL_ITEM_TXT;
wxTreeItemId TREE_ID_CONTROL_ITEM_LABEL;

bool DRAW_CONTROL = false;
wxString DRAW_TYPE;

bool FIRST_GENERATED_CODE = false;

bool ARG_OPEN_FILE = false;
wxString ARG_OPEN_FILE_PATH;

// ------------------------------------------------
// Custum USER GLOBAL Functions Declaration
// ------------------------------------------------

bool SAVE_FILE_UTF8(wxString TXT, wxString PATH, bool OuverWrite);
void SET_NEW_FILE(wxString PATH);
void Exit();
void SET_TREE_FILES_LIST();

void UI_PARSE_SOURCE();
void UI_MANAGER(unsigned int ID_FILE);
void UI_UPDATE_SETTINGS(unsigned int ID);
void UI_EMPTY_SETTINGS();
void UI_GENERAT_ALIF_CODE();
void UI_MOVE_CONTROL(int ID);
void UI_RESIZE_CONTROL(int ID);
void UI_CONTROL_CAPTION_UPDATE(int ID);
void UI_DRAW_NEW_CONTROL(wxString TYPE, int X, int Y);
void UI_REMOVE_CONTROL(unsigned int ID);
void AUI_GUI_DESIGNER(bool SHOW);

// --------------------------------
// Movable Controls ID Array
// --------------------------------

// General

wxString ID_UI_CONTROL_CURRENT_TYPE;

// Window 

int ID_UI_WINDOW = GET_ID();
bool WINDOW_IS_CONSTRUCTED = false;
wxString ID_UI_WINDOW_NAME_CURRENT;
unsigned int ID_UI_WINDOW_CURRENT =0;

wxString UI_WINDOW_IDENTIFIER[64];
wxString UI_WINDOW_CAPTION[64];
int UI_WINDOW_X[64];
int UI_WINDOW_Y[64];
int UI_WINDOW_W[64];
int UI_WINDOW_H[64];

static std::map<wxString, bool> UI_WINDOW_IS_SET;

// Control 

unsigned int UI_CONTROL_TOTAL = 0;
wxString UI_CONTROL_WINDOW[64];
wxString UI_CONTROL_TYPE[64];
wxString UI_CONTROL_IDENTIFIER[64];
wxString UI_CONTROL_CAPTION[64];
int UI_CONTROL_X[64];
int UI_CONTROL_Y[64];
int UI_CONTROL_W[64];
int UI_CONTROL_H[64];
std::map<wxString, bool> UI_CONTROL_INSIDE_WINDOW_IS_SET;

static std::map<wxString, bool> UI_CONTROL_IS_SET;

unsigned int ID_UI_CONTROL_CURRENT =0;

// Tree 

wxTreeItemId TREE_ID_WINDOW_ROOT;
wxTreeItemId TREE_ID_WINDOW_ITEM[64];
unsigned int TREE_WINDOW_TOTAL_ITEM;
wxTreeItemId TREE_ID_WINDOW_LAST_BOLD;

// wxAui Panels

wxAuiManager AUI_MANAGER;

// ****************************************************************************
// ** Class Window Definition *************************************************
// ****************************************************************************

// ------------------------------------------------
// Class Window Main Definition
// ------------------------------------------------

class CLASS_WINDOW_MAIN : public wxFrame
{
    public:
		// ------------------------------------------------
		// Window Main Class Close
		// ------------------------------------------------
	
		void OnClose(wxCloseEvent& event);

		// ------------------------------------------------
		// Window Main Class Constructeur
		// ------------------------------------------------
		
        CLASS_WINDOW_MAIN ();
		
		// ------------------------------------------------
		// Window Main Class Destructeur
		// ------------------------------------------------
		
        virtual ~CLASS_WINDOW_MAIN();
		
		// ------------------------------------------------
		// Window Main User Functions Declaration
		// ------------------------------------------------
		
		// Menu

        void OnOpen(wxCommandEvent &event);
		void OnSave(wxCommandEvent &event);
        void OnSaveAs(wxCommandEvent &event);
		void OnCloseMenu(wxCommandEvent& event);

		void EDIT_UNDO(wxCommandEvent& event);
		void EDIT_REDO(wxCommandEvent& event);
		void EDIT_CUT(wxCommandEvent& event);
		void EDIT_COPY(wxCommandEvent& event);
		void EDIT_PASTE(wxCommandEvent& event);
		void EDIT_DELETE(wxCommandEvent& event);

		void FIND(wxCommandEvent& event);
		void FIND_NEXT(wxCommandEvent& event);
		void REPLACE(wxCommandEvent& event);
		void REPLACE_ALL(wxCommandEvent& event);

		void CODE_RTL(wxCommandEvent &event);
        void CODE_LTR(wxCommandEvent &event);

		void BUILD(wxCommandEvent &event);
		void COMPILE(wxCommandEvent& event);
		bool COMPILE_NOW();
		void RUN(wxCommandEvent& event);

		void PROP_IDE(wxCommandEvent& event);
		void PROP_FONT(wxCommandEvent& event);
		void PROP_COMPILER(wxCommandEvent& event);

		void UPDATE(wxCommandEvent& event);
		//void UPDATE_BY_FILE(wxCommandEvent& event);

		void HELP_PDF(wxCommandEvent& event);
		//void HELP_HTML(wxCommandEvent& event);
		void HELP_ONLINE(wxCommandEvent& event);

		void ABOUT_IDE(wxCommandEvent& event);
		void ABOUT_BUG(wxCommandEvent& event);

		#if  __APPLE__
			// Mac About
			void OnAbout(wxCommandEvent &event);	
		#endif

		// Tool bar
        void OnNew(wxCommandEvent &event);

		void SET_BINARY_PATH();
		void UPDATE_NOW(wxCommandEvent &event);
		void TREE_FILES_DCLICK(wxTreeEvent& event);
		void TREE_WINDOW_DCLICK(wxTreeEvent& event);
		void TREE_WINDOW_RCLICK(wxTreeEvent& event);
		void TREE_CONTROLES_DCLICK(wxTreeEvent& event);
		void PROPERTIES_CHANGED(wxPropertyGridEvent& event);

		#ifdef ALIF_STUDIO_NO_STC
			//void CODE_CHARADDED(wxCommandEvent& event); // Onlly KeyDown.
			void CODE_KEYDOWN(wxKeyEvent& event);
		#else
			//void CODE_CHANGE(wxStyledTextEvent& event);
			void CODE_CHARADDED(wxStyledTextEvent& event);
			void CODE_AUTOCOMP_COMPLETED(wxStyledTextEvent& event);
		#endif

		//void CODE_DROP(wxDropFilesEvent& event);

    private:
		DECLARE_NO_COPY_CLASS(CLASS_WINDOW_MAIN) // To Search in google...
        DECLARE_EVENT_TABLE() // Déclaration d'une table STATIQUE d'évènements
};

// ------------------------------------------------
// Class UI Window Definition
// ------------------------------------------------

class UI_CLASS_WINDOW : public wxDialog
{
    public:
		// ------------------------------------------------
		// UI Window Class Close
		// ------------------------------------------------
	
		void OnClose(wxCloseEvent& event);
		
		// ------------------------------------------------
		// UI Window Class Constructeur
		// ------------------------------------------------
		
        UI_CLASS_WINDOW(wxString CAPTION, wxPoint pos, wxSize size);
		
		// ------------------------------------------------
		// UI Window Class Destructeur
		// ------------------------------------------------
		
        virtual ~UI_CLASS_WINDOW();
		
		// ------------------------------------------------
		// UI Window Functions
		// ------------------------------------------------
		
		virtual bool AcceptsFocusFromKeyboard() const { return false; } // Disable Keyboard Tab
		
		void OnNavigationKey(wxNavigationKeyEvent& event);
		void OnResize(wxSizeEvent& event);

    private:
		DECLARE_NO_COPY_CLASS(UI_CLASS_WINDOW)
        DECLARE_EVENT_TABLE()
};

// ****************************************************************************
// ** OBJ Declaration ************************************************************
// ****************************************************************************

// ------------------------------------------------
// Windows OBJs Declaration
// ------------------------------------------------

CLASS_WINDOW_MAIN* OBJ_CLASS_WINDOW_MAIN;
UI_CLASS_WINDOW* OBJ_UI_CLASS_WINDOW;

// ------------------------------------------------
// Window Main User Widget OBJ Declaration
// ------------------------------------------------

wxMenuBar* MENU_BAR;

wxMenu* MENU_FILE;
wxMenu* MENU_EDIT;
wxMenu* MENU_SEARCH;
wxMenu* MENU_VIEW;
wxMenu* MENU_COMPILE;
wxMenu* MENU_SETTINGS;
wxMenu* MENU_UPDATE;
wxMenu* MENU_LEARN;
wxMenu* MENU_ABOUT;

#if  __APPLE__
	// Mac Help
	wxMenu *menuHelp = new wxMenu;
#endif

wxToolBar* OBJ_TOOLBAR;

#ifdef ALIF_STUDIO_NO_STC
	wxTextCtrl*	OBJ_CODE;
#else
	wxStyledTextCtrl*	OBJ_CODE;
#endif

wxTextCtrl*			OBJ_LOG;

wxTreeCtrl* OBJ_TREE_FILES_LIST;
wxTreeCtrl* OBJ_TREE_WINDOW;
wxTreeCtrl* OBJ_TREE_CONTROLS;

wxPropertyGrid* OBJ_PROPERTIES;

// ****************************************************************************
// ** Class DrawPanel Definition ************************************************
// ****************************************************************************

class DrawPanel : public wxPanel
{
	public:
		DrawPanel(wxDialog* parent);
		void OnPaintEvent(wxPaintEvent & evt); // Called by System, when re-drawing is needed.
		void render(wxDC& dc); // re-drawing
		virtual bool AcceptsFocusFromKeyboard() const { return false; } // Disable Keyboard Tab
		
		void OnClick(wxMouseEvent& event); // UI Window clicked
		void OnRightClick(wxMouseEvent& event);
		void OnEnter(wxMouseEvent& event);
		void OnLeave(wxMouseEvent& event);
		DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(DrawPanel, wxPanel)
	EVT_PAINT			(DrawPanel::OnPaintEvent)
	//EVT_LEFT_DOWN		(DrawPanel::OnClick)
	EVT_LEFT_UP			(DrawPanel::OnClick)
	EVT_RIGHT_UP		(DrawPanel::OnRightClick)
	EVT_ENTER_WINDOW	(DrawPanel::OnEnter)
	EVT_LEAVE_WINDOW	(DrawPanel::OnLeave)
END_EVENT_TABLE()

void DrawPanel::OnRightClick(wxMouseEvent& event)
{
	event.Skip();
	
	this->SetCursor( wxNullCursor ); 
	
	DRAW_CONTROL = false;
}

void DrawPanel::OnEnter(wxMouseEvent& event)
{
	event.Skip();
	
	// List : http://docs.wxwidgets.org/2.8.8/wx_wxcursor.html#wxcursor
	
	/*
	// not workin!
	if (UI_WINDOW_W[ID_UI_WINDOW_CURRENT] != OBJ_UI_CLASS_WINDOW->GetSize().x || 
		UI_WINDOW_H[ID_UI_WINDOW_CURRENT] != OBJ_UI_CLASS_WINDOW->GetSize().y)
	{
		UI_WINDOW_W[ID_UI_WINDOW_CURRENT] = OBJ_UI_CLASS_WINDOW->GetSize().x;
		UI_WINDOW_H[ID_UI_WINDOW_CURRENT] = OBJ_UI_CLASS_WINDOW->GetSize().y;
		
		UI_UPDATE_SETTINGS(0);
		UI_GENERAT_ALIF_CODE();
	}
	*/
	
	/* 
	// annuler bcs window start at center in gui designer!
	if (UI_WINDOW_X[ID_UI_WINDOW_CURRENT] != OBJ_UI_CLASS_WINDOW->GetPosition().x || 
		UI_WINDOW_Y[ID_UI_WINDOW_CURRENT] != OBJ_UI_CLASS_WINDOW->GetPosition().y)
	{
		UI_WINDOW_X[ID_UI_WINDOW_CURRENT] = OBJ_UI_CLASS_WINDOW->GetPosition().x;
		UI_WINDOW_Y[ID_UI_WINDOW_CURRENT] = OBJ_UI_CLASS_WINDOW->GetPosition().y;
			
		UI_UPDATE_SETTINGS(0);
		UI_GENERAT_ALIF_CODE();
	}
	*/
	
	if (DRAW_CONTROL)
	{
		this->SetCursor( wxCURSOR_SIZING ); 
	}
	else
	{
		this->SetCursor( wxNullCursor ); 
	}
}

void DrawPanel::OnLeave(wxMouseEvent& event)
{
	event.Skip();
	
	if (!DRAW_CONTROL)
	{
		this->SetCursor( wxNullCursor ); 
	}
}

void DrawPanel::OnClick(wxMouseEvent& event)
{
	event.Skip();
	
	this->SetCursor( wxNullCursor ); 
	
	if (DRAW_CONTROL)
	{
		DRAW_CONTROL = false;
		UI_DRAW_NEW_CONTROL(DRAW_TYPE, event.GetX(), event.GetY());
	}
	else
	{
		if (ID_UI_CONTROL_CURRENT_TYPE != "WINDOW")
		{
			ID_UI_CONTROL_CURRENT_TYPE = "WINDOW";
			ID_UI_CONTROL_CURRENT = 0;
			
			// 0 Mean is Window, not Control, 
			// ID of Window already set when User double click on Window List
			UI_UPDATE_SETTINGS(0);
		}
	}
}

DrawPanel::DrawPanel(wxDialog* parent) : wxPanel(parent, wxID_ANY)
{
	// Construction
}

void DrawPanel::OnPaintEvent(wxPaintEvent & evt) // Called by System, when re-drawing is needed.
{
	evt.Skip();
    wxPaintDC dc(this);
    render(dc);
}

void DrawPanel::render(wxDC&  dc)
{
	wxSize size = this->GetSize();
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen( wxPen( *wxBLACK, 2, wxPENSTYLE_DOT ) );
	
	for (int x=0; x < size.GetWidth(); x += 8)
	{
		for (int y=0; y < size.GetHeight(); y += 8)
		{
			dc.DrawPoint(x-1, y-1);
		}
	}
}

// --------------------------------
// Base Panell to hold all ctrls
// --------------------------------

DrawPanel* OBJ_WINDOW_BASE_PANEL;

// ****************************************************************************
// ** Class Movable Widgets Definition ****************************************
// ****************************************************************************

// --------------------------------
// Class Movable Controls
// --------------------------------

template <class T>
class Movable : public T
{
	// Move Var
	bool dragging;
    int x,y;
    int height, width;
    int top, left, topScreen, leftScreen;
    wxPanel* LOCAL_PARENT;
	unsigned int LOCAL_ID;
	
	// Resize VAR
	enum
	{ NONE, RIGHTBOTTOM, RIGHT, BOTTOM } m_sizing;
	int m_curX, m_curY, m_difX, m_difY;
	int m_resizeBorder = 10;
	wxSize m_minSize = wxSize(10, 10);
	
	// VAR
	bool IS_RESIZE = false;
	bool IS_MOVE = false;
	
private:

// ...

protected:

	// Attach the event handler manually
    void ConnectEvents()
    {
		this->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(Movable<T>::onMouseDown));
        this->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(Movable<T>::onMouseUp));
		this->Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(Movable<T>::onMouseRightUp));
        this->Connect(wxEVT_MOTION, wxMouseEventHandler(Movable<T>::onMove));
		this->Connect(wxEVT_SET_CURSOR, wxSetCursorEventHandler(Movable<T>::OnSetCursor));
    }

public:
	
	virtual bool AcceptsFocusFromKeyboard() const { return false; } // Disable Keyboard Tab
	
	// -------------------------------------------------------------
    // Instantiation
	// -------------------------------------------------------------
	
	Movable(unsigned int ID, wxString CAPTION, wxPoint pos, wxSize size, long style) : T (OBJ_WINDOW_BASE_PANEL, ID, CAPTION, pos, size, style) 
	//wxTE_READONLY)
	{
		// Base
		Movable::LOCAL_PARENT = OBJ_WINDOW_BASE_PANEL;
		LOCAL_ID = ID;
        ConnectEvents();
		
		// VAR
		IS_RESIZE = false;
		IS_MOVE = false;
		
		// Move VAR
        dragging = false;
		
		// Resize VAR
		m_sizing = NONE;
		m_curX = -1;
		m_curY = -1;
		m_minSize = wxSize(10, 10);
		m_resizeBorder = 10;
    }

	// -------------------------------------------------------------
	
	void OnSetCursor(wxSetCursorEvent& e)
	{
		if (e.GetX() >= this->GetSize().x - m_resizeBorder && e.GetY() >= this->GetSize().y - m_resizeBorder)
		{
			e.SetCursor(wxCursor(wxCURSOR_SIZENWSE));
			IS_RESIZE = true;
			IS_MOVE = false;
		}
		else if (e.GetX() >= this->GetSize().x - m_resizeBorder)
		{
			e.SetCursor(wxCursor(wxCURSOR_SIZEWE));
			IS_RESIZE = true;
			IS_MOVE = false;
		}
		else if (e.GetY() >= this->GetSize().y - m_resizeBorder)
		{
			e.SetCursor(wxCursor(wxCURSOR_SIZENS));
			IS_RESIZE = true;
			IS_MOVE = false;
		}
		else
		{
			e.SetCursor(*wxSTANDARD_CURSOR);
			IS_RESIZE = false;
			IS_MOVE = true;
		}
	}
	
	// -------------------------------------------------------------
	
    void onMouseDown(wxMouseEvent& e)
    {
		if (DRAW_CONTROL)
			DRAW_CONTROL = false;
		
		if(IS_RESIZE)
		{
			if (m_sizing == NONE)
			{
				if (e.GetX() >= this->GetSize().x - m_resizeBorder && e.GetY() >= this->GetSize().y - m_resizeBorder)
					m_sizing = RIGHTBOTTOM;
				else if (e.GetX() >= this->GetSize().x - m_resizeBorder)
					m_sizing = RIGHT;
				else if (e.GetY() >= this->GetSize().y - m_resizeBorder)
					m_sizing = BOTTOM;
      
				if (m_sizing != NONE)
				{
					m_difX = this->GetSize().x - e.GetX();
					m_difY = this->GetSize().y - e.GetY();

					#ifdef _WIN32
						this->CaptureMouse();
					#else
						// Infinity loop Crash
						//this->CaptureMouse();
					#endif

					onMove(e);
				}
			}
		}
		else if(IS_MOVE)
		{
			this->SetCursor( wxCURSOR_SIZING );

			#ifdef _WIN32
				this->CaptureMouse();
			#else
				// Infinity loop Crash
				//this->CaptureMouse();
			#endif

			x = e.GetX();
			y = e.GetY();
			LOCAL_PARENT->GetScreenPosition(&topScreen, &leftScreen);
			dragging=true;
		}
    }

    // -------------------------------------------------------------
	
	void onMouseRightUp(wxMouseEvent& e)
	{
		e.Skip(false);
		
		UI_REMOVE_CONTROL(LOCAL_ID);
	}
	
	 // -------------------------------------------------------------
	 
    void onMouseUp(wxMouseEvent& e)
    {
		e.Skip();
		
		if(IS_RESIZE)
		{
			if (m_sizing != NONE)
			{
				m_sizing = NONE;
				this->ReleaseMouse();
    
				wxScreenDC dc;
				wxPen pen(*wxBLACK, 1, wxPENSTYLE_DOT_DASH);
    
				dc.SetPen(pen);
				dc.SetBrush(*wxTRANSPARENT_BRUSH);
				dc.SetLogicalFunction(wxINVERT);
    
				//wxPoint pos = ClientToScreen(wxPoint(0, 0));
				wxPoint pos = this->GetParent()->ClientToScreen(this->GetPosition());
    
				dc.DrawRectangle(pos.x, pos.y, m_curX, m_curY);
  
				dc.SetLogicalFunction(wxCOPY);
				dc.SetPen(wxNullPen);
				dc.SetBrush(wxNullBrush);
    
				this->SetSize(m_curX, m_curY);
				
				//wxCommandEvent event(wxEVT_PANEL_RESIZED, this->GetId());
					//event.SetEventObject(this);
				//GetEventHandler()->ProcessEvent(event);
			}
		}
		else if(IS_MOVE)
		{
			this->SetCursor( wxNullCursor );
			this->ReleaseMouse();
			dragging=false;
		}
		
		if (this->GetPosition().x != UI_CONTROL_X[LOCAL_ID] || this->GetPosition().y != UI_CONTROL_Y[LOCAL_ID])
		{
			OBJ_UI_CLASS_WINDOW->Refresh();
			OBJ_UI_CLASS_WINDOW->Update();
			
			UI_CONTROL_X[LOCAL_ID] = this->GetPosition().x;
			UI_CONTROL_Y[LOCAL_ID] = this->GetPosition().y;
			
			ID_UI_CONTROL_CURRENT_TYPE = "CONTROL";
			ID_UI_CONTROL_CURRENT = LOCAL_ID;
			UI_UPDATE_SETTINGS(LOCAL_ID);
			UI_GENERAT_ALIF_CODE();
		}
		else if (this->GetSize().x != UI_CONTROL_W[LOCAL_ID] || this->GetSize().y != UI_CONTROL_H[LOCAL_ID])
		{
			OBJ_UI_CLASS_WINDOW->Refresh();
			OBJ_UI_CLASS_WINDOW->Update();
			
			UI_CONTROL_W[LOCAL_ID] = this->GetSize().x;
			UI_CONTROL_H[LOCAL_ID] = this->GetSize().y;
			
			ID_UI_CONTROL_CURRENT_TYPE = "CONTROL";
			ID_UI_CONTROL_CURRENT = LOCAL_ID;
			UI_UPDATE_SETTINGS(LOCAL_ID);
			UI_GENERAT_ALIF_CODE();
		}
		else if (ID_UI_CONTROL_CURRENT != LOCAL_ID)
		{
			OBJ_UI_CLASS_WINDOW->Refresh();
			OBJ_UI_CLASS_WINDOW->Update();
			
			ID_UI_CONTROL_CURRENT_TYPE = "CONTROL";
			ID_UI_CONTROL_CURRENT = LOCAL_ID;
			UI_UPDATE_SETTINGS(LOCAL_ID);
		}
		
		this->SetFocus();
		m_curX = m_curY = -1;
    }

    // -------------------------------------------------------------
	
	void onMove(wxMouseEvent& e)
	{
		e.Skip();
		
		if(IS_RESIZE)
		{
			if (m_sizing != NONE)
			{
				wxScreenDC dc;
				wxPen pen(*wxBLACK, 1, wxPENSTYLE_DOT_DASH);
    
				dc.SetPen(pen);
				dc.SetBrush(*wxTRANSPARENT_BRUSH);
				dc.SetLogicalFunction(wxINVERT);
    
				//wxPoint pos = ClientToScreen(wxPoint(0, 0));
				wxPoint pos = this->GetParent()->ClientToScreen(this->GetPosition());
    
				if (m_curX >= 0 && m_curY >= 0)
					dc.DrawRectangle(pos.x, pos.y, m_curX, m_curY);
      
				if (m_sizing == RIGHT || m_sizing == RIGHTBOTTOM)
					m_curX = e.GetX() + m_difX;
				else
					m_curX = this->GetSize().x;
    
				if (m_sizing == BOTTOM || m_sizing == RIGHTBOTTOM)  
					m_curY = e.GetY() + m_difY;
				else
					m_curY = this->GetSize().y;
      
				if (m_curX < m_minSize.x) m_curX = m_minSize.x;
				if (m_curY < m_minSize.y) m_curY = m_minSize.y;
    
				dc.DrawRectangle(pos.x, pos.y, m_curX, m_curY);
  
				dc.SetLogicalFunction(wxCOPY);
				dc.SetPen(wxNullPen);
				dc.SetBrush(wxNullBrush);
			}
		}
		else if(IS_MOVE)
		{
			wxPoint mouseOnScreen = this->ScreenToClient(wxGetMousePosition());

			this->GetSize(&height, &width);
			this->GetScreenPosition(&top, &left);

			// If "dragging" then left button is held down while moving
			if (dragging)
			{
				wxSetCursor( wxNullCursor );
				
				wxPoint mouseOnScreen = wxGetMousePosition();
				
				int newx = mouseOnScreen.x - x;
				int newy = mouseOnScreen.y - y;
				
				this->Move( LOCAL_PARENT->ScreenToClient( wxPoint(newx, newy) ) );
			}
			else  // not "dragging"
			{
				//this->SetLabel(wxString::Format("%d %d %d %d", mouseOnScreen.x, height, mouseOnScreen.y, width));
				
				// Change cursor to signify that pointer is on border
				if  (((mouseOnScreen.x > 0) && (mouseOnScreen.y > 0)) && ((mouseOnScreen.x < height) && (mouseOnScreen.y < width)))
                ::wxSetCursor( wxNullCursor );
			
				else if (mouseOnScreen.x == 0)
					::wxSetCursor( wxCURSOR_SIZEWE );
				else if (mouseOnScreen.y == 0)
					::wxSetCursor( wxCURSOR_SIZENS );
				else
					::wxSetCursor( wxNullCursor );
			}
		}
    }
	// -------------------------------------------------------------
};

// --------------------------------
// Movable Controls OBJ Array
// --------------------------------

Movable<wxButton> *OBJ_UI_BUTTON_ARRAY[64];
Movable<wxTextCtrl> *OBJ_UI_TXT_ARRAY[64];
Movable<wxStaticText> *OBJ_UI_LABEL_ARRAY[64]; // change it his txt bcs label is hidden if empty caption...

// --------------------------------
// UI Usable
// --------------------------------

void UI_BUTTON_BUILD(bool STATUS)
{
	// Menu
	MENU_BAR->EnableTop(4, STATUS);
	
	// ToolBar
	OBJ_TOOLBAR->EnableTool(ID_TOOLBAR_BUILD, STATUS);
}

void UI_BUTTON_SAVE(bool STATUS)
{
	// Menu
	MENU_BAR->EnableTop(1, STATUS);
	MENU_BAR->EnableTop(2, STATUS);
	MENU_BAR->EnableTop(3, STATUS);

	// Under Menu
	MENU_BAR->Enable(wxID_SAVE, STATUS);
	MENU_BAR->Enable(wxID_SAVEAS, STATUS);

	// ToolBar
	OBJ_TOOLBAR->EnableTool(ID_TOOLBAR_SAVE, STATUS);

	OBJ_LOG->Clear();
}

// -------------------------------

void OPEN_NEW_FILE(wxString FILE_PATH)
{
	if (!wxFileName::FileExists(FILE_PATH))
	{
		wxMessageBox(wxT("عذرا، الملف لم يعد موجودا \n\n") + FILE_PATH, wxT("خطأ"), wxICON_WARNING);

		// Hide Aui Designer Panels
		AUI_GUI_DESIGNER(false);
	}
	else
	{
		wxFileName fn ( FILE_PATH );
		if (fn.GetExt() == "alif")
			CURRENT_FILE_EXTENSION = "ALIF";
		else if (fn.GetExt() == "alifui")
			CURRENT_FILE_EXTENSION = "ALIFUI";
		else if (fn.GetExt() == "aliflib")
			CURRENT_FILE_EXTENSION = "ALIFLIB";
		else
		{
			wxMessageBox("امتداد الملف غير معروف - " + fn.GetExt(), wxT("خطأ"), wxICON_WARNING);

			//return false;
			CURRENT_FILE_EXTENSION = "ALIF";

			// Hide Aui Designer Panels
			AUI_GUI_DESIGNER(false);
		}

		PATH_DIRECTORY = fn.GetPathWithSep();

		SET_NEW_FILE(FILE_PATH);
		
		SET_TREE_FILES_LIST();

		wxTreeItemId TREE_TO_SELECT_ID;
		for (unsigned int i = 1; i <= TREE_FILES_TOTAL_ITEM; i++) // first file ID in list is 1
		{
			if (FILE_PATH == TREE_FILES_PATH[i])
			{
				TREE_TO_SELECT_ID = TREE_ID_FILES_ITEM[i];

				OBJ_TREE_FILES_LIST->SetItemBold(TREE_TO_SELECT_ID, true);
				TREE_ID_FILES_LAST_BOLD = TREE_TO_SELECT_ID;

				UI_MANAGER(i);

				break;
			}
		}
	}
}

// ****************************************************************************
// ** Event Static Table ******************************************************
// ****************************************************************************

// ------------------------------------------------
// Window Main Event Table
// ------------------------------------------------

BEGIN_EVENT_TABLE(CLASS_WINDOW_MAIN, wxFrame)

	EVT_CLOSE(CLASS_WINDOW_MAIN::OnClose)
	
	// Menu
		// File
		EVT_MENU(wxID_OPEN, CLASS_WINDOW_MAIN::OnOpen)
		EVT_MENU(wxID_SAVE, CLASS_WINDOW_MAIN::OnSave)
		EVT_MENU(wxID_SAVEAS, CLASS_WINDOW_MAIN::OnSaveAs)
		EVT_MENU(wxID_EXIT, CLASS_WINDOW_MAIN::OnCloseMenu)
		// Edit
		EVT_MENU(wxID_UNDO, CLASS_WINDOW_MAIN::EDIT_UNDO)
		EVT_MENU(wxID_REDO, CLASS_WINDOW_MAIN::EDIT_REDO)
		EVT_MENU(wxID_CUT, CLASS_WINDOW_MAIN::EDIT_CUT)
		EVT_MENU(wxID_COPY, CLASS_WINDOW_MAIN::EDIT_COPY)
		EVT_MENU(wxID_PASTE, CLASS_WINDOW_MAIN::EDIT_PASTE)
		EVT_MENU(wxID_DELETE, CLASS_WINDOW_MAIN::EDIT_DELETE)
		// Search
		EVT_MENU(wxID_FIND, CLASS_WINDOW_MAIN::FIND)
		EVT_MENU(ID_MENU_FIND_NEXT, CLASS_WINDOW_MAIN::FIND_NEXT)
		EVT_MENU(wxID_REPLACE, CLASS_WINDOW_MAIN::REPLACE)
		EVT_MENU(ID_MENU_REPLACE_ALL, CLASS_WINDOW_MAIN::REPLACE_ALL)
		// View
		EVT_MENU(ID_MENU_LTR, CLASS_WINDOW_MAIN::CODE_LTR)
		EVT_MENU(ID_MENU_RTL, CLASS_WINDOW_MAIN::CODE_RTL)
		// Project
		EVT_MENU(ID_MENU_BUILD, CLASS_WINDOW_MAIN::BUILD)
		EVT_MENU(ID_MENU_COMPILE, CLASS_WINDOW_MAIN::COMPILE)
		EVT_MENU(ID_MENU_RUN, CLASS_WINDOW_MAIN::RUN)
		// Prop
		EVT_MENU(ID_MENU_SETTING_IDE, CLASS_WINDOW_MAIN::PROP_IDE)
		EVT_MENU(ID_MENU_SETTING_FONT, CLASS_WINDOW_MAIN::PROP_FONT)
		EVT_MENU(ID_MENU_SETTING_COMPILER, CLASS_WINDOW_MAIN::PROP_COMPILER)
		// Update
		EVT_MENU(ID_MENU_UPDATE, CLASS_WINDOW_MAIN::UPDATE)
		//EVT_MENU(ID_MENU_UPDATE_BY_FILE, CLASS_WINDOW_MAIN::UPDATE_BY_FILE)
		// Help
		EVT_MENU(ID_MENU_LEARN_PDF, CLASS_WINDOW_MAIN::HELP_PDF)
		//EVT_MENU(ID_MENU_LEARN_HTML, CLASS_WINDOW_MAIN::HELP_HTML)
		EVT_MENU(ID_MENU_LEARN_ONLINE, CLASS_WINDOW_MAIN::HELP_ONLINE)
		// About
		EVT_MENU(ID_MENU_ABOUT_IDE, CLASS_WINDOW_MAIN::ABOUT_IDE)
		EVT_MENU(ID_MENU_ABOUT_BUG, CLASS_WINDOW_MAIN::ABOUT_BUG)

		#if  __APPLE__
			// Mac About
			EVT_MENU(wxID_ABOUT, CLASS_WINDOW_MAIN::OnAbout)
		#endif

	// ----------

	#ifdef ALIF_STUDIO_NO_STC

		// We use Connect()
		
		//EVT_TEXT(ID_CODE, CLASS_WINDOW_MAIN::CODE_CHARADDED)
		//EVT_KEY_DOWN(ID_CODE, CLASS_WINDOW_MAIN::CODE_KEYDOWN)

	#else

		//EVT_STC_CHANGE(ID_CODE, CLASS_WINDOW_MAIN::CODE_CHANGE) // Key Down Event ! // EVT_STC_CHANGE
		EVT_STC_CHARADDED(ID_CODE, CLASS_WINDOW_MAIN::CODE_CHARADDED) 
		EVT_STC_CLIPBOARD_PASTE(ID_CODE, CLASS_WINDOW_MAIN::CODE_CHARADDED)
		EVT_STC_AUTOCOMP_COMPLETED(ID_CODE, CLASS_WINDOW_MAIN::CODE_AUTOCOMP_COMPLETED) // Add Space after Token
		//EVT_STC_DO_DROP (ID_CODE, CLASS_WINDOW_MAIN::CODE_DROP)

	#endif
	
	EVT_TREE_ITEM_ACTIVATED			(ID_TREE_FILES, CLASS_WINDOW_MAIN::TREE_FILES_DCLICK)
	EVT_TREE_ITEM_ACTIVATED			(ID_TREE_CONTROLS, CLASS_WINDOW_MAIN::TREE_CONTROLES_DCLICK)
	
	EVT_TREE_ITEM_ACTIVATED			(ID_TREE_WINDOWS, CLASS_WINDOW_MAIN::TREE_WINDOW_DCLICK)
	EVT_TREE_ITEM_RIGHT_CLICK		(ID_TREE_WINDOWS, CLASS_WINDOW_MAIN::TREE_WINDOW_RCLICK)
	//EVT_TREE_ITEM_MENU				(ID_TREE_WINDOWS, CLASS_WINDOW_MAIN::TREE_WINDOW_RCLICK)
	//EVT_TREE_ITEM_MENU				(ID_TREE_WINDOWS, CLASS_WINDOW_MAIN::TREE_WINDOW_RCLICK)
	
	EVT_TREE_BEGIN_DRAG				(ID_TREE_CONTROLS, CLASS_WINDOW_MAIN::TREE_CONTROLES_DCLICK)
	//EVT_TREE_ITEM_COLLAPSED		(ID_TREE_CONTROLS, CLASS_WINDOW_MAIN::TREE_CONTROLES_DCLICK)
	
	EVT_PG_CHANGED(ID_PROPERTIES, CLASS_WINDOW_MAIN::PROPERTIES_CHANGED)
	
END_EVENT_TABLE()

// ------------------------------------------------
// UI Window Event Table
// ------------------------------------------------

BEGIN_EVENT_TABLE(UI_CLASS_WINDOW, wxDialog)

	EVT_CLOSE			(UI_CLASS_WINDOW::OnClose)
	EVT_SIZE			(UI_CLASS_WINDOW::OnResize)
	EVT_NAVIGATION_KEY	(UI_CLASS_WINDOW::OnNavigationKey)
	
END_EVENT_TABLE()

// ****************************************************************************
// ** Constructeur ***************************************************************
// ****************************************************************************

// ------------------------------------------------
// Window Main Constructeur
// ------------------------------------------------

CLASS_WINDOW_MAIN :: CLASS_WINDOW_MAIN() : 
	wxFrame(NULL, ID_WINDOW_MAIN, 
	wxT("ألف ستوديو [نسخة تجريبية]"),
	wxPoint(50, 50),
	#ifdef _WIN32
		wxSize(800, 600),
	#elif  __APPLE__
		wxSize(800, 600),
	#else
		wxSize(4000, 4000), // GTK STC Design Fix when First Resize.
	#endif
	wxDEFAULT_FRAME_STYLE)
	//wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | 
	// wxCLIP_CHILDREN | wxRESIZE_BORDER) 
{
	// -------------------------
	// wxAUI Construction
	// -------------------------
	// notify wxAUI which frame to use
	AUI_MANAGER.SetManagedWindow(this);
	
	// -------------------------
	// Icon Construction
	// -------------------------
	
	//SetIcon(wxIcon(sample_xpm));
	
	// -------------------------
	// Toolbar Construction
	// -------------------------
	
	OBJ_TOOLBAR = CreateToolBar(wxTB_DEFAULT_STYLE); // wxTB_TOP

    wxBitmap ID_ART_PROVIDER_OPEN = wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR);
    wxBitmap ID_ART_PROVIDER_COMPILE = wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE, wxART_TOOLBAR);
    wxBitmap ID_ART_PROVIDER_SAVE = wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR);
    wxBitmap ID_ART_PROVIDER_NEW = wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR);
	wxBitmap ID_ART_PROVIDER_HELP = wxArtProvider::GetBitmap(wxART_HELP_BOOK, wxART_TOOLBAR);
	wxBitmap ID_ART_PROVIDER_UPDATE = wxArtProvider::GetBitmap(wxART_GO_DOWN, wxART_TOOLBAR);

	OBJ_TOOLBAR->AddStretchableSpace(); // To Set Next Tools at Right
	
	OBJ_TOOLBAR->AddTool(ID_TOOLBAR_UPDATE, wxT("تحديث"), ID_ART_PROVIDER_UPDATE);
	OBJ_TOOLBAR->AddTool(ID_TOOLBAR_HELP, wxT("كتاب"), ID_ART_PROVIDER_HELP);
	
	//OBJ_TOOLBAR->AddStretchableSpace(); // To Set Tools at Right
	OBJ_TOOLBAR->AddSeparator();
	
	OBJ_TOOLBAR->AddTool(ID_TOOLBAR_BUILD, wxT("ترجم و نفد"), ID_ART_PROVIDER_COMPILE);
	OBJ_TOOLBAR->AddSeparator();
	OBJ_TOOLBAR->AddTool(ID_TOOLBAR_SAVE, wxT("حفظ الملف"), ID_ART_PROVIDER_SAVE);
	OBJ_TOOLBAR->AddTool(ID_TOOLBAR_OPEN, wxT("فتح ملف"), ID_ART_PROVIDER_OPEN);
	OBJ_TOOLBAR->AddTool(ID_TOOLBAR_NEW, wxT("ملف جديد"), ID_ART_PROVIDER_NEW);

    OBJ_TOOLBAR->Realize();
	
	Connect(ID_TOOLBAR_NEW, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(CLASS_WINDOW_MAIN::OnNew));
	Connect(ID_TOOLBAR_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(CLASS_WINDOW_MAIN::OnOpen));
	Connect(ID_TOOLBAR_BUILD, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(CLASS_WINDOW_MAIN::BUILD));
	Connect(ID_TOOLBAR_UPDATE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(CLASS_WINDOW_MAIN::UPDATE));
	Connect(ID_TOOLBAR_HELP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(CLASS_WINDOW_MAIN::HELP_PDF));
	
	// -------------------------
	// Menu Construction
	// -------------------------
	
	MENU_BAR = new wxMenuBar();
		MENU_FILE = new wxMenu();
		MENU_EDIT = new wxMenu();
		MENU_SEARCH = new wxMenu();
		MENU_VIEW = new wxMenu();
		MENU_COMPILE = new wxMenu();
		MENU_SETTINGS = new wxMenu();
		MENU_UPDATE = new wxMenu();
		MENU_LEARN = new wxMenu();
		MENU_ABOUT = new wxMenu();

		#if  __APPLE__
			menuHelp = new wxMenu();
		#endif
	
	// الملف
	MENU_FILE->Append(wxID_OPEN, wxT("فتح مجلد"));
	MENU_FILE->AppendSeparator();
	MENU_FILE->Append(wxID_SAVE, wxT("حفظ الملف"));
	MENU_FILE->Append(wxID_SAVEAS, wxT("حفظ ملف جديد..."));
	MENU_FILE->AppendSeparator();
	MENU_FILE->Append(wxID_EXIT, wxT("خروج"));
	// تحرير
	MENU_EDIT->Append(wxID_UNDO, wxT("تراجع عن"));
	MENU_EDIT->Append(wxID_REDO, wxT("إعادة"));
	MENU_EDIT->AppendSeparator();
	MENU_EDIT->Append(wxID_CUT, wxT("قص"));
	MENU_EDIT->Append(wxID_COPY, wxT("نسخ"));
	MENU_EDIT->Append(wxID_PASTE, wxT("لصق"));
	MENU_EDIT->Append(wxID_DELETE, wxT("حذف"));
	// بحث
	MENU_SEARCH->Append(wxID_FIND, wxT("بحث"));
	MENU_SEARCH->Append(ID_MENU_FIND_NEXT, wxT("بحث عن التالي"));
	MENU_SEARCH->AppendSeparator();
	MENU_SEARCH->Append(wxID_REPLACE, wxT("استبدل"));
	MENU_SEARCH->Append(ID_MENU_REPLACE_ALL, wxT("استبدل الكل"));
	// عرض
	MENU_VIEW->Append(ID_MENU_LTR, wxT("الشيفرة - من اليسار ال اليمين"));
	MENU_VIEW->Append(ID_MENU_RTL, wxT("الشيفرة - من اليمين ال اليسار"));
	// المشروع
	MENU_COMPILE->Append(ID_MENU_BUILD, wxT("ترجم و نفذ"));
	MENU_COMPILE->AppendSeparator();
	MENU_COMPILE->Append(ID_MENU_COMPILE, wxT("ترجم"));
	MENU_COMPILE->Append(ID_MENU_RUN, wxT("نفذ"));
	// إعدادات
	MENU_SETTINGS->Append(ID_MENU_SETTING_IDE, wxT("إعدادات بيئة التطوير"));
	MENU_SETTINGS->Append(ID_MENU_SETTING_FONT, wxT("إعدادات خط النص"));
	MENU_SETTINGS->AppendSeparator();
	MENU_SETTINGS->Append(ID_MENU_SETTING_COMPILER, wxT("إعدادات مترجم ألف"));
	// تحديث
	MENU_UPDATE->Append(ID_MENU_UPDATE, wxT("تحديث عن طريق الانترنت"));
	//MENU_UPDATE->Append(ID_MENU_UPDATE_BY_FILE, wxT("تحديث عن طريق ملف"));
	// تعليم
	MENU_LEARN->Append(ID_MENU_LEARN_PDF, wxT(" (PDF) كتاب لغة البرمجة العربية ألف"));
	//MENU_LEARN->Append(ID_MENU_LEARN_HTML, wxT("كتاب لغة البرمجة العربية ألف - HTML"));
	MENU_LEARN->Append(ID_MENU_LEARN_ONLINE, wxT(" (Online) كتاب لغة البرمجة العربية ألف"));
	// About
	MENU_ABOUT->Append(ID_MENU_ABOUT_IDE, wxT("حول ألف ستوديو"));
	MENU_ABOUT->Append(ID_MENU_ABOUT_BUG, wxT("بلغ عن خطأ برمجي"));

	#if  __APPLE__
		// Mac About
		menuHelp->Append(wxID_ABOUT, _("&About Alif Studio\tCtrl+D"));
	#endif

	// --- --- ---
	
	MENU_BAR->Append(MENU_FILE, wxT(" الملف "));
	MENU_BAR->Append(MENU_EDIT, wxT(" تحرير "));
	MENU_BAR->Append(MENU_SEARCH, wxT(" بحث "));
	MENU_BAR->Append(MENU_VIEW, wxT(" العرض "));
	MENU_BAR->Append(MENU_COMPILE, wxT(" المشروع "));
	MENU_BAR->Append(MENU_SETTINGS, wxT(" إعدادات "));
	MENU_BAR->Append(MENU_UPDATE, wxT(" تحديث "));
	MENU_BAR->Append(MENU_LEARN, wxT(" تعليم "));
	MENU_BAR->Append(MENU_ABOUT, wxT(" حول "));

	#if  __APPLE__
		// Mac Help
		MENU_BAR->Append(menuHelp, _("&Help"));
	#endif
	
	this->SetMenuBar(MENU_BAR);

	#ifdef _WIN32
		MENU_BAR->SetLayoutDirection(wxLayout_RightToLeft);
	#elif  __APPLE__
		//MENU_BAR->SetLayoutDirection(wxLayout_RightToLeft);
	#else
		MENU_BAR->SetLayoutDirection(wxLayout_RightToLeft);
	#endif
	
	// ---------------------------
	// Properties Construction
	// ---------------------------
	
    // Construct wxPropertyGrid control
    OBJ_PROPERTIES = new wxPropertyGrid(this, ID_PROPERTIES, wxDefaultPosition, wxDefaultSize,
											wxPG_AUTO_SORT | // Automatic sorting after items added
											wxPG_SPLITTER_AUTO_CENTER | // Automatically center splitter until user manually adjusts it
											// Default style
											wxPG_DEFAULT_STYLE );
	
    // Window style flags are at premium, so some less often needed ones are
    // available as extra window styles (wxPG_EX_xxx) which must be set using
    // SetExtraStyle member function. wxPG_EX_HELP_AS_TOOLTIPS, for instance,
    // allows displaying help strings as tool tips.
    OBJ_PROPERTIES->SetExtraStyle( wxPG_EX_HELP_AS_TOOLTIPS );
	
	OBJ_PROPERTIES->Append( new wxPropertyCategory(wxT(" الأداة ")) );
		OBJ_PROPERTIES->Append( new wxStringProperty(wxT(" النوع "), wxPG_LABEL, "") );
		OBJ_PROPERTIES->Append( new wxStringProperty(wxT(" التعريف "), wxPG_LABEL, "") );
		OBJ_PROPERTIES->DisableProperty( wxT(" النوع ") );
		
	OBJ_PROPERTIES->Append( new wxPropertyCategory(wxT(" الموقع ")) );
		OBJ_PROPERTIES->Append( new wxIntProperty(wxT(" الأفصول (x) "), wxPG_LABEL, 0) );
		OBJ_PROPERTIES->Append( new wxIntProperty(wxT(" الأرتوب (y) "), wxPG_LABEL, 0) );
		
	OBJ_PROPERTIES->Append( new wxPropertyCategory(wxT(" الحجم ")) );
		OBJ_PROPERTIES->Append( new wxIntProperty(wxT(" العرض "), wxPG_LABEL, 0) );
		OBJ_PROPERTIES->Append( new wxIntProperty(wxT(" الأرتفاع "), wxPG_LABEL, 0) );
		
	OBJ_PROPERTIES->Append( new wxPropertyCategory(wxT(" المحتوى ")) );
		OBJ_PROPERTIES->Append( new wxLongStringProperty(wxT(" النص "), wxPG_LABEL, "") ); // wxStringProperty

	#ifdef _WIN32
		OBJ_PROPERTIES->SetLayoutDirection(wxLayout_RightToLeft);
	#elif  __APPLE__
		//OBJ_PROPERTIES->SetLayoutDirection(wxLayout_RightToLeft);
	#else
		//OBJ_PROPERTIES->SetLayoutDirection(wxLayout_RightToLeft);
	#endif

	OBJ_PROPERTIES->Enable(false);
	
	/*
	// -------------------------
	// ToolBar Images
	// -------------------------
	
	wxImageList* CONTROLS_TOOLBAR_IMAGES_LIST_FILES = new wxImageList(24, 24);
	CONTROLS_TOOLBAR_IMAGES_LIST_FILES->Add(wxIcon(folder_open_bleu_xpm)); // 0 Folder
	CONTROLS_TOOLBAR_IMAGES_LIST_FILES->Add(wxIcon(file_bleu_xpm)); // 1 Alif
	CONTROLS_TOOLBAR_IMAGES_LIST_FILES->Add(wxIcon(multi_window_xpm)); // 2 UI
	CONTROLS_TOOLBAR_IMAGES_LIST_FILES->Add(wxIcon(file_red_xpm)); // 3 Lib

	// ---------------------------
	// ToolBar Construction
	// ---------------------------
	
    // create some toolbars
    wxAuiToolBar* tb1 = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
																wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW);
    
	//tb1->SetImageList(CONTROLS_TOOLBAR_IMAGES_LIST_FILES);
	tb1->SetToolBitmapSize(wxSize(21,21));
	
    tb1->AddTool(wxID_ANY, wxT("Test"), wxArtProvider::GetBitmap(wxART_ERROR));
    tb1->AddSeparator();
    tb1->AddTool(wxID_ANY, wxT("Test"), 0);
    tb1->AddTool(wxID_ANY, wxT("Test"), 1);
    tb1->AddTool(wxID_ANY, wxT("Test"), wxArtProvider::GetBitmap(wxART_WARNING));
    tb1->AddTool(wxID_ANY, wxT("Test"), wxArtProvider::GetBitmap(wxART_MISSING_IMAGE));
    //tb1->SetCustomOverflowItems(prepend_items, append_items);
    tb1->Realize();
	*/
	
	// -------------------------
	// Controles Images
	// -------------------------
	
	wxImageList* CONTROLS_IMAGES_LIST = new wxImageList(21, 21);
	CONTROLS_IMAGES_LIST->Add(wxIcon(window_xpm)); // 0 Button
	CONTROLS_IMAGES_LIST->Add(wxIcon(button_xpm)); // 1 Button
	CONTROLS_IMAGES_LIST->Add(wxIcon(text_ctrl_xpm)); // 2 TXT
	CONTROLS_IMAGES_LIST->Add(wxIcon(static_text_xpm)); // 3 Label
	
	// ---------------------------
	// Controles List Construction
	// ---------------------------
	
	OBJ_TREE_CONTROLS = new wxTreeCtrl(this, ID_TREE_CONTROLS, wxPoint(0,0), wxSize(400,400), 
	wxTR_NO_BUTTONS | wxTR_SINGLE | wxTR_ROW_LINES | wxTR_HIDE_ROOT);
	
	OBJ_TREE_CONTROLS->SetImageList(CONTROLS_IMAGES_LIST);
	
	TREE_ID_CONTROL_ROOT = OBJ_TREE_CONTROLS->AddRoot(wxT("الأدوات"), 0, 0);
	
	TREE_ID_CONTROL_ITEM_BUTTON = OBJ_TREE_CONTROLS->AppendItem(TREE_ID_CONTROL_ROOT, wxT(" زر "), 1, 1);
	TREE_ID_CONTROL_ITEM_TXT = OBJ_TREE_CONTROLS->AppendItem(TREE_ID_CONTROL_ROOT, wxT(" نص "), 2, 2);
	TREE_ID_CONTROL_ITEM_LABEL = OBJ_TREE_CONTROLS->AppendItem(TREE_ID_CONTROL_ROOT, wxT(" ملصق "), 3, 3);
	
	OBJ_TREE_CONTROLS->ExpandAll();
	
	// ---------------------------
	// Splitter Construction
	// ---------------------------
	
	//OBJ_MAIN_SIZER = new wxBoxSizer(wxHORIZONTAL);
	//OBJ_CODE_SIZER = new wxBoxSizer(wxHORIZONTAL);
	//OBJ_LOG_SIZER = new wxBoxSizer(wxHORIZONTAL);
	//OBJ_MAIN_SPLITTER = new wxSplitterWindow(this, wxID_ANY);
	//OBJ_PANEL_1 = new wxPanel(OBJ_MAIN_SPLITTER, wxID_ANY);
	//OBJ_PANEL_2 = new wxPanel(OBJ_MAIN_SPLITTER, wxID_ANY);
	
	// ---------------------------
	// Code Text Construction
	// ---------------------------
	
	#ifdef _WIN32

		// SCINTILLA Code - Microsoft Windows
			
		/*
		OBJ_CODE = new wxStyledTextCtrl(OBJ_PANEL_1, wxID_ANY, wxPoint(0, 0), wxSize(600, 10000), 
		wxTE_MULTILINE | wxWANTS_CHARS | wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB |  
		WS_EX_RIGHT | WS_EX_RTLREADING | WS_EX_LEFTSCROLLBAR | WS_EX_LAYOUTRTL | 
		SCI_CHARRIGHT | SCI_WORDRIGHTEND | SCI_CHARRIGHTEXTEND | SCI_WORDRIGHTENDEXTEND | SCI_SETMARGINRIGHT | SCI_WORDRIGHT | SCI_WORDRIGHTEXTEND | SCI_DELWORDRIGHT | SCI_DELWORDRIGHTEND | SCI_WORDPARTRIGHT | SCI_WORDPARTRIGHTEXTEND);
		// |WS_EX_LAYOUTRTL|ALIGN_RIGHT|SCI_LAYOUTRTL|SCI_WORDRIGHT
		// wxBORDER_NONE | wxTE_NO_VSCROLL
		//OBJ_CODE_SCINTILLA->SetLayoutDirection(SCI_WORDRIGHT);
		//OBJ_CODE_SCINTILLA->Clear();
		//OBJ_CODE_SCINTILLA->ClearAll();

		OBJ_CODE->StyleSetCharacterSet(0, wxSTC_CHARSET_ARABIC); // Only Windows, GTK not.
		OBJ_CODE->SetCodePage(wxSTC_CP_UTF8);
		OBJ_CODE->SetInitialSize(size);
		*/

		//OBJ_CODE = new wxStyledTextCtrl(OBJ_PANEL_1,wxID_ANY,wxDefaultPosition,this->GetSize());
		OBJ_CODE = new wxStyledTextCtrl(this, ID_CODE, wxPoint(0, 0), wxSize(500, 200), 
		wxTE_MULTILINE | wxWANTS_CHARS | wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB |  wxBORDER_NONE | wxVERTICAL |
		WS_EX_RIGHT | WS_EX_RTLREADING | WS_EX_LEFTSCROLLBAR | WS_EX_LAYOUTRTL | 
		SCI_CHARRIGHT | SCI_WORDRIGHTEND | SCI_CHARRIGHTEXTEND | SCI_WORDRIGHTENDEXTEND | SCI_SETMARGINRIGHT | 
		SCI_WORDRIGHT | SCI_WORDRIGHTEXTEND | SCI_DELWORDRIGHT | SCI_DELWORDRIGHTEND | SCI_WORDPARTRIGHT | 
		SCI_WORDPARTRIGHTEXTEND);
		
		OBJ_CODE->StyleSetCharacterSet(0, wxSTC_CHARSET_ARABIC); // Only Windows, GTK not.
		OBJ_CODE->SetCodePage(wxSTC_CP_UTF8);
		
 		//wxFont font(12,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL);
		wxFont CODE_FONT (13,
		wxFONTFAMILY_SWISS, // wxFONTFAMILY_SWISS
		wxFONTSTYLE_NORMAL, // wxFONTSTYLE_ITALIC
		wxFONTWEIGHT_NORMAL, // wxFONTWEIGHT_NORMAL / wxFONTWEIGHT_BOLD
		false, // 
		"Courier New");
 		OBJ_CODE->StyleSetFont(wxSTC_STYLE_DEFAULT, CODE_FONT);
		
	#elif  __APPLE__

		// Mac OS X

		#ifdef ALIF_STUDIO_NO_STC

			OBJ_CODE = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(0, 0), wxSize(700, 400), // wxTextCtrl(OBJ_PANEL_2..
			wxTE_MULTILINE | wxBORDER_NONE | wxRIGHT | wxALIGN_RIGHT);

			OBJ_CODE->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CLASS_WINDOW_MAIN::CODE_KEYDOWN), NULL, this);
			//OBJ_CODE->Connect(wxEVT_TEXT, wxCommandEventHandler(CLASS_WINDOW_MAIN::CODE_CHARADDED), NULL, this);

			wxFont* CODE_FONT = new wxFont(18,
			wxFONTFAMILY_MODERN, // wxFONTFAMILY_SWISS
			wxFONTSTYLE_NORMAL, // wxFONTSTYLE_ITALIC
			wxFONTWEIGHT_NORMAL, // wxFONTWEIGHT_NORMAL / wxFONTWEIGHT_BOLD
			false, // 
			"Courier New");
			OBJ_CODE->SetFont(*CODE_FONT);

		#else

			OBJ_CODE = new wxStyledTextCtrl(this, ID_CODE, wxPoint(0, 0), wxSize(500, 200), 
			wxTE_MULTILINE | wxBORDER_NONE | wxVERTICAL);
			// | wxRIGHT | wxALIGN_RIGHT | wxWANTS_CHARS | 
			//wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB |  
			//SCI_CHARRIGHT | SCI_WORDRIGHTEND | SCI_CHARRIGHTEXTEND | SCI_WORDRIGHTENDEXTEND | SCI_SETMARGINRIGHT | 
			//SCI_WORDRIGHT | SCI_WORDRIGHTEXTEND | SCI_DELWORDRIGHT | SCI_DELWORDRIGHTEND | SCI_WORDPARTRIGHT | SCI_WORDPARTRIGHTEXTEND);
			
			// wxLEFTSCROLLBAR wxLAYOUTRTL wxRTLREADING
			// WS_EX_RIGHT WS_EX_RTLREADING WS_EX_LEFTSCROLLBAR WS_EX_LAYOUTRTL 

			//OBJ_CODE->StyleSetCharacterSet(0, wxSTC_CHARSET_ARABIC); // Only Windows, GTK not.
			OBJ_CODE->SetCodePage(wxSTC_CP_UTF8);
			
			//wxFont font(12,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL);
			wxFont CODE_FONT (20,
			wxFONTFAMILY_DEFAULT, // wxFONTFAMILY_SWISS
			wxFONTSTYLE_NORMAL, // wxFONTSTYLE_ITALIC
			wxFONTWEIGHT_NORMAL, // wxFONTWEIGHT_NORMAL / wxFONTWEIGHT_BOLD
			false, // 
			"Al Bayan"); // Courier New
			OBJ_CODE->StyleSetFont(wxSTC_STYLE_DEFAULT, CODE_FONT);

		#endif
		
	#else
		// Linux

		/*    this saved, maybe help when using SCINTILLA on Linux, this is kanet dyal txtctrl lol
			
		OBJ_CODE = new wxTextCtrl(OBJ_PANEL_1, ID_CODE, wxT(""), wxPoint(0, 0), 
		wxSize(600, 10000), wxTE_MULTILINE | wxBORDER_NONE | wxTE_NO_VSCROLL | 
		wxWANTS_CHARS | wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
		*/
			
		//wxTE_RIGHT | wxALIGN_RIGHT
		//wxTE_NO_VSCROLL | wxBORDER_NONE | wxTE_PROCESS_ENTER
		//EM_SETBIDIOPTIONS | EM_SETEDITSTYLE
		//WS_EX_RIGHT | WS_EX_RTLREADING | WS_EX_LEFTSCROLLBAR | WS_EX_LAYOUTRTL
		//wxHSCROLL | wxTE_RICH2 | wxTE_RIGHT
		//OBJ_CODE->SetLayoutDirection(wxLayout_RightToLeft);

		OBJ_CODE = new wxStyledTextCtrl(this, ID_CODE, wxPoint(0, 0), wxSize(500, 200), 
		wxTE_MULTILINE | wxWANTS_CHARS | wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB |  wxBORDER_NONE | wxVERTICAL |
		wxRIGHT | 
		SCI_CHARRIGHT | SCI_WORDRIGHTEND | SCI_CHARRIGHTEXTEND | SCI_WORDRIGHTENDEXTEND | SCI_SETMARGINRIGHT | 
		SCI_WORDRIGHT | SCI_WORDRIGHTEXTEND | SCI_DELWORDRIGHT | SCI_DELWORDRIGHTEND | SCI_WORDPARTRIGHT | 
		SCI_WORDPARTRIGHTEXTEND);
		
		// wxLEFTSCROLLBAR wxLAYOUTRTL wxRTLREADING
		// WS_EX_RIGHT WS_EX_RTLREADING WS_EX_LEFTSCROLLBAR WS_EX_LAYOUTRTL 

		OBJ_CODE->StyleSetCharacterSet(0, wxSTC_CHARSET_ARABIC); // Only Windows, GTK not.
		OBJ_CODE->SetCodePage(wxSTC_CP_UTF8);
			
 		//wxFont font(12,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL);
		wxFont CODE_FONT (13,
		wxFONTFAMILY_DEFAULT, // wxFONTFAMILY_SWISS
		wxFONTSTYLE_NORMAL, // wxFONTSTYLE_ITALIC
		wxFONTWEIGHT_NORMAL, // wxFONTWEIGHT_NORMAL / wxFONTWEIGHT_BOLD
		false, // 
		"Courier New");
 		OBJ_CODE->StyleSetFont(wxSTC_STYLE_DEFAULT, CODE_FONT);
	#endif

	#ifdef ALIF_STUDIO_NO_STC

		// --------------------------------------
		// TextCtrl Code - Global for all OS
		// --------------------------------------

		OBJ_CODE->SetForegroundColour(wxColour(50, 54, 58));
		OBJ_CODE->SetBackgroundColour(wxColour(250, 251, 252));
		OBJ_CODE->SetMargins(10, 10);

	#else

		// --------------------------------------
		// SCINTILLA Code - Global for all OS
		// --------------------------------------
		
		enum
		{
			MARGIN_LINE_NUMBERS,
			MARGIN_FOLD // Not used right now.
		};
		
		// Backgroud Color
		OBJ_CODE->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColour(50, 54, 58));
		OBJ_CODE->StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColour(250, 251, 252));
		
		// Save Code Background and Foreground colors
		OBJ_CODE->StyleClearAll();
		
		// RTL
		#ifdef _WIN32
			OBJ_CODE->SetLayoutDirection(wxLayout_RightToLeft);
		#elif  __APPLE__
			OBJ_CODE->SetLayoutDirection(wxLayout_RightToLeft);
			//OBJ_CODE->SetLayoutDirection(SCI_WORDRIGHT);
		#else
			OBJ_CODE->SetLayoutDirection(wxLayout_RightToLeft);
		#endif
		
		// SCROLLBAR
		OBJ_CODE->SetUseHorizontalScrollBar(true);
		OBJ_CODE->SetUseVerticalScrollBar(true);

		// Word wrap
		//OBJ_CODE->SetWrapMode (wxSTC_WRAP_WORD); // wxSCI_WRAP_NONE
		
		// Lex Lang
		OBJ_CODE->SetLexer(wxSTC_LEX_LUA);
		
		// Line number
		OBJ_CODE->SetMarginWidth (MARGIN_LINE_NUMBERS, 50);
		OBJ_CODE->StyleSetForeground (wxSTC_STYLE_LINENUMBER, wxColour (167, 176, 199) );
		OBJ_CODE->StyleSetBackground (wxSTC_STYLE_LINENUMBER, wxColour (239, 241, 245));
		OBJ_CODE->SetMarginType (MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);
		
		// Code Maring
		OBJ_CODE->SetMarginRight(2); // Line at the end of code line.
		OBJ_CODE->SetMarginLeft(12); // Margin between Line number and Code
		//OBJ_CODE->SetMarginWidth(0,0);
		
		// Tab
		OBJ_CODE->SetTabWidth(4);
		OBJ_CODE->SetUseTabs(1);
		
		
		// Color
		OBJ_CODE->StyleSetForeground (wxSTC_C_STRING,					wxColour(90,102,112)); // "hello"
		OBJ_CODE->StyleSetForeground (wxSTC_C_PREPROCESSOR,				wxColour(90,102,112)); // #ألف
		OBJ_CODE->StyleSetForeground (wxSTC_C_IDENTIFIER,				wxColour(90,102,112)); // codes..
		OBJ_CODE->StyleSetForeground (wxSTC_C_NUMBER,					wxColour(90,102,112)); // 1 2 3
		OBJ_CODE->StyleSetForeground (wxSTC_C_CHARACTER,				wxColour(90,102,112)); // 'hello'
		OBJ_CODE->StyleSetForeground (wxSTC_C_WORD,						wxColour(90,102,112)); // عدد  نص
		OBJ_CODE->StyleSetForeground (wxSTC_C_WORD2,					wxColour(90,102,112)); // نافذة  أداة
		OBJ_CODE->StyleSetForeground (wxSTC_C_COMMENT,					wxColour(90,102,112)); // / * test...
		OBJ_CODE->StyleSetForeground (wxSTC_C_COMMENTLINE,				wxColour(90,102,112)); // test
		OBJ_CODE->StyleSetForeground (wxSTC_C_OPERATOR,					wxColour(90,102,112)); // + * & % -
		// Unknown color
		OBJ_CODE->StyleSetForeground (wxSTC_C_COMMENTDOC,				wxColour(90,102,112)); // 
		OBJ_CODE->StyleSetForeground (wxSTC_C_COMMENTDOCKEYWORD,		wxColour(90,102,112)); // 
		OBJ_CODE->StyleSetForeground (wxSTC_C_COMMENTDOCKEYWORDERROR,	wxColour(90,102,112)); // 
		OBJ_CODE->StyleSetForeground (wxSTC_C_VERBATIM,					wxColour(90,102,112)); // 
		// Word Style
		//OBJ_CODE->StyleSetBold(wxSTC_C_PREPROCESSOR, true);
		//OBJ_CODE->StyleSetBold(wxSTC_C_WORD, true);
		//OBJ_CODE->StyleSetBold(wxSTC_C_WORD2, true);
		

		OBJ_CODE->StyleSetForeground (wxSTC_LUA_CHARACTER,			wxColour(90,102,112)); // unknown
		OBJ_CODE->StyleSetForeground (wxSTC_LUA_COMMENT,			wxColour(170,176,190)); // unknown
		OBJ_CODE->StyleSetForeground (wxSTC_LUA_COMMENTDOC,			wxColour(170,176,190)); // unknown
		OBJ_CODE->StyleSetForeground (wxSTC_LUA_STRINGEOL,			wxColour(90,102,112)); // unknown
		OBJ_CODE->StyleSetForeground (wxSTC_LUA_DEFAULT,			wxColour(90,102,112)); // unknown
		OBJ_CODE->StyleSetForeground (wxSTC_LUA_LITERALSTRING,		wxColour(90,102,112)); // unknown
		
		OBJ_CODE->StyleSetForeground (wxSTC_LUA_COMMENTLINE,		wxColour(170,176,190));		// --comment...
		OBJ_CODE->StyleSetForeground (wxSTC_LUA_IDENTIFIER,			wxColour(90,102,112));		// codes..
		OBJ_CODE->StyleSetForeground (wxSTC_LUA_NUMBER,				wxColour(215,105,61));		// 1 2 3 // 205,128,101
		OBJ_CODE->StyleSetForeground (wxSTC_LUA_OPERATOR,			wxColour(197,107,116));		// + * & % -
		OBJ_CODE->StyleSetForeground (wxSTC_LUA_PREPROCESSOR,		wxColour(90,102,112));		// #
		OBJ_CODE->StyleSetForeground (wxSTC_LUA_STRING,				wxColour(164,192,140));		//  "hello"

		OBJ_CODE->StyleSetForeground (wxSTC_LUA_WORD,				wxColour(204,123,132));		// red bold : نافذة الف 
		//OBJ_CODE->StyleSetForeground (wxSTC_LUA_WORD1,			wxColour(197,107,116));		// 
		OBJ_CODE->StyleSetForeground (wxSTC_LUA_WORD2,				wxColour(172,116,177));		// blue bold : رئيسية
		OBJ_CODE->StyleSetForeground (wxSTC_LUA_WORD3,				wxColour(91,142,200));		// blue : عدد
		OBJ_CODE->StyleSetForeground (wxSTC_LUA_WORD4,				wxColour(172,116,177));		// pink : true false
		// For future use..
		//OBJ_CODE->StyleSetForeground (wxSTC_LUA_WORD5,				wxColour(0,0,0));		// 
		//OBJ_CODE->StyleSetForeground (wxSTC_LUA_WORD6,				wxColour(0,0,0));		// 

		// moov 137,95,175
		// bleu fati7 56,159,200
		// pink 179,138,172

		// Enable current line highlighting
		OBJ_CODE->SetCaretLineVisible(true);
		OBJ_CODE->SetCaretLineBackground(wxColour(247,248,249));
		
		// Code Special Color
		/*
		OBJ_CODE->StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColour(147, 161, 161));
			OBJ_CODE->StyleSetBackground(wxSTC_STYLE_INDENTGUIDE, wxColour(255, 255, 255));
		OBJ_CODE->StyleSetBackground(wxSTC_STYLE_CONTROLCHAR, wxColour(101, 123, 131));
			OBJ_CODE->StyleSetForeground(wxSTC_STYLE_CONTROLCHAR, wxColour(255, 255, 255));
		OBJ_CODE->StyleSetBackground(wxSTC_STYLE_BRACELIGHT, wxColour(101, 123, 131));
			OBJ_CODE->StyleSetForeground(wxSTC_STYLE_BRACELIGHT, wxColour(255, 255, 255));
		OBJ_CODE->StyleSetBackground(wxSTC_STYLE_BRACEBAD, wxColour(101, 123, 131));
			OBJ_CODE->StyleSetForeground(wxSTC_STYLE_BRACEBAD, wxColour(255, 255, 255));
		*/
		
		// Key words
		wxString CPP_KEY_WORDS = "wxString wxT Asm auto bool break case catch char class const_cast continue default delete do double else enum dynamic_cast extern false float for union unsigned using friend goto if inline int long mutable virtual namespace new operator private protected public register void reinterpret_cast return short signed sizeof static static_cast volatile struct switch template this throw true try typedef typeid wchar_t while wxMessageBox wxAboutBox wxBeginBusyCursor wxBell wxCreateFileTipProvider wxDirSelector wxFileSelector wxEndBusyCursor wxGenericAboutBox wxGetColourFromUser wxGetFontFromUser wxGetMultipleChoices wxGetNumberFromUser wxGetPasswordFromUser wxGetTextFromUser wxGetMultipleChoice wxGetSingleChoice wxGetSingleChoiceIndex wxGetSingleChoiceData wxIsBusy wxMessageBox wxShowTip alignas alignof and and_eq asm auto bitand bitor catch char16_t char32_t class compl const constexpr const_cast continue decltype default delete do double dynamic_cast else enum explicit export extern false float for friend goto if inline int long mutable namespace new noexcept not not_eq nullptr operator or or_eq private protected public register reinterpret_cast return short signed sizeof static static_assert static_cast struct switch template this thread_local throw true try typedef typeid typename union using virtual void volatile wchar_t while xor xor_eq"; // add wx class like wxString..
		
		// ألف أضف مكتبة رئيسية ـسـ  واجهة خاص نهاية كلما نافذة دالة عدد نص كائن إذا و أو سطر رجوع
		// صنف أداة نقر زر نص ملصق إظهار إخفاء تدمير عنوان نص تجميد عرض محتوى ارتفاع أفصول أرتوب 

		OBJ_CODE->SetKeyWords(0, wxT(" ألف طرفية أضف مكتبة نافذة صنف دالة _س_ واجهة "));
			OBJ_CODE->StyleSetBold(wxSTC_LUA_WORD, true);

		OBJ_CODE->SetKeyWords(1, wxT(" هدم بناء نقر رئيسية " + CPP_KEY_WORDS));
			OBJ_CODE->StyleSetBold(wxSTC_LUA_WORD2, false);
			
		OBJ_CODE->SetKeyWords(2, wxT(" أداة زر ملصق كلما عدد متغير ثابت منطق نص كائن إذا و أو وإلا نهاية "));
			OBJ_CODE->StyleSetBold(wxSTC_LUA_WORD3, false);

		OBJ_CODE->SetKeyWords(3, wxT(" سطر كسر إرجاع صحيح خطأ خاص "));
			OBJ_CODE->StyleSetBold(wxSTC_LUA_WORD4, false);
		
		// For future use..
		//OBJ_CODE->SetKeyWords(3, wxT(""));
			//OBJ_CODE->StyleSetBold(wxSTC_LUA_WORD4, false);
		//OBJ_CODE->SetKeyWords(4, wxT(""));
			//OBJ_CODE->StyleSetBold(wxSTC_LUA_WORD5, false);
		//OBJ_CODE->SetKeyWords(5, wxT(""));
			//OBJ_CODE->StyleSetBold(wxSTC_LUA_WORD6, true);
		
		// ---------------------------
		// Drag And Drop Alif files
		// ---------------------------
		
		//OBJ_CODE->DragAcceptFiles(true);
		//OBJ_CODE->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(CLASS_WINDOW_MAIN::CODE_DROP), NULL, this);

		// ---------------------------
		// Error Style
		// ---------------------------

		OBJ_CODE->IndicatorSetStyle(ID_CODE_ERROR_INDICATOR, wxSTC_INDIC_TT);
		OBJ_CODE->IndicatorSetForeground(ID_CODE_ERROR_INDICATOR, wxColour(197,107,116));
		
		// ---------------------------
		// Line end
		// ---------------------------
		// wxSTC_EOL_CRLF (Windows), wxSTC_EOL_CR (Macintosh) and wxSTC_EOL_LF (Linux).
		
		#ifdef _WIN32
			OBJ_CODE->SetEOLMode(wxSTC_EOL_LF);
		#elif  __APPLE__
			OBJ_CODE->SetEOLMode(wxSTC_EOL_LF);
		#else
			OBJ_CODE->SetEOLMode(wxSTC_EOL_LF);
		#endif

	#endif
	
	// ---------------------------
	// Log Text
	// ---------------------------

	#ifdef _WIN32

		OBJ_LOG = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(0, 0), wxSize(700, 400), // wxTextCtrl(OBJ_PANEL_2..
		wxTE_MULTILINE | wxTE_READONLY | wxBORDER_NONE);
		//WS_EX_RIGHT | WS_EX_RTLREADING | WS_EX_LEFTSCROLLBAR | WS_EX_LAYOUTRTL | wxTE_RIGHT

	#elif  __APPLE__

		OBJ_LOG = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(0, 0), wxSize(700, 400), // wxTextCtrl(OBJ_PANEL_2..
		wxTE_MULTILINE | wxTE_READONLY | wxBORDER_NONE | wxRIGHT | wxALIGN_RIGHT);

	#else

		OBJ_LOG = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(0, 0), wxSize(700, 400), // wxTextCtrl(OBJ_PANEL_2..
		wxTE_MULTILINE | wxTE_READONLY | wxBORDER_NONE);

	#endif
	
	#ifdef _WIN32
		OBJ_LOG->SetLayoutDirection(wxLayout_RightToLeft);
	#elif  __APPLE__
		//OBJ_LOG->SetLayoutDirection(wxLayout_RightToLeft);
	#else
		OBJ_LOG->SetLayoutDirection(wxLayout_RightToLeft);
	#endif
	
	#if  __APPLE__

		// Mac OS Log

		// Log Text - Font
		wxFont* LOG_FONT = new wxFont(18,
		wxFONTFAMILY_MODERN, // wxFONTFAMILY_SWISS
		wxFONTSTYLE_NORMAL, // wxFONTSTYLE_ITALIC
		wxFONTWEIGHT_NORMAL, // wxFONTWEIGHT_NORMAL / wxFONTWEIGHT_BOLD
		false, // 
		"Courier New");
		OBJ_LOG->SetFont(*LOG_FONT);

	#else

		// Window / Linux - Log

		// Log Text - Font
		wxFont* LOG_FONT = new wxFont(12,
		wxFONTFAMILY_MODERN, // wxFONTFAMILY_SWISS
		wxFONTSTYLE_NORMAL, // wxFONTSTYLE_ITALIC
		wxFONTWEIGHT_NORMAL, // wxFONTWEIGHT_NORMAL / wxFONTWEIGHT_BOLD
		false, // 
		"Courier New");
		OBJ_LOG->SetFont(*LOG_FONT);

	#endif

	// Black
		//OBJ_LOG->SetForegroundColour(wxColour(255, 128, 0));
		//OBJ_LOG->SetBackgroundColour(wxColour(41, 49, 52)); // 41, 49, 52
	// White
		OBJ_LOG->SetForegroundColour(wxColour(167, 65, 75)); // Red
		OBJ_LOG->SetBackgroundColour(wxColour(244, 244, 244));
	
	// Log Text - Maring
	OBJ_LOG->SetMargins(20, 20);
	
	// -------------------------
	// Splinting
	// -------------------------
	
	//OBJ_MAIN_SPLITTER->SetSashGravity(0.5);
	//OBJ_MAIN_SPLITTER->SetMinimumPaneSize(50); // Smalest size the
	//OBJ_MAIN_SIZER->Add(OBJ_MAIN_SPLITTER, 1, wxEXPAND|wxALL, 0);
	//OBJ_CODE_SIZER->Add(OBJ_CODE, 1, wxEXPAND|wxALL, 0);
	//OBJ_PANEL_1->SetSizer(OBJ_CODE_SIZER);
	//OBJ_LOG_SIZER->Add(OBJ_LOG, 1, wxEXPAND|wxALL, 0);
	//OBJ_PANEL_2->SetSizer(OBJ_LOG_SIZER);
	//OBJ_MAIN_SPLITTER->SplitHorizontally(OBJ_PANEL_1, OBJ_PANEL_2);
	//this->SetSizer(OBJ_MAIN_SIZER);
	//OBJ_MAIN_SIZER->SetSizeHints(this);
	//OBJ_MAIN_SPLITTER->SetSashPosition(1000);
	
	// -------------------------
	// Window Style
	// -------------------------
	
	//OBJ_CLASS_WINDOW_MAIN->SetBackgroundColour(wxColour(* wxYELLOW));
	//OBJ_PANEL_1->SetBackgroundColour(wxColour(239, 241, 245));
	//OBJ_PANEL_2->SetBackgroundColour(wxColour(244, 244, 244));
	
	// -------------------------
	// Tree
	// -------------------------
	
	OBJ_TREE_FILES_LIST = new wxTreeCtrl(this, ID_TREE_FILES, wxPoint(0,0), wxSize(400,400), 
	wxTR_NO_BUTTONS | wxTR_SINGLE | wxTR_ROW_LINES);
	
	OBJ_TREE_WINDOW = new wxTreeCtrl(this, ID_TREE_WINDOWS, wxPoint(0,0), wxSize(400,400));
	
	// -------------------------
	// Tree Files List Images
	// -------------------------
	
	wxImageList* TREE_IMAGES_LIST_FILES = new wxImageList(24, 24);
	TREE_IMAGES_LIST_FILES->Add(wxIcon(folder_open_bleu_xpm)); // 0 Folder
	TREE_IMAGES_LIST_FILES->Add(wxIcon(file_bleu_xpm)); // 1 Alif
	TREE_IMAGES_LIST_FILES->Add(wxIcon(multi_window_xpm)); // 2 UI
	TREE_IMAGES_LIST_FILES->Add(wxIcon(file_red_xpm)); // 3 Lib
	
	wxImageList* TREE_IMAGES_LIST_WINDOW = new wxImageList(21, 21);
	TREE_IMAGES_LIST_WINDOW->Add(wxIcon(multi_window_xpm)); // 0
	TREE_IMAGES_LIST_WINDOW->Add(wxIcon(window_xpm)); // 1
	
	OBJ_TREE_FILES_LIST->SetImageList(TREE_IMAGES_LIST_FILES);
	//OBJ_TREE_WINDOW->SetImageList(TREE_IMAGES_LIST_WINDOW);
	
	// -------------------------
	// Tree Files List Data
	// -------------------------
	
	TREE_FILES_TOTAL_ITEM = 0;
	TREE_FILES_HAVE_ITEM = false;

	TREE_WINDOW_TOTAL_ITEM = 0;
	
	// -------------------------
	// wxAUI Add Widgets
	// -------------------------
	
	// add the panes to the manager

	// #if defined(_WIN32) || defined(__APPLE__)

	#ifdef _WIN32

		AUI_MANAGER.AddPane(OBJ_CODE, //wxCENTER);
		wxAuiPaneInfo().CaptionVisible(false).CloseButton(false).BestSize(wxSize(wxDefaultCoord,wxDefaultCoord)).
		MinSize(100,100).MaxSize(wxDefaultCoord,wxDefaultCoord).Resizable(false).Floatable(false).Center() );
		
		AUI_MANAGER.AddPane(OBJ_TREE_FILES_LIST, //wxLEFT, 
		wxAuiPaneInfo().Caption(wxT(" المـلـفـات ")).CloseButton(false).BestSize(wxSize(200,wxDefaultCoord)).
		MinSize(50,50).MaxSize(500,500).Resizable(true).Floatable(true).Left() );
		
		AUI_MANAGER.AddPane(OBJ_TREE_WINDOW, //wxLEFT, 
		wxAuiPaneInfo().Caption(wxT(" الـواجـهـة ")).CloseButton(false).BestSize(wxSize(200,wxDefaultCoord)).
		MinSize(50,50).MaxSize(500,500).Resizable(true).Floatable(true).Right() );
		
		AUI_MANAGER.AddPane(OBJ_TREE_CONTROLS, //wxRIGHT, 
		wxAuiPaneInfo().Caption(wxT(" الأداوات ")).CloseButton(false).BestSize(wxSize(200,wxDefaultCoord)).
		MinSize(50,50).MaxSize(500,500).Resizable(true).Floatable(true).Right() );
		
		AUI_MANAGER.AddPane(OBJ_PROPERTIES, //wxRIGHT, 
		wxAuiPaneInfo().Caption(wxT(" الـخـصـائـص ")).CloseButton(false).BestSize(wxSize(200,wxDefaultCoord)).
		MinSize(50,50).MaxSize(500,500).Resizable(true).Floatable(true).Right() );
		
		AUI_MANAGER.AddPane(OBJ_LOG, //wxBOTTOM, 
		wxAuiPaneInfo().Caption(wxT(" الـرسـائـل ")).CloseButton(false).BestSize(wxSize(wxDefaultCoord,100)).
		MinSize(50,50).MaxSize(wxDefaultCoord,500).Resizable(true).Floatable(true).Bottom() );

	#elif  __APPLE__

		AUI_MANAGER.AddPane(OBJ_CODE, //wxCENTER);
		wxAuiPaneInfo().CaptionVisible(false).CloseButton(false).BestSize(wxSize(wxDefaultCoord,wxDefaultCoord)).
		MinSize(100,100).MaxSize(wxDefaultCoord,wxDefaultCoord).Resizable(false).Floatable(false).Center() );
		
		AUI_MANAGER.AddPane(OBJ_TREE_FILES_LIST, //wxLEFT, 
		wxAuiPaneInfo().Caption(wxT(" المـلـفـات ")).CloseButton(false).BestSize(wxSize(200,wxDefaultCoord)).
		MinSize(50,50).MaxSize(500,500).Resizable(true).Floatable(true).Left() );
		
		AUI_MANAGER.AddPane(OBJ_TREE_WINDOW, //wxLEFT, 
		wxAuiPaneInfo().Caption(wxT(" الـواجـهـة ")).CloseButton(false).BestSize(wxSize(200,wxDefaultCoord)).
		MinSize(50,50).MaxSize(500,500).Resizable(true).Floatable(true).Right() );
		
		AUI_MANAGER.AddPane(OBJ_TREE_CONTROLS, //wxRIGHT, 
		wxAuiPaneInfo().Caption(wxT(" الأداوات ")).CloseButton(false).BestSize(wxSize(200,wxDefaultCoord)).
		MinSize(50,50).MaxSize(500,500).Resizable(true).Floatable(true).Right() );
		
		AUI_MANAGER.AddPane(OBJ_PROPERTIES, //wxRIGHT, 
		wxAuiPaneInfo().Caption(wxT(" الـخـصـائـص ")).CloseButton(false).BestSize(wxSize(200,wxDefaultCoord)).
		MinSize(50,50).MaxSize(500,500).Resizable(true).Floatable(true).Right() );
		
		AUI_MANAGER.AddPane(OBJ_LOG, //wxBOTTOM, 
		wxAuiPaneInfo().Caption(wxT(" الـرسـائـل ")).CloseButton(false).BestSize(wxSize(wxDefaultCoord,100)).
		MinSize(50,50).MaxSize(wxDefaultCoord,500).Resizable(true).Floatable(true).Bottom() );

	#else

		AUI_MANAGER.AddPane(OBJ_CODE, //wxCENTER);
		wxAuiPaneInfo().CaptionVisible(false).CloseButton(false).BestSize(wxSize(wxDefaultCoord,wxDefaultCoord)).
		MinSize(100,100).MaxSize(wxDefaultCoord,wxDefaultCoord).Resizable(false).Floatable(false).Center() );
		
		AUI_MANAGER.AddPane(OBJ_TREE_FILES_LIST, //wxLEFT, 
		wxAuiPaneInfo().Caption(wxT(" المـلـفـات ")).CloseButton(false).BestSize(wxSize(200,wxDefaultCoord)).
		MinSize(50,50).MaxSize(500,500).Resizable(true).Floatable(true).Left() );

		AUI_MANAGER.AddPane(OBJ_PROPERTIES, //wxRIGHT, 
		wxAuiPaneInfo().Caption(wxT(" الـخـصـائـص ")).CloseButton(false).BestSize(wxSize(200,wxDefaultCoord)).
		MinSize(50,50).MaxSize(500,500).Resizable(true).Floatable(true).Right() );

		AUI_MANAGER.AddPane(OBJ_TREE_CONTROLS, //wxRIGHT, 
		wxAuiPaneInfo().Caption(wxT(" الأداوات ")).CloseButton(false).BestSize(wxSize(200,wxDefaultCoord)).
		MinSize(50,50).MaxSize(500,500).Resizable(true).Floatable(true).Right() );

		AUI_MANAGER.AddPane(OBJ_TREE_WINDOW, //wxLEFT, 
		wxAuiPaneInfo().Caption(wxT(" الـواجـهـة ")).CloseButton(false).BestSize(wxSize(200,wxDefaultCoord)).
		MinSize(50,50).MaxSize(500,500).Resizable(true).Floatable(true).Right() );
		
		AUI_MANAGER.AddPane(OBJ_LOG, //wxBOTTOM, 
		wxAuiPaneInfo().Caption(wxT(" الـرسـائـل ")).CloseButton(false).BestSize(wxSize(wxDefaultCoord,100)).
		MinSize(50,50).MaxSize(wxDefaultCoord,500).Resizable(true).Floatable(true).Bottom() );

	#endif
	
	// -------------------------
	// wxAUI Update
	// -------------------------

	 AUI_MANAGER.Update();

	// -------------------------
	// UI Initilizing
	// -------------------------

	UI_BUTTON_SAVE(false);
	UI_BUTTON_BUILD(false);
	#ifdef ALIF_STUDIO_NO_STC
		OBJ_CODE->Clear();
	#else
		OBJ_CODE->ClearAll();
	#endif
	OBJ_CODE->Enable(false);
	OBJ_CODE->SetEditable(false);
	OBJ_LOG->Clear();

	FIND_WORD = wxT("");

	OBJ_LOG->SetEditable(false);

	// -------------------------
	// Wrong Path Installation
	// -------------------------

	if (PATH_CONTAIN_SPACE)
	{
		wxString Fixed_Path;

		#ifdef _WIN32
			Fixed_Path = "C:\\Alif\\";
		#elif  __APPLE__
			Fixed_Path = "/Applications/Alif Studio.app";
		#else
			Fixed_Path = "/usr/local/bin/";
		#endif

		wxMessageBox(wxT("انتباه ! \n"
						"لقد قمتم بتتبيت ألف ستوديو في مسار \nفيه مجلدات باسماء تحتوي على مسافات \n\n")
						+ INSTALL_PATH + 
						wxT("\n\nبرنامج مترجم ألف لا يستطيع أن يعمل \nداخل مجلد باسم يحتوي على مسافة فراغ\n"
						"المرجو إزالة ألف ستوديو، ثم إعادة تتبيته \nفي مجلد أخر، على سبيل المثال \n\n") + Fixed_Path, 
						wxT("انتباه !"), wxICON_WARNING);
	}
}

// ------------------------------------------------
// UI Window Constructeur
// ------------------------------------------------

UI_CLASS_WINDOW :: UI_CLASS_WINDOW(wxString CAPTION, wxPoint pos, wxSize size) : 
	wxDialog(NULL, ID_UI_WINDOW, CAPTION, pos, size, 
	#ifdef _WIN32
		wxCAPTION | wxRESIZE_BORDER)
	#elif __APPLE__
		wxCAPTION | wxRESIZE_BORDER | wxSTAY_ON_TOP)
	#else
		wxCAPTION | wxRESIZE_BORDER | wxSTAY_ON_TOP)
	#endif
	// wxTHICK_FRAME | wxSTAY_ON_TOP
	// wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN) 
	// wxFRAME_NO_TASKBAR | wxFRAME_FLOAT_ON_PARENT | wxRESIZE_BORDER
{
	Centre();
}

// ****************************************************************************
// ** Destructeur ****************************************************************
// ****************************************************************************

// ------------------------------------------------
// Window Main Destructeur
// ------------------------------------------------

CLASS_WINDOW_MAIN::~CLASS_WINDOW_MAIN()
{
	// after execution of OnClose()
	// deinitialize the frame manager
    AUI_MANAGER.UnInit();
}

// ------------------------------------------------
// UI Window Destructeur
// ------------------------------------------------

UI_CLASS_WINDOW::~UI_CLASS_WINDOW()
{
	// ...
}

// ****************************************************************************
// ** Application *************************************************************
// ****************************************************************************

// ------------------------------------------------
// Application Class
// ------------------------------------------------

// Toute application wxWidgets doit normalement posséder une classe dérivée de wxApp
// Déclaration de notre classe dérivée de wxApp

class MyApp : public wxApp
{
    public:
	
		// ------------------------------------------------
		// Application Class Constructeur
		// ------------------------------------------------
		
        MyApp();
		
		// ------------------------------------------------
		// Application Class Destructeur
		// ------------------------------------------------
		
		~MyApp();
		
		// ------------------------------------------------
		// Application Class Initialization
		// ------------------------------------------------
		
		virtual bool OnInit();
		
		// ------------------------------------------------
		// Application Class Exit
		// ------------------------------------------------
		
        virtual int OnExit();
        
	private:
		//bool silent_mode;
		DECLARE_NO_COPY_CLASS(MyApp)
};

// ------------------------------------------------
// Set Main Application Class
// ------------------------------------------------

DECLARE_APP(MyApp);

// ------------------------------------------------
// Redirect Execution to Main Application Class
// ------------------------------------------------

IMPLEMENT_APP(MyApp);

// ------------------------------------------------
// Application Class Constructeur
// ------------------------------------------------

MyApp::MyApp()
{
	//wxMessageBox(wxT("app Constructeur"));
}

// ------------------------------------------------
// Application Class Destructeur
// ------------------------------------------------

MyApp::~MyApp()
{
	//wxMessageBox(wxT("app Destructeur"));
}

// ------------------------------------------------
// Application Class Exit
// ------------------------------------------------

int MyApp::OnExit()
{
	//wxMessageBox(wxT("app exite"));
	return 0;
}

// ------------------------------------------------
// Trim
// ------------------------------------------------

wxString TRIM(wxString STR)
{
	STR = STR.Trim(false);
	return STR.Trim(true);
}

void AUI_GUI_DESIGNER(bool SHOW)
{
	// wxAUI Hide GUI Panels

	wxAuiPaneInfo& PI_Window = AUI_MANAGER.GetPane(OBJ_TREE_WINDOW);
	wxAuiPaneInfo& PI_Control = AUI_MANAGER.GetPane(OBJ_TREE_CONTROLS);
	wxAuiPaneInfo& PI_Prop = AUI_MANAGER.GetPane(OBJ_PROPERTIES);

	if (SHOW)
	{
		if (PI_Window.IsOk())
			PI_Window.Show();

		if (PI_Control.IsOk())
			PI_Control.Show();

		if (PI_Prop.IsOk())
			PI_Prop.Show();

		AUI_MANAGER.Update();
	}
	else
	{
		if (PI_Window.IsOk() && PI_Window.IsShown())
			PI_Window.Hide();

		if (PI_Control.IsOk() && PI_Control.IsShown())
			PI_Control.Hide();

		if (PI_Prop.IsOk() && PI_Prop.IsShown())
			PI_Prop.Hide();

		AUI_MANAGER.Update();
	}
}

// ------------------------------------------------
// Application Class Initialization
// ------------------------------------------------

bool MyApp::OnInit()
{
	// ------------------------------------------------
	// Application Class IF Failure
	// ------------------------------------------------
	
	// to ignore wx auto command arguments Parser 
	// wxCmdLineParser()

	//if (!wxApp::OnInit())
		//return false;

	// -------------------------
	// ARGv
	// -------------------------

	// myapp.bin -A -B -C
	// wxApp::argc 		= 4
	// wxApp::argv[0]	= 'myapp.bin'
	// wxApp::argv[1]	= '-A'
	// ...

	if ((wxApp::argc - 1) == 1 && wxFileName::FileExists(wxApp::argv[1]))
	{
		// Double click on file

		ARG_OPEN_FILE = true;
		ARG_OPEN_FILE_PATH = wxApp::argv[1];
	}
	else if (wxApp::argc > 1)
	{
		ARG_OPEN_FILE = false;

		/*
		wxString ARG[wxApp::argc];

		wxMessageBox(CONVERT_INT_TO_STRING(wxApp::argc - 1));

		for (int i = 1; i < wxApp::argc; i++)
		{
			ARG[i] = wxApp::argv[i];

			wxMessageBox(ARG[i]);
		}
		*/
	}

	wxString NEW_UPDATER_PATH;

	#ifdef _WIN32

		// Windows

		// C:\Program Files (x86)\Alif Studio
		//									 -->\alif.exe
		//									 -->\bin\gcc.exe
		//									 -->\include\wx\wx.h
		//									 -->\lib\libwxmsw31u_alif_lib_core
		//									 -->\aliflib\msg.aliflib

		// 'C:\Program Files (x86)\Alif Studio\alifstudio.exe'
		wxFileName fname( ::wxStandardPaths::Get().GetExecutablePath() );
		INSTALL_PATH = fname.GetPathWithSep();

		PATH_FULL_ALIF_COMPILER = INSTALL_PATH + "alif.exe";
		PATH_FULL_UPDATE = INSTALL_PATH + "mupdate.exe";
		NEW_UPDATER_PATH = INSTALL_PATH + "_mupdate.exe";
		PATH_FULL_PDF = INSTALL_PATH + "Alif_Arabic_Programming_Language.pdf";

		// C:\Users\test\AppData\Local
		wxFileName tfname(wxStandardPaths::Get().GetTempDir());
		PATH_TEMP = tfname.GetPath();

		PATH_FULL_SOURCE = "";			// When User dclick on files list
		SOURCE_FILE_NAME = "";			// When User dclick on files list
		CURRENT_FILE_EXTENSION = "";	// When User dclick on files list
		PATH_FULL_EXECUTABLE = "";		// When User click on Build.. SET_BINARY_PATH()
		PATH_FULL_LOG = "";				// When User Click on Build..
		PATH_DIRECTORY = "";			// When User dclick on Open

		// -------------------------
		// Get Alif Version - Windows
		// -------------------------

		string LINE;
		bool LINE_ONE = true;

		ifstream FILE_STREAM(INSTALL_PATH + "win32_alif_version.inf");

		if (!FILE_STREAM.is_open())
		{
			wxMessageBox(wxT(" الملف التالي غير موجود \n \n ") + 
					INSTALL_PATH + "win32_alif_version.inf" + 
					wxT(" \n \n المرجو اعادت ثتبيت البرنامج \n او تحميله من الموقع الرسمي \n \n www.aliflang.org"));
		}
		else
		{
			while (getline(FILE_STREAM, LINE))
			{
				if (LINE_ONE)
				{
					LINE_ONE = false;
				}
				else
				{
					LINE = TRIM(LINE);

					ALIF_VERSION = LINE;
					break;
				}
			}
		}

		// Check if GCC Path containe space
		// because GCC, Windres don't work correctly
		// if path containes blank spaces.

		std::size_t found = INSTALL_PATH.find(" ");

		if (found == std::string::npos)
		{
			// Initialized again for exception !
			PATH_CONTAIN_SPACE = false;
		}
		else
		{
			PATH_CONTAIN_SPACE = true;
		}

	#elif  __APPLE__

		// Mac OS X
		
		// alif_1.0-1.pkg
		//		/usr/local/bin/alif
		//		/Library/Application Support/Aliflang/Alif_Compiler/aliflib/aliflib.inf
		//		/Library/Application Support/Aliflang/Alif_Compiler/mac_alif_version.inf
		//		/Library/Application Support/Aliflang/Alif_Compiler/Alif_Arabic_Programming_Language
		//		/Library/Application Support/Aliflang/Alif_Compiler/alif.icns
		//		/Library/Application Support/Aliflang/Alif_Compiler/Info.plist

		// aliflibwx_3.1-1.pkg
		//		/Library/Application Support/Aliflang/Alif_Lib_WX/include/[wx][boost][utf8]
		//		/Library/Application Support/Aliflang/Alif_Lib_WX/lib/[.a]
		//		/Library/Application Support/Aliflang/Alif_Lib_WX/mac_aliflibwx_version.inf
		
		// alifstudio_1.0-1.pkg
		//		/Applications/Alif Studio.app
		//		/Library/Application Support/Aliflang/Alif_Studio/mac_alifstudio_version.inf
		//		/Library/Application Support/Aliflang/Alif_Studio/alifstudio.png
		//		/Library/Application Support/Aliflang/Alif_Studio/mupdate

		wxFileName fname( ::wxStandardPaths::Get().GetExecutablePath() );
		INSTALL_PATH = fname.GetPathWithSep();

		PATH_FULL_ALIF_COMPILER = "/usr/local/bin/alif";
		PATH_FULL_UPDATE = "/Library/Application Support/Aliflang/Alif_Studio/mupdate";
		NEW_UPDATER_PATH = "/Library/Application Support/Aliflang/Alif_Studio/_mupdate";
		PATH_FULL_PDF = "/Library/Application Support/Aliflang/Alif_Compiler/Alif_Arabic_Programming_Language.pdf";

		// /var/tmp
		wxFileName tfname(wxStandardPaths::Get().GetTempDir());
		PATH_TEMP = tfname.GetPath();

		if (PATH_TEMP.Len() < 4)
			PATH_TEMP = "/tmp";

		PATH_FULL_SOURCE = "";			// When User dclick on files list
		SOURCE_FILE_NAME = "";			// When User dclick on files list
		CURRENT_FILE_EXTENSION = "";	// When User dclick on files list
		PATH_FULL_EXECUTABLE = "";		// When User click on Build.. SET_BINARY_PATH()
		PATH_FULL_LOG = "";				// When User Click on Build..
		PATH_DIRECTORY = "";			// When User dclick on Open

		// -------------------------
		// Get Alif Version - Mac OS
		// -------------------------

		string LINE;
		bool LINE_ONE = true;

		ifstream FILE_STREAM("/Library/Application Support/Aliflang/Alif_Compiler/mac_alif_version.inf");

		if (!FILE_STREAM.is_open())
		{
			wxMessageBox(wxT(" الملف التالي غير موجود \n \n /Library/Application Support/Aliflang/Alif_Compiler/mac_alif_version.inf \n \n المرجو اعادت ثتبيت البرنامج \n او تحميله من الموقع الرسمي \n \n www.aliflang.org"));
		}
		else
		{
			while (getline(FILE_STREAM, LINE))
			{
				if (LINE_ONE)
				{
					LINE_ONE = false;
				}
				else
				{
					LINE = TRIM(LINE);

					ALIF_VERSION = LINE;
					break;
				}
			}
		}

		/*
		// Check if GCC Path containe space
		// because GCC, Windres don't work correctly
		// if path containes blank spaces.

		// Remove '' from Path
		// /.../.../Alif Studio.app/Contents/MacOS/

		INSTALL_PATH = INSTALL_PATH.substr(0, (INSTALL_PATH.length() - 16) );

		std::size_t found = INSTALL_PATH.find_last_of("/\\");

		INSTALL_PATH = INSTALL_PATH.substr(0, found + 1);

		found = INSTALL_PATH.find(" ");

		if (found == std::string::npos)
		{
			// Initialized again for exception !
			PATH_CONTAIN_SPACE = true;
		}
		else
		{
			PATH_CONTAIN_SPACE = true;
		}
		*/

	#else

		// Linux

		// aliflang_1.0-1.deb
		//		/usr/local/bin/alif.bin
		//		/usr/local/lib/aliflib/risalah.aliflib
		//		/usr/local/share/aliflang/copyright
		//		/usr/local/share/aliflang/linux_alif_version.inf
		//		/usr/local/share/aliflang/Alif_Arabic_Programming_Language

		// aliflibwx_3.1-1.deb
		//		/usr/local/include/aliflibwx/[wx][boost][utf8]
		//		/usr/local/lib/aliflibwx/[.a]
		//		/usr/local/share/aliflibwx/linux_aliflibwx_version.inf
		//		/usr/share/doc/aliflibwx/copyright (txt)

		// alifstudio_1.0-1.deb
		//		/usr/local/bin/alifstudio.bin
		//		/usr/local/bin/mupdate.bin
		//		/usr/local/share/alifstudio/copyright
		//		/usr/local/share/alifstudio/linux_alifstudio_version.inf
		//		/usr/share/alifstudio/alifstudio.ico
		//		/usr/share/alifstudio/alifstudio.png
		//		/usr/share/applications/'Alif Studio'

		wxFileName fname( ::wxStandardPaths::Get().GetExecutablePath() );
		INSTALL_PATH = fname.GetPathWithSep();

		PATH_FULL_ALIF_COMPILER = INSTALL_PATH + "alif";
		PATH_FULL_UPDATE = INSTALL_PATH + "mupdate";
		NEW_UPDATER_PATH = INSTALL_PATH + "_mupdate";
		PATH_FULL_PDF = "/usr/local/share/aliflang/Alif_Arabic_Programming_Language.pdf";

		// /var/tmp
		wxFileName tfname(wxStandardPaths::Get().GetTempDir());
		PATH_TEMP = tfname.GetPath();

		if (PATH_TEMP.Len() < 4)
			PATH_TEMP = "/var/tmp";

		PATH_FULL_SOURCE = "";			// When User dclick on files list
		SOURCE_FILE_NAME = "";			// When User dclick on files list
		CURRENT_FILE_EXTENSION = "";	// When User dclick on files list
		PATH_FULL_EXECUTABLE = "";		// When User click on Build.. SET_BINARY_PATH()
		PATH_FULL_LOG = "";				// When User Click on Build..
		PATH_DIRECTORY = "";			// When User dclick on Open

		// -------------------------
		// Get Alif Version - Linux
		// -------------------------

		string LINE;
		bool LINE_ONE = true;

		ifstream FILE_STREAM("/usr/local/share/aliflang/linux_alif_version.inf");

		if (!FILE_STREAM.is_open())
		{
			wxMessageBox(wxT(" الملف التالي غير موجود \n \n /usr/local/share/aliflang/linux_alif_version.inf \n \n المرجو اعادت ثتبيت البرنامج \n او تحميله من الموقع الرسمي \n \n www.aliflang.org"));
		}
		else
		{
			while (getline(FILE_STREAM, LINE))
			{
				if (LINE_ONE)
				{
					LINE_ONE = false;
				}
				else
				{
					LINE = TRIM(LINE);

					ALIF_VERSION = LINE;
					break;
				}
			}
		}

		/*
		// Check if GCC Path containe space
		// because GCC, Windres don't work correctly
		// if path containes blank spaces.

		std::size_t found = INSTALL_PATH.find(" ");

		if (found == std::string::npos)
		{
			// Initialized again for exception !
			PATH_CONTAIN_SPACE = false;
		}
		else
		{
			PATH_CONTAIN_SPACE = true;
		}
		*/

	#endif

	// -------------------------
	// Splash IMG Array
	// -------------------------
	/*
	wxInitAllImageHandlers();

	wxSplashScreen* Alif_Studio_Splash = new wxSplashScreen(wxGetBitmapFromMemory(welcome, sizeof(welcome)), 
															wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT, // wxSPLASH_NO_TIMEOUT
															500, NULL, 5000, wxDefaultPosition, wxDefaultSize,
															wxBORDER_SIMPLE | wxFRAME_NO_TASKBAR); // wxSTAY_ON_TOP
	// 2 Secondes of Freez,
	// then 0.5 Seconde Splash timeout
	wxSleep(2);

	// Splash
	wxYield();
	*/
	// -------------------------
	// Splash PNG
	// -------------------------
	
	wxInitAllImageHandlers();

	#ifdef _WIN32

		if (wxFileName::FileExists(INSTALL_PATH + "alifstudio.png"))

	#elif  __APPLE__

		if (wxFileName::FileExists("/Library/Application Support/Aliflang/Alif_Studio/alifstudio.png"))

	#else

		if (wxFileName::FileExists("/usr/share/alifstudio/alifstudio.png"))

	#endif
	{
		wxBitmap bitmap;
		#ifdef _WIN32

			if (bitmap.LoadFile(INSTALL_PATH + "alifstudio.png", wxBITMAP_TYPE_PNG))
			{
				wxSplashScreen* Alif_Studio_Splash = new wxSplashScreen(bitmap, 
																		wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT, // wxSPLASH_NO_TIMEOUT
																		500, NULL, 5000, wxDefaultPosition, wxDefaultSize,
																		wxBORDER_SIMPLE | wxFRAME_NO_TASKBAR); // wxSTAY_ON_TOP
				// 2 Secondes of Freez, 
				// then 0.5 Seconde Splash timeout
				wxSleep(2);

				// Splash
				wxYield();
			}

		#elif  __APPLE__

			if (bitmap.LoadFile("/Library/Application Support/Aliflang/Alif_Studio/alifstudio.png", wxBITMAP_TYPE_PNG))
			{
				wxSplashScreen* Alif_Studio_Splash = new wxSplashScreen(bitmap, 
																		wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT, // wxSPLASH_NO_TIMEOUT
																		500, NULL, 5000, wxDefaultPosition, wxDefaultSize,
																		wxBORDER_SIMPLE | wxFRAME_NO_TASKBAR); // wxSTAY_ON_TOP
				// 2 Secondes of Freez, 
				// then 0.5 Seconde Splash timeout
				wxSleep(2);

				// Splash
				wxYield();
			}

		#else

			if (bitmap.LoadFile("/usr/share/alifstudio/alifstudio.png", wxBITMAP_TYPE_PNG))
			{
				wxSplashScreen* Alif_Studio_Splash = new wxSplashScreen(bitmap, 
																		wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT, // wxSPLASH_NO_TIMEOUT
																		2000, NULL, 5000, wxDefaultPosition, wxDefaultSize,
																		wxBORDER_SIMPLE | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP); // wxSTAY_ON_TOP
			}

		#endif
	}

	// ------------------------------------------------
	// Update Updater
	// ------------------------------------------------

	if (wxFileName::FileExists(NEW_UPDATER_PATH))
	{
		if (wxRemoveFile(PATH_FULL_UPDATE))
		{
			wxRenameFile(NEW_UPDATER_PATH, PATH_FULL_UPDATE);
		}
	}

	// ------------------------------------------------
	// Check Setup
	// ------------------------------------------------

	if (!wxFileName::FileExists(PATH_FULL_ALIF_COMPILER))
	{
		wxMessageBox(wxT(" مترجم ألف غير موجود \n \n ") + 
					PATH_FULL_ALIF_COMPILER + 
					wxT(" \n \n المرجو اعادت ثتبيت البرنامج \n او تحميله من الموقع الرسمي \n \n www.aliflang.org"), wxT("ألف ستوديو ") + ALIF_STUDIO_VERSION);
		
		return false; // Exit
	}
	
	// ------------------------------------------------
	// Application Class -> Widget Constructeur
	// ------------------------------------------------
	
	OBJ_CLASS_WINDOW_MAIN = new CLASS_WINDOW_MAIN();
	
	#ifdef _WIN32
		OBJ_CLASS_WINDOW_MAIN->SetLayoutDirection(wxLayout_RightToLeft);
	#elif  __APPLE__
		//OBJ_CLASS_WINDOW_MAIN->SetLayoutDirection(wxLayout_RightToLeft);
	#else
		OBJ_CLASS_WINDOW_MAIN->SetLayoutDirection(wxLayout_RightToLeft);
	#endif
	
	SetTopWindow(OBJ_CLASS_WINDOW_MAIN);
	OBJ_CLASS_WINDOW_MAIN->Center(true);
	OBJ_CLASS_WINDOW_MAIN->Maximize(true);
	OBJ_CLASS_WINDOW_MAIN->Refresh();
	OBJ_CLASS_WINDOW_MAIN->Show();

	// -------------------------
	// ARG Open File
	// -------------------------

	if (ARG_OPEN_FILE)
	{
		OPEN_NEW_FILE(ARG_OPEN_FILE_PATH);
	}
	else
	{
		// Hide Aui Designer Panels
		AUI_GUI_DESIGNER(false);
	}
	
	// ------------------------------------------------
    return true;
}

// ****************************************************************************
// ** Class Functions *********************************************************
// ****************************************************************************

// ------------------------------------------------
// UI Window Close
// ------------------------------------------------

void UI_CLASS_WINDOW::OnClose(wxCloseEvent& event)
{
	event.Skip();
}

// -------------------------------

void UI_CLASS_WINDOW::OnNavigationKey(wxNavigationKeyEvent& event)
{
	event.Skip(false); // To disable Tab key.
}

// -------------------------------

void UI_CLASS_WINDOW::OnResize(wxSizeEvent& event)
{
	event.Skip();
	
	UI_WINDOW_W[ID_UI_WINDOW_CURRENT] = OBJ_UI_CLASS_WINDOW->GetSize().x;
	UI_WINDOW_H[ID_UI_WINDOW_CURRENT] = OBJ_UI_CLASS_WINDOW->GetSize().y;
	ID_UI_CONTROL_CURRENT_TYPE = "WINDOW";
	ID_UI_CONTROL_CURRENT = 0;
	
	UI_UPDATE_SETTINGS(0);
	
	if (!WINDOW_RESIZING_BY_SYSTEM) // Be sur its not resize from OS on window creation!
		UI_GENERAT_ALIF_CODE();
}

// ------------------------------------------------
// Window Main Close
// ------------------------------------------------

void CLASS_WINDOW_MAIN::OnClose(wxCloseEvent& event)
{
	event.Skip(false);
	Exit();
}

// -------------------------------

void CLASS_WINDOW_MAIN::OnCloseMenu(wxCommandEvent& event)
{
	event.Skip(false);
	Exit();
}

// -------------------------------

void CLASS_WINDOW_MAIN::TREE_CONTROLES_DCLICK(wxTreeEvent& event)
{
	event.Skip();

	if (!WINDOW_IS_CONSTRUCTED)
		return;
	
	wxTreeItemId TREE_SELECTED_ID;
	TREE_SELECTED_ID = event.GetItem();
	
	if (TREE_SELECTED_ID == OBJ_TREE_CONTROLS->GetRootItem())
	{
		return;
	}
	else if (TREE_SELECTED_ID == TREE_ID_CONTROL_ITEM_BUTTON)
	{
		DRAW_CONTROL = true;
		DRAW_TYPE = "BUTTON";
		//this->SetCursor( wxCURSOR_HAND );
	}
	else if (TREE_SELECTED_ID == TREE_ID_CONTROL_ITEM_TXT)
	{
		DRAW_CONTROL = true;
		DRAW_TYPE = "TXT";
	}
	else if (TREE_SELECTED_ID == TREE_ID_CONTROL_ITEM_LABEL)
	{
		DRAW_CONTROL = true;
		DRAW_TYPE = "LABEL";
	}
}

// -------------------------------

void CLASS_WINDOW_MAIN::TREE_WINDOW_RCLICK(wxTreeEvent& event)
{
	event.Skip();
	
	wxTreeItemId TREE_SELECTED_ID;
	TREE_SELECTED_ID = event.GetItem();
	
	if (TREE_SELECTED_ID == OBJ_TREE_WINDOW->GetRootItem())
		return;
	
	for (unsigned int i = 1; i <= TREE_WINDOW_TOTAL_ITEM; i++) // First Window ID is 1
	{
		if (TREE_SELECTED_ID == TREE_ID_WINDOW_ITEM[i])
		{
			//wxMessageBox(UI_WINDOW_IDENTIFIER[i]);
		}
	}
}

// -------------------------------

void CLASS_WINDOW_MAIN::TREE_WINDOW_DCLICK(wxTreeEvent& event)
{
	event.Skip();
	
	UI_EMPTY_SETTINGS();
	
	wxTreeItemId TREE_SELECTED_ID;
	TREE_SELECTED_ID = event.GetItem();
	
	if (TREE_SELECTED_ID == OBJ_TREE_WINDOW->GetRootItem())
		return;
	
	for (unsigned int i = 1; i <= TREE_WINDOW_TOTAL_ITEM; i++) // First Window ID is 1
	{
		if (TREE_SELECTED_ID.GetID() == TREE_ID_WINDOW_ITEM[i])
		{
			ID_UI_WINDOW_NAME_CURRENT = UI_WINDOW_IDENTIFIER[i];
			ID_UI_WINDOW_CURRENT = i;
			
			ID_UI_CONTROL_CURRENT = 0;
			ID_UI_CONTROL_CURRENT_TYPE = ""; // No Window or Control is selected!
			
			if (WINDOW_IS_CONSTRUCTED) // Check if there already bold win list
				OBJ_TREE_WINDOW->SetItemBold(TREE_ID_WINDOW_LAST_BOLD, false);

			OBJ_TREE_WINDOW->SetItemBold(TREE_SELECTED_ID, true);
			TREE_ID_WINDOW_LAST_BOLD = TREE_SELECTED_ID;
			
			// ----------------------------------------
			// Destroy Window
			// ----------------------------------------
			
			if (WINDOW_IS_CONSTRUCTED)
			{
				OBJ_UI_CLASS_WINDOW->Destroy();
				WINDOW_IS_CONSTRUCTED = false;
			}
			
			// ----------------------------------------
			// Create Window
			// ----------------------------------------
			
			WINDOW_RESIZING_BY_SYSTEM = true;	// To ignore resize event set by system
			FIRST_GENERATED_CODE = true;		// To ignore user-save code when generating first code
			
			OBJ_UI_CLASS_WINDOW = new UI_CLASS_WINDOW(UI_WINDOW_CAPTION[i], wxPoint(UI_WINDOW_X[i], UI_WINDOW_Y[i]), wxSize(UI_WINDOW_W[i], UI_WINDOW_H[i]));
			
			OBJ_WINDOW_BASE_PANEL = new DrawPanel((wxDialog*) OBJ_UI_CLASS_WINDOW);
			
			#ifdef _WIN32
				OBJ_UI_CLASS_WINDOW->SetLayoutDirection(wxLayout_RightToLeft);
			#elif  __APPLE__
				//OBJ_UI_CLASS_WINDOW->SetLayoutDirection(wxLayout_RightToLeft);
			#else
				OBJ_UI_CLASS_WINDOW->SetLayoutDirection(wxLayout_RightToLeft);
			#endif

			WINDOW_IS_CONSTRUCTED = true;
			
			// ----------------------------------------
			// Create Controls
			// ----------------------------------------
			
			for (unsigned int CONTROL_ID = 1; CONTROL_ID <= UI_CONTROL_TOTAL; CONTROL_ID++)
			{
				if (UI_CONTROL_WINDOW[CONTROL_ID] == ID_UI_WINDOW_NAME_CURRENT)
				{
					if (UI_CONTROL_TYPE[CONTROL_ID] == "BUTTON")
					{
						// Create Button
						
						OBJ_UI_BUTTON_ARRAY[CONTROL_ID] = new Movable<wxButton>(CONTROL_ID, UI_CONTROL_CAPTION[CONTROL_ID], wxPoint(UI_CONTROL_X[CONTROL_ID], UI_CONTROL_Y[CONTROL_ID]), wxSize(UI_CONTROL_W[CONTROL_ID], UI_CONTROL_H[CONTROL_ID]), 
						0);
						
						// Set to current selection
						
						// Set settings list
					}
					else if (UI_CONTROL_TYPE[CONTROL_ID] == "TXT")
					{
						// Create TXT
						
						OBJ_UI_TXT_ARRAY[CONTROL_ID] = new Movable<wxTextCtrl>(CONTROL_ID, UI_CONTROL_CAPTION[CONTROL_ID], wxPoint(UI_CONTROL_X[CONTROL_ID], UI_CONTROL_Y[CONTROL_ID]), wxSize(UI_CONTROL_W[CONTROL_ID], UI_CONTROL_H[CONTROL_ID]),
						wxTE_RIGHT); // wxTE_MULTILINE | 
						
						// Set to current selection
						
						// Set settings list
					}
					else if (UI_CONTROL_TYPE[CONTROL_ID] == "LABEL")
					{
						// Create LABEL
						
						OBJ_UI_LABEL_ARRAY[CONTROL_ID] = new Movable<wxStaticText>(CONTROL_ID, UI_CONTROL_CAPTION[CONTROL_ID], wxPoint(UI_CONTROL_X[CONTROL_ID], UI_CONTROL_Y[CONTROL_ID]), wxSize(UI_CONTROL_W[CONTROL_ID], UI_CONTROL_H[CONTROL_ID]),
						wxST_NO_AUTORESIZE | wxALIGN_RIGHT | wxST_ELLIPSIZE_END);
						
						// Set to current selection
						
						// Set settings list
					}
				}
			}
			
			// ----------------------------------------
			// Show Window
			// ----------------------------------------
			
			OBJ_UI_CLASS_WINDOW->Show(true);
			OBJ_UI_CLASS_WINDOW->SetFocus();
			OBJ_UI_CLASS_WINDOW->Refresh();
			OBJ_UI_CLASS_WINDOW->Update();
			
			WINDOW_RESIZING_BY_SYSTEM = false; // To allow resize event by user
		}
	}
}

// -------------------------------

void CLASS_WINDOW_MAIN::TREE_FILES_DCLICK(wxTreeEvent& event)
{
	event.Skip();
	
	wxTreeItemId TREE_SELECTED_ID;
	
	TREE_SELECTED_ID = event.GetItem();
	
	if (TREE_SELECTED_ID == OBJ_TREE_FILES_LIST->GetRootItem())
		return;
	
	#ifdef ALIF_STUDIO_NO_STC
		if (CODE_MODIFIED)
	#else
		if (OBJ_CODE->IsModified())
	#endif
	{
		int answer = wxMessageBox(wxT("لقد غيرت محتوى الملف، هل تود حفظ الملف ؟"), wxT("خروج"), wxYES_NO | wxCANCEL, OBJ_CLASS_WINDOW_MAIN);
		if (answer == wxYES)
		{
			// Save
			if (!SAVE_FILE_UTF8(OBJ_CODE->GetValue(), PATH_FULL_SOURCE, true))
			{
				wxMessageBox(wxT("لم تنجح عملية تسجيل الملف \nحاول مع ملف باسم فيه حروف إنجليزية فقط"), wxT("خطأ"), wxICON_WARNING);
				return;
			}
		}
		else if (answer == wxCANCEL)
		{
			return;
		}
	}
	
	OBJ_TREE_FILES_LIST->SetItemBold(TREE_ID_FILES_LAST_BOLD, false);
	
	for (unsigned int i = 1; i <= TREE_FILES_TOTAL_ITEM; i++) // first file ID in list is 1
	{
		if (TREE_SELECTED_ID.GetID() == TREE_ID_FILES_ITEM[i])
		{
			if (!wxFileName::FileExists(TREE_FILES_PATH[i]))
			{
				wxMessageBox(wxT("عذرا، الملف لم يعد موجودا \n\n") + TREE_FILES_PATH[i], wxT("خطأ"), wxICON_WARNING);
				return;
			}

			OBJ_TREE_FILES_LIST->SetItemBold(TREE_SELECTED_ID, true);
			TREE_ID_FILES_LAST_BOLD = TREE_SELECTED_ID;
			
			SET_NEW_FILE(TREE_FILES_PATH[i]);

			CURRENT_FILE_EXTENSION = TREE_FILES_TYPE[i];

			UI_MANAGER(i);

			break;
		}
	}
}

// -------------------------------

void CLASS_WINDOW_MAIN::CODE_LTR(wxCommandEvent &event)
{
	event.Skip();
	OBJ_CODE->SetLayoutDirection (wxLayout_LeftToRight);
}

// -------------------------------

void CLASS_WINDOW_MAIN::CODE_RTL(wxCommandEvent &event)
{
	event.Skip();
	OBJ_CODE->SetLayoutDirection (wxLayout_RightToLeft);
}

// -------------------------------
/*
void CLASS_WINDOW_MAIN::CODE_DROP(wxDropFilesEvent& event)
{
	wxMessageBox("DROP");
}
*/
// -------------------------------
/*
void CLASS_WINDOW_MAIN::CODE_CHANGE(wxStyledTextEvent& event)
{
	event.Skip(true);

	if (PATH_FULL_SOURCE.IsEmpty())
		return;
	
	OBJ_LOG->AppendText("\nKey... " + CONVERT_INT_TO_STRING(event.GetKey()) );
	//CODE_MODIFIED = true;
}
*/
// -------------------------------

#ifndef ALIF_STUDIO_NO_STC

	bool IS_INSIDE_WORD = true;

	void CLASS_WINDOW_MAIN::CODE_AUTOCOMP_COMPLETED(wxStyledTextEvent& event)
	{
		event.Skip();

		#ifdef ALIF_STUDIO_NO_STC
			OBJ_CODE->AppendText(" ");
		#else
			OBJ_CODE->AddText(" ");
		#endif
		
		IS_INSIDE_WORD = false; // Allow Auto Complete
	}

#endif

// -------------------------------

#ifdef ALIF_STUDIO_NO_STC

	void CLASS_WINDOW_MAIN::CODE_KEYDOWN(wxKeyEvent& event) // wxKeyEventHandler
	{
		event.Skip();

		// macOS

		if (!PATH_FULL_SOURCE.IsEmpty())
			CODE_MODIFIED = true;
		
		// Pasring UI Code source
		if (CURRENT_FILE_EXTENSION == "ALIFUI")
			// TODO: 'Del' and 'Space' events dont run when editing UI code source!
			UI_PARSE_SOURCE();
	}

#endif

// -------------------------------

#ifndef ALIF_STUDIO_NO_STC

	//bool LAST_IS_OPERATOR = false;

	void CLASS_WINDOW_MAIN::CODE_CHARADDED(wxStyledTextEvent& event)
	{
		event.Skip();

		// Pasring UI Code source
		if (CURRENT_FILE_EXTENSION == "ALIFUI")
		{
			// TODO: 'Del' and 'Space' events dont run when editing UI code source!
			
			UI_PARSE_SOURCE();
			return;
		}

		// Clear Error Indicators

		if (CODE_ERROR_INDICATOR)
		{
			unsigned int TOTAL_LINES = (OBJ_CODE->GetLineCount()) - 1; // First line is 0

			int endPsn = 0;
			int startPsn = 0;
			int len = 0;

			for (unsigned int LINE_NUMBER = 0; LINE_NUMBER <= TOTAL_LINES; LINE_NUMBER++)
			{
				endPsn = OBJ_CODE->GetLineEndPosition(LINE_NUMBER);
				startPsn = OBJ_CODE->PositionFromLine(LINE_NUMBER);
				len = endPsn - startPsn;

				OBJ_CODE->IndicatorClearRange(startPsn, len);
			}

			OBJ_CODE->MarkerDeleteAll(1);

			CODE_ERROR_INDICATOR = false;
		}

		// AutoComplet List

		// ----------------------------------------
		// Universal_Character_Set_characters
		// ----------------------------------------
		// Left-to-right embedding (U+202A)
		// Right-to-left embedding (U+202B)
		// Pop directional formatting (U+202C)
		// Left-to-right override (U+202D)
		// Right-to-left override (U+202E)
		// Left-to-right isolate (U+2066)
		// Right-to-left isolate (U+2067)
		// First strong isolate (U+2068)
		// Pop directional isolate (U+2069)
		// ----------------------------------------

		// ----------------------------------------
		// Auto Complete
		// ----------------------------------------

		// TODO: Need Works.. not always shown !

		// Fix Show-Hide liste in the same seconde
		// by igiore show in some case.

		OBJ_CODE->AutoCompCancel(); // Clear AutoComList

		int CODE_CHAR_ASCII = event.GetKey();
		
		if (CODE_CHAR_ASCII == 32  ||	// Espace
			CODE_CHAR_ASCII == 58  ||	// :
			CODE_CHAR_ASCII == 127 ||	// Del
			CODE_CHAR_ASCII == 13)		// Enter
		{
			//if (CODE_CHAR_ASCII == 58) // :
				//LAST_IS_OPERATOR = true;
			//else
				//LAST_IS_OPERATOR = false;

			IS_INSIDE_WORD = false; // Allow Auto Complete
			return;
		}

		if (OBJ_CODE->GetLineLength(OBJ_CODE->GetCurrentLine()) < 2) // Empty Line
			IS_INSIDE_WORD = false;

		if (IS_INSIDE_WORD)
			return;
		
		if (CODE_CHAR_ASCII < 128) // ASCII
		{
			// Yes, ASCII, But we want allow some exception !
			//if (CODE_CHAR_ASCII != 35) // #, AutoComp in this version, dont allow use of '#...' key words
				return;
		}

		// pointer.. obj..
		//wxStyledTextCtrl* stc = (wxStyledTextCtrl*)event.GetEventObject();

		// Find the word start
		int currentPos = OBJ_CODE->GetCurrentPos();
		// Find the current pos
		int wordStartPos = OBJ_CODE->WordStartPosition(currentPos, true);
		
		// hum..
		//OBJ_CODE->AutoCompSetAutoHide(true);
		//OBJ_CODE->AutoCompSetCancelAtStart(false);
		
		// Display the autocompletion list
		int lenEntered = currentPos - wordStartPos;

		if (lenEntered > 0)
		{
			// ع 1593
			// د 1583
			// ن 1606
			// ص 1589
			// ح 1581
			// 1573 إ
			// 1575 ا
			// 1571 أ
			// 1589 ص
			// 1603 ك
			// 1605 م
			// 1585 ر
			// 1582 خ
			// 1608 و
			// 1607 ه
			// 1576 ب
			// 1587 س
			// 1586 ز
			// 1578 ت
			// 1579 ث
			// 1591 ط

			OBJ_CODE->AutoCompSetAutoHide(true);
			OBJ_CODE->AutoCompSetCancelAtStart(true); // Clear AutoList if Backspace pressed

			if (CODE_CHAR_ASCII == 1593) // ع
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("عدد"));
			}
			else if (CODE_CHAR_ASCII == 1583) // د
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("دالة"));
			}
			else if (CODE_CHAR_ASCII == 1606) // ن
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("نهاية نافذة نقر نص"));
			}
			else if (CODE_CHAR_ASCII == 1605) // م
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("مكتبة منطق متغير ملصق"));
			}
			else if (CODE_CHAR_ASCII == 1608) // و
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("وإلا و واجهة"));
			}
			else if (CODE_CHAR_ASCII == 1585) // ر
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("رئيسية"));
			}
			else if (CODE_CHAR_ASCII == 1589) // ص
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("صنف صحيح"));
			}
			else if (CODE_CHAR_ASCII == 1582) // خ
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("خاص خطأ"));
			}
			else if (CODE_CHAR_ASCII == 1581) // ح
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("نص"));
			}
			else if (CODE_CHAR_ASCII == 1603) // ك
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("كلما كائن"));
			}
			else if (CODE_CHAR_ASCII == 1607) // ه
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("هدم"));
			}
			else if (CODE_CHAR_ASCII == 1576) // ب
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("بناء"));
			}
			else if (CODE_CHAR_ASCII == 1587) // س
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("سطر"));
			}
			else if (CODE_CHAR_ASCII == 1586) // ز
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("زر"));
			}
			else if (CODE_CHAR_ASCII == 1578 || // ت
					 CODE_CHAR_ASCII == 1579)   // ث
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("ثابت"));
			}
			else if (CODE_CHAR_ASCII == 1591) // ط
			{
				OBJ_CODE->AutoCompShow(lenEntered, wxT("طرفية"));
			}
			else if (CODE_CHAR_ASCII == 1573 || // إ
					 CODE_CHAR_ASCII == 1571 || // أ
					 CODE_CHAR_ASCII == 1575) 	// ا
			{
				//const wxString& S = wxT("إ");
				//OBJ_CODE->AutoCompSelect(S); // CRASH !
				
				OBJ_CODE->AutoCompSetAutoHide(false);
				OBJ_CODE->AutoCompShow(lenEntered, wxT("إذا أو ألف إرجاع أضف أداة"));
			}
			else
			 OBJ_CODE->AutoCompCancel();

			//if (LAST_IS_OPERATOR) // :
				//OBJ_CODE->AutoCompShow(lenEntered, wxT("إظهار  إخفاء تدمير عنوان تجميد"));
			//else
				//OBJ_CODE->AutoCompShow(lenEntered, wxT("رئيسية  كلما نافذة   نص  منطق صحيح خطأ إذا و أو وإلا سطر رجوع صنف كائن خاص"));
		}

		IS_INSIDE_WORD = true;
		//LAST_IS_OPERATOR = false;
	}

#endif

// -------------------------------

void CLASS_WINDOW_MAIN::OnSave(wxCommandEvent &event)
{
	event.Skip();
	
    //OBJ_CODE->SaveFile(PATH_FULL_SOURCE); // Not working for UTF8 inwx 3.1 Beta
	
	if (!PATH_FULL_SOURCE.IsEmpty())
	{
		if (SAVE_FILE_UTF8(OBJ_CODE->GetValue(), PATH_FULL_SOURCE, true))
		{
			#ifdef ALIF_STUDIO_NO_STC
				CODE_MODIFIED = false;
			#else
				OBJ_CODE->SetSavePoint();
			#endif
		}
		else
		{
			wxMessageBox(wxT("لم تنجح عملية تسجيل الملف \nحاول مع ملف باسم فيه حروف إنجليزية فقط"), wxT("خطأ"), wxICON_WARNING);
			return;
		}
	}
}

// -------------------------------

void CLASS_WINDOW_MAIN::OnSaveAs(wxCommandEvent &event)
{
	event.Skip();

	int response;
	wxString Path;

	if (CURRENT_FILE_EXTENSION == "ALIFUI")
	{
		wxFileDialog *saveDialog = new wxFileDialog(this, wxT("حفظ الملف"), PATH_DIRECTORY, SOURCE_FILE_NAME + ".alifui", 
		wxT("واجهة  ( * . alifui )|*.alifui|ألف  ( * . alif )|*.alif|مكتبة  ( * . aliflib )|*.aliflib"), wxFD_SAVE);

		response = saveDialog->ShowModal();
		Path = saveDialog->GetPath();
	}
	else if (CURRENT_FILE_EXTENSION == "ALIFLIB")
	{
		wxFileDialog *saveDialog = new wxFileDialog(this, wxT("حفظ الملف"), PATH_DIRECTORY, SOURCE_FILE_NAME + ".aliflib", 
		wxT("مكتبة  ( * . aliflib )|*.aliflib|واجهة  ( * . alifui )|*.alifui|ألف  ( * . alif )|*.alif"), wxFD_SAVE);

		response = saveDialog->ShowModal();
		Path = saveDialog->GetPath();
	}
	else
	{
		wxFileDialog *saveDialog = new wxFileDialog(this, wxT("حفظ الملف"), PATH_DIRECTORY, SOURCE_FILE_NAME + ".alif", 
		wxT("ألف  ( * . alif )|*.alif|واجهة  ( * . alifui )|*.alifui|مكتبة  ( * . aliflib )|*.aliflib"), wxFD_SAVE);

		response = saveDialog->ShowModal();
		Path = saveDialog->GetPath();
	}
	
    if (response == wxID_OK)
	{
		if (SAVE_FILE_UTF8(OBJ_CODE->GetValue(), Path, false))
		{
			#ifdef ALIF_STUDIO_NO_STC
				CODE_MODIFIED = false;
			#else
				OBJ_CODE->SetSavePoint();
			#endif

			wxFileName fn ( Path );
			if (fn.GetExt() == "alif")
				CURRENT_FILE_EXTENSION = "ALIF";
			else if (fn.GetExt() == "alifui")
				CURRENT_FILE_EXTENSION = "ALIFUI";
			else if (fn.GetExt() == "aliflib")
				CURRENT_FILE_EXTENSION = "ALIFLIB";
			else
			{
				wxMessageBox("امتداد الملف غير معروف - " + fn.GetExt(), wxT("خطأ"), wxICON_WARNING);
				return;
			}
				
			PATH_DIRECTORY = fn.GetPathWithSep();

			SET_NEW_FILE(Path);
			
			SET_TREE_FILES_LIST();

			wxTreeItemId TREE_TO_SELECT_ID;

			for (unsigned int i = 1; i <= TREE_FILES_TOTAL_ITEM; i++) // first file ID in list is 1
			{
				if (Path == TREE_FILES_PATH[i])
				{
					TREE_TO_SELECT_ID = TREE_ID_FILES_ITEM[i];

					OBJ_TREE_FILES_LIST->SetItemBold(TREE_TO_SELECT_ID, true);
					TREE_ID_FILES_LAST_BOLD = TREE_TO_SELECT_ID;

					UI_MANAGER(i);

					break;
				}
			}
		}
		else if (!wxFileName::FileExists(Path))
		{
			wxMessageBox(wxT("لم تنجح عملية تسجيل الملف \nحاول مع ملف باسم فيه حروف إنجليزية فقط"), wxT("خطأ"), wxICON_WARNING);
			return;
		}
    }
}

// -------------------------------

void CLASS_WINDOW_MAIN::EDIT_UNDO(wxCommandEvent &event)
{
	event.Skip();

	OBJ_CODE->Undo();
}

// -------------------------------

void CLASS_WINDOW_MAIN::EDIT_REDO(wxCommandEvent &event)
{
	event.Skip();

	OBJ_CODE->Redo();
}

// -------------------------------

void CLASS_WINDOW_MAIN::EDIT_CUT(wxCommandEvent &event)
{
	event.Skip();

	OBJ_CODE->Cut();
}

// -------------------------------

void CLASS_WINDOW_MAIN::EDIT_COPY(wxCommandEvent &event)
{
	event.Skip();

	OBJ_CODE->Copy();
}

// -------------------------------

void CLASS_WINDOW_MAIN::EDIT_PASTE(wxCommandEvent &event)
{
	event.Skip();

	OBJ_CODE->Paste();
}

// -------------------------------

void CLASS_WINDOW_MAIN::EDIT_DELETE(wxCommandEvent &event)
{
	event.Skip();

	#ifdef ALIF_STUDIO_NO_STC
		// TODO: There no remove seletion, so get x,y of current selection, and remove it !
	#else
		OBJ_CODE->Clear(); // Remove selections
	#endif
}

// -------------------------------

void CLASS_WINDOW_MAIN::FIND(wxCommandEvent &event)
{
	event.Skip();

	#ifdef ALIF_STUDIO_NO_STC
		wxMessageBox(wxT("بحث - هده الخاصية مازالت تحت التطوير، المرجو تحديث البرنامج عبر الانترنت  من حين لآخر\n\nللمزيد من المعلومات يرجى زيارة الموقع الرسمي : \n\nw w w . a l i f l a n g . o r g"));
		return;
	#endif

	wxString TXT_TO_SEARCH;

	TXT_TO_SEARCH = wxGetTextFromUser(wxT("المرجو إدخال الجملة المراد البحث عنها"), 
										wxT("بحث"), 
										FIND_WORD, 
										OBJ_CLASS_WINDOW_MAIN, 
										wxDefaultCoord, 
										wxDefaultCoord, 
										true);

	if (TXT_TO_SEARCH.IsEmpty())
		return;

	FIND_WORD = TXT_TO_SEARCH;

	FIND_LEN = TXT_TO_SEARCH.Len(); // If i found how to select text, so P to LEN

	#ifdef ALIF_STUDIO_NO_STC
		int P = -1; // what about TextStrl ? (for Mac OS)
	#else
		int P = OBJ_CODE->FindText(0, OBJ_CODE->GetLastPosition(), TXT_TO_SEARCH); // TODO: FindText() is Win32 API !!! so what about Linux and Mac ?
	#endif

	if (P > -1)
	{
		IS_FOUND = true;
		FIND_LAST_POS = P;

		#ifdef ALIF_STUDIO_NO_STC
			// No GotoPos for TextCtrl.. so move to RichTextCtrl ??
			OBJ_CODE->SetInsertionPoint(P);
		#else
			OBJ_CODE->GotoPos(P);
			OBJ_CODE->SetSelectionStart(P);
			OBJ_CODE->SetSelectionEnd(P + FIND_LEN);
		#endif
	}
	else
	{
		IS_FOUND = false;
		wxMessageBox(wxT("انتهت عملية البحث !"), wxT("بحث"), wxICON_EXCLAMATION);
	}
	
}

// -------------------------------

void CLASS_WINDOW_MAIN::FIND_NEXT(wxCommandEvent &event)
{
	event.Skip();

	if (IS_FOUND)
	{
		#ifdef ALIF_STUDIO_NO_STC
			int P = -1; // what about TextStrl ? (for Mac OS)
		#else
			int P = OBJ_CODE->FindText(FIND_LAST_POS + 1, OBJ_CODE->GetLastPosition(), FIND_WORD);
		#endif

		if (P > -1)
		{
			IS_FOUND = true;
			FIND_LAST_POS = P;

			#ifdef ALIF_STUDIO_NO_STC
				// No GotoPos for TextCtrl.. so move to RichTextCtrl ??
				OBJ_CODE->SetInsertionPoint(P);
			#else
				OBJ_CODE->GotoPos(P);
				OBJ_CODE->SetSelectionStart(P);
				OBJ_CODE->SetSelectionEnd(P + FIND_LEN);
			#endif
		}
		else
		{
			//IS_FOUND = false;
			FIND_LAST_POS = 0;

			wxMessageBox(wxT("انتهت عملية البحث !"), wxT("بحث"), wxICON_EXCLAMATION);
		}
	}
}

// -------------------------------

void CLASS_WINDOW_MAIN::REPLACE(wxCommandEvent &event)
{
	event.Skip();

	wxMessageBox(wxT("استبدل - هده الخاصية مازالت تحت التطوير، المرجو تحديث البرنامج عبر الانترنت  من حين لآخر\n\nللمزيد من المعلومات يرجى زيارة الموقع الرسمي : \n\nw w w . a l i f l a n g . o r g"));
}

// -------------------------------

void CLASS_WINDOW_MAIN::REPLACE_ALL(wxCommandEvent &event)
{
	event.Skip();

	wxMessageBox(wxT("استبدل الكل - هده الخاصية مازالت تحت التطوير، المرجو تحديث البرنامج عبر الانترنت  من حين لآخر\n\nللمزيد من المعلومات يرجى زيارة الموقع الرسمي : \n\nw w w . a l i f l a n g . o r g"));
}

// ---------------------
// Change Binary Path
// ---------------------

void CLASS_WINDOW_MAIN::SET_BINARY_PATH()
{
	#ifdef _WIN32
	
		wxFileDialog *saveDialog = new wxFileDialog(this, wxT(" ملف تنفيدي "), PATH_DIRECTORY, SOURCE_FILE_NAME, 
			wxT("ملف تنفيدي  ( * . EXE )|*.exe"), wxFD_SAVE);
		
		if (saveDialog->ShowModal() == wxID_OK)
		{
			PATH_FULL_EXECUTABLE = saveDialog->GetPath();
		}

	#elif __APPLE__

		wxFileDialog *saveDialog = new wxFileDialog(this, wxT(" ملف تنفيدي "), PATH_DIRECTORY, SOURCE_FILE_NAME, 
			wxT("ملف تنفيدي  ( * . APP )|*.app"), wxFD_SAVE);
		
		if (saveDialog->ShowModal() == wxID_OK)
		{
			PATH_FULL_EXECUTABLE = saveDialog->GetPath();
		}
		
	#else

		wxFileDialog *saveDialog = new wxFileDialog(this, wxT(" ملف تنفيدي "), PATH_DIRECTORY, SOURCE_FILE_NAME, 
			wxT(" ملف تنفيدي - Linux Binary "), wxFD_SAVE);
		
		if (saveDialog->ShowModal() == wxID_OK)
		{
			PATH_FULL_EXECUTABLE = saveDialog->GetPath();
		}

	#endif
}

// ---------------------
// Compile
// ---------------------

void CLASS_WINDOW_MAIN::COMPILE(wxCommandEvent &event)
{
	event.Skip();

	CLASS_WINDOW_MAIN::COMPILE_NOW();
}

bool CLASS_WINDOW_MAIN::COMPILE_NOW()
{
	// Check if GCC Path containe space
	// because GCC, Windres don't work correctly
	// if path containes blank spaces.
	if (PATH_CONTAIN_SPACE)
	{
		wxString Fixed_Path;

		#ifdef _WIN32
			Fixed_Path = "C:\\Alif\\";
		#elif  __APPLE__
			Fixed_Path = "/Applications/Alif Studio.app";
		#else
			Fixed_Path = "/usr/local/bin/";
		#endif

		wxMessageBox(wxT("انتباه ! \n"
						"لقد قمتم بتتبيت ألف ستوديو في مسار \nفيه مجلدات باسماء تحتوي على مسافات \n\n")
						+ INSTALL_PATH + 
						wxT("\n\nبرنامج مترجم ألف لا يستطيع أن يعمل \nداخل مجلد باسم يحتوي على مسافة فراغ\n"
						"المرجو إزالة ألف ستوديو، ثم إعادة تتبيته \nفي مجلد أخر، على سبيل المثال \n\n") + Fixed_Path, 
						wxT("انتباه !"), wxICON_WARNING);

		return false;
	}
	
	// Check if compiler still existe...
	if (!wxFileName::FileExists(PATH_FULL_ALIF_COMPILER))
	{
		wxMessageBox(wxT(" مترجم ألف غير موجود \n \n ") + 
					PATH_FULL_ALIF_COMPILER + 
					wxT(" \n \n المرجو اعادت ثتبيت البرنامج \n او تحميله من الموقع الرسمي \n \n www.aliflang.org"));
		
		return false;
	}

	// Check if there a code..
	if (OBJ_CODE->IsEmpty())
	{
		wxMessageBox(wxT("لاتوجد شيفرة للترجمة !"), wxT("ترجمة"), wxICON_EXCLAMATION);
		return false;
	}

	// Check if user already chose binary path
	if(PATH_FULL_EXECUTABLE.IsEmpty())
	{
		CLASS_WINDOW_MAIN::SET_BINARY_PATH();

		if(PATH_FULL_EXECUTABLE == "")
			return false;
	}
	
	long PID;
	wxArrayString OUTPUT_ARRAY;
	wxArrayString ERROR_ARRAY;
    wxString COMMAND;
	
	OBJ_LOG->Clear();
	
	// --------------------------------------
	// Wait Window
	// --------------------------------------
	
	#ifdef _WIN32

		//wxWindowDisabler disableAll; // not working on Windows.

		// wxWidgets 3.1.0 - Windows
		/*
		wxBusyInfo info
		(
			wxBusyInfoFlags()
            .Parent(this)
            .Icon(wxArtProvider::GetIcon(wxART_EXECUTABLE_FILE, wxART_OTHER, wxSize(64, 64)))
            .Title(wxT("جاري الترجمة، المرجو الانتظار.."))
            .Text(wxT("Alif Compiler 1.0 - Window"))
            .Foreground(*wxWHITE)
            .Background(*wxBLACK)
            .Transparency(4*wxALPHA_OPAQUE/5)
		);
		*/
		
		wxBusyInfo wait(wxT(" ..جاري الترجمة، المرجو الانتظار "));
		
	#elif  __APPLE__
	
		wxWindowDisabler disableAll;
		wxBusyInfo wait(wxT(" ..جاري الترجمة، المرجو الانتظار "));
		
	#else
		
		wxWindowDisabler disableAll;
		wxBusyInfo wait(wxT(" ..جاري الترجمة، المرجو الانتظار "));
		
	#endif
	
	// --------------------------------------
	// Save File
	// --------------------------------------
	
	if (SAVE_FILE_UTF8( OBJ_CODE->GetValue(), PATH_FULL_SOURCE, true))
	{
		#ifdef ALIF_STUDIO_NO_STC
			CODE_MODIFIED = false;
		#else
			OBJ_CODE->SetSavePoint();
		#endif
	}
	else
	{
		wxMessageBox(wxT("لم تنجح عملية تسجيل الملف \nحاول مع ملف باسم فيه حروف إنجليزية فقط"), wxT("خطأ"), wxICON_WARNING);
		return false;
	}

	// --------------------------------------
	// New Log file
	// --------------------------------------

	wxString RAND = CONVERT_INT_TO_STRING(GET_ID());

	// Tmp Log
	#ifdef _WIN32
		PATH_FULL_LOG = PATH_TEMP + "\\alif_" + RAND + ".log";
	#else
		PATH_FULL_LOG = PATH_TEMP + "/alif_" + RAND + ".log";
	#endif

	// --------------------------------------
	// Working Path Argument
	// --------------------------------------

	// Add Working Path as Argument
	wxFileName FileName_Source(PATH_FULL_SOURCE); // Get result by FileName_Source.GetPath();
	
	// --------------------------------------
	// Command
	// --------------------------------------

	#ifdef _WIN32

		// --------------------------------------
		// Create Bath file
		// --------------------------------------
		
		wxString BATH_PATH = PATH_TEMP + "\\alifstudio_" + RAND + "_build.bat";

		wxString BATH_SCRIPT =  "@echo off\n"
							 	"cls\n"
							 	"if exist \"" + PATH_FULL_EXECUTABLE + "\" del \"" + PATH_FULL_EXECUTABLE + "\"\n" 
								"if exist \"" + PATH_FULL_LOG + "\" del \"" + PATH_FULL_LOG + "\"\n" 
							 	"\"" + PATH_FULL_ALIF_COMPILER + "\" \"" + PATH_FULL_SOURCE + "\" \"" + PATH_FULL_EXECUTABLE + "\" \"" + PATH_FULL_LOG + "\" -workpath=" + FileName_Source.GetPath() + "\n";
		
		if (!SAVE_FILE_UTF8(BATH_SCRIPT, BATH_PATH, true))
		{
			wxMessageBox(wxT("لم تنجح عملية انشاء الملف مؤقت"), wxT("خطأ"), wxICON_WARNING);
			return false;
		}

		if (!wxFileName::FileExists(BATH_PATH))
		{
			wxMessageBox(wxT("لم تنجح عملية البحث عن الملف المؤقت"), wxT("خطأ"), wxICON_WARNING);
			return false;
		}
	
		COMMAND = "\"";
		COMMAND.Append(BATH_PATH);
		COMMAND.Append("\"");
		
	#else
		// Remove Binary file
		COMMAND = "rm -f \"";
		COMMAND.Append(PATH_FULL_EXECUTABLE);
		COMMAND.Append("\"");
		wxExecute(COMMAND);
		
		// Remove Log file
		COMMAND = "rm -f \"";
		COMMAND.Append(PATH_FULL_LOG);
		COMMAND.Append("\"");
		wxExecute(COMMAND);
		
		COMMAND = "\"";
		COMMAND.Append(PATH_FULL_ALIF_COMPILER);
		COMMAND.Append("\" \"");
		COMMAND.Append(PATH_FULL_SOURCE);
		COMMAND.Append("\" \"");
		COMMAND.Append(PATH_FULL_EXECUTABLE);
		COMMAND.Append("\" \"");
		COMMAND.Append(PATH_FULL_LOG);
		COMMAND.Append("\" -workpath=" + FileName_Source.GetPath());
	#endif

	#ifndef ALIF_STUDIO_NO_STC

		// Clear Error Indicators
		if (CODE_ERROR_INDICATOR)
		{
			unsigned int TOTAL_LINES = (OBJ_CODE->GetLineCount()) - 1; // First line is 0

			int endPsn = 0;
			int startPsn = 0;
			int len = 0;

			for (unsigned int LINE_NUMBER = 0; LINE_NUMBER <= TOTAL_LINES; LINE_NUMBER++)
			{
				endPsn = OBJ_CODE->GetLineEndPosition(LINE_NUMBER);
				startPsn = OBJ_CODE->PositionFromLine(LINE_NUMBER);
				len = endPsn - startPsn;

				OBJ_CODE->IndicatorClearRange(startPsn, len);
			}

			OBJ_CODE->MarkerDeleteAll(1);

			CODE_ERROR_INDICATOR = false;
		}

	#endif

	// Calcul compiling time
	clock_t CLOCK_START = clock();

	// wxEXEC_ASYNC imidiat return
	// wxEXEC_SYNC wait prog to exit
    PID = wxExecute(COMMAND, OUTPUT_ARRAY, ERROR_ARRAY);

	// Time
	clock_t CLOCK_END = clock();
	double CLOCK_ELAPSED = double(CLOCK_END - CLOCK_START) / CLOCKS_PER_SEC;

	bool PID_ERROR = false;

    if ( PID == 0 || CURRENT_FILE_EXTENSION == "ALIFLIB" )
    {
		OBJ_LOG->Clear();

		if (wxFileName::FileExists(PATH_FULL_LOG))
		{
			// Read Log File
			OBJ_LOG->LoadFile(PATH_FULL_LOG);
		}

		if (OBJ_LOG->IsEmpty())
		{
			OBJ_LOG->SetForegroundColour(wxColour(121, 160, 88)); // Green

			OBJ_LOG->AppendText(wxT("تمت عملية الترجمة بنجاح \n\n"));
			OBJ_LOG->AppendText(wxT("ملف الشيفرة		: " + PATH_FULL_SOURCE + " \n"));
			OBJ_LOG->AppendText(wxT("الملف التنفيذي		: " + PATH_FULL_EXECUTABLE + " \n"));
			OBJ_LOG->AppendText(wxT("زمن الترجمة		: " + CONVERT_DOUBLE_TO_STRING(CLOCK_ELAPSED) + wxT(" ثانية ")));
		}
		else
		{
			OBJ_LOG->SetForegroundColour(wxColour(167, 65, 75)); // Red

			wxString LINE;
			wxString ERROR_FILE_PATH;
			
			LINE = OBJ_LOG->GetLineText(0);

			ERROR_FILE_PATH = LINE.Mid(10);
			
			int LINE_NUMBER = 0;

			LINE = OBJ_LOG->GetLineText(1);

			LINE_NUMBER = CONVERT_STRING_TO_INT(LINE.Mid(10)) - 1;

			if (LINE_NUMBER > 0 && LINE_NUMBER < 10000)
			{
				if (ERROR_FILE_PATH != PATH_FULL_SOURCE)
				{
					// Error in other file, so open it
					OPEN_NEW_FILE(ERROR_FILE_PATH);

					// Read Log File again becose its cleared 
					// by open new file process
					OBJ_LOG->LoadFile(PATH_FULL_LOG);
				}

				#ifdef ALIF_STUDIO_NO_STC

					// TODO: Goto line number (LINE_NUMBER), in wxTextCtrl

				#else

					int endPsn = 0;
					int startPsn = 0;
					int len = 0;

					endPsn = OBJ_CODE->GetLineEndPosition(LINE_NUMBER);
					startPsn = OBJ_CODE->PositionFromLine(LINE_NUMBER);
					len = endPsn - startPsn;

					OBJ_CODE->SetIndicatorCurrent(ID_CODE_ERROR_INDICATOR);
					OBJ_CODE->IndicatorFillRange(startPsn, len);

					OBJ_CODE->MarkerAdd(LINE_NUMBER, 1);
					OBJ_CODE->MarkerSetBackground(1, wxColour(210,140,148));
					OBJ_CODE->MarkerSetForeground(1, wxColour(197,107,116));

					OBJ_CODE->GotoLine(LINE_NUMBER);

					//CODE_ERROR_INDICATOR_START = startPsn;
					//CODE_ERROR_INDICATOR_LEN = len;
					//CODE_ERROR_INDICATOR_LINE_NUMBER = LINE_NUMBER;
					CODE_ERROR_INDICATOR = true; // To Let clear Error Indicators

				#endif
			}

			PID_ERROR = true;
		}
    }
	else
	{
		// Alif Compiler Exit Core Crash..
		// Read Consol output.

		OBJ_LOG->SetForegroundColour(wxColour(167, 65, 75)); // Red

		wxString OUTPUT_STRING;

		for (unsigned int i = 0; i < OUTPUT_ARRAY.GetCount(); ++i)
		{
			OUTPUT_STRING += wxT('\n');
			OUTPUT_STRING += OUTPUT_ARRAY[i];
		}
		
		OBJ_LOG->AppendText(OUTPUT_STRING);

		PID_ERROR = true;
	}
	
	//wxString cmd;
	//cmd = "notepad.exe";
	//MyPipedProcess *process = new MyPipedProcess(this, cmd);
	//m_pidLast = wxExecute(cmd, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER); //, &m_executeEnv
	
	/*
	#ifdef __WXMSW__
	wxArrayString output;
	wxArrayString errors;
	wxArrayString CMD;
	wxExecuteEnv ENV;
	
	//ENV = "D:\\@IGS\\App\\_Software\\Alif\\Win";
	
	//ENV = "\"D:\\@IGS\\App\\_Software\\Alif\\Win\"";
	CMD = "\"D:\\@IGS\\App\\_Software\\Alif\\Win\\alif.exe\" \"C:\\Users\\saif\\Desktop\\test.alif\" //\"C:\\Users\\saif\\Desktop\\testCMD.exe\" 2> D:\\LLL.log";
	
	//CMD = "D:\\@IGS\\App\\_Software\\Alif_Studio\\B.bat";
	//CMD = "D:\\@IGS\\App\\_Software\\Alif\\Win\\_alif\\tmp\\4288_lk.bat";
	
	int xxxx = wxExecute (CMD, output, errors);
	
	OBJ_CODE->AppendText(wxString(xxxx));
	
	//int code = wxExecute(cmd, wxEXEC_SYNC | GetExecFlags(), NULL, &env);
	//wxLogStatus(wxT("Process '%s' terminated with exit code %d."),cmd.c_str(), code);
	
	//wxExecute(wxString::Format("\"D:\\@IGS\\App\\_Software\\Alif\\Win\\alif.exe\" \"C:\\Users\\saif\\Desktop\\test.alif\" //\"C:\\Users\\saif\\Desktop\\test.exe\"", 
	//"\"C:\\Users\\saif\\Desktop\""));
	//fullPath.c_str()));
	#endif
	*/
	
	#ifdef _WIN32
		wxTheApp->Yield();
	#elif  __APPLE__
		wxTheApp->Yield();
	#else
		wxTheApp->Yield();
	#endif

	// Clean
	if (wxFileName::FileExists(PATH_FULL_LOG))
	{
		wxRemoveFile(PATH_FULL_LOG);
	}
	
	#ifdef _WIN32
		if (wxFileName::FileExists(BATH_PATH))
		{
			wxRemoveFile(BATH_PATH);
		}
	#endif

	if (PID_ERROR)
		return false;
	
	return true;
}

// ---------------------
// RUN
// ---------------------

void CLASS_WINDOW_MAIN::RUN(wxCommandEvent &event)
{
	event.Skip();

	if (PATH_FULL_EXECUTABLE.empty())
		wxMessageBox(wxT("يجب ترجمة الشيفرة أولا"), wxT("تنفيد"), wxICON_EXCLAMATION);

	#ifdef __APPLE__
		else if (wxDirExists(PATH_FULL_EXECUTABLE)) // MyBin.app
	#else
		else if (wxFileName::FileExists(PATH_FULL_EXECUTABLE)) // MyBin / MyBin.exe
	#endif
	{
		// Run Compiled App

		wxString COMMAND;
			
		#ifdef __APPLE__
			COMMAND = "open \"";
			COMMAND.Append(PATH_FULL_EXECUTABLE);
			COMMAND.Append("\"");
		#else
			COMMAND = "\"";
			COMMAND.Append(PATH_FULL_EXECUTABLE);
			COMMAND.Append("\"");
		#endif

		wxExecute(COMMAND, wxEXEC_ASYNC);
	}
	else
		wxMessageBox(wxT("الملف التنفيذي غير موجود \nحاول ترجمة الشيفرة أولا"), wxT("تنفيد"), wxICON_EXCLAMATION);
}

// ---------------------
// Compile / RUN
// ---------------------

void CLASS_WINDOW_MAIN::BUILD(wxCommandEvent &event)
{
	event.Skip();

	if (!CLASS_WINDOW_MAIN::COMPILE_NOW())
		return;

	#ifdef __APPLE__
		if (wxFileName::FileExists(PATH_FULL_EXECUTABLE + wxT("/Contents/MacOS/alif")))
	#else
		if (wxFileName::FileExists(PATH_FULL_EXECUTABLE))
	#endif
		CLASS_WINDOW_MAIN::RUN(event);
}

// -------------------------------

void CLASS_WINDOW_MAIN::PROP_IDE(wxCommandEvent &event)
{
	event.Skip();

	wxMessageBox(wxT("إعدادات بيئة التطوير - هده الخاصية مازالت تحت التطوير، المرجو تحديث البرنامج عبر الانترنت  من حين لآخر\n\nللمزيد من المعلومات يرجى زيارة الموقع الرسمي : \n\nw w w . a l i f l a n g . o r g"));
}

// -------------------------------

void CLASS_WINDOW_MAIN::PROP_FONT(wxCommandEvent &event)
{
	event.Skip();

	wxMessageBox(wxT("إعدادات خط النص - هده الخاصية مازالت تحت التطوير، المرجو تحديث البرنامج عبر الانترنت  من حين لآخر\n\nللمزيد من المعلومات يرجى زيارة الموقع الرسمي : \n\nw w w . a l i f l a n g . o r g"));

/*
	wxFont CODE_FONT (14,
		wxFONTFAMILY_SWISS, // wxFONTFAMILY_SWISS
		wxFONTSTYLE_NORMAL, // wxFONTSTYLE_ITALIC
		wxFONTWEIGHT_NORMAL, // wxFONTWEIGHT_NORMAL / wxFONTWEIGHT_BOLD
		false, // 
		"Courier New");
 		OBJ_CODE->StyleSetFont(wxSTC_STYLE_DEFAULT, CODE_FONT);
*/

/*
	wxFontData fontData;
	wxFont     theFont;
	wxColour   colour;
 
	//theFont = OBJ_CODE->GetFont();
	//fontData.SetInitialFont(theFont);

	//colour = OBJ_CODE->GetForegroundColour();
	//fontData.SetColour(colour);

	fontData.SetShowHelp(true);
 
	wxFontDialog *dialog = new wxFontDialog( this );

	if ( dialog->ShowModal() == wxID_OK )
	{
		fontData = dialog->GetFontData();

		theFont = fontData.GetChosenFont();

		OBJ_CODE->StyleSetFont(wxSTC_STYLE_DEFAULT, theFont);
		//theText->SetForegroundColour(fontData.GetColour());
	}
*/
}

// -------------------------------

void CLASS_WINDOW_MAIN::PROP_COMPILER(wxCommandEvent &event)
{
	event.Skip();

	wxMessageBox(wxT("إعدادات مترجم ألف - هده الخاصية مازالت تحت التطوير، المرجو تحديث البرنامج عبر الانترنت  من حين لآخر\n\nللمزيد من المعلومات يرجى زيارة الموقع الرسمي : \n\nw w w . a l i f l a n g . o r g"));
}

// -------------------------------

void CLASS_WINDOW_MAIN::UPDATE(wxCommandEvent &event)
{
	event.Skip();

	// This Func is called by Update Menu,
	// and Update Tool Bar.

	UPDATE_NOW(event);
}

// -------------------------------
/*
void CLASS_WINDOW_MAIN::UPDATE_BY_FILE(wxCommandEvent &event)
{
	event.Skip();

	wxMessageBox(wxT("تحديث عن طريق ملف - هده الخاصية مازالت تحت التطوير، المرجو تحديث البرنامج عبر الانترنت  من حين لآخر\n\nللمزيد من المعلومات يرجى زيارة الموقع الرسمي : \n\nw w w . a l i f l a n g . o r g"));
}
*/
// -------------------------------

void CLASS_WINDOW_MAIN::HELP_PDF(wxCommandEvent &event)
{
	event.Skip();

	//Alif_Arabic_Programming_Language.pdf

	if (wxFileName::FileExists(PATH_FULL_PDF))
	{
		// wxEXEC_ASYNC imidiat return
		// wxEXEC_SYNC wait prog to exit
		// wxExecute(PATH_FULL_PDF, wxEXEC_ASYNC);
		#ifdef _WIN32
			::ShellExecute(NULL, _T("open"), PATH_FULL_PDF, NULL, NULL, SW_SHOWNORMAL);
		#elif  __APPLE__
			wxExecute( wxT("open \"") + PATH_FULL_PDF + wxT("\""));
		#else
			wxExecute( wxT("xdg-open \"") + PATH_FULL_PDF + wxT("\""));
		#endif		
	}
	else
	{
		wxMessageBox(wxT(" الملف التالي غير موجود \n \n ") + 
					PATH_FULL_PDF + 
					wxT(" \n \n المرجو اعادت ثتبيت البرنامج \n او تحميله من الموقع الرسمي \n \n www.aliflang.org"));
	}
}

// -------------------------------
/*
void CLASS_WINDOW_MAIN::HELP_HTML(wxCommandEvent &event)
{
	event.Skip();

	wxMessageBox(wxT("كتاب - هده الخاصية مازالت تحت التطوير، المرجو تحديث البرنامج عبر الانترنت  من حين لآخر\n\nللمزيد من المعلومات يرجى زيارة الموقع الرسمي : \n\nw w w . a l i f l a n g . o r g"));
}
*/
// -------------------------------

void CLASS_WINDOW_MAIN::HELP_ONLINE(wxCommandEvent &event)
{
	event.Skip();

	wxLaunchDefaultBrowser("http://www.aliflang.org/learn");
}

// -------------------------------

#if  __APPLE__
	// Mac About

	void CLASS_WINDOW_MAIN::OnAbout(wxCommandEvent &event)
	{
		event.Skip();

		// This is special for Mac OS

		CLASS_WINDOW_MAIN::ABOUT_IDE(event);
	}

#endif

// -------------------------------

void CLASS_WINDOW_MAIN::ABOUT_IDE(wxCommandEvent &event)
{
	event.Skip();

	wxAboutDialogInfo info;
		info.SetVersion(wxT("ألف ستوديو ") + ALIF_STUDIO_VERSION + wxT(" نسخة تجريبية"));
		info.SetDescription(wxT("برنامج ألف ستوديو و برنامج مترجم ألف\nهي برامج مفتوحة المصدر تحت رخصة\nجنو العمومية الإصدار الثالث GPLv3\n\nمكتبات ألف القياسية هي مكتبات مفتوحة المصدر\nتحت رخصة جنو المكتبة العمومية LGPLv3"));
		info.SetWebSite(wxT("https://www.aliflang.org"));
		info.SetName(wxT("حول"));
	wxAboutBox(info);
}

// -------------------------------

void CLASS_WINDOW_MAIN::ABOUT_BUG(wxCommandEvent &event)
{
	event.Skip();

	wxLaunchDefaultBrowser("http://www.aliflang.org/bug");

	/*
	wxAboutDialogInfo info;
		info.SetVersion(wxT("مترجم ألف ") + ALIF_VERSION);
		info.SetDescription(wxT("برنامج مترجم ألف هو برنامج خاص بلغة البرمجة ألف، \nصمم و كتب من طرف المبرمج حسن دراكة سنة 2018، \nو هدا البرنامج يطور حاليا من طرف مجتمع مطوري لغة ألف، \nهدا البرنامج مجاني و لكم حرية التصرف فيه تحت شروط رخصة كنو النسخة الثالتة "));
		info.SetWebSite(wxT("https://www.aliflang.org"));
		info.SetName(wxT("حول"));
	wxAboutBox(info);
	*/
}

// -------------------------------

void CLASS_WINDOW_MAIN::OnOpen(wxCommandEvent &event)
{
	event.Skip();
	
	#ifdef ALIF_STUDIO_NO_STC
		if (CODE_MODIFIED)
	#else
		if (OBJ_CODE->IsModified())
	#endif
	{
		int answer = wxMessageBox(wxT("لقد غيرت محتوى الملف، هل تود حفظ الملف ؟"), wxT("خروج"), wxYES_NO | wxCANCEL, OBJ_CLASS_WINDOW_MAIN);
		if (answer == wxYES)
		{
			// Save
			if (!SAVE_FILE_UTF8(OBJ_CODE->GetValue(), PATH_FULL_SOURCE, true))
			{
				wxMessageBox(wxT("لم تنجح عملية تسجيل الملف \nحاول مع ملف باسم فيه حروف إنجليزية فقط"), wxT("خطأ"), wxICON_WARNING);
				return;
			}
		}
		else if (answer == wxCANCEL)
		{
			return;
		}
	}
	
    //wxFileDialog *openDialog = new wxFileDialog(this, wxT("فتح ملف جديد"), wxT(""), wxT(""),
	//wxT("ألف  ( * . alif )|*.alif|واجهة  ( * . alifui )|*.alifui|مكتبة  ( * . aliflib )|*.aliflib"), 
	//wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

	//wxDirDialog *openDialog(this, wxT(" فتح مجلد "), wxT(""), wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST | wxDD_CHANGE_DIR);

	wxDirDialog *openDialog = new wxDirDialog(this, wxT(" فتح مجلد "), wxT(""), 
	wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST | wxDD_CHANGE_DIR, wxDefaultPosition);
	
    //int response = openDialog->ShowDialog
	
	if (openDialog->ShowModal() == wxID_OK)
    {
        wxString DIALOG_PATH = openDialog->GetPath();

        if (!DIALOG_PATH.IsEmpty())
        {
			PATH_DIRECTORY = DIALOG_PATH;

			#ifdef _WIN32
				PATH_DIRECTORY.append(wxT("\\"));
			#else
				PATH_DIRECTORY.append(wxT("/"));
			#endif

			if (WINDOW_IS_CONSTRUCTED)
			{
				OBJ_UI_CLASS_WINDOW->Destroy();
				WINDOW_IS_CONSTRUCTED = false;
			}
			
			// Clear Window List
			if (TREE_WINDOW_TOTAL_ITEM > 0)
				OBJ_TREE_WINDOW->DeleteAllItems();
			
			// Initilizing
			UI_EMPTY_SETTINGS();

			// UI Initilizing
			UI_BUTTON_SAVE(false);
			UI_BUTTON_BUILD(false);
			#ifdef ALIF_STUDIO_NO_STC
				OBJ_CODE->Clear();
			#else
				OBJ_CODE->ClearAll();
			#endif
			OBJ_CODE->Enable(false);
			OBJ_CODE->SetEditable(false);
			OBJ_LOG->Clear();

			// Initilizing
			SET_NEW_FILE("");

			SET_TREE_FILES_LIST();
        }
    }
}

// ---------------------
// Update
// ---------------------

void CLASS_WINDOW_MAIN::UPDATE_NOW(wxCommandEvent &event)
{
	event.Skip();

	//OBJ_LOG->AppendText(wxT("\n ---------------- "));
	//OBJ_LOG->AppendText(wxT("\n PATH_DIRECTORY : ") + PATH_DIRECTORY + " \n");
	//OBJ_LOG->AppendText(wxT("\n PATH_FULL_SOURCE : ") + PATH_FULL_SOURCE + " \n");
	//OBJ_LOG->AppendText(wxT("\n SOURCE_FILE_NAME : ") + SOURCE_FILE_NAME + " \n");
	//OBJ_LOG->AppendText(wxT("\n ---------------- "));
	//return;

	if (!wxFileName::FileExists(PATH_FULL_UPDATE))
	{
		wxMessageBox(wxT(" الملف التالي غير موجود \n \n ") + 
					PATH_FULL_UPDATE + 
					wxT(" \n \n المرجو اعادت ثتبيت البرنامج \n او تحميله من الموقع الرسمي \n \n www.aliflang.org"));

		return;
	}

	#ifdef ALIF_STUDIO_NO_STC
		if (CODE_MODIFIED)
	#else
		if (OBJ_CODE->IsModified())
	#endif
	{
		int answer = wxMessageBox(wxT("لقد غيرت محتوى الملف، هل تود حفظ الملف ؟"), wxT("خروج"), wxYES_NO, OBJ_CLASS_WINDOW_MAIN);
		if (answer == wxYES)
		{
			CLASS_WINDOW_MAIN::OnSave(event);
		}
	}

	wxString COMMAND;

	#ifdef _WIN32

		// --------------------------------------
		// Create Bath file
		// --------------------------------------
		
		wxString RAND = CONVERT_INT_TO_STRING(GET_ID());
		
		wxString BATH_PATH = PATH_TEMP + "\\alifstudio_" + RAND + "_update.bat";

		wxString BATH_SCRIPT =  "@echo off\n"
							 	"cls\n"
								"echo. \n"
								"echo ------------------- \n"
								"echo Alif Studio Updater \n"
								"echo ------------------- \n"
								"echo. \n"
								"echo * Run as Administrator... \n"
							 	"\"" + PATH_FULL_UPDATE + "\" " +  CONVERT_INT_TO_STRING(wxGetProcessId());
		
		if (!SAVE_FILE_UTF8(BATH_SCRIPT, BATH_PATH, true))
		{
			wxMessageBox(wxT("لم تنجح عملية انشاء الملف مؤقت"), wxT("خطأ"), wxICON_WARNING);
			return;
		}

		if (!wxFileName::FileExists(BATH_PATH))
		{
			wxMessageBox(wxT("لم تنجح عملية البحث عن الملف المؤقت"), wxT("خطأ"), wxICON_WARNING);
			return;
		}
	
		COMMAND = "\"";
		COMMAND.Append(BATH_PATH);
		COMMAND.Append("\"");
		
		//COMMAND = "\"" + PATH_FULL_UPDATE + "\" " +  CONVERT_INT_TO_STRING(wxGetProcessId());
		
	#else

		COMMAND = "\"" + PATH_FULL_UPDATE + "\" " +  CONVERT_INT_TO_STRING(wxGetProcessId());

	#endif

	// wxEXEC_ASYNC imidiat return
	// wxEXEC_SYNC wait prog to exit

	wxExecute(COMMAND, wxEXEC_ASYNC);
}

// ---------------------
// New
// ---------------------

void CLASS_WINDOW_MAIN::OnNew(wxCommandEvent &event)
{
	event.Skip();

	#ifdef ALIF_STUDIO_NO_STC
		if (CODE_MODIFIED)
	#else
		if (OBJ_CODE->IsModified())
	#endif
	{
		int answer = wxMessageBox(wxT("لقد غيرت محتوى الملف، هل تود حفظ الملف ؟"), wxT("خروج"), wxYES_NO, OBJ_CLASS_WINDOW_MAIN);
		if (answer == wxYES)
		{
			CLASS_WINDOW_MAIN::OnSave(event);
		}
	}

	int response;
	wxString Path;

	wxFileDialog *saveDialog = new wxFileDialog(this, wxT("إنشاء ملف جديد"), PATH_DIRECTORY, wxT("New_Alif_File.alif"), // ملف_جديد
		wxT("ألف  ( * . alif )|*.alif|واجهة  ( * . alifui )|*.alifui|مكتبة  ( * . aliflib )|*.aliflib"), wxFD_SAVE);

	response = saveDialog->ShowModal();
	
	Path = saveDialog->GetPath();
	
    if (response == wxID_OK)
	{
		wxString SAMPLE_CODE;

		wxFileName fn ( Path );

		if (fn.GetExt() == "alif")
		{
			CURRENT_FILE_EXTENSION = "ALIF";

			OBJ_CODE->SetLayoutDirection (wxLayout_RightToLeft);

			/*
			SAMPLE_CODE = wxT(	"\n#ألف\n"
								" أداة نافذة (رئيسية, 0, 0, 400, 400, \"عنوان النافدة هنا\") \n\n"
								"	أداة زر (رئيسية, زر_1, 277, 321, 100, 32, \"زر جديد 1\") \n"
								"	أداة زر (رئيسية, زر_2, 14, 321, 100, 32, \"زر جديد 2\") \n"
								"	أداة نص (رئيسية, نص_1, 79, 149, 234, 30, \"نص جديد\") \n"
								"	أداة ملصق (رئيسية, ملصق_1, 84, 111, 224, 28, \"ملصق جديد\") ");

			string UI_SAMPLE_CODE_LINE;

			if (SAVE_FILE_UTF8(SAMPLE_CODE, (fn.GetPathWithSep() + fn.GetName() + ".alifui"), false))
			{
				UI_SAMPLE_CODE_LINE = "#واجهة \"" + fn.GetName() + "\" \n\n";
			}
			else
			{
				UI_SAMPLE_CODE_LINE = " ";
				wxMessageBox(wxT("لم تنجح عملية تسجيل الملف \nحاول مع ملف باسم فيه حروف إنجليزية فقط"), wxT("خطأ"), wxICON_WARNING);
			}
			*/

			SAMPLE_CODE = wxT(	"\n#ألف\n\n"
								//UI_SAMPLE_CODE_LINE +
								"نافذة رئيسية \n\n"
								"	دالة رئيسية \n\n"
								//"		رسالة ( \"السلام عليكم\" ) \n\n"
								//"	نهاية دالة \n\n"
								//"	دالة نقر زر_1 () \n\n"
								"		عدد ن = 6 + 4\n"
								"		نص م = \"مرحبا، ن = \" + ن\n\n"
								"		رسالة (م)\n\n"
								"	نهاية دالة \n\n"
								"نهاية نافذة \n");
		}
		else if (fn.GetExt() == "alifui")
		{
			CURRENT_FILE_EXTENSION = "ALIFUI";

			OBJ_CODE->SetLayoutDirection (wxLayout_RightToLeft);

			SAMPLE_CODE = wxT(	"\n#ألف\n"
								" أداة نافذة (رئيسية, 10, 25, 400, 400, \"عنوان النافدة هنا\") \n\n"
								"	أداة زر		(رئيسية, زر_1, 277, 321, 100, 32, \"زر جديد 1\") \n"
								"	أداة زر		(رئيسية, زر_2, 14, 321, 100, 32, \"زر جديد 2\") \n"
								"	أداة نص		(رئيسية, نص_1, 79, 149, 234, 30, \"نص جديد\") \n"
								"	أداة ملصق	(رئيسية, ملصق_1, 84, 111, 224, 28, \"ملصق جديد\") ");
		}
		else if (fn.GetExt() == "aliflib")
		{
			CURRENT_FILE_EXTENSION = "ALIFLIB";

			OBJ_CODE->SetLayoutDirection (wxLayout_LeftToRight);

			SAMPLE_CODE = wxT( R"( 
// --------------------------------------------
// -- wxWidgets شرح كيفية صنع مكتبة ألف جديدة بلغة سي++ و مكتبات
// -- للحصول على معلومات أكثر، أو نشر الشيفرة، المرجو زيارة موقع مجتمع ألف
// -- aliflang.org/community
// --------------------------------------------

#include <wx/wx.h>

int ID_FRAME_1 = 5000;

// --------------------------------------------
// -- الإعلان العام للدوال
// --------------------------------------------

void Alif_Library_Example_1 (wxString msg);
double Alif_Library_Example_2 (double foo, double bar);

// --------------------------------------------

class FRAME_1 : public wxFrame
{
    public:
        FRAME_1 ();
        virtual ~FRAME_1();
    private:
		DECLARE_NO_COPY_CLASS(FRAME_1)
        DECLARE_EVENT_TABLE()
};

FRAME_1 *OBJ_FRAME_1;

BEGIN_EVENT_TABLE(FRAME_1, wxFrame)

END_EVENT_TABLE()

FRAME_1 :: FRAME_1() : 
	wxFrame(NULL, ID_FRAME_1, 
	wxT("تجربة مكتبة !"),
	wxPoint(50, 50),
	wxSize(450, 450)) 
{
	// --------------------------------------------
	// -- نداء / استعمال الدوال
	// --------------------------------------------

	Alif_Library_Example_1( wxT("أهلا") );

	double test = Alif_Library_Example_2(6, 4);

	// --------------------------------------------
}

FRAME_1::~FRAME_1(){}

class MyApp : public wxApp
{
    public:
        MyApp();	
		~MyApp();	
		virtual bool OnInit();	
        virtual int OnExit();    
	private:
		DECLARE_NO_COPY_CLASS(MyApp)
};

DECLARE_APP(MyApp);

IMPLEMENT_APP(MyApp);

MyApp::MyApp(){}
MyApp::~MyApp(){}
int MyApp::OnExit(){return 0;}

bool MyApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;
	OBJ_FRAME_1 = new FRAME_1();
    OBJ_FRAME_1->Show();
	SetTopWindow(OBJ_FRAME_1);
	OBJ_FRAME_1->Refresh();
    return true;
}

// --------------------------------------------
// -- الدوال
// --------------------------------------------

void Alif_Library_Example_1 (wxString msg)
{
	wxMessageBox( msg );
}

double Alif_Library_Example_2 (double foo, double bar)
{
	return foo + bar;
}
 )" );

		}
		else
		{
			wxMessageBox("امتداد الملف غير معروف : " + fn.GetExt(), wxT("خطأ"), wxICON_WARNING);
			return;
		}

		if (SAVE_FILE_UTF8(SAMPLE_CODE, Path, false))
		{
			// UI Initilizing
			/*
			UI_BUTTON_SAVE(true);
			if (CURRENT_FILE_EXTENSION == "ALIF")
				UI_BUTTON_BUILD(true);
			else
				UI_BUTTON_BUILD(false);
			OBJ_CODE->ClearAll();
			OBJ_CODE->Enable(true);
			OBJ_CODE->SetEditable(true);
			OBJ_LOG->Clear();
			*/

			PATH_DIRECTORY = fn.GetPathWithSep();

			SET_NEW_FILE(Path);
			
			SET_TREE_FILES_LIST();	

			wxTreeItemId TREE_TO_SELECT_ID;

			for (unsigned int i = 1; i <= TREE_FILES_TOTAL_ITEM; i++) // first file ID in list is 1
			{
				if (Path == TREE_FILES_PATH[i])
				{
					TREE_TO_SELECT_ID = TREE_ID_FILES_ITEM[i];

					OBJ_TREE_FILES_LIST->SetItemBold(TREE_TO_SELECT_ID, true);
					TREE_ID_FILES_LAST_BOLD = TREE_TO_SELECT_ID;

					UI_MANAGER(i);

					break;
				}
			}
		}
		else if (!wxFileName::FileExists(Path))
		{
			wxMessageBox(wxT("لم تنجح عملية تسجيل الملف \nحاول مع ملف باسم فيه حروف إنجليزية فقط"), wxT("خطأ"), wxICON_WARNING);
			return;
		}
    }

	/*
	OBJ_LOG->AppendText(wxT("\n ID_UI_CONTROL_CURRENT_TYPE : "));
	OBJ_LOG->AppendText(ID_UI_CONTROL_CURRENT_TYPE);
	OBJ_LOG->AppendText(wxT(" | "));
	OBJ_LOG->AppendText(wxT("ID_UI_WINDOW_NAME_CURRENT : "));
	OBJ_LOG->AppendText(ID_UI_WINDOW_NAME_CURRENT);
	OBJ_LOG->AppendText(wxT(" | "));
	OBJ_LOG->AppendText(wxT("ID_UI_WINDOW_CURRENT : "));
	OBJ_LOG->AppendText(CONVERT_INT_TO_STRING(ID_UI_WINDOW_CURRENT));
	OBJ_LOG->AppendText(wxT(" | "));
	OBJ_LOG->AppendText(wxT("UI_WINDOW_IDENTIFIER[ID_UI_WINDOW_CURRENT] : "));
	OBJ_LOG->AppendText(UI_WINDOW_IDENTIFIER[ID_UI_WINDOW_CURRENT]);
	OBJ_LOG->AppendText(wxT(" | "));
	OBJ_LOG->AppendText(wxT("UI_WINDOW_CAPTION[ID_UI_WINDOW_CURRENT] : "));
	OBJ_LOG->AppendText(UI_WINDOW_CAPTION[ID_UI_WINDOW_CURRENT]);
	OBJ_LOG->AppendText(wxT(" | "));
	OBJ_LOG->AppendText(wxT("UI_CONTROL_TOTAL : "));
	OBJ_LOG->AppendText(CONVERT_INT_TO_STRING(UI_CONTROL_TOTAL));
	*/
}

// -------------------------------

void CLASS_WINDOW_MAIN::PROPERTIES_CHANGED(wxPropertyGridEvent& event)
{
	if (IGNORE_UPDATE_SETTINGS_EVENT)
		return;
	
	event.Skip();
	
	if (ID_UI_CONTROL_CURRENT_TYPE == "WINDOW")
	{
		wxPGProperty* PROPERTIES = event.GetProperty();
		wxString PROPERTIES_NAME = PROPERTIES->GetName();
		wxString NEW_WINDOW_NAME = event.GetValue().GetString();
		
		if (PROPERTIES_NAME == wxT(" التعريف "))
		{
			// change all controls Window name to this new Window name
			for (unsigned int i = 1; i <= UI_CONTROL_TOTAL; i++)
			{
				if (UI_CONTROL_WINDOW[i] == ID_UI_WINDOW_NAME_CURRENT)
				{
					UI_CONTROL_WINDOW[i] = NEW_WINDOW_NAME;
				}
			}
			
			// change window name in window list tree
			if (ID_UI_CONTROL_CURRENT_TYPE == "WINDOW")
			{
				for (unsigned int i = 1; i <= TREE_WINDOW_TOTAL_ITEM; i++)
				{
					if (UI_WINDOW_IDENTIFIER[i] == UI_WINDOW_IDENTIFIER[ID_UI_WINDOW_CURRENT])
					{
						OBJ_TREE_WINDOW->SetItemText(TREE_ID_WINDOW_ITEM[i], NEW_WINDOW_NAME);
						//wxMessageBox("found");
					}
				}
			}
			
			// change window name now
			ID_UI_WINDOW_NAME_CURRENT = NEW_WINDOW_NAME;
			UI_WINDOW_IDENTIFIER[ID_UI_WINDOW_CURRENT] = NEW_WINDOW_NAME;
		}
		else if (PROPERTIES_NAME == wxT(" النص "))
		{
			UI_WINDOW_CAPTION[ID_UI_WINDOW_CURRENT] = event.GetValue().GetString();
			UI_CONTROL_CAPTION_UPDATE(0);
		}
		else if (PROPERTIES_NAME == wxT(" الأفصول (x) "))
		{
			UI_WINDOW_X[ID_UI_WINDOW_CURRENT] = wxAtoi(event.GetValue().GetString());
			UI_MOVE_CONTROL(0);
		}
		else if (PROPERTIES_NAME == wxT(" الأرتوب (y) "))
		{
			UI_WINDOW_Y[ID_UI_WINDOW_CURRENT] = wxAtoi(event.GetValue().GetString());
			UI_MOVE_CONTROL(0);
		}
		else if (PROPERTIES_NAME == wxT(" العرض "))
		{
			UI_WINDOW_W[ID_UI_WINDOW_CURRENT] = wxAtoi(event.GetValue().GetString());
			UI_RESIZE_CONTROL(0);
		}
		else if (PROPERTIES_NAME == wxT(" الأرتفاع "))
		{
			UI_WINDOW_H[ID_UI_WINDOW_CURRENT] = wxAtoi(event.GetValue().GetString());
			UI_RESIZE_CONTROL(0);
		}
	}
	else if (ID_UI_CONTROL_CURRENT_TYPE == "CONTROL")
	{
		wxPGProperty* PROPERTIES = event.GetProperty();
		wxString PROPERTIES_NAME = PROPERTIES->GetName();
	
		if (PROPERTIES_NAME == wxT(" التعريف "))
		{
			UI_CONTROL_IDENTIFIER[ID_UI_CONTROL_CURRENT] = event.GetValue().GetString();
		}
		else if (PROPERTIES_NAME == wxT(" النص "))
		{
			UI_CONTROL_CAPTION[ID_UI_CONTROL_CURRENT] = event.GetValue().GetString();
			UI_CONTROL_CAPTION_UPDATE(ID_UI_CONTROL_CURRENT);
		}
		else if (PROPERTIES_NAME == wxT(" الأفصول (x) "))
		{
			UI_CONTROL_X[ID_UI_CONTROL_CURRENT] = wxAtoi(event.GetValue().GetString());
			UI_MOVE_CONTROL(ID_UI_CONTROL_CURRENT);
		}
		else if (PROPERTIES_NAME == wxT(" الأرتوب (y) "))
		{
			UI_CONTROL_Y[ID_UI_CONTROL_CURRENT] = wxAtoi(event.GetValue().GetString());
			UI_MOVE_CONTROL(ID_UI_CONTROL_CURRENT);
		}
		else if (PROPERTIES_NAME == wxT(" العرض "))
		{
			UI_CONTROL_W[ID_UI_CONTROL_CURRENT] = wxAtoi(event.GetValue().GetString());
			UI_RESIZE_CONTROL(ID_UI_CONTROL_CURRENT);
		}
		else if (PROPERTIES_NAME == wxT(" الأرتفاع "))
		{
			UI_CONTROL_H[ID_UI_CONTROL_CURRENT] = wxAtoi(event.GetValue().GetString());
			UI_RESIZE_CONTROL(ID_UI_CONTROL_CURRENT);
		}
	}
	
	UI_GENERAT_ALIF_CODE();
}

// ****************************************************************************
// ** Custum Functions ***********************************************************
// ****************************************************************************

void UI_GENERAT_ALIF_CODE()
{
	wxString WINDOWS[64];
	unsigned int WINDOWS_TOTAL = 0;
	wxString ALIF_CODE;
	
	ALIF_CODE.Append(wxT("\n #ألف \n"));
	
	// Create Windows List
	for (unsigned int i =1; i <= TREE_WINDOW_TOTAL_ITEM; i++)
	{
		WINDOWS_TOTAL++;
		WINDOWS[WINDOWS_TOTAL] = UI_WINDOW_IDENTIFIER[i];
	}
	
	// For every Window.. 
	for (unsigned int i = 1; i <= WINDOWS_TOTAL; i++)
	{
		//أداة نافذة (رئيسية, 0, 0, 500, 500, "نافذة نافذة نافنافدةدة")
		//أداة نافذة (نافدتي, 30, 30, 500, 500, "الثانية")
		
		ALIF_CODE.Append(wxT("\n أداة نافذة (") + UI_WINDOW_IDENTIFIER[i] + ", " 
		+ CONVERT_INT_TO_STRING(UI_WINDOW_X[i]) + ", " 
		+ CONVERT_INT_TO_STRING(UI_WINDOW_Y[i]) + ", " 
		+ CONVERT_INT_TO_STRING(UI_WINDOW_W[i]) + ", " 
		+ CONVERT_INT_TO_STRING(UI_WINDOW_H[i]) + ", " 
		+ "\"" + UI_WINDOW_CAPTION[i] + "\") \n\n");
		
		// For every control
		for (unsigned int x =1; x <= UI_CONTROL_TOTAL; x++)
		{
			if (UI_CONTROL_WINDOW[x] == WINDOWS[i])
			{
				if (UI_CONTROL_TYPE[x] == "BUTTON")
				{
					ALIF_CODE.Append(wxT("	أداة زر		(") + UI_CONTROL_WINDOW[x] + ", " 
					+ UI_CONTROL_IDENTIFIER[x] + ", " 
					+ CONVERT_INT_TO_STRING(UI_CONTROL_X[x]) + ", " 
					+ CONVERT_INT_TO_STRING(UI_CONTROL_Y[x]) + ", " 
					+ CONVERT_INT_TO_STRING(UI_CONTROL_W[x]) + ", " 
					+ CONVERT_INT_TO_STRING(UI_CONTROL_H[x]) + ", " 
					+ "\"" + UI_CONTROL_CAPTION[x] + "\") \n");
				}
				else if (UI_CONTROL_TYPE[x] == "TXT")
				{
					ALIF_CODE.Append(wxT("	أداة نص		(") + UI_CONTROL_WINDOW[x] + ", " 
					+ UI_CONTROL_IDENTIFIER[x] + ", " 
					+ CONVERT_INT_TO_STRING(UI_CONTROL_X[x]) + ", " 
					+ CONVERT_INT_TO_STRING(UI_CONTROL_Y[x]) + ", " 
					+ CONVERT_INT_TO_STRING(UI_CONTROL_W[x]) + ", " 
					+ CONVERT_INT_TO_STRING(UI_CONTROL_H[x]) + ", " 
					+ "\"" + UI_CONTROL_CAPTION[x] + "\") \n");
				}
				else if (UI_CONTROL_TYPE[x] == "LABEL")
				{
					ALIF_CODE.Append(wxT("	أداة ملصق	(") + UI_CONTROL_WINDOW[x] + ", " 
					+ UI_CONTROL_IDENTIFIER[x] + ", "
					+ CONVERT_INT_TO_STRING(UI_CONTROL_X[x]) + ", " 
					+ CONVERT_INT_TO_STRING(UI_CONTROL_Y[x]) + ", " 
					+ CONVERT_INT_TO_STRING(UI_CONTROL_W[x]) + ", " 
					+ CONVERT_INT_TO_STRING(UI_CONTROL_H[x]) + ", " 
					+ "\"" + UI_CONTROL_CAPTION[x] + "\") \n");
				}
			}
		}
	}

	// Todo : Search and Replace line, aulieu de replace all text!
	#ifdef ALIF_STUDIO_NO_STC
		OBJ_CODE->Clear();
		OBJ_CODE->AppendText(ALIF_CODE);
	#else
		OBJ_CODE->ClearAll();
		OBJ_CODE->SetText(ALIF_CODE);
	#endif

	//if (!FIRST_GENERATED_CODE)
		//CODE_MODIFIED = true;
	//else
	if (FIRST_GENERATED_CODE)
		// Ignore the first generated Code when creating window..
		FIRST_GENERATED_CODE = false;
}

void UI_MOVE_CONTROL(int ID)
{
	// x,y already set.
	// but cotrl not moved.
	
	if (ID < 1 && ID_UI_CONTROL_CURRENT_TYPE == "WINDOW")
	{
		OBJ_UI_CLASS_WINDOW->Move(wxPoint(UI_WINDOW_X[ID_UI_WINDOW_CURRENT], UI_WINDOW_Y[ID_UI_WINDOW_CURRENT]) );
	}
	else if (ID > 0 && ID_UI_CONTROL_CURRENT_TYPE == "CONTROL")
	{
		if (UI_CONTROL_TYPE[ID_UI_CONTROL_CURRENT] == "BUTTON")
		{
			OBJ_UI_BUTTON_ARRAY[ID]->Move(wxPoint(UI_CONTROL_X[ID], UI_CONTROL_Y[ID]) );
		}
		else if (UI_CONTROL_TYPE[ID_UI_CONTROL_CURRENT] == "TXT")
		{
			OBJ_UI_TXT_ARRAY[ID]->Move(wxPoint(UI_CONTROL_X[ID], UI_CONTROL_Y[ID]) );
		}
		else if (UI_CONTROL_TYPE[ID_UI_CONTROL_CURRENT] == "LABEL")
		{
			OBJ_UI_LABEL_ARRAY[ID]->Move(wxPoint(UI_CONTROL_X[ID], UI_CONTROL_Y[ID]) );
		}
	}
}

// -------------------------------

void UI_RESIZE_CONTROL(int ID)
{
	// w,h already set.
	// but cotrl not resezed.
	
	if (ID < 1 && ID_UI_CONTROL_CURRENT_TYPE == "WINDOW")
	{
		OBJ_UI_CLASS_WINDOW->SetSize( UI_WINDOW_W[ID_UI_WINDOW_CURRENT], UI_WINDOW_H[ID_UI_WINDOW_CURRENT] );
	}
	else if (ID > 0 && ID_UI_CONTROL_CURRENT_TYPE == "CONTROL")
	{
		if (UI_CONTROL_TYPE[ID_UI_CONTROL_CURRENT] == "BUTTON")
		{
			OBJ_UI_BUTTON_ARRAY[ID]->SetSize( UI_CONTROL_W[ID], UI_CONTROL_H[ID] );
		}
		else if (UI_CONTROL_TYPE[ID_UI_CONTROL_CURRENT] == "TXT")
		{
			OBJ_UI_TXT_ARRAY[ID]->SetSize( UI_CONTROL_W[ID], UI_CONTROL_H[ID] );
		}
		else if (UI_CONTROL_TYPE[ID_UI_CONTROL_CURRENT] == "LABEL")
		{
			OBJ_UI_LABEL_ARRAY[ID]->SetSize( UI_CONTROL_W[ID], UI_CONTROL_H[ID] );
		}
	}
}

// -------------------------------

void UI_CONTROL_CAPTION_UPDATE(int ID)
{
	// cap already set in glb var.
	// but not in ctrl.
	
	if (ID < 1 && ID_UI_CONTROL_CURRENT_TYPE == "WINDOW")
	{
		OBJ_UI_CLASS_WINDOW->SetLabel( UI_WINDOW_CAPTION[ID_UI_WINDOW_CURRENT] );
	}
	else if (ID > 0 && ID_UI_CONTROL_CURRENT_TYPE == "CONTROL")
	{
		if (UI_CONTROL_TYPE[ID_UI_CONTROL_CURRENT] == "BUTTON")
		{
			OBJ_UI_BUTTON_ARRAY[ID]->SetLabel( UI_CONTROL_CAPTION[ID] );
		}
		else if (UI_CONTROL_TYPE[ID_UI_CONTROL_CURRENT] == "TXT")
		{
			OBJ_UI_TXT_ARRAY[ID]->SetLabel( UI_CONTROL_CAPTION[ID] );
		}
		else if (UI_CONTROL_TYPE[ID_UI_CONTROL_CURRENT] == "LABEL")
		{
			OBJ_UI_LABEL_ARRAY[ID]->SetLabel( UI_CONTROL_CAPTION[ID] );
		}
	}
	
}

// -------------------------------

void UI_EMPTY_SETTINGS()
{
	IGNORE_UPDATE_SETTINGS_EVENT = true;
	
	wxPGProperty* PROPERTIES_ID;

	PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" التعريف "));
		OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, "");
	
	PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" الأفصول (x) "));
		OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, 0);
	
	PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" الأرتوب (y) "));
		OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, 0);
	
	PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" الأرتفاع "));
		OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, 0);
	
	PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" العرض "));
		OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, 0);
	
	PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" النص "));
		OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, "");
	
	IGNORE_UPDATE_SETTINGS_EVENT = false;
	
	OBJ_PROPERTIES->Enable(false);
	
	ID_UI_CONTROL_CURRENT = 0;
	ID_UI_CONTROL_CURRENT_TYPE = "";
	ID_UI_WINDOW_NAME_CURRENT = "";
	ID_UI_WINDOW_CURRENT = 0;
}

// -------------------------------

void UI_UPDATE_SETTINGS(unsigned int ID)
{
	OBJ_PROPERTIES->Enable(true);
	
	ID_UI_CONTROL_CURRENT = ID;
	
	IGNORE_UPDATE_SETTINGS_EVENT = true;
	
	if (ID < 1 && ID_UI_CONTROL_CURRENT_TYPE == "WINDOW")
	{
		wxPGProperty* PROPERTIES_ID;

		PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" التعريف "));
			OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, UI_WINDOW_IDENTIFIER[ID_UI_WINDOW_CURRENT]);
	
		PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" الأفصول (x) "));
			OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, UI_WINDOW_X[ID_UI_WINDOW_CURRENT]);
	
		PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" الأرتوب (y) "));
			OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, UI_WINDOW_Y[ID_UI_WINDOW_CURRENT]);
	
		PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" العرض "));
			OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, UI_WINDOW_W[ID_UI_WINDOW_CURRENT]);
	
		PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" الأرتفاع "));
			OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, UI_WINDOW_H[ID_UI_WINDOW_CURRENT]);
	
		PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" النص "));
			OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, UI_WINDOW_CAPTION[ID_UI_WINDOW_CURRENT]);
	}
	else if (ID > 0 && ID_UI_CONTROL_CURRENT_TYPE == "CONTROL")
	{
		wxPGProperty* PROPERTIES_ID;

		PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" التعريف "));
			OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, UI_CONTROL_IDENTIFIER[ID]);
	
		PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" الأفصول (x) "));
			OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, UI_CONTROL_X[ID]);
	
		PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" الأرتوب (y) "));
			OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, UI_CONTROL_Y[ID]);
	
		PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" الأرتفاع "));
			OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, UI_CONTROL_W[ID]);
	
		PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" العرض "));
			OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, UI_CONTROL_H[ID]);
	
		PROPERTIES_ID = OBJ_PROPERTIES->GetPropertyByName(wxT(" النص "));
			OBJ_PROPERTIES->ChangePropertyValue(PROPERTIES_ID, UI_CONTROL_CAPTION[ID]);
	}
	
	IGNORE_UPDATE_SETTINGS_EVENT = false;
	
	// ----------------------------------------------------------------------------------

	// Debuging..
/*
	if (ID < 1)
	{
		OBJ_LOG->AppendText(wxT("\n Window ID : "));
		OBJ_LOG->AppendText(CONVERT_INT_TO_STRING(ID_UI_WINDOW_CURRENT));
		OBJ_LOG->AppendText(wxT(" | "));
		OBJ_LOG->AppendText(wxT("IDENTIFIER : "));
		OBJ_LOG->AppendText(UI_WINDOW_IDENTIFIER[ID_UI_WINDOW_CURRENT]);
		return;
	}
	
	OBJ_LOG->AppendText(wxT("\n ID : "));
	OBJ_LOG->AppendText(CONVERT_INT_TO_STRING(ID));
	OBJ_LOG->AppendText(wxT(" | "));
	OBJ_LOG->AppendText(wxT("UI_CONTROL_WINDOW : "));
	OBJ_LOG->AppendText(UI_CONTROL_WINDOW[ID]);
	OBJ_LOG->AppendText(wxT(" | "));
	OBJ_LOG->AppendText(wxT("TYPE : "));
	OBJ_LOG->AppendText(UI_CONTROL_TYPE[ID]);
	OBJ_LOG->AppendText(wxT(" | "));
	OBJ_LOG->AppendText(wxT("IDENTIFIER : "));
	OBJ_LOG->AppendText(UI_CONTROL_IDENTIFIER[ID]);
	OBJ_LOG->AppendText(wxT(" | "));
	OBJ_LOG->AppendText(wxT("CAPTION : "));
	OBJ_LOG->AppendText(UI_CONTROL_CAPTION[ID]);
	OBJ_LOG->AppendText(wxT(" | "));
	OBJ_LOG->AppendText(wxT("X : "));
	OBJ_LOG->AppendText(CONVERT_INT_TO_STRING(UI_CONTROL_X[ID]));
	OBJ_LOG->AppendText(wxT(" | "));
	OBJ_LOG->AppendText(wxT("Y : "));
	OBJ_LOG->AppendText(CONVERT_INT_TO_STRING(UI_CONTROL_Y[ID]));
	OBJ_LOG->AppendText(wxT(" | "));
	OBJ_LOG->AppendText(wxT("W : "));
	OBJ_LOG->AppendText(CONVERT_INT_TO_STRING(UI_CONTROL_W[ID]));
	OBJ_LOG->AppendText(wxT(" | "));
	OBJ_LOG->AppendText(wxT("H : "));
	OBJ_LOG->AppendText(CONVERT_INT_TO_STRING(UI_CONTROL_H[ID]));
*/
}

// -------------------------------

void Exit()
{
	if (PATH_FULL_SOURCE.IsEmpty())
	{
		if (WINDOW_IS_CONSTRUCTED)
			OBJ_UI_CLASS_WINDOW->Destroy();
				
		OBJ_CLASS_WINDOW_MAIN->Destroy();

		return;
	}

	#ifdef ALIF_STUDIO_NO_STC
		if (CODE_MODIFIED)
	#else
		if (OBJ_CODE->IsModified())
	#endif
	{
		int answer = wxMessageBox(wxT("لقد غيرت محتوى الملف، هل تود حفظ الملف ؟"), wxT("خروج"), wxYES_NO | wxCANCEL, OBJ_CLASS_WINDOW_MAIN);
		if (answer == wxYES)
		{
			// Save
			if (SAVE_FILE_UTF8( OBJ_CODE->GetValue(), PATH_FULL_SOURCE, true))
			{
				if (WINDOW_IS_CONSTRUCTED)
					OBJ_UI_CLASS_WINDOW->Destroy();
				
				OBJ_CLASS_WINDOW_MAIN->Destroy();
			}
			else
			{
				wxMessageBox(wxT("لم تنجح عملية تسجيل الملف \nحاول مع ملف باسم فيه حروف إنجليزية فقط"), wxT("خطأ"), wxICON_WARNING);
				// Stay on app to not lost data
			}
		}
		else if (answer == wxCANCEL)
		{
			// Stay on app
		}
		else
		{
			// Exit
			if (WINDOW_IS_CONSTRUCTED)
				OBJ_UI_CLASS_WINDOW->Destroy();
			
			OBJ_CLASS_WINDOW_MAIN->Destroy();
		}
	}
	else
	{
		if (WINDOW_IS_CONSTRUCTED)
			OBJ_UI_CLASS_WINDOW->Destroy();
		
		OBJ_CLASS_WINDOW_MAIN->Destroy();
	}
}

// -------------------------------

void SET_TREE_FILES_LIST()
{
	wxString FILENAME;
	wxString FILE_EXTENSION;
	bool NEXT = false;
	unsigned int i;
	wxDir OBJ_DIR(PATH_DIRECTORY);

	// Initialization
	
	// Clear File List
	if (TREE_FILES_HAVE_ITEM)
	{
		//OBJ_TREE_FILES_LIST->Delete(0);
		OBJ_TREE_FILES_LIST->DeleteAllItems();
		TREE_FILES_HAVE_ITEM = false;
	}

	TREE_FILES_TOTAL_ITEM = 0;
	i = 1;
	
	// DIR
	TREE_ID_FILES_ROOT = OBJ_TREE_FILES_LIST->AddRoot(PATH_DIRECTORY, 0, 0);

	// Set to have items, 
	// to igior Linux crash when delete empty list
	// bcs Root is an item.
	TREE_FILES_HAVE_ITEM = true;
	
	// *.alif
	FILE_EXTENSION = wxT("*.alif");
	NEXT = OBJ_DIR.GetFirst(&FILENAME, FILE_EXTENSION, wxDIR_FILES);
	while ( NEXT )
	{
		if (FILENAME != "")
		{
			wxFileName fname( FILENAME );
			TREE_ID_FILES_ITEM[i] = OBJ_TREE_FILES_LIST->AppendItem(TREE_ID_FILES_ROOT, 
			fname.GetName() + wxT(" . ألف"), 1, 1);
			TREE_FILES_TYPE[i] = "ALIF";
			TREE_FILES_PATH[i] = PATH_DIRECTORY + FILENAME;
			TREE_FILES_TOTAL_ITEM++;
			i++;
			NEXT = OBJ_DIR.GetNext(&FILENAME);
		}
	}
	
	// *.alifui
	
	FILE_EXTENSION = wxT("*.alifui");
	NEXT = OBJ_DIR.GetFirst(&FILENAME, FILE_EXTENSION, wxDIR_FILES);
	while ( NEXT )
	{
		if (FILENAME != "")
		{
			wxFileName fname( FILENAME );
			TREE_ID_FILES_ITEM[i] = OBJ_TREE_FILES_LIST->AppendItem(TREE_ID_FILES_ROOT, 
			fname.GetName() + wxT(" . واجهة"), 2, 2);
			TREE_FILES_TYPE[i] = "ALIFUI";
			TREE_FILES_PATH[i] = PATH_DIRECTORY + FILENAME;
			TREE_FILES_TOTAL_ITEM++;
			i++;
			NEXT = OBJ_DIR.GetNext(&FILENAME);
		}
	}
	
	// *.aliflib
	
	FILE_EXTENSION = wxT("*.aliflib");
	NEXT = OBJ_DIR.GetFirst(&FILENAME, FILE_EXTENSION, wxDIR_FILES);
	while ( NEXT )
	{
		if (FILENAME != "")
		{
			wxFileName fname( FILENAME );
			TREE_ID_FILES_ITEM[i] = OBJ_TREE_FILES_LIST->AppendItem(TREE_ID_FILES_ROOT, 
			fname.GetName() + wxT(" . مكتبة"), 3, 3);
			TREE_FILES_TYPE[i] = "ALIFLIB";
			TREE_FILES_PATH[i] = PATH_DIRECTORY + FILENAME;
			TREE_FILES_TOTAL_ITEM++;
			i++;
			NEXT = OBJ_DIR.GetNext(&FILENAME);
		}
	}
	
	// Expand
	OBJ_TREE_FILES_LIST->ExpandAll();
}

// -------------------------------

wxString UI_CONTROL_TYPE_ARABIC(unsigned int ID)
{
	if (UI_CONTROL_TYPE[ID] == "BUTTON")
		return wxT("زر");

	else if (UI_CONTROL_TYPE[ID] == "TXT")
		return wxT("نص");

	else if (UI_CONTROL_TYPE[ID] == "LABEL")
		return wxT("ملصق");

	else
		return wxT("");
}

// -------------------------------

void UI_REMOVE_CONTROL(unsigned int ID)
{
	bool REMOVED = false;
	
	int answer = wxMessageBox(wxT("هل تود إزالة الأداة الثالية بشكل نهائي ؟ \n\nالنوع : ") + 
	UI_CONTROL_TYPE_ARABIC(ID) + wxT("\n\nالتعريف : ") + UI_CONTROL_IDENTIFIER[ID], 
	wxT("إزالة الأداة"), wxYES_NO | wxICON_EXCLAMATION, OBJ_UI_CLASS_WINDOW );
	
	if (answer == wxYES)
	{
		if (UI_CONTROL_TYPE[ID] == "BUTTON")
		{
			if (OBJ_UI_BUTTON_ARRAY[ID]->Destroy())
				REMOVED = true;
			else
				REMOVED = false;
		}
		else if (UI_CONTROL_TYPE[ID] == "TXT")
		{
			if (OBJ_UI_TXT_ARRAY[ID]->Destroy())
				REMOVED = true;
			else
				REMOVED = false;
		}
		else if (UI_CONTROL_TYPE[ID] == "LABEL")
		{
			if (OBJ_UI_LABEL_ARRAY[ID]->Destroy())
				REMOVED = true;
			else
				REMOVED = false;
		}
		else
			REMOVED = false;
		
		if (REMOVED)
		{
			UI_CONTROL_TYPE[ID] = "";
			UI_CONTROL_CAPTION[ID] = "";
			UI_CONTROL_IDENTIFIER[ID] = "";
			UI_CONTROL_WINDOW[ID] = "";
			
			// Select Window after delete control
			ID_UI_CONTROL_CURRENT_TYPE = "WINDOW";
			ID_UI_CONTROL_CURRENT = 0;
			UI_UPDATE_SETTINGS(0);
			UI_GENERAT_ALIF_CODE();
		}
		else
			wxMessageBox(wxT("لم تنجح عملية ازالة الاداة"), wxT("خطأ"), wxICON_WARNING);
	}
}

// -------------------------------

static std::map<int, bool> UI_ERROR_LINES_STATUS;

#ifndef ALIF_STUDIO_NO_STC

	void UILOG_COLOR()
	{
		unsigned int TOTAL_LINES = (OBJ_CODE->GetLineCount()) - 1; // First line is 0

		int endPsn = 0;
		int startPsn = 0;
		int len = 0;

		for (unsigned int LINE_NUMBER = 0; LINE_NUMBER <= TOTAL_LINES; LINE_NUMBER++)
		{
			endPsn = OBJ_CODE->GetLineEndPosition(LINE_NUMBER);
			startPsn = OBJ_CODE->PositionFromLine(LINE_NUMBER);
			len = endPsn - startPsn;

			if (UI_ERROR_LINES_STATUS[LINE_NUMBER])
			{
				OBJ_CODE->SetIndicatorCurrent(ID_CODE_ERROR_INDICATOR);
				OBJ_CODE->IndicatorFillRange(startPsn, len);
			}
			else
			{
				OBJ_CODE->IndicatorClearRange(startPsn, len);
			}
		}

		// ------------------------

		//int endPsn = OBJ_CODE->GetLineEndPosition(LINE_NUMBER);
		//int startPsn= OBJ_CODE->PositionFromLine(LINE_NUMBER);
		//int len = endPsn - startPsn;

		//OBJ_CODE->SetIndicatorCurrent(ID_CODE_ERROR_INDICATOR);
		//OBJ_CODE->IndicatorFillRange(startPsn, len);
		
		// ------------------------

		//int errorStyleNo = 32; //wxSTC_STYLE_BRACEBAD;

		//OBJ_CODE->StyleSetForeground(errorStyleNo, wxColour(50, 54, 58));
		//OBJ_CODE->StyleSetBackground(errorStyleNo, wxColour(250, 251, 252));

		//OBJ_CODE->StartStyling(0);
		//OBJ_CODE->SetStyling(10, errorStyleNo);

		// ------------------------
	}

#endif

void UILOG(int LINE_NUMBER, wxString MSG)
{
	UI_ERROR_LINES_STATUS[LINE_NUMBER] = true;

	OBJ_LOG->AppendText(wxT("سطر ("));
	OBJ_LOG->AppendText(CONVERT_INT_TO_STRING(LINE_NUMBER + 1));
	OBJ_LOG->AppendText(wxT(") : "));
	OBJ_LOG->AppendText(MSG);
	OBJ_LOG->AppendText(wxT(" \n"));

	UILOG_ERROR = true;
}

void UI_DRAW_NEW_CONTROL(wxString TYPE, int X, int Y)
{
	if (TYPE == "BUTTON" ||
		TYPE == "TXT" ||
		TYPE == "LABEL")
	{
		UI_CONTROL_TOTAL++; // First Control ID is 1
		
		UI_CONTROL_TYPE[UI_CONTROL_TOTAL] = TYPE;
		UI_CONTROL_WINDOW[UI_CONTROL_TOTAL] = ID_UI_WINDOW_NAME_CURRENT;
		
		UI_CONTROL_X[UI_CONTROL_TOTAL] = X;
		UI_CONTROL_Y[UI_CONTROL_TOTAL] = Y;
		
		if (TYPE == "BUTTON")
		{
			UI_CONTROL_IDENTIFIER[UI_CONTROL_TOTAL] = wxT("زر") + CONVERT_INT_TO_STRING(UI_CONTROL_TOTAL);
			UI_CONTROL_CAPTION[UI_CONTROL_TOTAL] = wxT("زر جديد");
			UI_CONTROL_W[UI_CONTROL_TOTAL] = 100;
			UI_CONTROL_H[UI_CONTROL_TOTAL] = 32;
			OBJ_UI_BUTTON_ARRAY[UI_CONTROL_TOTAL] = new Movable<wxButton>(UI_CONTROL_TOTAL, UI_CONTROL_CAPTION[UI_CONTROL_TOTAL], wxPoint(UI_CONTROL_X[UI_CONTROL_TOTAL], UI_CONTROL_Y[UI_CONTROL_TOTAL]), wxSize(UI_CONTROL_W[UI_CONTROL_TOTAL], UI_CONTROL_H[UI_CONTROL_TOTAL]),
			0);
		}
		else if (TYPE == "TXT")
		{
			UI_CONTROL_IDENTIFIER[UI_CONTROL_TOTAL] = wxT("نص") + CONVERT_INT_TO_STRING(UI_CONTROL_TOTAL);
			UI_CONTROL_CAPTION[UI_CONTROL_TOTAL] = wxT("نص جديد");
			UI_CONTROL_W[UI_CONTROL_TOTAL] = 128;
			UI_CONTROL_H[UI_CONTROL_TOTAL] = 24;
			OBJ_UI_TXT_ARRAY[UI_CONTROL_TOTAL] = new Movable<wxTextCtrl>(UI_CONTROL_TOTAL, UI_CONTROL_CAPTION[UI_CONTROL_TOTAL], wxPoint(UI_CONTROL_X[UI_CONTROL_TOTAL], UI_CONTROL_Y[UI_CONTROL_TOTAL]), wxSize(UI_CONTROL_W[UI_CONTROL_TOTAL], UI_CONTROL_H[UI_CONTROL_TOTAL]),
			wxTE_RIGHT); // wxTE_MULTILINE | 
		}
		else if (TYPE == "LABEL")
		{
			UI_CONTROL_IDENTIFIER[UI_CONTROL_TOTAL] = wxT("ملصق") + CONVERT_INT_TO_STRING(UI_CONTROL_TOTAL);
			UI_CONTROL_CAPTION[UI_CONTROL_TOTAL] = wxT("ملصق جديد");
			UI_CONTROL_W[UI_CONTROL_TOTAL] = 138;
			UI_CONTROL_H[UI_CONTROL_TOTAL] = 28;
			OBJ_UI_LABEL_ARRAY[UI_CONTROL_TOTAL] = new Movable<wxStaticText>(UI_CONTROL_TOTAL, UI_CONTROL_CAPTION[UI_CONTROL_TOTAL], wxPoint(UI_CONTROL_X[UI_CONTROL_TOTAL], UI_CONTROL_Y[UI_CONTROL_TOTAL]), wxSize(UI_CONTROL_W[UI_CONTROL_TOTAL], UI_CONTROL_H[UI_CONTROL_TOTAL]),
			wxST_NO_AUTORESIZE | wxALIGN_RIGHT | wxST_ELLIPSIZE_END);
		}
		
		ID_UI_CONTROL_CURRENT = UI_CONTROL_TOTAL;
		ID_UI_CONTROL_CURRENT_TYPE = "CONTROL";
		
		UI_UPDATE_SETTINGS(UI_CONTROL_TOTAL);
		
		UI_GENERAT_ALIF_CODE();
	}
}

void UI_PARSE_SOURCE()
{
	if (WINDOW_IS_CONSTRUCTED)
	{
		OBJ_UI_CLASS_WINDOW->Destroy();
		WINDOW_IS_CONSTRUCTED = false;
	}

	// Clear Window List
	if (TREE_WINDOW_TOTAL_ITEM > 0)
		OBJ_TREE_WINDOW->DeleteAllItems();

	UI_EMPTY_SETTINGS();

	#ifndef ALIF_STUDIO_NO_STC
		// Clear Error Indicators
		if (CODE_ERROR_INDICATOR)
		{
			unsigned int TOTAL_LINES = (OBJ_CODE->GetLineCount()) - 1; // First line is 0

			int endPsn = 0;
			int startPsn = 0;
			int len = 0;

			for (unsigned int LINE_NUMBER = 0; LINE_NUMBER <= TOTAL_LINES; LINE_NUMBER++)
			{
				endPsn = OBJ_CODE->GetLineEndPosition(LINE_NUMBER);
				startPsn = OBJ_CODE->PositionFromLine(LINE_NUMBER);
				len = endPsn - startPsn;

				OBJ_CODE->IndicatorClearRange(startPsn, len);
			}

			OBJ_CODE->MarkerDeleteAll(1);

			CODE_ERROR_INDICATOR = false;
		}
	#endif

	#ifndef ALIF_STUDIO_NO_STC
		OBJ_CODE->AnnotationSetStyles(2, "wxRED");
		//OBJ_CODE->BraceHighlight(2,4);
	#endif

	// ---------------------------------------------------------
	// Create Window List Root
	// ---------------------------------------------------------

	TREE_WINDOW_TOTAL_ITEM = 0;

	UI_ERROR_LINES_STATUS.clear();

	OBJ_LOG->Clear();
	
	// Initialization
	ID_UI_CONTROL_CURRENT_TYPE = "";
	ID_UI_WINDOW_NAME_CURRENT = "";
	ID_UI_WINDOW_CURRENT = 0;
	ID_UI_CONTROL_CURRENT = 0;
	UI_CONTROL_TOTAL = 0;
	UILOG_ERROR = false;
	UI_WINDOW_IS_SET.clear();
	UI_CONTROL_IS_SET.clear();
	UI_CONTROL_INSIDE_WINDOW_IS_SET.clear();
	
	TREE_ID_WINDOW_ROOT = OBJ_TREE_WINDOW->AddRoot(SOURCE_FILE_NAME, 0, 0);
	
	// ---------------------
	// Lines loop
	// ---------------------
		
	#ifdef ALIF_STUDIO_NO_STC
		unsigned int TOTAL_LINES = (OBJ_CODE->GetNumberOfLines()) - 1; // First line is 0
	#else
		unsigned int TOTAL_LINES = (OBJ_CODE->GetLineCount()) - 1; // First line is 0
	#endif

	for (unsigned int LINE_NUMBER = 0; LINE_NUMBER <= TOTAL_LINES; LINE_NUMBER++)
	{
		// ---------------------
		// Lexer (Char loop)
		// ---------------------

		#ifdef ALIF_STUDIO_NO_STC
			wxString LINE = OBJ_CODE->GetLineText(LINE_NUMBER);
		#else
			wxString LINE = OBJ_CODE->GetLine(LINE_NUMBER);
		#endif

		LINE = TRIM(LINE);

		if (LINE.IsEmpty() || 
			LINE.Mid(0, 1) == wxT("'"))
			continue;

		unsigned int TOTAL_TOKENS = 0;
		int LINE_LEN = LINE.Len();

		//wxString TOKENS[LINE_LEN];
		wxString *TOKENS = new wxString[LINE_LEN];

		bool LAST_IS_SPACE = false;
		bool IS_INSIDE_STRING = false;
		
		for (int CHAR_NUMBER = 0; CHAR_NUMBER < LINE_LEN; CHAR_NUMBER++)
		{
			wxString CHAR = LINE.Mid(CHAR_NUMBER, 1);

			// Fixing same chars..

			if (CHAR == "۰")
				CHAR = "0";
			else if (CHAR == "۱")
				CHAR = "1";
			else if (CHAR == "۲")
				CHAR = "2";
			else if (CHAR == "۳")
				CHAR = "3";
			else if (CHAR == "۴")
				CHAR = "4";
			else if (CHAR == "۵")
				CHAR = "5";
			else if (CHAR == "۶")
				CHAR = "6";
			else if (CHAR == "۷")
				CHAR = "7";
			else if (CHAR == "۸")
				CHAR = "8";
			else if (CHAR == "۹")
				CHAR = "9";
			else if (CHAR == "“")
				CHAR = "\"";
			else if (CHAR == "”")
				CHAR = "\"";
			else if (CHAR == "‘")
				CHAR = "'";
			
			// Start Parsing

			if (CHAR.IsEmpty())
			{
				continue;
			}
			else if ((CHAR == "\n") || (CHAR == "\r") || (CHAR == "\r\n") || (CHAR == "\0"))
			{
				break;
			}
			else if (CHAR == wxT("\""))
			{
				if (IS_INSIDE_STRING)
				{
					TOTAL_TOKENS++;
				}
				else
				{
					if (!TOKENS[TOTAL_TOKENS].IsEmpty())
						TOTAL_TOKENS++;
				}
				
				TOKENS[TOTAL_TOKENS].append(wxT("\""));

				TOTAL_TOKENS++;

				IS_INSIDE_STRING = !IS_INSIDE_STRING;

				LAST_IS_SPACE = false;
			}
			else if (CHAR == wxT(" ") || CHAR == wxT("	")) // Space or Tab
			{
				if (IS_INSIDE_STRING)
				{
					TOKENS[TOTAL_TOKENS].append(CHAR);
				}	
				else
				{
					if (!LAST_IS_SPACE)
					{
						if (!TOKENS[TOTAL_TOKENS].IsEmpty())
							TOTAL_TOKENS++;
						
						LAST_IS_SPACE = true;
					}
				}
			}
			else if (	(CHAR == wxT(")") ) || 
						(CHAR == wxT("(") ) || 
						(CHAR == wxT(",") ) || 
						(CHAR == wxT("،") ))
			{
				if (IS_INSIDE_STRING)
				{
					TOKENS[TOTAL_TOKENS].append(CHAR);
				}
				else
				{
					if (!TOKENS[TOTAL_TOKENS].IsEmpty())
						TOTAL_TOKENS++;
					
					TOKENS[TOTAL_TOKENS].append(CHAR);
					TOTAL_TOKENS++;
					LAST_IS_SPACE = false;					
				}
			}
			else
			{
				if ( CHAR != wxT("") && 
					(CHAR != "\n") && (CHAR != "\r") && (CHAR != "\r\n") && (CHAR != "\0"))
				{
					TOKENS[TOTAL_TOKENS].append(CHAR);
					LAST_IS_SPACE = false;
				}
			}
		}

		// ---------------------
		// Parser (Tokens loop)
		// ---------------------

		if (TOKENS[0] == wxT("'"))
		{
			// Igiore comment line
			UI_ERROR_LINES_STATUS[LINE_NUMBER + 1] = false; // Good line
			continue;
		}
		else if (TOKENS[0] == wxT("#ألف") || TOKENS[0] == wxT("#الف"))
		{
			if (TOKENS[1] == wxT(""))
			{
				UI_ERROR_LINES_STATUS[LINE_NUMBER + 1] = false; // Good line
				continue;
			}
			else if (TOKENS[1] != wxT("'"))
			{
				UILOG(LINE_NUMBER, wxT("أمر غير معروف [") + TOKENS[1] + wxT("]")); // Error line
				continue;
			}
		}
		else if (TOKENS[0] != wxT("أداة"))
		{
			UILOG(LINE_NUMBER, wxT("أمر غير معروف [") + TOKENS[0] + wxT("]")); // Error line
			continue;
		}

		if (TOKENS[1] ==  wxT("نافذة"))
		{
			if (TOKENS[2] != wxT("("))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة '(' قبل '" + TOKENS[2] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[4] != wxT(","))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ',' قبل '" + TOKENS[4] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[6] != wxT(","))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ',' قبل '" + TOKENS[6] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[8] != wxT(","))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ',' قبل '" + TOKENS[8] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[10] != wxT(","))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ',' قبل '" + TOKENS[10] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[12] != wxT(","))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ',' قبل '" + TOKENS[12] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[13] != wxT("\""))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ' \" ' قبل '" + TOKENS[13] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[15] != wxT("\""))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ' \" ' قبل '" + TOKENS[15] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[16] != wxT(")"))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ')' بعد '" + TOKENS[16] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[17] != wxT(""))
			{
				UILOG(LINE_NUMBER, wxT("أمر غير معروف [") + TOKENS[17] + wxT("]")); // Error line
				continue;
			}
			else if (UI_WINDOW_IS_SET[TOKENS[3]])
			{
				UILOG(LINE_NUMBER, wxT("نافذة موجودة مسبقا [") + TOKENS[3] + wxT("]")); // Error line
				continue;
			}

			// ---------------------------------------------------------
			// Create Window, And Window List Tree
			// ---------------------------------------------------------

			TREE_WINDOW_TOTAL_ITEM++; // First Window ID is 1
			
			UI_WINDOW_IDENTIFIER[TREE_WINDOW_TOTAL_ITEM] = TOKENS[3];
			UI_WINDOW_X[TREE_WINDOW_TOTAL_ITEM] = wxAtoi(TOKENS[5]);
			UI_WINDOW_Y[TREE_WINDOW_TOTAL_ITEM] = wxAtoi(TOKENS[7]);
			UI_WINDOW_W[TREE_WINDOW_TOTAL_ITEM] = wxAtoi(TOKENS[9]);
			UI_WINDOW_H[TREE_WINDOW_TOTAL_ITEM] = wxAtoi(TOKENS[11]);
			UI_WINDOW_CAPTION[TREE_WINDOW_TOTAL_ITEM] = TOKENS[14];

			UI_WINDOW_IS_SET[TOKENS[3]] = true;
			
			TREE_ID_WINDOW_ITEM[TREE_WINDOW_TOTAL_ITEM] = OBJ_TREE_WINDOW->
															AppendItem(TREE_ID_WINDOW_ROOT, 
															UI_WINDOW_IDENTIFIER[TREE_WINDOW_TOTAL_ITEM], 1, 1);
			
			continue;
		}
		else if (TOKENS[1] ==  wxT("زر") || 
				 TOKENS[1] ==  wxT("نص") ||
				 TOKENS[1] ==  wxT("ملصق"))
		{
			if (TOKENS[2] !=  wxT("("))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة '(' قبل '" + TOKENS[2] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[4] !=  wxT(","))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ',' قبل '" + TOKENS[4] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[6] !=  wxT(","))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ',' قبل '" + TOKENS[6] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[8] !=  wxT(","))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ',' قبل '" + TOKENS[8] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[10] !=  wxT(","))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ',' قبل '" + TOKENS[10] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[12] !=  wxT(","))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ',' قبل '" + TOKENS[12] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[14] !=  wxT(","))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ',' قبل '" + TOKENS[14] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[15] != wxT("\""))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة '1 \" ' قبل '" + TOKENS[15] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[17] != wxT("\""))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة '2 \" ' قبل '" + TOKENS[17] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[18] !=  wxT(")"))
			{
				UILOG(LINE_NUMBER, wxT("إشارة مفقودة ')' بعد '" + TOKENS[18] + "' ")); // Error line
				continue;
			}
			else if (TOKENS[19] != wxT(""))
			{
				UILOG(LINE_NUMBER, wxT("أمر غير معروف [") + TOKENS[19] + wxT("]")); // Error line
				continue;
			}
			else if (UI_CONTROL_INSIDE_WINDOW_IS_SET[TOKENS[3] + TOKENS[5]]) // (UI_CONTROL_IS_SET[TOKENS[5]])
			{
				UILOG(LINE_NUMBER, wxT("أداة موجودة مسبقا [") + TOKENS[5] + wxT("]")); // Error line
				continue;
			}
			else if (!UI_WINDOW_IS_SET[TOKENS[3]])
			{
				UILOG(LINE_NUMBER, wxT("نافذة غير موجودة [") + TOKENS[3] + wxT("]")); // Error line
				continue;
			}

			// ---------------------------------------------------------
			// Create Control
			// ---------------------------------------------------------

			UI_CONTROL_TOTAL++; // First Control ID is 1
			
			if (TOKENS[1] == wxT("زر"))
				UI_CONTROL_TYPE[UI_CONTROL_TOTAL] = "BUTTON";
			else if (TOKENS[1] == wxT("نص"))
				UI_CONTROL_TYPE[UI_CONTROL_TOTAL] = "TXT";
			else if (TOKENS[1] == wxT("ملصق"))
				UI_CONTROL_TYPE[UI_CONTROL_TOTAL] = "LABEL";
			
			UI_CONTROL_WINDOW[UI_CONTROL_TOTAL] = TOKENS[3];
			UI_CONTROL_IDENTIFIER[UI_CONTROL_TOTAL] = TOKENS[5];
			UI_CONTROL_X[UI_CONTROL_TOTAL] = wxAtoi(TOKENS[7]);
			UI_CONTROL_Y[UI_CONTROL_TOTAL] = wxAtoi(TOKENS[9]);
			UI_CONTROL_W[UI_CONTROL_TOTAL] = wxAtoi(TOKENS[11]);
			UI_CONTROL_H[UI_CONTROL_TOTAL] = wxAtoi(TOKENS[13]);
			UI_CONTROL_CAPTION[UI_CONTROL_TOTAL] = TOKENS[16];

			UI_CONTROL_INSIDE_WINDOW_IS_SET[TOKENS[3] + TOKENS[5]] = true; // Only for Parcins Code.

			UI_CONTROL_IS_SET[TOKENS[5]] = true;

			continue;
		}
		else
		{
			UILOG(LINE_NUMBER, wxT("أمر غير معروف [") + TOKENS[1] + wxT("]، على العموم يمكنك استخدام الأدوات التالية : نافذة، زر، نص، ملصق")); // Error line
			continue;
		}

		UI_ERROR_LINES_STATUS[LINE_NUMBER + 1] = false; // Good line

		// Free Memory
		delete [] TOKENS;

	} // End line loop.

	

	// ---------------------------------------------------------
	// Color Error Lines
	// ---------------------------------------------------------
	
	if (TREE_WINDOW_TOTAL_ITEM < 1 || UILOG_ERROR)
	{
		// There no Window in this 
		// Source Code, or there some errors
		// So we need Clean

		OBJ_TREE_WINDOW->DeleteAllItems();

		// Initialization
		ID_UI_CONTROL_CURRENT_TYPE = "";
		ID_UI_WINDOW_NAME_CURRENT = "";
		ID_UI_WINDOW_CURRENT = 0;
		ID_UI_CONTROL_CURRENT = 0;
		UI_CONTROL_TOTAL = 0;
		UILOG_ERROR = false;
		UI_WINDOW_IS_SET.clear();
		UI_CONTROL_IS_SET.clear();

		UI_EMPTY_SETTINGS();
		#ifndef ALIF_STUDIO_NO_STC
			UILOG_COLOR();
		#endif
	}
	else
	{
		#ifndef ALIF_STUDIO_NO_STC
			UILOG_COLOR();
		#endif
		OBJ_TREE_WINDOW->ExpandAll();
	}	
}

void UI_MANAGER(unsigned int ID_FILE)
{
	if (WINDOW_IS_CONSTRUCTED)
	{
		OBJ_UI_CLASS_WINDOW->Destroy();
		WINDOW_IS_CONSTRUCTED = false;
	}
	
	// Clear Window List
	if (TREE_WINDOW_TOTAL_ITEM > 0)
		OBJ_TREE_WINDOW->DeleteAllItems();

	UI_EMPTY_SETTINGS();

	wxString EXTENSION = TREE_FILES_TYPE[ID_FILE];
	
	if (EXTENSION == "ALIFUI")
	{
		// Alif UI

		OBJ_CODE->SetLayoutDirection (wxLayout_RightToLeft);

		// ---------------------
		// UI Initilizing
		// ---------------------

		#ifndef ALIF_STUDIO_NO_STC
			OBJ_CODE->SetLexer(wxSTC_LEX_LUA);
		#endif

		// UI Initilizing
		UI_BUTTON_SAVE(true);
		UI_BUTTON_BUILD(false);
		#ifdef ALIF_STUDIO_NO_STC
			OBJ_CODE->Clear();
		#else
			OBJ_CODE->ClearAll();
		#endif
		OBJ_CODE->Enable(true);
		OBJ_CODE->SetEditable(true);
		OBJ_LOG->Clear();
		
		//OBJ_CODE->Show(false);
		//OBJ_CODE->Destroy();

		OBJ_CODE->LoadFile(TREE_FILES_PATH[ID_FILE]);
		
		UI_PARSE_SOURCE();

		// Show Aui Designer Panels
		AUI_GUI_DESIGNER(true);
	}
	else if (EXTENSION == "ALIFLIB")
	{
		// Alif Library

		OBJ_CODE->SetLayoutDirection (wxLayout_LeftToRight);

		// TODO: Create func to StyleClearAll(), and switch styles between Lib and Alif
		//OBJ_CODE->SetLexer(wxSTC_LEX_CPP);

		// UI Initilizing
		UI_BUTTON_SAVE(true);
		UI_BUTTON_BUILD(true);
		#ifdef ALIF_STUDIO_NO_STC
			OBJ_CODE->Clear();
		#else
			OBJ_CODE->ClearAll();
		#endif
		OBJ_CODE->Enable(true);
		OBJ_CODE->SetEditable(true);
		OBJ_LOG->Clear();

		OBJ_CODE->LoadFile(TREE_FILES_PATH[ID_FILE]);

		// Hide Aui Designer Panels
		AUI_GUI_DESIGNER(false);
	}
	else
	{
		// Alif Source Code

		OBJ_CODE->SetLayoutDirection (wxLayout_RightToLeft);

		#ifndef ALIF_STUDIO_NO_STC
			OBJ_CODE->SetLexer(wxSTC_LEX_LUA);
		#endif

		// UI Initilizing
		UI_BUTTON_SAVE(true);
		UI_BUTTON_BUILD(true);
		#ifdef ALIF_STUDIO_NO_STC
			OBJ_CODE->Clear();
		#else
			OBJ_CODE->ClearAll();
		#endif
		OBJ_CODE->Enable(true);
		OBJ_CODE->SetEditable(true);
		OBJ_LOG->Clear();

		OBJ_CODE->LoadFile(TREE_FILES_PATH[ID_FILE]);

		// Hide Aui Designer Panels
		AUI_GUI_DESIGNER(false);
	}
}

// -------------------------------

bool SAVE_FILE_UTF8(wxString TXT, wxString PATH, bool OuverWrite)
{
	if (wxFileName::FileExists(PATH))
	{
		if (!OuverWrite)
		{
			int answer = wxMessageBox(wxT("الملف موجد مسبقا، هل تود تغييره ؟ \n\n" + PATH), 
			wxT("خروج"), wxYES_NO, OBJ_CLASS_WINDOW_MAIN);
			
			if (answer == wxNO)
			{
				return false;
			}
		}
	}

	/*
	wxMessageBox("H");

	wxTextFile file( PATH );
	
	if(file.Open())
	{
		file.AddLine("OKOK");
		file.Write();
		file.Close();

		return true;
	}
	else
	{
		file.Close();
		return false;
	}
	*/

	/*
	wxCharBuffer TXT_UTF8 = TXT.ToUTF8();

	std::wstring wPATH = PATH.ToStdWstring();

	const wchar_t* ww = "wPATH";

	std::wifstream FILE_STREAM(ww, std::ifstream::in | std::ifstream::binary);

	if(FILE_STREAM.is_open())
	{
		//FILE_STREAM << TXT_UTF8;
		FILE_STREAM.close();
		return true;
	}
	else
	{
		FILE_STREAM.close();
		return false;
	}
	*/

	std::ofstream FILE_STREAM;
	FILE_STREAM.open(PATH, std::ios::out | std::ios::binary);
	
	if (FILE_STREAM.fail())
	{
		FILE_STREAM.close();
		return false;
	}
	else
	{
		wxCharBuffer TXT_UTF8 = TXT.ToUTF8();
		
		FILE_STREAM << TXT_UTF8;
		FILE_STREAM.close();
		return true;
	}
}

// -------------------------------

void SET_NEW_FILE(wxString PATH)
{
	#ifndef ALIF_STUDIO_NO_STC

		// Clear Error Indicators
		if (CODE_ERROR_INDICATOR)
		{
			unsigned int TOTAL_LINES = (OBJ_CODE->GetLineCount()) - 1; // First line is 0

			int endPsn = 0;
			int startPsn = 0;
			int len = 0;

			for (unsigned int LINE_NUMBER = 0; LINE_NUMBER <= TOTAL_LINES; LINE_NUMBER++)
			{
				endPsn = OBJ_CODE->GetLineEndPosition(LINE_NUMBER);
				startPsn = OBJ_CODE->PositionFromLine(LINE_NUMBER);
				len = endPsn - startPsn;

				OBJ_CODE->IndicatorClearRange(startPsn, len);
			}

			OBJ_CODE->MarkerDeleteAll(1);

			CODE_ERROR_INDICATOR = false;
		}

	#endif

	if (PATH.IsEmpty())
	{
		// Need initializing
		// When Open Folder.

		PATH_FULL_SOURCE = "";
		SOURCE_FILE_NAME = "";
		PATH_FULL_EXECUTABLE = "";
		OBJ_CLASS_WINDOW_MAIN->SetLabel(wxT("ألف ستوديو [نسخة تجريبية]"));
		OBJ_CODE->DiscardEdits();

		return; // TODO: Remove this to test debuging tool in windows
	}

	// Set Full Path of Source Code File
	PATH_FULL_SOURCE = PATH;
	
	// Set File Name
	wxFileName fname( PATH );
	SOURCE_FILE_NAME = fname.GetName();

	// Initializ binary path
	PATH_FULL_EXECUTABLE = ""; // or tree_file_binpath[] to let user chose one time only !
	
	// Title
	OBJ_CLASS_WINDOW_MAIN->SetLabel(SOURCE_FILE_NAME + wxT(" - ألف ستوديو [نسخة تجريبية]"));

	OBJ_CODE->DiscardEdits();

	#ifdef ALIF_STUDIO_NO_STC
		CODE_MODIFIED = false;
	#else
		OBJ_CODE->SetSavePoint();
	#endif
}
