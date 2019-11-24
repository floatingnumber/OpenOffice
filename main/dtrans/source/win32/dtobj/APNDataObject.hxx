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




#ifndef _APNDATAOBJECT_HXX_
#define _APNDATAOBJECT_HXX_

#include <systools/win32/comtools.hxx>

//------------------------------------------------------------------------
// deklarations
//------------------------------------------------------------------------

/*
	an APartment Neutral dataobject wrapper; this wrapper of a IDataObject
	pointer can be used from any apartment without RPC_E_WRONG_THREAD
	which normally occurs if an apartment tries to use an interface 
	pointer of another apartment; we use containment to hold the original
	DataObject
*/
class CAPNDataObject : public IDataObject
{
public:	
	CAPNDataObject( IDataObjectPtr rIDataObject );
	virtual ~CAPNDataObject( );

	//-----------------------------------------------------------------
    //IUnknown interface methods
	//-----------------------------------------------------------------

    STDMETHODIMP           QueryInterface(REFIID iid, LPVOID* ppvObject);
    STDMETHODIMP_( ULONG ) AddRef( );
    STDMETHODIMP_( ULONG ) Release( );

	//-----------------------------------------------------------------
    // IDataObject interface methods
	//-----------------------------------------------------------------

    STDMETHODIMP GetData( LPFORMATETC pFormatetc, LPSTGMEDIUM pmedium );
    STDMETHODIMP GetDataHere( LPFORMATETC pFormatetc, LPSTGMEDIUM pmedium );
    STDMETHODIMP QueryGetData( LPFORMATETC pFormatetc );
    STDMETHODIMP GetCanonicalFormatEtc( LPFORMATETC pFormatectIn, LPFORMATETC pFormatetcOut );
    STDMETHODIMP SetData( LPFORMATETC pFormatetc, LPSTGMEDIUM pmedium, BOOL fRelease );
    STDMETHODIMP EnumFormatEtc( DWORD dwDirection, IEnumFORMATETC** ppenumFormatetc );
    STDMETHODIMP DAdvise( LPFORMATETC pFormatetc, DWORD advf, LPADVISESINK pAdvSink, DWORD* pdwConnection );
    STDMETHODIMP DUnadvise( DWORD dwConnection );
    STDMETHODIMP EnumDAdvise( LPENUMSTATDATA* ppenumAdvise );

	operator IDataObject*( );

private:
	HRESULT MarshalIDataObjectIntoCurrentApartment( IDataObject** ppIDataObj );

private:
	IDataObjectPtr  m_rIDataObjectOrg;
	HGLOBAL			m_hGlobal;
	LONG			m_nRefCnt;

// prevent copy and assignment
private:
	CAPNDataObject( const CAPNDataObject& theOther );
	CAPNDataObject& operator=( const CAPNDataObject& theOther );
};

#endif
