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

   ____________________________________________http://www.SMProcess.com______ 
*//*!
   @file	 IFxNote.h
   @brief This is the main interface file for the Fx plugin Note interface.

   IFxNote interface allows to a Fx plugin to publish Fx internal notes/messages
   to the FxEngine application.
*//*_______________________________________________________________________*/
#pragma once

//! FxEngine Framework definitions
#include "FxDef.h"

namespace FEF {

//! Note Callback function
	typedef Void (FEF_CALLBACK FXNOTECALLBACK)(const std::string strNote, FX_HANDLE hFx, FX_HANDLE hObserverId, FX_PTR dwParam);

//! Note Callback object
class FXENGINE_EXP CFxNoteCallback {
public:
	virtual Void FxNoteCallback(const std::string strNote, FX_HANDLE hFx, FX_HANDLE hObserverId, FX_PTR dwParam);
};

//! FxEngine interfaces
class IFxNote;

//! The IFxNote class is the Fx plugin note interface.
class IFxNote {
public:
	/*----------------------------------------------------------------------*//*!
	FxPublishNote().

	The FxPublishNote method sends a Fx note to observers.
    @param strNote: Variable that contains the Fx note to publish.

	@return	FX_OK if success, otherwise an FX error code.
    *//*-----------------------------------------------------------------------*/
	virtual Int32 FxPublishNote( /* [in] */const std::string strNote) PURE;

	/*----------------------------------------------------------------------*//*!
	FxReleaseInterface().

	The FxReleaseInterface method releases the IFxNote interface.
	See FxGetInterface method of IFx interface.
    @param None.

	@return	The new reference count.
    *//*-----------------------------------------------------------------------*/
	virtual Int32 FxReleaseInterface() PURE;
};
} //namespace FEF

