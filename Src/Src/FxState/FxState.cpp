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

   ____________________________________________http://www.SMProcess.com______ */
#include "FxTrace/FxTrace.h"
#include "FxDef.h"
#include "FxState.h"
#include "boost/lexical_cast.hpp"

namespace FEF {

#define FXSTATE_VERSION        "5.0.0.0"	//<! Y: compnent interface, Z: component code
/* History */
/*	2.0.0.0
	Use Boost thread library
	Add asynchronous trace library
*/
/*	2.0.0.0 (09/07/2007)
	Add CState::_StateObserverCS
*/
/*	3.0.0.0 (03/10/2007)
	Replace Char by std::string.
*/
/*	4.0.0.0 (03/01/2009)
	Added FxGetState method.
	Added FxPublishState.
*/
/*	5.0.0.0 (07/08/2012)
	Attached a CFxAPC object to the Fx
	Removed the CSate object
*/
// initialise the static instance count.
//Uint32 CFxState::_dwInstanceCount = 0;

CFxState::CFxState(const std::string strFxName, Uint32 dwFxEngineInstance, Uint32 dwFxInstance, CFxAPC* pFxAPC)
{
	_LastFxState = FX_UNDEFINED_STATE;

	_dwThisInstance = dwFxInstance;	//!< For message trace only

	_strFxName = strFxName;

	std::string strFxTrace = "";
#ifdef WIN32
	strFxTrace =  GetTemporaryDir() + "\\SMProcess.log\\FxState";
#else
	strFxTrace =  GetTemporaryDir() + "/SMProcess.log/FxState";
#endif

#ifdef WIN32
	if (_access(strFxTrace.c_str(), 0) == 0)
#else
    if (access(strFxTrace.c_str(), 0) == 0)
#endif
   
#ifdef WIN32
		_FxLogFile = strFxTrace + "\\" + _strFxName.c_str() + "_Log" + boost::lexical_cast<std::string>(dwFxEngineInstance) + boost::lexical_cast<std::string>(_dwThisInstance) + ".txt";
#else
		_FxLogFile = strFxTrace + "/" + _strFxName.c_str() + "_Log" + boost::lexical_cast<std::string>(dwFxEngineInstance) + boost::lexical_cast<std::string>(_dwThisInstance) + ".txt";
#endif
		_FxLogFile.erase( remove(_FxLogFile.begin(),_FxLogFile.end(),' ') , _FxLogFile.end() );
#ifdef WIN32
        if (_access(_FxLogFile.c_str(), 0) == 0) {
#else
        if (access(_FxLogFile.c_str(), 0) == 0) {
#endif
            std::string strOldLogFile;
#ifdef WIN32
			strOldLogFile = strFxTrace + "\\" + _strFxName.c_str() + "_Log" + boost::lexical_cast<std::string>(dwFxEngineInstance) + boost::lexical_cast<std::string>(_dwThisInstance) + ".old";
#else
			strOldLogFile = strFxTrace + "/" + _strFxName.c_str() + "_Log" + boost::lexical_cast<std::string>(dwFxEngineInstance) + boost::lexical_cast<std::string>(_dwThisInstance) + ".old";
#endif
			strOldLogFile.erase( remove(strOldLogFile.begin(),strOldLogFile.end(),' ') , strOldLogFile.end() );
            remove(strOldLogFile.c_str());
            rename(strOldLogFile.c_str(), strOldLogFile.c_str());
		}
	_pLogTrace = new CFxLog(_FxLogFile, LOG_IS_TRACE);

	_pLogTrace->FxTrace( "********************************************************************************");
	_pLogTrace->FxTrace( "              FxEngine:        FxState %s",FXSTATE_VERSION);
	_pLogTrace->FxTrace( "********************************************************************************");
	_pLogTrace->FxTrace( "");
    _pLogTrace->FxTrace( "====== FxState constructor (instance number is %d) ======", dwFxInstance);

	/*! Get the APC */
	_pFxAPC = NULL;
	_pFxAPC = pFxAPC;
}

CFxState::~CFxState()
{
	AutoLock lock(_StateCS);

    StateObserverIter Iter;
	for (Iter = _StateObserverMap.begin(); (Iter != _StateObserverMap.end()) && !(_StateObserverMap.empty()); Iter++)
		SAFE_DELETE_OBJECT(Iter->second);

    _StateObserverMap.clear();

	_pLogTrace->FxTrace( "===== FxState destructor (instance number is %d) =====", _dwThisInstance);
	
	SAFE_DELETE_OBJECT(_pLogTrace);
	lock.unlock();
}

Int32 CFxState::AttachFxObserver( /* [in] */CFxStateCallback* pFxStateCallback,
								/* [in] */FX_PTR dwParam,
								/* [in] */FX_HANDLE hObserverId)
{
	AutoLock lock(_StateCS);
    AutoLock lockObserver(_StateObserverCS);

	if(pFxStateCallback == NULL)
	{
		return FX_INVALPARAM;
	}

	FX_STATE_OBSERVER* pFxObserver = NULL;
	pFxObserver = new FX_STATE_OBSERVER;
	if(pFxObserver == NULL)
		return FX_NOMEM;

	pFxObserver->pObserverClass = pFxStateCallback;
	pFxObserver->pObserverFnct = NULL;
	pFxObserver->dwParam = dwParam;

	_StateObserverMap.insert(make_pair(hObserverId, pFxObserver));

	if(_LastFxState > FX_UNDEFINED_STATE)
	{
		CNotifyObjects* pParam = CNotifyObjects::Create( (FX_PTR)hObserverId, (FX_PTR)_LastFxState);
		if(_pFxAPC)
			_pFxAPC->AddAPCObject((CFxAPCCallBack*)this, pParam);
	}

	return FX_OK;
}

Int32 CFxState::AttachFxObserverEx(   /* [in] */FXSTATECALLBACK* pFxStateCallback,
									/* [in] */FX_PTR dwParam,
									/* [in] */FX_HANDLE hObserverId)
{
	AutoLock lock(_StateCS);
    AutoLock lockObserver(_StateObserverCS);

	if(pFxStateCallback == NULL)
	{
		return FX_INVALPARAM;
	}

	FX_STATE_OBSERVER* pFxObserver = NULL;
	pFxObserver = new FX_STATE_OBSERVER;
	if(pFxObserver == NULL)
		return FX_NOMEM;

	pFxObserver->pObserverClass = NULL;
	pFxObserver->pObserverFnct = pFxStateCallback;
	pFxObserver->dwParam = dwParam;

	_StateObserverMap.insert(make_pair(hObserverId, pFxObserver));

	if(_LastFxState > FX_UNDEFINED_STATE)
	{
		CNotifyObjects* pParam = CNotifyObjects::Create( (FX_PTR)hObserverId, (FX_PTR)_LastFxState);
		if(_pFxAPC)
			_pFxAPC->AddAPCObject((CFxAPCCallBack*)this, pParam);
	}

	return FX_OK;
}

Int32 CFxState::DetachFxObserver(/* [in] */FX_HANDLE hObserverId)
{
	AutoLock lock(_StateCS);
    AutoLock lockObserver(_StateObserverCS);

	if(hObserverId == NULL)
		return FX_INVALPARAM;
	
	StateObserverIter Iter;
	Iter = _StateObserverMap.find(hObserverId);
	if( Iter != _StateObserverMap.end() )
	{
		SAFE_DELETE_OBJECT(Iter->second);
		_StateObserverMap.erase(Iter);
	}

	return FX_OK;
}

Int32 CFxState::FxPublishState( /* [in] */FX_STATE FxState)
{
	AutoLock lock(_StateCS); //!< different than lockObserver to allow FEF_XXX Call in callback state

	_LastFxState = FxState;

	StateObserverIter Iter;
	for (Iter = _StateObserverMap.begin(); (Iter != _StateObserverMap.end()) && !(_StateObserverMap.empty()); Iter++)
	{
		CNotifyObjects* pParam = CNotifyObjects::Create( (FX_PTR)(Iter->first), (FX_PTR)FxState);
		if(_pFxAPC)
			_pFxAPC->AddAPCObject((CFxAPCCallBack*)this, pParam);
	}

	return FX_OK;
}

Int32 CFxState::FxGetState( /* [out] */FX_STATE *pFxState) 
{
	AutoLock lock(_StateCS);
	if(pFxState == NULL)
		return FX_INVALPARAM;

	*pFxState = _LastFxState;

	return FX_OK;
}

std::string CFxState::GetFxStringState(/* [in] */FX_STATE FxState)
{
	return GetConstToString(FX_STATE_CONST, FxState);
}

Int32 CFxState::APCCallBack(Void* pObject)
{
	Int32 hr = FX_OK;

	CNotifyObjects* pStateObject = (CNotifyObjects*)pObject;
	FX_HANDLE hObserverId = (FX_HANDLE)(pStateObject->_qParam1);

	AutoLock lock(_StateObserverCS);

	StateObserverIter Iter;
	/*! Object still exists ? */
	Iter = _StateObserverMap.find(hObserverId);
	if(Iter != _StateObserverMap.end())
	{
		if(Iter->second->pObserverClass)
			Iter->second->pObserverClass->FxStateCallback((FX_STATE)(pStateObject->_qParam2), _hFx, (Iter->first), Iter->second->dwParam);
		if(Iter->second->pObserverFnct)
			Iter->second->pObserverFnct((FX_STATE)(pStateObject->_qParam2), _hFx, (Iter->first), Iter->second->dwParam);
	}
	pStateObject->Release();

	return hr;
}

 } //namespace FEF
