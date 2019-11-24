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


#ifndef _SVX_GRFCROP_HXX
#define _SVX_GRFCROP_HXX

#include <svl/poolitem.hxx>
#include "svx/svxdllapi.h"


#define GRFCROP_VERSION_SWDEFAULT		0
#define GRFCROP_VERSION_MOVETOSVX 		1

class SVX_DLLPUBLIC SvxGrfCrop : public SfxPoolItem
{
	sal_Int32	nLeft, nRight, nTop, nBottom;
public:
    SvxGrfCrop( sal_uInt16  );
	SvxGrfCrop( sal_Int32 nLeft,	sal_Int32 nRight,
				sal_Int32 nTop,		sal_Int32 nBottom,
                sal_uInt16  );
	virtual ~SvxGrfCrop();

	// "pure virtual Methoden" vom SfxPoolItem
	virtual int            		operator==( const SfxPoolItem& ) const;
	virtual SfxPoolItem*		Create(SvStream &, sal_uInt16 nVer) const;
	virtual SvStream&			Store(SvStream &, sal_uInt16 nIVer) const;
	virtual SfxItemPresentation GetPresentation( SfxItemPresentation ePres,
									SfxMapUnit eCoreMetric,
									SfxMapUnit ePresMetric,
									String &rText,
                                    const IntlWrapper* pIntl = 0 ) const;
	virtual	sal_Bool        	 QueryValue( com::sun::star::uno::Any& rVal,
										sal_uInt8 nMemberId = 0 ) const;
	virtual	sal_Bool			 PutValue( const com::sun::star::uno::Any& rVal,
										sal_uInt8 nMemberId = 0 );

	void SetLeft( sal_Int32 nVal )		{ nLeft = nVal; }
	void SetRight( sal_Int32 nVal )		{ nRight = nVal; }
	void SetTop( sal_Int32 nVal )		{ nTop = nVal; }
	void SetBottom( sal_Int32 nVal )	{ nBottom = nVal; }

	sal_Int32 GetLeft() const			{ return nLeft; }
	sal_Int32 GetRight() const 			{ return nRight; }
	sal_Int32 GetTop() const			{ return nTop; }
	sal_Int32 GetBottom() const			{ return nBottom; }

	inline SvxGrfCrop& operator=( const SvxGrfCrop& rCrop )
		{
			nLeft = rCrop.GetLeft(); 		nTop = rCrop.GetTop();
			nRight = rCrop.GetRight();		nBottom = rCrop.GetBottom();
			return *this;
		}
};


#endif  // _GRFATR_HXX
