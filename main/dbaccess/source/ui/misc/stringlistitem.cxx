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
#include "precompiled_dbui.hxx"

#ifndef _DBAUI_STRINGLISTITEM_HXX_
#include "stringlistitem.hxx"
#endif

//.........................................................................
namespace dbaui
{
//.........................................................................

using namespace ::com::sun::star::uno;

//=========================================================================
//= OStringListItem
//=========================================================================
TYPEINIT1(OStringListItem, SfxPoolItem);
//-------------------------------------------------------------------------
OStringListItem::OStringListItem(sal_Int16 _nWhich, const Sequence< ::rtl::OUString >& _rList)
	:SfxPoolItem(_nWhich)
	,m_aList(_rList)
{
}

//-------------------------------------------------------------------------
OStringListItem::OStringListItem(const OStringListItem& _rSource)
	:SfxPoolItem(_rSource)
	,m_aList(_rSource.m_aList)
{
}

//-------------------------------------------------------------------------
int OStringListItem::operator==(const SfxPoolItem& _rItem) const
{
	const OStringListItem* pCompare = PTR_CAST(OStringListItem, &_rItem);
	if ((!pCompare) || (pCompare->m_aList.getLength() != m_aList.getLength()))
		return 0;

	// compare all strings individually
	const ::rtl::OUString* pMyStrings = m_aList.getConstArray();
	const ::rtl::OUString* pCompareStrings = pCompare->m_aList.getConstArray();

	for (sal_Int32 i=0; i<m_aList.getLength(); ++i, ++pMyStrings, ++pCompareStrings)
		if (!pMyStrings->equals(*pCompareStrings))
			return 0;

	return 1;
}

//-------------------------------------------------------------------------
SfxPoolItem* OStringListItem::Clone(SfxItemPool* /* _pPool */) const
{
	return new OStringListItem(*this);
}

//.........................................................................
}	// namespace dbaui
//.........................................................................

