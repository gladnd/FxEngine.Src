/* __________________________________________________________________________
	
	FxEngine Framework. 
	Copyright (c) 2005, 2012 Sylvain Machel, SMProcess.

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
#include "FxNote.h"
#include "boost/lexical_cast.hpp"

namespace FEF {

#define FXNOTE_VERSION        "1.0.0.0"	//<! Y: compnent interface, Z: component code
/* History */
/*	1.0.0.0
	Creation
*/

// initialise the static instance count.
//Uint32 CFxNote::_dwInstanceCount = 0;

CFxNote::CFxNote(std::string strFxName, Uint32 dwFxEngineInstance, Uint32 dwFxInstance, CFxAPC* pFxAPC)
{
	_dwThisInstance = dwFxInstance;	//!< For message trace only
	
	std::string strFxTrace = "";
#ifdef WIN32
	strFxTrace =  GetTemporaryDir() + "\\SMProcess.log\\FxNote";
#else
	strFxTrace =  GetTemporaryDir() + "/SMProcess.log/FxNote";
#endif

#ifdef WIN32
	if (_access(strFxTrace.c_str(), 0) == 0)
#else
    if (access(strFxTrace.c_str(), 0) == 0)
#endif
   
#ifdef WIN32
		_FxLogFile = strFxTrace + "\\" + strFxName.c_str() + "_Log" + boost::lexical_cast<std::string>(dwFxEngineInstance) + boost::lexical_cast<std::string>(_dwThisInstance) + ".txt";
#else
		_FxLogFile = strFxTrace + "/" + strFxName.c_str() + "_Log" + boost::lexical_cast<std::string>(dwFxEngineInstance) + boost::lexical_cast<std::string>(_dwThisInstance) + ".txt";
#endif
		_FxLogFile.erase( remove(_FxLogFile.begin(),_FxLogFile.end(),' ') , _FxLogFile.end() );
#ifdef WIN32
        if (_access(_FxLogFile.c_str(), 0) == 0) {
#else
        if (access(_FxLogFile.c_str(), 0) == 0) {
#endif
            std::string strOldLogFile;
#ifdef WIN32
			strOldLogFile = strFxTrace + "\\" + strFxName.c_str() + "_Log" + boost::lexical_cast<std::string>(dwFxEngineInstance) + boost::lexical_cast<std::string>(_dwThisInstance) + ".old";
#else
			strOldLogFile = strFxTrace + "/" + strFxName.c_str() + "_Log" + boost::lexical_cast<std::string>(dwFxEngineInstance) + boost::lexical_cast<std::string>(_dwThisInstance) + ".old";
#endif
			strOldLogFile.erase( remove(strOldLogFile.begin(),strOldLogFile.end(),' ') , strOldLogFile.end() );
            remove(strOldLogFile.c_str());
            rename(strOldLogFile.c_str(), strOldLogFile.c_str());
		}
	_pLogTrace = new CFxLog(_FxLogFile, LOG_IS_TRACE);

	_pLogTrace->FxTrace( "********************************************************************************");
	_pLogTrace->FxTrace( "              FxEngine:        FxNote %s",FXNOTE_VERSION);
	_pLogTrace->FxTrace( "********************************************************************************");
	_pLogTrace->FxTrace( "");
    _pLogTrace->FxTrace( "====== FxNote constructor (instance number is %d) ======", dwFxInstance);

	/*! Launch the Fx thread */
	_pFxAPC = NULL;
	_pFxAPC = pFxAPC;
}

CFxNote::~CFxNote()
{
	AutoLock lock(_NoteCS);

    NoteObserverIter Iter;
	for (Iter = _NoteObserverMap.begin(); (Iter != _NoteObserverMap.end()) && !(_NoteObserverMap.empty()); Iter++)
		SAFE_DELETE_OBJECT(Iter->second);

    _NoteObserverMap.clear();
	
	_pLogTrace->FxTrace( "===== FxNote destructor (instance number is %d) =====", _dwThisInstance);
	
	SAFE_DELETE_OBJECT(_pLogTrace);
	lock.unlock();
}

Int32 CFxNote::AttachFxObserver(/* [in] */CFxNoteCallback* pFxNoteCallback,
								/* [in] */FX_PTR dwParam,
								/* [in] */FX_HANDLE hObserverId)
{
	AutoLock lock(_NoteCS);
    AutoLock lockObserver(_NoteObserverCS);

	if(pFxNoteCallback == NULL)
		return FX_INVALPARAM;
	
	FX_NOTE_OBSERVER* pFxObserver = NULL;
	pFxObserver = new FX_NOTE_OBSERVER;
	if(pFxObserver == NULL)
		return FX_NOMEM;

	pFxObserver->pObserverClass = pFxNoteCallback;
	pFxObserver->pObserverFnct = NULL;
	pFxObserver->dwParam = dwParam;

	_NoteObserverMap.insert(make_pair(hObserverId, pFxObserver));

	return FX_OK;
}

Int32 CFxNote::AttachFxObserverEx( /* [in] */FXNOTECALLBACK* pFxNoteCallback,
								 /* [in] */FX_PTR dwParam,
								 /* [in] */FX_HANDLE hObserverId)
{
	AutoLock lock(_NoteCS);
    AutoLock lockObserver(_NoteObserverCS);

	if(pFxNoteCallback == NULL)
		return FX_INVALPARAM;
	
	FX_NOTE_OBSERVER* pFxObserver = NULL;
	pFxObserver = new FX_NOTE_OBSERVER;
	if(pFxObserver == NULL)
		return FX_NOMEM;

	pFxObserver->pObserverClass = NULL;
	pFxObserver->pObserverFnct = pFxNoteCallback;
	pFxObserver->dwParam = dwParam;

	_NoteObserverMap.insert(make_pair(hObserverId, pFxObserver));

	return FX_OK;
}

Int32 CFxNote::DetachFxObserver(/* [in] */FX_HANDLE hObserverId)
{
	AutoLock lock(_NoteCS);
    AutoLock lockObserver(_NoteObserverCS);

	if(hObserverId == NULL)
		return FX_INVALPARAM;

	NoteObserverIter Iter;
	Iter = _NoteObserverMap.find(hObserverId);
	if( Iter != _NoteObserverMap.end() )
	{
		SAFE_DELETE_OBJECT(Iter->second);
		_NoteObserverMap.erase(Iter);
	}

	return FX_OK;
}

Int32 CFxNote::FxPublishNote( /* [in] */const std::string strNote)
{
	AutoLock lock(_NoteCS); //!< different than lockObserver to allow FEF_XXX Call in callback state

	NoteObserverIter Iter;
	for (Iter = _NoteObserverMap.begin(); (Iter != _NoteObserverMap.end()) && !(_NoteObserverMap.empty()); Iter++)
	{
		std::string *pstrNote = new std::string(strNote.c_str());
		CNotifyObjects* pParam = CNotifyObjects::Create( (FX_PTR)(Iter->first), (FX_PTR)pstrNote);
		if(_pFxAPC)
			_pFxAPC->AddAPCObject((CFxAPCCallBack*)this, pParam);
	}

	return FX_OK;
}

Int32 CFxNote::APCCallBack(Void* pObject)
{
	Int32 hr = FX_OK;

	CNotifyObjects* pNoteObject = (CNotifyObjects*)pObject;
	FX_HANDLE hObserverId = (FX_HANDLE)(pNoteObject->_qParam1);

	AutoLock lock(_NoteObserverCS);
	std::string *pstrNote = (std::string*)(pNoteObject->_qParam2);
	std::string strNote(*pstrNote);

	NoteObserverIter Iter;
	/*! Object still exists ? */
	Iter = _NoteObserverMap.find(hObserverId);
	if(Iter != _NoteObserverMap.end())
	{
		if(Iter->second->pObserverClass)
			Iter->second->pObserverClass->FxNoteCallback(strNote, _hFx, (Iter->first), Iter->second->dwParam);
		if(Iter->second->pObserverFnct)
			Iter->second->pObserverFnct(strNote, _hFx, (Iter->first), Iter->second->dwParam);
	}
	SAFE_DELETE_OBJECT(pstrNote);
	pNoteObject->Release();

	return hr;
}

 } //namespace FEF
