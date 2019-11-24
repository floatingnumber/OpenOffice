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



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_sw.hxx"


#include <doc.hxx>
#include <txttxmrk.hxx>
#include <swfont.hxx>
#include <tox.hxx>
#include <ndtxt.hxx>

SwTxtTOXMark::SwTxtTOXMark( SwTOXMark& rAttr,
            xub_StrLen const nStartPos, xub_StrLen const*const pEnd)
    : SwTxtAttrEnd( rAttr, nStartPos, nStartPos )
    , m_pTxtNode( 0 )
    , m_pEnd( 0 )
{
    rAttr.pTxtAttr = this;
    if ( !rAttr.GetAlternativeText().Len() )
    {
        m_nEnd = *pEnd;
        m_pEnd = & m_nEnd;
    }
    else
    {
        SetHasDummyChar(true);
    }
    SetDontMoveAttr( true );
    SetOverlapAllowedAttr( true );
}

SwTxtTOXMark::~SwTxtTOXMark()
{
}

xub_StrLen* SwTxtTOXMark::GetEnd()
{
    return m_pEnd;
}

void SwTxtTOXMark::CopyTOXMark( SwDoc* pDoc )
{
	SwTOXMark& rTOX = (SwTOXMark&)GetTOXMark();
	TOXTypes    eType   = rTOX.GetTOXType()->GetType();
	sal_uInt16      nCount  = pDoc->GetTOXTypeCount( eType );
	const SwTOXType* pType = 0;
	const XubString& rNm = rTOX.GetTOXType()->GetTypeName();

	// kein entsprechender Verzeichnistyp vorhanden -> anlegen
	// sonst verwenden
	for(sal_uInt16 i=0; i < nCount; ++i)
	{
		const SwTOXType* pSrcType = pDoc->GetTOXType(eType, i);
		if(pSrcType->GetTypeName() == rNm )
		{
			pType = pSrcType;
			break;
		}
	}
	// kein entsprechender Typ vorhanden -> neu erzeugen
	//
	if(!pType)
	{
		pDoc->InsertTOXType( SwTOXType( eType, rNm ) );
		pType = pDoc->GetTOXType(eType, 0);
	}
	// Verzeichnistyp umhaengen
	//
	((SwTOXType*)pType)->Add( &rTOX );
}
