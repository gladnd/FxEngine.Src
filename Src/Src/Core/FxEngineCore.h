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
#pragma once

#define FX_NAMESPACE

#include "IFxBase.h"

#include "FxParam/FxParam.h"
#include "FxPin/FxPin.h"
#include "FxPinManager/FxPinManager.h"
#include "FxRefClock/FxRefClock.h"
#include "FxState/FxState.h"
#include "FxNote/FxNote.h"
#include "FxConst2String/FxConst2String.h"
#include "FxUtils/FxUtils.h"

#include <vector>
#include <map>
#include <set>

namespace FEF {
#define FXENGINE_VERSION        "5.1.0.0"	//<! W: Framework interface, X: Framework code, Y: component interface, Z: component code
/* History */
/*	2.0.0.0 (01/05/2006)
	Add IFxPin::GetMediaTypeCount
	Add IFxPin::GetMediaType
--------------------------------------------
	2.1.0.0 (20/05/2006)
	Add FEF prefix for FxEngine API
	Add GetModuleHandle() to copy Fx to temp directory
--------------------------------------------
	2.2.0.0 (28/05/2006)
	Add FxPinManager::InitFxPins method to initialize pins
--------------------------------------------
	2.3.0.0 (04/06/2006)
	Remove CState in AddFx method if already exists
--------------------------------------------
	2.3.0.1 (26/06/2006)
	Add DisableThreadLibraryCalls() in DllMain to optimize
--------------------------------------------
	2.4.0.0 (12/08/2006)
	Replace Windows QueueUserAPC with BOOST thread library
	Add asynchronous trace library
	Remove the media pool allocation
--------------------------------------------
	2.5.0.0 (25/09/2006)
	Add FEF_ConnectPinEx with a specified media type to the connection
	Add the connection media type in Fxengine design files saved and loaded
--------------------------------------------
	2.6.0.0 (02/12/2006)
	Add XML library to export FxEngine
	Rename IFxPin::GetConnexionMediaType to IFxPin::GetConnectionMediaType
	Add IFxMedia::SetSize method
	Add IFxMedia::SetFxMediaName method
	Add IFxMedia::GetFxMediaName method
--------------------------------------------
	2.6.0.1 (17/12/2006)
	Add to lock(_PinCS) to CFxPin::Flush and CFxPin::WaitForIFxMedia methods
	Use GlobalFree and GlobalAlloc instead new and delete in FxMedia data
--------------------------------------------
	2.7.0.0 (01/01/2007)
	Fix pin's state connexion
--------------------------------------------
	2.7.0.1 (27/01/2007)
	Fix VC80 runtimes libraries
	Update XML library to manage long file name with space.
--------------------------------------------
	2.7.0.2 (16/04/2007)
	Fix FxStateLib memory leak in desturctor class.
    Fix FEF_GetFxEngineVersion (Run-Time Check Failure #2)
    Update FEF_GetFxInfo method (Add Fx scope)
    Fix "new boost::boost_thread" in FxAPC.cpp, add join in destructor
    move new CState in addfx, allocate when its a good fx only
--------------------------------------------
	2.7.0.3 (08/07/2007)
	Fix FxStateLib, add mutex in observer callback (allow to call FEF_XXX in callback).
    Add IFxBase::GetFxFrame method.
    Add IFxBase::UpdateFxParam method.
    Fix ClearVector method.
    Add export fx parameters.
    Fix complex fx parameters.
--------------------------------------------
	2.8.0.0 (25/07/2007)
    Add IFxParam::RemoveFxParam method.
    Add IFxPinManager::Remove method.
    Create media pool for each pin.
    Disconnect out pin before.
    Add default, min and max value in FX_PARAM structure
--------------------------------------------
	2.8.0.1 (16/08/2007)
    Unlock mutex before call pin callbacks (FxPin and FxWait)
--------------------------------------------
	3.0.0.0 (03/10/2007)
    Replace Char by std::string.
    Add const to CFxParam::GetFxParam
    Add FX_DESCRIPTOR to FEF_GetFxInfo
    Add const to FEF_GetFxInfo
    Linux intregration
--------------------------------------------
	3.1.0.0 (29/12/2007)
    Disable all pin callbacks before disconnect (DisConnectFxPin and DisconnectAllFxPin)
--------------------------------------------
	3.2.0.0 (22/01/2008)
	Add string parameter to IFxParam interface
    Fix RemoveFxParam method, memory leak
--------------------------------------------
	3.3.0.0 (05/03/2008)
    Stop pins (FX_STOP and stop callback) before disconnect them
    Remove pins in releaseFx method instead of disconnect calling (avoid double sending of PIN_NOT_CONNECTED message 
    Fixed IFxPin::WaitForIFxMedia method (release pin links when pin is removed)
--------------------------------------------
	3.4.0.0 (03/04/2008)
    Added COMPLEX_TYPE component in IFxPcmFormat::GetBitsPerSample (32bit)
    Added BGR video/image submedia type
--------------------------------------------
	3.5.0.0 (29/04/2008)
    Added _CSObserver critical section to allow all state callback FEF re-entrance functions
    Added CPin::WaitForEndDataInQueue including PeekMessage to avoid sendmessage windows hang
--------------------------------------------
	3.6.0.0 (15/06/2008)
	Call Quit infinite waiting time for all pin of both Fx in Stop/StopEngine/Remove/Pause/PauseFxEngine and DisConnectFxPin.
--------------------------------------------
	3.7.0.0 (18/07/2008)
	Added visual 2008 version: update Boost to 3.5.0 to this version.
	Added Autolock using Boost 3.5.0
	Updated FxTypes.h
	Unlock mutex before to call CFxPin::SendFxPinStateEx(PIN_NOT_CONNECTED) in DisconnectPin method.
	Updated exit threadAPC in FxTrace.cpp (decrease number of object and remove the null added to exit)
--------------------------------------------
	3.8.0.0 (12/08/2008)
	Fix delete temporary fx when error occurs in addFx method.
	Can transmit flush even if pin is stopped.
	Do not remove pins before IFxBase::Release method calling.
	Do not call pinstate callback during the IFxBase::Release calling.
	Fresh connected pin can deliver and receive FxMedia immediatly.
--------------------------------------------
	4.0.0.0 (10/09/2008)
	Added cplusplus namespace FEF
	Updated Bool in FxTypes.h
--------------------------------------------
	4.1.0.0 (10/10/2008)
	Fixed BGR Video sub type. 
	Added IFxMedia::Copy, enables to duplicate FxMedia object.
--------------------------------------------
	4.2.0.0 (23/01/2009)
	Added IFxPin::InitStream, initializes down stream.
	Updated IFxPinCallback::FxPin method. Added FX_STREAM_STATE.
	Updated GetMediaType method. Remove ** and put * instead.
	Added IFxState::FxGetState method.
	Added IFxState::FxRePublishState method.
	Added FX_STREAM_INIT_STATE, FX_TIMEOUT_STATE, FX_ERROR_SUBMEDIA_PIN_STATE
		  FX_ERROR_MEMORY_STATE.
--------------------------------------------
	4.3.0.0 (14/04/2009)
	Fixed IFxMedia::Release, Re-initialize all Media parameters
--------------------------------------------
	4.4.0.0 (20/06/2009)
	Open source version
--------------------------------------------
	4.5.0.0 (08/02/2010)
	Fixed log file names
	Removed intenpestives log (e.g CFxPin::GetPinState entry)
	Included <sstream> instead of the deprecated header <strstream.h>
--------------------------------------------
	4.6.0.0 (10/10/2010)
    Allow to connect more than one input pins to an output pin.
	Added IFxPin::GetConnectedPinNumber method.
	Added IFxPin::GetNextPinConnected method.
--------------------------------------------
	4.7.0.0 (29/11/2010)
    Fixed _OutPinCS critical section calls in CFxPin
--------------------------------------------
	4.8.0.0 (19/04/2011)
    Add the method GetFxRunningState to retrieve the Fx running state.
    Try to start/stop/pause all Fx, even if one of them is failed 
--------------------------------------------
	4.9.0.0 (22/02/2012)
	FEF_StopFxEngine
	On Win32, fixed hangups by removing WM_PAINT messages handling in ::CFxPin  
--------------------------------------------
	5.0.0.0 (07/07/2012)
	Added Fx note feature
	Set only one APC for FxNote and FxState  
	Added TOGGLE_XXX macro to protect IFxBase methods calls
	Updated FEF_AttachFxObserver and FEF_AttachFxObserverEx methods
	Fixed loopback feature(connection on the same FX) 
--------------------------------------------
	5.0.1.0 (06/09/2012)
	Fixed the Flush method.
*/



class FX_COMPONENT : public boost::mutex::scoped_lock
{
public:
	FX_COMPONENT():
    boost::mutex::scoped_lock(_CS, boost::defer_lock)
	{}
	
