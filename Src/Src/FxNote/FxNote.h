/* __________________________________________________________________________
	
	FxEngine Framework. 
	Copyright (c) 2005, 2009 Sylvain Machel, SMProcess.

	This file is part of FxEngine Framework.
    The FxEngine Framework library is free software; you can redistribute it
	and/or modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library.
	if not,  If not, see <http://www.gnu.org/licenses/>.

   ____________________________________________http://www.SMProcess.com______ 
*//*!
   @file	 FxNote.h
   @brief This is the main interface file for the Fx plugin Note interface.

   IFxNote interface allows to a Fx plugin to send note/message with
   FxEngine application.
*//*_______________________________________________________________________*/
#pragma once
//! FxEngine Framework definitions
#include "FxDef.h"
#include "IFxNote.h"

#include "FxUtils/FxUtils.h"
#include "FxUtils/NotifyObjects.h"

#include "FxUtils/FxAPC.h"

namespace FEF {

//! FxEngine interfaces
class CFxNote;
class CFxLog;

typedef struct _FX_NOTE_OBSERVER {
	CFxNoteCallback*	pObserverClass; 
	FXNOTECALLBACK*     pObserverFnct;
	FX_PTR				dwParam;
} FX_NOTE_OBSERVER, *PFX_NOTE_OBSERVER;

typedef std::map<FX_HANDLE, FX_NOTE_OBSERVER*>  NoteObserverMap;
typedef NoteObserverMap::iterator   NoteObserverIter;

//! The IFxState class is the Fx plugin Note interface.
class CFxNote :	public IFxNote,
				public CFxAPCCallBack {

public:
	CFxNote(std::string strFxName, Uint32 dwFxEngineInstance, Uint32 dwFxInstance, CFxAPC* pFxAPC);
	virtual ~CFxNote();

public:

	/*----------------------------------------------------------------------*//*!
	AttachFxObserver().

	The AttachFxObserver method attaches a Fx state observer to the Fx.

    @param None.

	@return	FX_OK if success, otherwise an FX error code.
    *//*-----------------------------------------------------------------------*/
	virtual Int32 AttachFxObserver(/* [in] */CFxNoteCallback* pFxNoteCallback,
								/* [in] */FX_PTR dwParam,
								/* [in] */FX_HANDLE hObserverId);

	/*----------------------------------------------------------------------*//*!
	AttachFxObserverEx().

	The AttachFxObserver method attaches a Fx state observer to the Fx.

    @param None.

	@return	FX_OK if success, otherwise an FX error code.
    *//*-----------------------------------------------------------------------*/
	virtual Int32 AttachFxObserverEx( /* [in] */FXNOTECALLBACK* pFxNoteCallback,
								 /* [in] */FX_PTR dwParam,
								 /* [in] */FX_HANDLE hObserverId);

	/*----------------------------------------------------------------------*//*!
	DetachFxObserver().

	The DetachFxObserver method detaches a Fx note observer to the Fx.

    @param hObserverId:		Variable that contains the observer identifier.

	@return	FX_OK if success, otherwise an FX error code.
    *//*-----------------------------------------------------------------------*/
	virtual Int32 DetachFxObserver(/* [in] */FX_HANDLE hObserverId);

	/*----------------------------------------------------------------------*//*!
	FxPublishNote().

	The FxPublishNote method sends a Fx note to their observers.
    @param strNote:		Variable that contain the Fx Note to publish.

	@return	FX_OK if success, otherwise an FX error code.
    *//*-----------------------------------------------------------------------*/
	virtual Int32 FxPublishNote( /* [in] */const std::string strNote);

	/*----------------------------------------------------------------------*//*!
	SetFxHandle().

	The SetFxHandle method sets the Fx handle.
    @param hFx:		Variable that contain the Fx handle.

	@return	FX_OK if success, otherwise an FX error code.
    *//*-----------------------------------------------------------------------*/
	virtual Void SetFxHandle(/* [in] */FX_HANDLE hFx)  {_hFx = hFx; return;}

private:
	//! Critical Section
	boost::mutex _NoteCS;
    boost::mutex _NoteObserverCS;

private:
	FX_HANDLE           _hFx;

private:
	std::string _FxLogFile;
	CFxLog* _pLogTrace;

	// Instance value to manage the right log messages.
	Uint32			_dwThisInstance;		// current instance

private:
	NoteObserverMap _NoteObserverMap;

//! State callback
public:
	CFxAPC* _pFxAPC;
	virtual Int32 APCCallBack(Void* pObject);
};

} //namespace FEF

