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


#include <hintids.hxx>


#include <svx/svdview.hxx>
#include <editeng/outliner.hxx>
//#ifndef _SVDVMARK_HXX //autogen
//#include <svx/svdvmark.hxx>
//#endif
#include <svx/svdobj.hxx>
#include <sot/exchange.hxx>
#include <sot/formats.hxx>
#include <sfx2/bindings.hxx>

#include <sfx2/viewfrm.hxx>
#include <fmturl.hxx>
#include <frmfmt.hxx>
#include <wrtsh.hxx>
#include <edtwin.hxx>
#ifndef _VIEW_HXX
#include <view.hxx>
#endif
#include <viewopt.hxx>
#include <swdtflvr.hxx>
#include <swmodule.hxx>
#ifndef _DOCSH_HXX
#include <docsh.hxx>
#endif
#include <wdocsh.hxx>
#include <swundo.hxx>

using namespace ::com::sun::star;

// no include "dbgoutsw.hxx" here!!!!!!

extern sal_Bool bNoInterrupt;
extern sal_Bool bFrmDrag;
extern sal_Bool bDDTimerStarted;

sal_Bool bExecuteDrag = sal_False;

void SwEditWin::StartDDTimer()
{
	aTimer.SetTimeoutHdl(LINK(this, SwEditWin, DDHandler));
	aTimer.SetTimeout(480);
	aTimer.Start();
	bDDTimerStarted = sal_True;
}


void SwEditWin::StopDDTimer(SwWrtShell *pSh, const Point &rPt)
{
	aTimer.Stop();
	bDDTimerStarted = sal_False;
	if(!pSh->IsSelFrmMode())
		(pSh->*pSh->fnSetCrsr)(&rPt,sal_False);
	aTimer.SetTimeoutHdl(LINK(this,SwEditWin, TimerHandler));
}

void SwEditWin::StartDrag( sal_Int8 /*nAction*/, const Point& rPosPixel )
{
	SwWrtShell &rSh = rView.GetWrtShell();
	if( rSh.GetDrawView() )
	{
		CommandEvent aDragEvent( rPosPixel, COMMAND_STARTDRAG, sal_True );
		if( rSh.GetDrawView()->Command( aDragEvent, this ) )
		{
			rView.GetViewFrame()->GetBindings().InvalidateAll(sal_False);
			return; // Event von der SdrView ausgewertet
		}
	}

	if ( !pApplyTempl && !rSh.IsDrawCreate() && !IsDrawAction())
	{
		sal_Bool bStart = sal_False, bDelSelect = sal_False;
		SdrObject *pObj = NULL;
		Point aDocPos( PixelToLogic( rPosPixel ) );
		if ( !rSh.IsInSelect() && rSh.ChgCurrPam( aDocPos, sal_True, sal_True))
			//Wir sind nicht beim Selektieren und stehen auf einer
			//Selektion
			bStart = sal_True;
		else if ( !bFrmDrag && rSh.IsSelFrmMode() &&
					rSh.IsInsideSelectedObj( aDocPos ) )
		{
			//Wir sind nicht am internen Draggen und stehen auf
			//einem Objekt (Rahmen, Zeichenobjekt)

			bStart = sal_True;
		}
		else if( !bFrmDrag && rView.GetDocShell()->IsReadOnly() &&
				OBJCNT_NONE != rSh.GetObjCntType( aDocPos, pObj ))
		{
			rSh.LockPaint();
            if( rSh.SelectObj( aDocPos, 0, pObj ))
				bStart = bDelSelect = sal_True;
			else
				rSh.UnlockPaint();
		}
		else
		{
			SwContentAtPos aSwContentAtPos( SwContentAtPos::SW_INETATTR );
			bStart = rSh.GetContentAtPos( aDocPos,
						aSwContentAtPos,
						sal_False );
		}

		if ( bStart && !bIsInDrag )
		{
			bMBPressed = sal_False;
			ReleaseMouse();
			bFrmDrag = sal_False;
			bExecuteDrag = sal_True;
			SwEditWin::nDDStartPosY = aDocPos.Y();
			SwEditWin::nDDStartPosX = aDocPos.X();
			aMovePos = aDocPos;
			StartExecuteDrag();
			if( bDelSelect )
			{
				rSh.UnSelectFrm();
				rSh.UnlockPaint();
			}
		}
	}
}

void SwEditWin::StartExecuteDrag()
{
	if( !bExecuteDrag || bIsInDrag )
		return;

	bIsInDrag = sal_True;

	SwTransferable* pTransfer = new SwTransferable( rView.GetWrtShell() );
    uno::Reference<
        datatransfer::XTransferable > xRef( pTransfer );

	pTransfer->StartDrag( this, aMovePos );
}

