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
   @file	 FxState.h
   @brief This is the main interface file for the Fx plugin State interface.

   IFxState interface allows to a Fx plugin to mail message with
   FxEngine application.
*//*_______________________________________________________________________*/
#pragma once
//! FxEngine Framework definitions
#include "FxDef.h"
#include "IFxState.h"

#include "FxUtils/FxUtils.h"
#include "FxUtils/NotifyObjects.h"

#include "FxUtils/FxAPC.h"
#include "FxConst2String/FxConst2String.h"

namespace FEF {

//! FxEngine interfaces
class CFxState;
class CFxLog;

typedef struct _FX_STATE_OBSERVER {
	CFxStateCallback*	 pObserverClass; 
	FXSTATECALLBACK*     pObserverFnct;
	FX_PTR				dwParam;
} FX_STATE_OBSERVER, *PFX_STATE_OBSERVER;

typedef std::map<FX_HANDLE, FX_STATE_OBSERVER*>  StateObserverMap;
typedef StateObserverMap::iterator   StateObserverIter;

//! The IFxState class is the Fx plugin State interface.
class CFxState :	public IFxState,
					public CFxAPCCallBack,
					public CFxConst2String {

public:
	CFxState(const std::string strFxName, Uint32 dwFxEngineInstance, Uint32 dwFxInstance, CFxAPC* pFxAPC);
	virtual ~CFxState();

public:

	/*----------------------------------------------------------------------*//*!
	AttachFxObserver().

	The AttachFxObserver method attaches a Fx state observer to the Fx.

    @param None.

	@return	FX_OK if success, otherwise an FX error code.
    *//*-----------------------------------------------------------------------*/
	virtual Int32 AttachFxObserver(/* [in] */CFxStateCallback* pFxNoteCallback,
								/* [in] */FX_PTR dwParam,
								/* [in] */FX_HANDLE hObserverId);

	/*----------------------------------------------------------------------*//*!
	AttachFxObserverEx().

	The AttachFxObserver method attaches a Fx state observer to the Fx.

    @param None.

	@return	FX_OK if success, otherwise an FX error code.
    *//*-----------------------------------------------------------------------*/
	virtual Int32 AttachFxObserverEx( /* [in] */FXSTATECALLBACK* pFxNoteCallback,
								 /* [in] */FX_PTR dwParam,
								 /* [in] */FX_HANDLE hObserverId);

	/*----------------------------------------------------------------------*//*!
	DetachFxObserver().

	The DetachFxObserver method detaches a Fx state observer to the Fx.

    @param hObserverId:		Variable that contains the observer identifier.
	
	@return	FX_OK if success, otherwise an FX error code.
    *//*-----------------------------------------------------------------------*/
	virtual Int32 DetachFxObserver(/* [in] */FX_HANDLE hObserverId);

	/*----------------------------------------------------------------------*//*!
	FxPublishState().

	The FxPublishState method sends a Fx state to its observer.
    @param FxState:		Variable that contain the Fx State to publish.

	@return	FX_OK if success, otherwise an FX error code.
    *//*-----------------------------------------------------------------------*/
	virtual Int32 FxPublishState( /* [in] */FX_STATE FxState);

	/*----------------------------------------------------------------------*//*!
	SetFxHandle().

	The SetFxHandle method sets the Fx handle.
    @param hFx:		Variable that contain the Fx handle.

	@return	FX_OK if success, otherwise an FX error code.
    *//*-----------------------------------------------------------------------*/
	virtual Void SetFxHandle(/* [in] */FX_HANDLE hFx)  {_hFx = hFx; return;}

	virtual Int32 FxGetState( /* [out] */FX_STATE *pFxState);

	virtual Int32 FxRePublishState() {return FxPublishState(_LastFxState);}

	std::string GetFxStringState(/* [in] */FX_STATE FxState);

private:
	//! Critical Sections
	boost::mutex _StateCS;
    boost::mutex _StateObserverCS;

private:
	StateObserverMap _StateObserverMap;

private:
	FX_STATE			_LastFxState;

private:
	FX_HANDLE           _hFx;

private:
	Bool			IsObserverExist(FX_STATE_OBSERVER* pStateObsvr);

public:
	Uint32 GetStateCount() const   { return (_pFxAPC->GetObjectNumberInQueue()); }

private:
	std::string			_strFxName;

private:
	std::string _FxLogFile;
	CFxLog* _pLogTrace;

	// Instance value to manage the right log messages.
	Uint32			_dwThisInstance;		// current instance

//! State callback
public:
	CFxAPC* _pFxAPC;
	virtual Int32 APCCallBack(Void* pObject);
};

} //namespace FEF

