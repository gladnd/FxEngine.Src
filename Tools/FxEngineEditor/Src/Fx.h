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
#ifndef _FX_H
#define _FX_H

#include "wx/dcbuffer.h"
#include "EditorDefs.h"
#include "FxEngine.h" 
using namespace FEF; //!< Using FxEngineFramework

#include "FxColor.h"
#include "DrawObject.h"
#include "NodeManager.h"

class CFx;
class CNode;
class CFxLine;

typedef std::list<CNode*>    ListOfNode;
typedef ListOfNode::iterator	  ListOfNodeIter;

typedef std::list<CFxLine*>    lFxLine;
typedef lFxLine::iterator	  lFxLineIter;

typedef std::list<IFxPin*>    lFx;
typedef lFx::iterator	      lFxIter;

typedef std::map<FX_HANDLE, CFx*> FxMap;
typedef std::vector<CDrawObject*>    vSelectedObject;

typedef std::list<std::string>    lFxEvent;
typedef lFxEvent::iterator	      lFxEventIter;


typedef std::map<IFxPin*, FX_PIN_TYPE> PinTypeMap;
typedef std::map<IFxPin*, std::string> PinNameMap;
typedef std::vector<IFxPin*> PinVector;

class CFx : public CFxColor, 
            public CDrawObject
{
public:
	CFx(CNodeManager *pNodeManager, FX_HANDLE hFxEngine, FX_HANDLE hFx, std::string strPath, Uint32 dwDefaultX = X_ORIGINE, Uint32 dwDefaultY = X_ORIGINE, std::string strFxName = "");
	//CFx(const CFx& Fx);
	virtual ~CFx();

private:
	FX_HANDLE _hFxEngine;
	FX_HANDLE _hFx;

    lFxEvent _strStatesList;
	lFxEvent _strNotesList;
    FX_RUNNING_STATE  _FxRunningState; //!< Start / Stop / Pause

    wxImage* _FxFrame;
    int _FrameHeight;
    int _FrameWidth;

    std::string	_strFxName;
	std::string	_strVersion;
	std::string _strFxPath;
	FX_TYPE _FxType;
    FX_SCOPE _FxScope;

	int  _wNameH;
    int  _wPinH;
    int  _wNameW;
    int  _wPinW;
    Uint16  _wPinCount;

private:
	ListOfNode	_NodeList;
    PinTypeMap  _PinTypeMap;
    PinNameMap  _PinNameMap;
    PinVector   _PinVector;
    Uint16      _wOutPinNb;
    Uint16      _wInPinNb;
	wxPoint*	_pFxUpleftPoint;
	wxSize*		_pFxSize;
	wxRect*		_pFxRect;

    wxPoint*	_pFxBitmapUpleftPoint;
	
    Bool        _IsSizesInit;

	wxColour	_rgbHandleColor;

	Bool		_ShouldDrawMarker;

	lFxLine		_FxLineList;

    wxString    _strFxInfos;
    wxString    _strFxInputPins;
    wxString    _strFxOutputPins;
    wxString    _strFxParameters;

    //! Critical Section
	boost::mutex _CSDraw;

/*! Node manager */
private:
	CNodeManager * _pNodeManager;

public:
	virtual Void Draw(wxDC* pDC);
	virtual Void DrawMarker(wxDC* pDC);
	Bool IsPointInFx(wxPoint& Point);
    virtual Bool IsObjectInRect(wxRect& Rect);
	virtual Void OffSetObject(Int32 dwX, Int32 dwY);
    virtual Void SetMarker(Bool ShouldDraw = FALSE);
	virtual wxRect GetRect();
	Bool AttachFxLine(IFxPin* pIFxPin, CFxLine* pFxLine);
	Bool DetachFxLine(CFxLine* pFxLine, IFxPin* pIFxPin = NULL);
	virtual Void EnableSetPath(Bool EnableSetPath);
	Bool UpdateFxPin();
    Void UpdateFxFrame();
    Void SetFxInPinText();
    Void SetFxOutPinText();
    Void InitFxPin();
	Void UpdateTxRx();
    Void AddState(std::string strFxState);
	Void AddNote(std::string strFxNote);
    std::list<std::string>& GetFxStateMsg() {return _strStatesList;}
	std::list<std::string>& GetFxNoteMsg() {return _strNotesList;}

	FX_HANDLE GethFxHandle() const   { return (_hFx); }

    Void SetFxName(std::string strFxName);
    const std::string GetFxName() const { return _strFxName; }

	const std::string GetstrFxPath() const   { return _strFxPath; }

    Uint32 GetFxPosX() const   { return ((_pFxRect->GetLeftTop())).x; }
	Uint32 GetFxPosY() const   { return ((_pFxRect->GetLeftTop())).y; }

    lFxLine& GetFxLines() { return _FxLineList; }

    wxString GetFxInfosText() {return _strFxInfos;}
    wxString GetFxInputPinText() {return _strFxInputPins;}
    wxString GetFxOutPinText() {return _strFxOutputPins;}
    wxString GetFxParamText() {return _strFxParameters;}

    //Void SetFxRunningState(FX_STATE FxRunningState) {_FxRunningState = FxRunningState;}

private:
	CNode* IsPinExist(IFxPin* pIFxPin);
    Void SetFxInfoText();
    Void SetFxParamText();
};
#endif
