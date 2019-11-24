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


#ifndef _IDROPTARGET_HXX_
#define _IDROPTARGET_HXX_ 

#include "target.hxx"

class IDropTargetImpl: public IDropTarget
{
    LONG m_nRefCount;
    // Calls to IDropTarget functions are delegated to a DropTarget.
    DropTarget& m_rDropTarget;

    virtual ~IDropTargetImpl(); // delete is only called by IUnknown::Release 
    IDropTargetImpl( const IDropTargetImpl& );
    IDropTargetImpl& operator=( const IDropTargetImpl& );
public:
    IDropTargetImpl( DropTarget& pTarget);

   	// IDropTarget
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
    virtual ULONG STDMETHODCALLTYPE AddRef( );
        
    virtual ULONG STDMETHODCALLTYPE Release( );

	virtual HRESULT STDMETHODCALLTYPE DragEnter( 
            /* [unique][in] */ IDataObject __RPC_FAR *pDataObj,
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD __RPC_FAR *pdwEffect);
        
	virtual HRESULT STDMETHODCALLTYPE DragOver( 
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD __RPC_FAR *pdwEffect);
        
	virtual HRESULT STDMETHODCALLTYPE DragLeave( ) ;
        
    virtual HRESULT STDMETHODCALLTYPE Drop( 
            /* [unique][in] */ IDataObject __RPC_FAR *pDataObj,
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD __RPC_FAR *pdwEffect);


};
#endif
