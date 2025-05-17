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

#include <vector>
#include <boost/utility.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include "FxUtils.h"

#include "NotifyObjects.h"
namespace FEF {

class CFxAPCCallBack
{
public:
	virtual signed long int APCCallBack(Void* pObject) = 0;
};

//! APC internal queue structure
struct FX_APC {
    CFxAPCCallBack *_pInstance;		//!< CFxAPCCallBack Instance
	Void *_pObject;       //!< Object in queue .

	FX_APC( CFxAPCCallBack *pInstance, Void *pObject)
		: _pInstance( pInstance), _pObject( pObject)
	{}
};

class CFxAPC : private boost::noncopyable
{

public:
    CFxAPC();
	~CFxAPC();

public:
    long StartAPC();
    long StopAPC();
	void AddAPCObject(CFxAPCCallBack *pInstance, Void* pObject);
	FX_APC* GetAPCObject();
	unsigned long GetObjectNumberInQueue() const {return _dwObjectNumber;}

public:
    unsigned long _dwObjectNumber;
    std::vector<FX_APC> _APCBuff;
	boost::condition APCBuff_not_empty;
	boost::mutex _CSThread;
    boost::mutex _CSAPC;

	bool _ShouldQuit;
    bool _IsTemporaryObject;

private:
	boost::thread* _pthreadAPC;	
};

} //namespace FEF
