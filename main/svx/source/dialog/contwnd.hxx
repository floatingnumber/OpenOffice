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



#ifndef _CONTWND_HXX
#define _CONTWND_HXX

#include <tools/poly.hxx>
#include <svx/graphctl.hxx>

/*************************************************************************
|*
|*
|*
\************************************************************************/

class ContourWindow : public GraphCtrl
{
	PolyPolygon			aPolyPoly;
	Color				aPipetteColor;
	Rectangle			aWorkRect;
	Link				aPipetteLink;
	Link				aPipetteClickLink;
	Link				aWorkplaceClickLink;
	sal_Bool				bPipetteMode;
	sal_Bool				bWorkplaceMode;
	sal_Bool				bClickValid;

protected:

	virtual void		MouseButtonDown(const MouseEvent& rMEvt);
	virtual void		MouseMove(const MouseEvent& rMEvt);
	virtual void		MouseButtonUp(const MouseEvent& rMEvt);
	virtual void		SdrObjCreated( const SdrObject& rObj );
	virtual	void		InitSdrModel();
	virtual	void		Paint( const Rectangle& rRect );

	void				CreatePolyPolygon();

public:

						ContourWindow( Window* pParent, const ResId& rResId );
						~ContourWindow();

	void				SetPolyPolygon( const PolyPolygon& rPolyPoly );
	const PolyPolygon&	GetPolyPolygon();

	void				SetPipetteMode( const sal_Bool bPipette ) { bPipetteMode = bPipette; }
	sal_Bool				IsPipetteMode() const { return bPipetteMode; }
	const Color&		GetPipetteColor() const { return aPipetteColor; }

	sal_Bool				IsClickValid() const { return bClickValid; }
	sal_Bool				IsContourChanged() const;

	void				SetWorkplaceMode( const sal_Bool bWorkplace ) { bWorkplaceMode = bWorkplace; }
	sal_Bool				IsWorkplaceMode() const { return bWorkplaceMode; }
	const Rectangle&	GetWorkRect() const { return aWorkRect; }

	void				SetPipetteHdl( const Link& rLink ) { aPipetteLink = rLink; }
	void				SetPipetteClickHdl( const Link& rLink ) { aPipetteClickLink = rLink; }

	void				SetWorkplaceClickHdl( const Link& rLink ) { aWorkplaceClickLink = rLink; }
};


#endif