	~FX_COMPONENT()
	{}
public:
	FX_PTR			    hDLL;
	std::string			strFxPath;
	std::string			strFxPathOnDisk; //!< use to save the real fx name (when it loaded twice)
	Bool				IsTemporaryFx;
	std::string			strFxName;
	IFxBase*			pFxBase;
	IFx*				pIFx;
	FX_RUNNING_STATE    FxState; //!< Using also to set pin state on connect
	CFxPinManager*		pFxPinManager;
	CFxParam*			pFxParam;
	boost::mutex		_CS;
};

//typedef struct _FX_COMPONENT {
//	
//	boost::mutex		CS;
//	boost::mutex::scoped_lock *plock; 
//} FX_COMPONENT, *PFX_COMPONENT;

typedef std::vector<FX_COMPONENT*> CFxVector;
typedef CFxVector::iterator CFxVectorIter;

typedef std::map<Uint32, Uint64> FxRefClockMap;
typedef FxRefClockMap::iterator FxRefClockMapIter;

typedef struct _FX_OBSERVER {
	CFxStateCallback* pFxStateClass;
	CFxNoteCallback* pFxNoteClass;
	FXSTATECALLBACK* pFxStateFnct;
	FXNOTECALLBACK* pFxNoteFnct;
	FX_PTR dwParam;
} FX_OBSERVER, *PFX_OBSERVER;

typedef std::set<PFX_OBSERVER> FxObserverSet;

typedef std::set<std::string> FxHandlePtrSet;

//Define the function prototype
typedef IFxBase* (FEF_CALLBACK FxBaseFnct)(void);

class CFxEngineCore :	public CFxPinObserver,
						public CFxRefClockObserver,
						public CFxStateCallback,
						public CFxConst2String
{
public:
	CFxEngineCore();
	~CFxEngineCore(void);

public:
	Int32 AddFx(const std::string strFx, FX_HANDLE* phFx, Bool ShouldProtect = TRUE);

	Int32 AddFxEx(IFxBase* pIFxBase, FX_HANDLE* phFx);

	Int32 RemoveFx(FX_HANDLE hFx);

	Int32 GetFxCount(Uint16* pwFxCount, Bool ShouldProtect = TRUE);

	Int32 GetFx(FX_HANDLE* phFx, Uint16 wFxIndex, Bool ShouldProtect = TRUE);

	Int32 GetFxNumber(FX_HANDLE hFx, Uint16* pwFxNumber, Bool ShouldProtect/* = TRUE*/);

	Int32 StartFxEngine();

	Int32 PauseFxEngine();

	Int32 StopFxEngine();

	Int32 StopFx(FX_HANDLE hFx, Bool ShouldProtect = TRUE);

	Int32 StartFx(FX_HANDLE hFx, Bool ShouldProtect = TRUE);

	Int32 PauseFx(FX_HANDLE hFx, Bool ShouldProtect = TRUE);

	Int32 GetFxInfo(FX_HANDLE hFx, const FX_DESCRIPTOR** ppFxDescriptor);

    Int32 GetFxRunningState(FX_HANDLE hFx, FX_RUNNING_STATE* pFxRunningState, Bool ShouldProtect = TRUE);

	Int32 GetFxState(FX_HANDLE hFx, FX_STATE* pFxState, Bool ShouldProtect = TRUE);

	Int32 AttachFxObserver(CFxStateCallback* pFxStateCallback, CFxNoteCallback* pFxNoteCallback, FX_PTR dwParam, FX_HANDLE* phObserverId);

	Int32 AttachFxObserver(FX_COMPONENT* pFx, CFxStateCallback* pFxStateCallback, CFxNoteCallback* pFxNoteCallback, FX_PTR dwParam, FX_OBSERVER* phObserverId);

	Int32 AttachFxObserverEx(FXSTATECALLBACK* pFxStateCallback, FXNOTECALLBACK* pFxNoteCallback, FX_PTR dwParam, FX_HANDLE* phObserverId);

	Int32 AttachFxObserverEx(FX_COMPONENT* pFx, FXSTATECALLBACK* pFxStateCallback, FXNOTECALLBACK* pFxNoteCallback, FX_PTR dwParam, FX_OBSERVER* phObserverId);

	Int32 DetachFxObserver(FX_HANDLE hObserverId);

	Int32 AttachFxObserver(FX_COMPONENT* pFx);

	Int32 GetFxPinCount(FX_HANDLE hFx, Uint16* pwPinCount, Bool ShouldProtect = TRUE);

	Int32 QueryFxPinInterface(FX_HANDLE hFx, IFxPin** ppIFxPin, Uint16 wPinIndex);

    Int32 UpdateFxParam(FX_HANDLE hFx, const std::string strParamName, FX_PARAMETER FxParameter, Bool ShouldProtect = TRUE);

	Int32 QueryFxParamInterface(FX_HANDLE hFx, IFxParam** ppIFxParam);

	Int32 ConnectFxPin(IFxPin* pFxPinIn, IFxPin* pFxPinOut, Bool ShouldProtect = TRUE);

	Int32 ConnectFxPinEx(IFxPin* pFxPinIn, IFxPin* pFxPinOut, PFX_MEDIA_TYPE pMediaType, Bool ShouldProtect = TRUE);

	Int32 DisConnectFxPin(IFxPin* pFxPin, Bool ShouldStopCallBack = TRUE, Bool ShouldProtect = TRUE);

	Int32 DisconnectAllFxPin(Bool ShouldStopCallBack = TRUE, Bool ShouldProtect = TRUE);

	Int32 GetFxRefClock(Uint64* pqRefClock, Uint32 dwId );

	Int32 SetFxRefClock(Uint64 qRefClock, Uint32 dwId );

	Int32 GetFxEngineRefClock(Uint64* pqRefClock);

	Int32 SetFxEngineRefClock(Uint64 qRefClock);

	Int32 SetFxRefClockError(FX_PTR qUserParam);

	Int32 DisplayFxPropertyPage(FX_HANDLE hFx, Pvoid pvWndParent);

    Int32 GetFxFrame(FX_HANDLE hFx, const Char** ppbFxFrame);

	Int32 GetFxUserInterface(FX_HANDLE hFx, Pvoid* ppvUserInterface);

	Int32 GetFxSubFxEngine(FX_HANDLE hFx, FX_HANDLE* phFxEngine);

	Int32 SaveFxEngine(const std::string strFilePath);

	Int32 LoadFxEngine(const std::string strFilePath);

private:
	CFxVector _FxVector;
	Void ClearFxVector(AutoLock &lock);
	Bool IsFxLoaded(FX_HANDLE hFx);
	Bool IsFxLoaded(std::string& str);

private:
	Void SetFxState(FX_PTR dwFx, FX_STATE FxState);
	FxObserverSet   _FxObserverSet;

private:
	//! Critical Sections
    boost::mutex _CS;
    boost::mutex _CSObserver;
	boost::mutex _CSRefClock;

private:
	std::string _FxLogFile;
	CFxLog* _pLogTrace;

	// Instance value to manage the right log messages.
	Uint32 _dwThisInstance;				// current instance

public:
	static Uint32 _dwInstanceCount; // total instances
    static FxHandlePtrSet _SetFxHandlePtr;

private:
	Void FxPinUpdate(CFxPin* pPin, Int32 sdwHr, FX_STATE FxState, FX_PTR dwUserParam);

private:
    Int32 DuplicateFx(std::string& str);
    FX_PTR LoadFxLibrary(std::string& str);
    Void FreeFxLibrary(FX_PTR hFx);
    FxBaseFnct* GetFxProcAddress(FX_PTR hFx, std::string strFctName);

private:
	FxRefClockMap _FxRefClock;
	Uint64 _qFxEngineRefClock;

private:
	Int32 DisConnectFxInPin(IFxPin* pFxPin, Bool ShouldStopCallBack = TRUE, Bool ShouldProtect = TRUE);
	Int32 DisConnectFxOutPin(IFxPin* pFxPin, Bool ShouldStopCallBack = TRUE, Bool ShouldProtect = TRUE);

//private:
//	HANDLE			_hThread;
//	//! the event to be set for the thread to terminate
//	HANDLE			_hTerminateThread;
//	//! the thread function only wait in an alertable state
//	static DWORD CALLBACK Thread( void* dwhEvt)
//	{
//		while( WaitForSingleObjectEx( (HANDLE)dwhEvt, INFINITE, TRUE)!=WAIT_OBJECT_0);
//		return( 0);
//	}
//
//	/*! APC wrapper for message. These function just convert a static function
//		call to a class method call. */
//    static void CALLBACK QueueFcnt( FX_PTR  dwpParam)
//	{
//		CNotifFunction* pParam = (CNotifFunction*)dwpParam;
//		if(pParam->_dwFunctType == 1)
//			((FX_COMPONENT*)(pParam->_dwParam1))->pIFxRefClockCallBack->FxRefClock((Uint64)pParam->_dwParam2, (Uint32)pParam->_dwParam3);
//		else if(pParam->_dwFunctType == 2)
//			((FX_COMPONENT*)(pParam->_dwParam1))->pIFxRefClockCallBack->FxEngineRefClock((Uint64)pParam->_dwParam2);
//		return;
//	}

};
} //namespace FEF
