/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#ifndef _DTRANS_X11_DNDCONTEXT_HXX
#define _DTRANS_X11_DNDCONTEXT_HXX

#include <com/sun/star/datatransfer/dnd/XDragSourceContext.hpp>
#include <com/sun/star/datatransfer/dnd/XDropTargetDropContext.hpp>
#include <com/sun/star/datatransfer/dnd/XDropTargetDragContext.hpp>
#include <cppuhelper/implbase1.hxx>

#include "tools/prex.h"
#include <X11/Xlib.h>
#include "tools/postx.h"

using namespace com::sun::star::uno;

namespace x11 {

	class SelectionManager;

	class DropTargetDropContext :
		public ::cppu::WeakImplHelper1<
	::com::sun::star::datatransfer::dnd::XDropTargetDropContext
	>
	{
		XLIB_Window                 m_aDropWindow;
		XLIB_Time                   m_nTimestamp;
		SelectionManager&           m_rManager;
		com::sun::star::uno::Reference< XInterface >     m_xManagerRef;
	public:
		DropTargetDropContext( XLIB_Window, XLIB_Time, SelectionManager& );
		virtual ~DropTargetDropContext();

		// XDropTargetDropContext
		virtual void SAL_CALL acceptDrop( sal_Int8 dragOperation ) throw();
		virtual void SAL_CALL rejectDrop() throw();
		virtual void SAL_CALL dropComplete( sal_Bool success ) throw();
	};

	class DropTargetDragContext :
		public ::cppu::WeakImplHelper1<
	::com::sun::star::datatransfer::dnd::XDropTargetDragContext
	>
	{
		XLIB_Window                 m_aDropWindow;
		XLIB_Time                   m_nTimestamp;
		SelectionManager&           m_rManager;
		com::sun::star::uno::Reference< XInterface >     m_xManagerRef;
	public:
		DropTargetDragContext( XLIB_Window, XLIB_Time, SelectionManager& );
		virtual ~DropTargetDragContext();

		// XDropTargetDragContext
		virtual void SAL_CALL acceptDrag( sal_Int8 dragOperation ) throw();
		virtual void SAL_CALL rejectDrag() throw();
	};

	class DragSourceContext :
		public ::cppu::WeakImplHelper1<
	::com::sun::star::datatransfer::dnd::XDragSourceContext
	>
	{
		XLIB_Window                 m_aDropWindow;
		XLIB_Time                   m_nTimestamp;
		SelectionManager&           m_rManager;
		com::sun::star::uno::Reference< XInterface >     m_xManagerRef;
	public:
		DragSourceContext( XLIB_Window, XLIB_Time, SelectionManager& );
		virtual ~DragSourceContext();

		// XDragSourceContext
		virtual sal_Int32	SAL_CALL getCurrentCursor() throw();
		virtual void		SAL_CALL setCursor( sal_Int32 cursorId ) throw();
		virtual void		SAL_CALL setImage( sal_Int32 imageId ) throw();
		virtual void		SAL_CALL transferablesFlavorsChanged() throw();
	};
} // namespace

#endif // _DTRANS_X11_DNDCONTEXT_HXX
