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
#include "FxAPC.h"
#include "NotifyObjects.h"
namespace FEF {

void threadAPC(CFxAPC* p) {
	AutoLock lkThread(p->_CSThread);
	FX_APC *pFxAPC;
	do {
		AutoLock lk(p->_CSAPC);
		while (p->_dwObjectNumber == 0)
            p->APCBuff_not_empty.wait(lk);
		if((p->_ShouldQuit == true) && ((p->_APCBuff.front())._pInstance == NULL)){
            p->_APCBuff.erase(p->_APCBuff.begin());
            --p->_dwObjectNumber;
			return;
        }
		
		pFxAPC = &(p->_APCBuff.front());
        lk.unlock();
		if( (pFxAPC->_pInstance->APCCallBack(pFxAPC->_pObject) == 1) && (p->_ShouldQuit != true) )
		{
			/*! repeate object */
		    lk.lock();
			p->APCBuff_not_empty.notify_one();
            lk.unlock();
		}	
		else
		{
            lk.lock();
			p->_APCBuff.erase(p->_APCBuff.begin());
			--p->_dwObjectNumber;
			lk.unlock();
		}
    } while (1); // -1 indicates end of buffer
}

CFxAPC::CFxAPC() :
_dwObjectNumber(0),
_ShouldQuit(false),
_pthreadAPC(NULL)
{
	_pthreadAPC = new boost::thread(boost::bind(&threadAPC, this));
}

CFxAPC::~CFxAPC()
{
    if(_pthreadAPC) {
	    _ShouldQuit = true;
	    AddAPCObject(NULL, NULL);
	    _pthreadAPC->join(); //! call join() to be sure that thread is created and finish
	    AutoLock lkThread(_CSThread);
    	
	    AutoLock lk(_CSAPC);
	    _APCBuff.clear();
	    if(_pthreadAPC)
		    delete (_pthreadAPC);
	    lk.unlock();

	    lkThread.unlock();
    }
}

long CFxAPC::StartAPC()
{
    AutoLock lk(_CSAPC);
    if(_pthreadAPC == NULL) {
        _dwObjectNumber = 0;
        _ShouldQuit = false;
        _pthreadAPC = new boost::thread(boost::bind(&threadAPC, this));
    }

    return 0;
}

long CFxAPC::StopAPC()
{
    if(_pthreadAPC) {
        _ShouldQuit = true;
	    AddAPCObject(NULL, NULL);
	    _pthreadAPC->join(); //! call join() to be sure that thread is created and finish
	    AutoLock lkThread(_CSThread);
    	
	    AutoLock lk(_CSAPC);
	    _APCBuff.clear();
	    if(_pthreadAPC)
		    delete (_pthreadAPC);
        _pthreadAPC = NULL;
	    lk.unlock();

	    lkThread.unlock();
    }
    return 0;
}

void CFxAPC::AddAPCObject(CFxAPCCallBack *pInstance, Void* pObject)
{
	AutoLock lk(_CSAPC);
	_APCBuff.push_back(FX_APC(pInstance, pObject));
	++_dwObjectNumber;
	APCBuff_not_empty.notify_one();
}

FX_APC* CFxAPC::GetAPCObject()
{
	FX_APC *pFxAPC;

	AutoLock lk(_CSAPC);
	if(_APCBuff.empty() == true)
		return NULL;
	pFxAPC = &(_APCBuff.front());
	_APCBuff.erase(_APCBuff.begin());
	--_dwObjectNumber;

	return pFxAPC;
}

 } //namespace FEF
