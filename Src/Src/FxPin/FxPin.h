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
#pragma once
#include "FxDef.h"
#include "IFxPin.h"
#include "IFxPinManager.h"
#include "IFxPinCallback.h"
#include "FxMediaPool/FxMediaPool.h"

#include "FxUtils/FxUtils.h"
#include "FxUtils/NotifyObjects.h"

#include "FxTrace/FxTrace.h"
#include "FxConst2String/FxConst2String.h"

#include "FxUtils/FxAPC.h"

#include <list>

#include <boost/timer.hpp>

using namespace std ;

namespace FEF {

//! Dump states
typedef enum _DUMP_STATE{
	DUMP_OFF = 0,
	DUMP_INIT,
	DUMP_STOP,
	DUMP_START
}DUMP_STATE;

class CFxPin;
class CFxAPC;

class CFxPinObserver {
public:
	virtual ~ CFxPinObserver(){};
	virtual Void FxPinUpdate(CFxPin* pPin, Int32 sdwHr, FX_STATE FxState, FX_PTR qUserParam ) PURE;
protected:
	CFxPinObserver(void){};
};

class CPinState : public CFxAPCCallBack {
public:
	CPinState(void);
	virtual ~CPinState();
public:
	CFxAPC* _pAPC;
	virtual Int32 APCCallBack(void* pObject);