void SwEditWin::DragFinished()
{
	DropCleanup();
	aTimer.SetTimeoutHdl( LINK(this,SwEditWin, TimerHandler) );
	bIsInDrag = sal_False;
}


void SwEditWin::DropCleanup()
{
	SwWrtShell &rSh =  rView.GetWrtShell();

	// Stati zuruecksetzen
	bNoInterrupt = sal_False;
	if ( bOldIdleSet )
	{
		((SwViewOption*)rSh.GetViewOptions())->SetIdle( bOldIdle );
		bOldIdleSet = sal_False;
	}
	if ( pUserMarker )
		CleanupDropUserMarker();
	else
		rSh.UnSetVisCrsr();

}

void SwEditWin::CleanupDropUserMarker()
{
	if ( pUserMarker )
	{
		delete pUserMarker;
		pUserMarker = 0;
		pUserMarkerObj = 0;
	}
}


//Messehack (MA,MBA)
void lcl_SelectShellForDrop( SwView &rView )
{
	if ( !rView.GetCurShell() )
		rView.SelectShell();
}

sal_Int8 SwEditWin::ExecuteDrop( const ExecuteDropEvent& rEvt )
{
	::lcl_SelectShellForDrop( GetView() );
	DropCleanup();
	sal_Int8 nRet = DND_ACTION_NONE;

	//Ein Drop auf eine offene OutlinerView geht uns nichts an (siehe auch QueryDrop)
	SwWrtShell &rSh = rView.GetWrtShell();
	const Point aDocPt( PixelToLogic( rEvt.maPosPixel ));
	SdrObject *pObj = 0;
	OutlinerView* pOLV;
	rSh.GetObjCntType( aDocPt, pObj );

	if( pObj && 0 != ( pOLV = rSh.GetDrawView()->GetTextEditOutlinerView() ))
	{
		Rectangle aRect( pOLV->GetOutputArea() );
		aRect.Union( pObj->GetLogicRect() );
		const Point aPos = pOLV->GetWindow()->PixelToLogic(rEvt.maPosPixel);
		if ( aRect.IsInside(aPos) )
		{
			rSh.StartAllAction();
//!!			sal_Int8 nRet = DND_ACTION_NONE/*pOLV->ExecuteDrop( rEvt )*/;
			rSh.EndAllAction();
			return nRet;
		}
	}


	// dvo 2002-05-27, #99027#: There's a special treatment for file lists with a single
	//                          element, that depends on the actual content of the
	//                          Transferable to be accessible. Since the transferable
	//                          may only be accessed after the drop has been accepted
	//                          (according to KA due to Java D&D), we'll have to
	//                          reevaluate the drop action once more _with_ the
	//                          Transferable.
	sal_uInt16 nEventAction;
	sal_Int8 nUserOpt = rEvt.mbDefault ? EXCHG_IN_ACTION_DEFAULT
									   : rEvt.mnAction;
    m_nDropAction = SotExchange::GetExchangeAction(
								GetDataFlavorExVector(),
                                m_nDropDestination,
								rEvt.mnAction,
//!!								rEvt.GetSourceOptions(),
                                nUserOpt, m_nDropFormat, nEventAction, 0,
								&rEvt.maDropEvent.Transferable );


	TransferableDataHelper aData( rEvt.maDropEvent.Transferable );
	nRet = rEvt.mnAction;
    if( !SwTransferable::PasteData( aData, rSh, m_nDropAction, m_nDropFormat,
                                m_nDropDestination, sal_False, rEvt.mbDefault, &aDocPt, nRet))
//!!    nRet = SFX_APP()->ExecuteDrop( rEvt );
		nRet = DND_ACTION_NONE;
	else if ( SW_MOD()->pDragDrop )
		//Bei internem D&D nicht mehr aufraeumen!
		SW_MOD()->pDragDrop->SetCleanUp( sal_False );

	return nRet;
}


