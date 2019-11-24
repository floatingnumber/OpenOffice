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
#include "precompiled_connectivity.hxx"
#include "dbase/DCode.hxx"
#include <com/sun/star/sdbcx/XColumnsSupplier.hpp>
#include "dbase/DIndex.hxx"
#include "dbase/DIndexIter.hxx"


using namespace connectivity::dbase;
using namespace connectivity::file;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::beans;
using namespace ::com::sun::star::sdbcx;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::container;

TYPEINIT1(OFILEOperandAttr, OOperandAttr);
// -----------------------------------------------------------------------------
OOperandAttr* OFILEAnalyzer::createOperandAttr(sal_Int32 _nPos,
											   const Reference< XPropertySet>& _xCol,
											   const Reference< XNameAccess>& _xIndexes)
{
	return new OFILEOperandAttr((sal_uInt16)_nPos,_xCol,_xIndexes);
}

//------------------------------------------------------------------
OFILEOperandAttr::OFILEOperandAttr(sal_uInt16 _nPos,
								   const Reference< XPropertySet>& _xColumn,
								   const Reference< XNameAccess>& _xIndexes)
	: OOperandAttr(_nPos,_xColumn)
{
	if(_xIndexes.is())
	{
		::rtl::OUString sName;
		Reference<XPropertySetInfo> xColInfo = _xColumn->getPropertySetInfo();
		Reference<XPropertySet> xIndex;

		Sequence< ::rtl::OUString> aSeq = _xIndexes->getElementNames();
		const ::rtl::OUString* pBegin = aSeq.getConstArray();
		const ::rtl::OUString* pEnd	  = pBegin + aSeq.getLength();
		for(;pBegin != pEnd;++pBegin)
		{
			_xIndexes->getByName(*pBegin) >>= xIndex;
			if(xIndex.is())
			{
				Reference<XColumnsSupplier> xColsSup(xIndex,UNO_QUERY);
				Reference<XNameAccess> xNameAccess = xColsSup->getColumns();
				_xColumn->getPropertyValue(OMetaConnection::getPropMap().getNameByIndex(PROPERTY_ID_NAME)) >>= sName;
				if(xNameAccess->hasByName(sName))
				{
					m_xIndex = xIndex;
					break;
				}
				else if(xColInfo->hasPropertyByName(OMetaConnection::getPropMap().getNameByIndex(PROPERTY_ID_REALNAME)))
				{
					_xColumn->getPropertyValue(OMetaConnection::getPropMap().getNameByIndex(PROPERTY_ID_REALNAME)) >>= sName;
					if(xNameAccess->hasByName(sName))
					{
						m_xIndex = xIndex;
						break;
					}
				}
			}
		}
	}

}
// -------------------------------------------------------------------------
sal_Bool OFILEOperandAttr::isIndexed() const
{
	return m_xIndex.is();
}
//------------------------------------------------------------------
OEvaluateSet* OFILEOperandAttr::preProcess(OBoolOperator* pOp, OOperand* pRight)
{
	OEvaluateSet* pEvaluateSet = NULL;
	if (isIndexed())
	{
		Reference<XUnoTunnel> xTunnel(m_xIndex,UNO_QUERY);
		if(xTunnel.is())
		{
			ODbaseIndex* pIndex = reinterpret_cast< ODbaseIndex* >( xTunnel->getSomething(ODbaseIndex::getUnoTunnelImplementationId()) );
			if(pIndex)
			{
				OIndexIterator* pIter = pIndex->createIterator(pOp,pRight);

				if (pIter)
				{
					pEvaluateSet = new OEvaluateSet();
					sal_uIntPtr nRec = pIter->First();
					while (nRec != NODE_NOTFOUND)
					{
						(*pEvaluateSet)[nRec] = nRec;
						nRec = pIter->Next();
					}
				}
				delete pIter;
			}
		}
	}
	return pEvaluateSet;
}


