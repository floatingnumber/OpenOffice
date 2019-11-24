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



#ifndef _SV_SALVD_H
#define _SV_SALVD_H

#include "premac.h"
#include <ApplicationServices/ApplicationServices.h>
#include "postmac.h"

#include "aqua/salconst.h"
#include "aqua/salcolorutils.hxx"
#include "aqua/salgdi.h"

#include "salvd.hxx"

#if PRAGMA_ONCE
	#pragma once
#endif

// =======================================================================

// =======================================================================

// -----------------
// - SalVirDevData -
// -----------------

struct SalVirDevData
{
};

typedef struct SalVirDevData   SalVirDevData;
typedef SalVirDevData         *SalVirDevDataPtr;
typedef SalVirDevDataPtr      *SalVirDevDataHandle;

// =======================================================================

class AquaSalGraphics;

// -----------------
// - SalVirDevData -
// -----------------

class AquaSalVirtualDevice : public SalVirtualDevice
{
private:
	bool mbGraphicsUsed;             // is Graphics used
    bool mbForeignContext;           // is mxContext from outside VCL
    CGContextRef mxBitmapContext;
    int mnBitmapDepth;
	CGLayerRef mxLayer;              // Quartz layer
	AquaSalGraphics* mpGraphics;     // current VirDev graphics

	void Destroy();

public:
    AquaSalVirtualDevice( AquaSalGraphics* pGraphic, long nDX, long nDY, sal_uInt16 nBitCount, const SystemGraphicsData *pData );
    virtual ~AquaSalVirtualDevice();
    
    virtual SalGraphics*			GetGraphics();
    virtual void					ReleaseGraphics( SalGraphics* pGraphics );
    virtual sal_Bool                    SetSize( long nNewDX, long nNewDY );
    virtual void                       GetSize( long& rWidth, long& rHeight );
};

// =======================================================================

#endif // _SV_SALVD_H
