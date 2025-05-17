/* __________________________________________________________________________
	
	FxEngine Framework. 
	Copyright (c) 2005, 2009 Sylvain Machel, SMProcess.

	This file is part of FxEngine Editor.
    FxEngine Editor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FxEngine Editor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FxEngine Editor. If not, see <http://www.gnu.org/licenses/>.

   ____________________________________________http://www.SMProcess.com______ */
#ifndef __FXENGINEEDITORDOCH__
#define __FXENGINEEDITORDOCH__

#include "wx/docview.h"
#include "wx/cmdproc.h"

#include "Fx.h"
#include "FxLine.h"
#include "NodeManager.h"

typedef std::map<FX_SUB_MEDIA_TYPE, FX_MAIN_MEDIA_TYPE> MediaMap;


//! Fx Sate and Notes thread structures
struct FX_EVENT
{
    FX_HANDLE _hFx;
	std::string _strEvent;
	FX_STATE    _FxState;
        
    FX_EVENT(FX_HANDLE hFx, std::string strEvent = "", FX_STATE FxState = FX_UNDEFINED_STATE) {
        _hFx = hFx; _strEvent = strEvent; _FxState = FxState;}
};

class DrawingDocument : public wxDocument
{
  DECLARE_DYNAMIC_CLASS(DrawingDocument)

 public:
  FxMap			_FxMap;
  lFxLine		_FxLineList;

  DrawingDocument();
  ~DrawingDocument();

#if wxUSE_STD_IOSTREAM
  wxSTD ostream& SaveObject(wxSTD ostream& text_stream);
  wxSTD istream& LoadObject(wxSTD istream& text_stream);
#else
  wxOutputStream& SaveObject(wxOutputStream& stream);
  wxInputStream& LoadObject(wxInputStream& stream);
#endif

  inline FxMap&   GetFxMap() {return _FxMap;};
  inline lFxLine& GetFxLine() {return _FxLineList;};

  inline FX_HANDLE& GetFxEngineHandle(){return _hFxEngine;}

	Void FxProperty(FX_HANDLE _hLastSelectedFx, Pvoid hWnd);
	Void StopFxOnly(CFx* pFx);
	Void PauseFxOnly(CFx* pFx);
	Void StartFxOnly(CFx* pFx);
	Void GetFxState(FX_HANDLE hFx, std::string& strFxState);

	CFx* GetFx(FX_HANDLE);
	FX_HANDLE PointInFx(wxPoint& Point);
	CFxLine* PointInFxLine(wxPoint& Point);
    Void RemoveFxObject(FX_HANDLE);
	Void RemoveFxLineObject(CFxLine* pFxLine, Bool ShouldLock = TRUE);
    CNode* CreateFxLine(wxPoint& Point, CFxLine** ppFxLine = NULL);
	Bool EndFxLine(CNode* pNode);
    Void AttachLineToFx(CFxLine* pFxLine, IFxPin* pInIfxPin, IFxPin* pOutIfxPin);
	Void DetachLineToFx(CFxLine* pFxLine, IFxPin* pIfxPin = NULL);
	CNode* PointInNode(wxPoint& Point, CFxLine** ppFxLine = NULL);
    Bool Connect(CFxLine* pFxLine, FX_MEDIA_TYPE* pMediaType = NULL);
	Void Disconnect(CFxLine* pFxLine);
    //Void SetFxEngineState(std::string strEngineState) {_strEngineState = strEngineState;}
    std::string GetFxEngineState() {return _strEngineState;}

public:
    Bool ProcessState(Frame *pMainFrame, FX_HANDLE _hLastSelectedFx);
    Void PushFxStates(FX_EVENT FxStateEvent) {_FxReceivedStates.push_back(FxStateEvent); return;}
	Void PushFxNotes(FX_EVENT FxNoteEvent) {_FxReceivedNotes.push_back(FxNoteEvent); return;}
	std::string AddEventTime(std::string strEvent);
private:
    std::vector<FX_EVENT> _FxReceivedStates;
	std::vector<FX_EVENT> _FxReceivedNotes;
    std::string _strEngineState;
    
public:
    virtual bool OnCreate(const wxString& path, long flags);
    Void CloseDocument();

/*! FxEngine objects */
private:
	FX_HANDLE _hFxEngine;
	FX_HANDLE _hFxState;

/*! Node manager */
private:
	CNodeManager _NodeManager;
public:
	CNodeManager *GetNodeMng() {return &_NodeManager;}

public:
    //! Critical Section
	boost::mutex _CSDoc;

};

#define DOODLE_CUT          1
#define DOODLE_ADD          2

class DrawingCommand: public wxCommand
{
 protected:
  CFx *_pFx;
  CFxLine *_pFxMine;
  DrawingDocument *doc;
  int cmd;
 public:
  DrawingCommand(const wxString& name, int cmd, DrawingDocument *ddoc, CFx *pFx, CFxLine *pFxMine);
  ~DrawingCommand();

  bool Do(void);
  bool Undo(void);
};

#endif
