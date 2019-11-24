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


#ifndef DBAUI_QUERYDESIGNFIELDUNDOACT_HXX
#define DBAUI_QUERYDESIGNFIELDUNDOACT_HXX

#ifndef DBAUI_GENERALUNDO_HXX
#include "GeneralUndo.hxx"
#endif
#ifndef _DBU_QRY_HRC_
#include "dbu_qry.hrc"
#endif
#ifndef DBAUI_QUERYDESIGN_OSELECTIONBROWSEBOX_HXX
#include "SelectionBrowseBox.hxx"
#endif


namespace dbaui
{
	// ================================================================================================
	// OQueryDesignFieldUndoAct - Basisklasse fuer Undos in der Feldauflistung im Abfrageentwurf

	
	class OQueryDesignFieldUndoAct : public OCommentUndoAction
	{
	protected:
		OSelectionBrowseBox*	pOwner;
		sal_uInt16					m_nColumnPostion;

		virtual void	Undo() = 0;
		virtual void	Redo() = 0;

	public:
		OQueryDesignFieldUndoAct(OSelectionBrowseBox* pSelBrwBox, sal_uInt16 nCommentID);
		virtual ~OQueryDesignFieldUndoAct();

		inline void SetColumnPosition(sal_uInt16 _nColumnPostion) 
		{ 
			m_nColumnPostion = _nColumnPostion; 
			OSL_ENSURE(m_nColumnPostion != BROWSER_INVALIDID,"Column position was not set add the undo action!");
			OSL_ENSURE(m_nColumnPostion < pOwner->GetColumnCount(),"Position outside the column count!");
		}
	};

	// ================================================================================================
	// OTabFieldCellModifiedUndoAct - Undo-Klasse fuer Aendern einer Zelle einer Spaltenbeschreibung

	class OTabFieldCellModifiedUndoAct : public OQueryDesignFieldUndoAct
	{
	protected:
		String		m_strNextCellContents;
		sal_Int32	m_nCellIndex;

	public:
		OTabFieldCellModifiedUndoAct(OSelectionBrowseBox* pSelBrwBox) 
			: OQueryDesignFieldUndoAct(pSelBrwBox, STR_QUERY_UNDO_MODIFY_CELL) 
			,m_nCellIndex(BROWSER_INVALIDID){ }

		inline void SetCellContents(const String& str)	{ m_strNextCellContents = str; }
		inline void SetCellIndex(sal_Int32 nIndex)		{ m_nCellIndex = nIndex; }

		virtual void Undo();
		virtual void Redo() { Undo(); }
	};

	// ================================================================================================
	// OTabFieldSizedUndoAct - Undo-Klasse fuer Aendern einer Spaltenbreite

	class OTabFieldSizedUndoAct : public OQueryDesignFieldUndoAct
	{
	protected:
		long		m_nNextWidth;

	public:
		OTabFieldSizedUndoAct(OSelectionBrowseBox* pSelBrwBox) : OQueryDesignFieldUndoAct(pSelBrwBox, STR_QUERY_UNDO_SIZE_COLUMN), m_nNextWidth(0) { }

		inline void SetOriginalWidth(long nWidth) { m_nNextWidth = nWidth; }

		virtual void Undo();
		virtual void Redo() { Undo(); }
	};

	// ================================================================================================
	// OTabFieldUndoAct - Basisklasse fuer Undos in der Feldauflistung im Abfrageentwurf, die mit Veraendern einer kompletten Feldbeschreibung zu tun haben

	class OTabFieldUndoAct : public OQueryDesignFieldUndoAct
	{
	protected:
		OTableFieldDescRef		pDescr;		// geloeschte Spaltenbeschreibung

	public:
		OTabFieldUndoAct(OSelectionBrowseBox* pSelBrwBox, sal_uInt16 nCommentID) : OQueryDesignFieldUndoAct(pSelBrwBox, nCommentID) { }

		void SetTabFieldDescr(OTableFieldDescRef pDescription) { pDescr = pDescription; }
	};

	// ================================================================================================
	// OTabFieldDelUndoAct - Undo-Klasse fuer Loeschen eines Feldes

	class OTabFieldDelUndoAct : public OTabFieldUndoAct
	{
	protected:
		virtual void Undo() { pOwner->EnterUndoMode();pOwner->InsertColumn(pDescr, m_nColumnPostion);pOwner->LeaveUndoMode(); }
		virtual void Redo() { pOwner->EnterUndoMode();pOwner->RemoveColumn(pDescr->GetColumnId());pOwner->LeaveUndoMode(); }

	public:
		OTabFieldDelUndoAct(OSelectionBrowseBox* pSelBrwBox) : OTabFieldUndoAct(pSelBrwBox, STR_QUERY_UNDO_TABFIELDDELETE) { }
	};

	// ================================================================================================
	// OTabFieldDelUndoAct - Undo-Klasse fuer Anlegen eines Feldes

	class OTabFieldCreateUndoAct : public OTabFieldUndoAct
	{
	protected:
		virtual void Undo() { pOwner->EnterUndoMode();pOwner->RemoveColumn(pDescr->GetColumnId());pOwner->LeaveUndoMode();}
		virtual void Redo() { pOwner->EnterUndoMode();pOwner->InsertColumn(pDescr, m_nColumnPostion);pOwner->LeaveUndoMode();}

	public:
		OTabFieldCreateUndoAct(OSelectionBrowseBox* pSelBrwBox) : OTabFieldUndoAct(pSelBrwBox, STR_QUERY_UNDO_TABFIELDCREATE) { }
	};

	// ================================================================================================
	// OTabFieldMovedUndoAct - Undo-class when a field was moved inside the selection

	class OTabFieldMovedUndoAct : public OTabFieldUndoAct
	{
	protected:
		virtual void Undo();
		virtual void Redo() 
		{ 
			Undo();
		}

	public:
		OTabFieldMovedUndoAct(OSelectionBrowseBox* pSelBrwBox) : OTabFieldUndoAct(pSelBrwBox, STR_QUERY_UNDO_TABFIELDMOVED) { }
	};
}
#endif // DBAUI_QUERYDESIGNFIELDUNDOACT_HXX