sal_uInt16 SwEditWin::GetDropDestination( const Point& rPixPnt, SdrObject ** ppObj )
{
    SwWrtShell &rSh = rView.GetWrtShell();
    const Point aDocPt( PixelToLogic( rPixPnt ) );
    if( rSh.ChgCurrPam( aDocPt )
        || rSh.IsOverReadOnlyPos( aDocPt )
        || rSh.DocPtInsideInputFld( aDocPt ) )
        return 0;

    SdrObject *pObj = NULL;
    const ObjCntType eType = rSh.GetObjCntType( aDocPt, pObj );

	//Drop auf OutlinerView (TextEdit im Drawing) soll diese selbst entscheiden!
	if( pObj )
	{
		OutlinerView* pOLV = rSh.GetDrawView()->GetTextEditOutlinerView();
		if ( pOLV )
		{
			Rectangle aRect( pOLV->GetOutputArea() );
			aRect.Union( pObj->GetLogicRect() );
			const Point aPos = pOLV->GetWindow()->PixelToLogic( rPixPnt );
			if( aRect.IsInside( aPos ) )
				return 0;
		}
	}

	//Auf was wollen wir denn gerade droppen?
	sal_uInt16 nDropDestination = 0;

	//Sonst etwas aus der DrawingEngine getroffen?
	if( OBJCNT_NONE != eType )
	{
		switch ( eType )
		{
		case OBJCNT_GRF:
			{
				sal_Bool bLink,
					bIMap = 0 != rSh.GetFmtFromObj( aDocPt )->GetURL().GetMap();
				String aDummy;
				rSh.GetGrfAtPos( aDocPt, aDummy, bLink );
				if ( bLink && bIMap )
					nDropDestination = EXCHG_DEST_DOC_LNKD_GRAPH_W_IMAP;
				else if ( bLink )
					nDropDestination = EXCHG_DEST_DOC_LNKD_GRAPHOBJ;
				else if ( bIMap )
					nDropDestination = EXCHG_DEST_DOC_GRAPH_W_IMAP;
				else
					nDropDestination = EXCHG_DEST_DOC_GRAPHOBJ;
			}
			break;
		case OBJCNT_FLY:
			if( rSh.GetView().GetDocShell()->ISA(SwWebDocShell) )
				nDropDestination = EXCHG_DEST_DOC_TEXTFRAME_WEB;
			else
				nDropDestination = EXCHG_DEST_DOC_TEXTFRAME;
			break;
		case OBJCNT_OLE:		nDropDestination = EXCHG_DEST_DOC_OLEOBJ; break;
		case OBJCNT_CONTROL:	/* no Action avail */
		case OBJCNT_SIMPLE:		nDropDestination = EXCHG_DEST_DOC_DRAWOBJ; break;
		case OBJCNT_URLBUTTON:	nDropDestination = EXCHG_DEST_DOC_URLBUTTON; break;
		case OBJCNT_GROUPOBJ:	nDropDestination = EXCHG_DEST_DOC_GROUPOBJ;		break;

		default: ASSERT( sal_False, "new ObjectType?" );
		}
	}
	if ( !nDropDestination )
	{
		if( rSh.GetView().GetDocShell()->ISA(SwWebDocShell) )
			nDropDestination = EXCHG_DEST_SWDOC_FREE_AREA_WEB;
		else
			nDropDestination = EXCHG_DEST_SWDOC_FREE_AREA;
	}
	if( ppObj )
		*ppObj = pObj;
	return nDropDestination;
}

