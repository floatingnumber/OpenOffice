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
#include "file/FCatalog.hxx"
#include "file/fcomp.hxx"
#include "file/fanalyzer.hxx"
#include "file/FResultSet.hxx"
#include "file/FPreparedStatement.hxx"
#include <connectivity/FValue.hxx>
#include <tools/debug.hxx>
#include "TKeyValue.hxx"
#include <rtl/logfile.hxx>

using namespace connectivity;
using namespace connectivity::file;
// -----------------------------------------------------------------------------
void OFileCatalog::refreshViews()
{}
void OFileCatalog::refreshGroups()
{}
void OFileCatalog::refreshUsers()
{
}
// -----------------------------------------------------------------------------
OPredicateInterpreter::~OPredicateInterpreter()
{
	while(!m_aStack.empty())
	{
		delete m_aStack.top();
		m_aStack.pop();
	}
	//	m_aStack.clear();
}
// -----------------------------------------------------------------------------
void OPredicateCompiler::Clean()
{
	for(OCodeList::reverse_iterator aIter = m_aCodeList.rbegin(); aIter != m_aCodeList.rend();++aIter)
	{
		delete *aIter;
	}
	m_aCodeList.clear();
}
// -----------------------------------------------------------------------------
void OSQLAnalyzer::clean()
{
	m_aCompiler->Clean();
}
// -----------------------------------------------------------------------------
void OSQLAnalyzer::bindParameterRow(OValueRefRow& _pRow)
{
	OCodeList& rCodeList	= m_aCompiler->m_aCodeList;
	for(OCodeList::iterator aIter = rCodeList.begin(); aIter != rCodeList.end();++aIter)
	{
		OOperandParam* pParam = PTR_CAST(OOperandParam,(*aIter));
		if ( pParam )
			pParam->bindValue(_pRow);
	}
}
// -----------------------------------------------------------------------------
void OPreparedStatement::scanParameter(OSQLParseNode* pParseNode,::std::vector< OSQLParseNode*>& _rParaNodes)
{
    RTL_LOGFILE_CONTEXT_AUTHOR( aLogger, "file", "Ocke.Janssen@sun.com", "OPreparedStatement::scanParameter" );
	DBG_ASSERT(pParseNode != NULL,"OResultSet: interner Fehler: ungueltiger ParseNode");

	// Parameter Name-Regel gefunden?
	if (SQL_ISRULE(pParseNode,parameter))
	{
		DBG_ASSERT(pParseNode->count() >= 1,"OResultSet: Parse Tree fehlerhaft");
		DBG_ASSERT(pParseNode->getChild(0)->getNodeType() == SQL_NODE_PUNCTUATION,"OResultSet: Parse Tree fehlerhaft");

		_rParaNodes.push_back(pParseNode);
		// Weiterer Abstieg nicht erforderlich
		return;
	}

	// Weiter absteigen im Parse Tree
	for (sal_uInt32 i = 0; i < pParseNode->count(); i++)
		scanParameter(pParseNode->getChild(i),_rParaNodes);
}
// -----------------------------------------------------------------------------
OKeyValue* OResultSet::GetOrderbyKeyValue(OValueRefRow& _rRow)
{
    RTL_LOGFILE_CONTEXT_AUTHOR( aLogger, "file", "Ocke.Janssen@sun.com", "OResultSet::GetOrderbyKeyValue" );
	sal_uInt32 nBookmarkValue = Abs((sal_Int32)(_rRow->get())[0]->getValue());

	OKeyValue* pKeyValue = OKeyValue::createKeyValue((sal_uInt32)nBookmarkValue);

	::std::vector<sal_Int32>::iterator aIter = m_aOrderbyColumnNumber.begin();
	for (;aIter != m_aOrderbyColumnNumber.end(); ++aIter)
	{
		OSL_ENSURE(*aIter < static_cast<sal_Int32>(_rRow->get().size()),"Invalid index for orderkey values!");
		pKeyValue->pushKey(new ORowSetValueDecorator((_rRow->get())[*aIter]->getValue()));
	}

	return pKeyValue;
}
// -----------------------------------------------------------------------------