	//! Critical Section
	boost::mutex _PinStateCS;
};

typedef struct _FX_PIN_OBSERVER {
	CFxPinObserver*	pObserver;
	FX_PTR			qUserParam;
} FX_PIN_OBSERVER, *PFX_PIN_OBSERVER;

typedef std::list<FX_PIN_OBSERVER*>  CFxPinObserverL;
typedef CFxPinObserverL::iterator   CFxPinObserverIter;

typedef std::list<CFxPin*>  CFxPinL;
typedef CFxPinL::iterator   CFxPinIter;

typedef std::pair< pair<boost::mutex*, boost::mutex*>, boost::mutex*> CSPair;

class CFxPin : public IFxPin,
			   public CFxConst2String,
			   public CFxAPCCallBack	{

public:
	CFxPin(PFX_PIN pPinInfo, std::string& strFxName, Uint32 dwFxEngineInstance, Uint32 dwFxInstance, Uint32 dwFxPinInstance, Int32& hr);

public:
	virtual ~CFxPin();

public:
	PFX_PIN _pPinInfo;

protected:
	Bool _IsPinConnected;
	Bool _IsConnectionMediaTypeSet;

public:
	virtual Void SendFxPinState(FX_PIN_STATE PinState);
    virtual Void SendFxPinStateEx(FX_PIN_STATE PinState);

protected:
	FX_MEDIA_TYPE _ConnectionMediaType;

protected:
	CFxMediaPool* _pFxMediaPool;
	Uint32	_dwLastTimeOut;	//<! Latest time out asked
	
protected:
	std::string _strFxName;

protected:
	std::string _FxLogFile;
	CFxLog* _pLogTrace;
	CFxLog* _pLogDump;

	Uint32 _dwFxEngineInstance;			// current FxEngine instance
	Uint32 _dwFxInstance;				// current Fx instance

	// Instance value to manage the right log messages.
	Uint32 _dwThisInstance;				// current instance

protected:
    std::string _strDumpFilePath;
	DUMP_STATE	_DumpState;

//! Interface IFxPin
public:
	virtual Int32 GetPinName(std::string& strPinName);				//!< Get the Pin Name.
	virtual Int32 GetPinType(FX_PIN_TYPE* pPinType);				//!< Get Pin type.

protected:
	//! Critical Section
	boost::mutex _PinCS;

	CSPair*			_pPinCallbackCS;
public:
	virtual Void	SetPinCallBackCS(CSPair* pPinCS);

protected:
	Uint32 _dwFxProcessTime;
	boost::timer _tElapse;

public:
	virtual Int32 ConnectPin(IFxPin*	 pFxPin = NULL)   PURE;					//!< Connect Pin.
	virtual Int32 DisconnectPin(IFxPin*	 pFxPin = NULL)   PURE;					//!< DisConnect Pin.
	virtual Int32 GetPinState(FX_PIN_STATE* pPinState);				//!< Current Pin Sate.
	virtual Int32 GetPinConnected(IFxPin** ppFxPin) PURE;
    virtual Int32 GetNextPinConnected(IFxPin** ppFxPin) PURE;

	virtual Int32 CheckMediaType(PFX_MEDIA_TYPE pMediaType);	//!< Check Pin media.
	virtual Int32 SetConnectionMediaType(PFX_MEDIA_TYPE pMediaType);	//!< Set Pin media of the connection.
	virtual Int32 GetMediaTypeCount(Uint16* pwMediaTypeCount);			//!< Return the number of mediatype for this Pin.
	virtual Int32 GetMediaType(PFX_MEDIA_TYPE pMediaType, Uint16 wMediaTypeIndex); //!< Get media types
	virtual Int32 GetConnectionMediaType(PFX_MEDIA_TYPE pMediaType);	//!< Get Pin media of the connecxion.

	virtual Int32 GetFreeMediaNumber(Uint32* pdwFreeMediaNumber);
	virtual Int32 DeliverMedia(IFxMedia* pIFxMedia)   PURE;					//!< Add buffer to the next input pin queue.
	virtual Int32 GetDeliveryMedia(IFxMedia** ppIFxMedia, Uint32 dwTimeOut = 0); //!< Retrieve free FxMedia to use

	virtual Int32 GetProcessTime(Uint32* pdwProcessingTime);

	virtual Int32 Flush()  PURE;
    virtual Int32 InitStream(IFxMedia* pIFxMedia)  PURE;

    virtual Int32 InitDumpData(const std::string strFilePath);
	virtual Int32 StartDumpData();
	virtual Int32 StopDumptData();

	virtual Int32 GetPinRunState(FX_RUNNING_STATE* pPinRunState);
	virtual Int32 SetPinRunState(FX_RUNNING_STATE PinRunState, Bool ShouldProtect = TRUE);

    virtual Int32 UpdateMediaPool(Uint32 dwSize, Uint32 dwNumber);

	virtual Void LinkFxPin(CFxPin* pFxPin, FX_PIN_TYPE PinType);
    virtual Void UnLinkFxPin(CFxPin* pFxPin);

	virtual Void SetFxMember(FX_HANDLE hFx) { _hFxMember = hFx; }
	virtual FX_HANDLE GetFxMember() const   { return (_hFxMember); }

    virtual Int32 WaitForIFxMedia(Uint32 dwTimeStamp, FX_PTR dwUser)  PURE;

	virtual Void QuitInfiniteTime();
	virtual Void WaitForEndDataInQueue();

	virtual Int32 GetTxRxBytes(/* [out] */Uint64* pqTxRxByte) {*pqTxRxByte = _pqTxRxByte; return FX_OK;}
	virtual Int32 GetConnectedPinNumber(Uint32 *pdwPinNumber) PURE;

public:
	virtual Int32 AttachObservers(CFxPinObserver* pFxPinObservers, FX_PTR qUserParam);
	virtual Int32 DetachObservers(CFxPinObserver* pFxPinObservers);
	virtual Void NotifyObservers(Int32 sdwHr, FX_STATE FxState);

protected:
	CFxPinObserverL _FxPinObservers;

protected:
	FX_HANDLE _hFxMember;

public:
	CFxPinL		_lpFxPinIn;
	CFxPinL		_lpFxPinOut;

public:
	Bool		_ShoudStopCallBack;

protected:
	FX_RUNNING_STATE	_PinRunState;

protected:
	Uint64  _pqTxRxByte;

//! Pin callback
public:
	CFxAPC* _pAPC;
	virtual Int32 APCCallBack(void* pObject) PURE;

//! Pin State callback
	friend class CPinState;
	CPinState _PinState;
};

class CFxInPin : public CFxPin {
public:
	CFxInPin(PFX_PIN pPinInfo, std::string& strFxName, Uint32 dwFxEngineInstance, Uint32 dwFxInstance, Uint32 dwFxPinInstance, Int32& hr);
public:
    virtual ~CFxInPin(){}
public:
    virtual Int32 DeliverMedia(IFxMedia* pIFxMedia);					//!< Add buffer to the next input pin queue.
	virtual Int32 Flush();											//!< Flush next pins
    virtual Int32 InitStream(IFxMedia* pIFxMedia);                  //!< Initialize stream
    virtual Int32 ConnectPin(IFxPin*	 pFxPin);					//!< Connect Pin.
	virtual Int32 DisconnectPin(IFxPin*	 pFxPin);					//!< DisConnect Pin.
    virtual Int32 GetPinConnected(IFxPin** ppFxPin);
	virtual Int32 GetNextPinConnected(IFxPin** ppFxPin) {*ppFxPin = NULL; return FX_OK;} //!< No more one pin connected here
    virtual Int32 WaitForIFxMedia(Uint32 dwTimeStamp, FX_PTR dwUser);
    virtual Int32 APCCallBack(void* pObject);
	virtual Int32 GetConnectedPinNumber(Uint32 *pdwPinNumber) {AutoLock lock(_PinCS); *pdwPinNumber = 1; return FX_OK;}
private:
    IFxPin*	 _pConnectedPin;	//!< The attached Pin
};

class CFxOutPin : public CFxPin {
public:
	CFxOutPin(PFX_PIN pPinInfo, std::string& strFxName, Uint32 dwFxEngineInstance, Uint32 dwFxInstance, Uint32 dwFxPinInstance, Int32& hr);
public:
	virtual ~CFxOutPin(){}
public:
    virtual Int32 DeliverMedia(IFxMedia* pIFxMedia);				//!< Add buffer to the next input pin queue
	virtual Int32 Flush();											//!< Flush next pins
    virtual Int32 InitStream(IFxMedia* pIFxMedia);                  //!< Initialize stream
    virtual Int32 ConnectPin(IFxPin*	 pFxPin);					//!< Connect Pin
	virtual Int32 DisconnectPin(IFxPin*	 pFxPin);					//!< DisConnect Pin
    virtual Int32 GetPinConnected(IFxPin** ppFxPin);
	virtual Int32 GetNextPinConnected(IFxPin** ppFxPin);			//!< Retrieve the next connected pin
    virtual Int32 WaitForIFxMedia(Uint32 dwTimeStamp, FX_PTR dwUser);
    virtual Int32 APCCallBack(void* pObject);
	virtual Int32 GetConnectedPinNumber(Uint32 *pdwPinNumber) {AutoLock lock(_PinCS); *pdwPinNumber = _ConnectedPinList.size(); return FX_OK;}
private:
    CFxPinL	 _ConnectedPinList;	//!< The attached Pin list
	CFxPinIter _Iter;           //!< Keep the list iterator
    
	boost::mutex _OutPinCS;     //!< Critical Section to protect connected list. cannot use _PinCS because DeliverMedia releases it
                                //!< before calling  the GetDeliveryMedia method
};

} //namespace FEF