sal_Int8 SwEditWin::AcceptDrop( const AcceptDropEvent& rEvt )
{
	if( rEvt.mbLeaving )
	{
		DropCleanup();
		return rEvt.mnAction;
	}

	if( rView.GetDocShell()->IsReadOnly() )
		return DND_ACTION_NONE;

	SwWrtShell &rSh = rView.GetWrtShell();

	Point aPixPt( rEvt.maPosPixel );

	// If the cursor is near the inner boundary
	// we attempt to scroll towards the desired direction.
	Point aPoint;
	Rectangle aWin(aPoint,GetOutputSizePixel());
	const int nMargin = 10;
	aWin.Left() += nMargin;
	aWin.Top() += nMargin;
	aWin.Right() -= nMargin;
	aWin.Bottom() -= nMargin;
	if(!aWin.IsInside(aPixPt)) {
		static sal_uLong last_tick = 0;
		sal_uLong current_tick = Time::GetSystemTicks();
		if((current_tick-last_tick) > 500) {
			last_tick = current_tick;
			if(!bOldIdleSet) {
				bOldIdle = rSh.GetViewOptions()->IsIdle();
				((SwViewOption *)rSh.GetViewOptions())->SetIdle(sal_False);
				bOldIdleSet = sal_True;
			}
			CleanupDropUserMarker();
			if(aPixPt.X() > aWin.Right()) aPixPt.X() += nMargin;
			if(aPixPt.X() < aWin.Left()) aPixPt.X() -= nMargin;
			if(aPixPt.Y() > aWin.Bottom()) aPixPt.Y() += nMargin;
			if(aPixPt.Y() < aWin.Top()) aPixPt.Y() -= nMargin;
			Point aDocPt(PixelToLogic(aPixPt));
			SwRect rect(aDocPt,Size(1,1));
			rSh.MakeVisible(rect);
		}
	}

	if(bOldIdleSet) {
		((SwViewOption *)rSh.GetViewOptions())->SetIdle( bOldIdle );
		bOldIdleSet = sal_False;
	}

	SdrObject *pObj = NULL;
    m_nDropDestination = GetDropDestination( aPixPt, &pObj );
    if( !m_nDropDestination )
		return DND_ACTION_NONE;

	sal_uInt16 nEventAction;
	sal_Int8 nUserOpt = rEvt.mbDefault ? EXCHG_IN_ACTION_DEFAULT
									   : rEvt.mnAction;

    m_nDropAction = SotExchange::GetExchangeAction(
								GetDataFlavorExVector(),
                                m_nDropDestination,
								rEvt.mnAction,
//!!								rEvt.GetSourceOptions(),
                                nUserOpt, m_nDropFormat, nEventAction );

    if( EXCHG_INOUT_ACTION_NONE != m_nDropAction )
	{
		const Point aDocPt( PixelToLogic( aPixPt ) );

		//Bei den default Aktionen wollen wir noch ein bischen mitreden.
		SwModule *pMod = SW_MOD();
		if( pMod->pDragDrop )
		{
			sal_Bool bCleanup = sal_False;
			//Zeichenobjekte in Kopf-/Fusszeilen sind nicht erlaubt

			SwWrtShell *pSrcSh = pMod->pDragDrop->GetShell();
			if( (pSrcSh->GetSelFrmType() == FRMTYPE_DRAWOBJ) &&
                pSrcSh->IsSelContainsControl() &&
				 (rSh.GetFrmType( &aDocPt, sal_False ) & (FRMTYPE_HEADER|FRMTYPE_FOOTER)) )
			{
				bCleanup = sal_True;
			}
			// keine positionsgeschuetzten Objecte verschieben!
			else if( DND_ACTION_MOVE == rEvt.mnAction &&
					 pSrcSh->IsSelObjProtected( FLYPROTECT_POS ) )
			{
				bCleanup = sal_True;
			}
			else if( rEvt.mbDefault )
			{
				// JP 13.08.98: internes Drag&Drop: bei gleichem Doc ein Move
				//				ansonten ein Copy - Task 54974
				nEventAction = pSrcSh->GetDoc() == rSh.GetDoc()
									? DND_ACTION_MOVE
									: DND_ACTION_COPY;
			}
			if ( bCleanup )
			{
				CleanupDropUserMarker();
				rSh.UnSetVisCrsr();
				return DND_ACTION_NONE;
			}
		}
		else
		{
			//D&D von ausserhalb des SW soll per default ein Copy sein.
			if( EXCHG_IN_ACTION_DEFAULT == nEventAction &&
				DND_ACTION_MOVE == rEvt.mnAction )
				nEventAction = DND_ACTION_COPY;

            if( (SOT_FORMATSTR_ID_SBA_FIELDDATAEXCHANGE == m_nDropFormat &&
                 EXCHG_IN_ACTION_LINK == m_nDropAction) ||
                 SOT_FORMATSTR_ID_SBA_CTRLDATAEXCHANGE == m_nDropFormat  )
			{
				SdrMarkView* pMView = PTR_CAST( SdrMarkView, rSh.GetDrawView() );
				if( pMView && !pMView->IsDesignMode() )
					return DND_ACTION_NONE;
			}
		}

		if ( EXCHG_IN_ACTION_DEFAULT != nEventAction )
            nUserOpt = (sal_Int8)nEventAction;

		// show DropCursor or UserMarker ?
        if( EXCHG_DEST_SWDOC_FREE_AREA_WEB == m_nDropDestination ||
            EXCHG_DEST_SWDOC_FREE_AREA == m_nDropDestination )
		{
			CleanupDropUserMarker();
            SwContentAtPos aCont( SwContentAtPos::SW_CONTENT_CHECK );
            if(rSh.GetContentAtPos(aDocPt, aCont))
                rSh.SwCrsrShell::SetVisCrsr( aDocPt );
		}
		else
		{
			rSh.UnSetVisCrsr();

			if ( pUserMarkerObj != pObj )
			{
				CleanupDropUserMarker();
				pUserMarkerObj = pObj;

				if(pUserMarkerObj)
				{
					pUserMarker = new SdrDropMarkerOverlay( *rSh.GetDrawView(), *pUserMarkerObj );
				}
			}
		}
		return nUserOpt;
	}

	CleanupDropUserMarker();
	rSh.UnSetVisCrsr();
//!!    return SFX_APP()->AcceptDrop( rEvt );
	return DND_ACTION_NONE;
}


IMPL_LINK( SwEditWin, DDHandler, Timer *, EMPTYARG )
{
	bDDTimerStarted = sal_False;
	aTimer.Stop();
	aTimer.SetTimeout(240);
	bMBPressed = sal_False;
	ReleaseMouse();
	bFrmDrag = sal_False;

	if ( rView.GetViewFrame() )
	{
		bExecuteDrag = sal_True;
		StartExecuteDrag();
	}
	return 0;
}



