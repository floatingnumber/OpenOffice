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



#ifndef _VCLCANVAS_TOOLS_HXX
#define _VCLCANVAS_TOOLS_HXX

#include <osl/mutex.hxx>
#include <vos/mutex.hxx>

#include <vcl/svapp.hxx>
#include <vcl/outdev.hxx>

#include <basegfx/polygon/b2dpolypolygon.hxx>

#include <com/sun/star/uno/Reference.hxx>
#include <com/sun/star/uno/Sequence.hxx>

#include <canvas/vclwrapper.hxx>
#include "outdevprovider.hxx"


class OutputDevice;
class Point;
class Size;

namespace basegfx 
{
    namespace matrix
    {
        class B2DHomMatrix;
    }
    namespace vector
    {
        class B2DVector;
    }
    namespace point
    {
        class B2DPoint;
    }
}

namespace com { namespace sun { namespace star { namespace awt 
{
    struct Point;
    struct Size;
    struct Rectangle;
} } } }

namespace com { namespace sun { namespace star { namespace drawing 
{
    struct HomogenMatrix3;
} } } }

namespace com { namespace sun { namespace star { namespace geometry 
{
    struct RealPoint2D;
    struct RealSize2D;
    struct RealRectangle2D;
} } } }

namespace com { namespace sun { namespace star { namespace rendering 
{
    struct RenderState;
    struct ViewState;
    class  XCanvas;
    class  XBitmap;
    class  XPolyPolygon2D;
} } } }


namespace vclcanvas
{
    namespace tools
    {
        ::BitmapEx
        bitmapExFromXBitmap( const ::com::sun::star::uno::Reference< 
                             ::com::sun::star::rendering::XBitmap >& );

        /** Setup VCL font and output position

        	@returns false, if no text output should happen
         */
        bool setupFontTransform( ::Point&													o_rPoint,
                                 ::Font& 													io_rVCLFont, 
                                 const ::com::sun::star::rendering::ViewState& 		viewState,
                                 const ::com::sun::star::rendering::RenderState& 	renderState,
                                 ::OutputDevice&											rOutDev );

        /** Predicate, to determine whether polygon is actually an axis-aligned rectangle

        	@return true, if the polygon is a rectangle.
         */
        bool isRectangle( const PolyPolygon& rPolyPoly );


        // Little helper to encapsulate locking into policy class
        class LocalGuard
        {
        public:
            LocalGuard() : 
                aGuard( Application::GetSolarMutex() )
            {
        	}

            /// To be compatible with CanvasBase mutex concept
            LocalGuard( const ::osl::Mutex& ) : 
                aGuard( Application::GetSolarMutex() )
            {
        	}

        private:
            ::vos::OGuard aGuard;        
        };

        class OutDevStateKeeper
        {
        public:
            explicit OutDevStateKeeper( OutputDevice& rOutDev ) :
                mpOutDev( &rOutDev ),
                mbMappingWasEnabled( mpOutDev->IsMapModeEnabled() ),
                mnAntiAliasing( mpOutDev->GetAntialiasing() )
            {
                init();
        	}

            explicit OutDevStateKeeper( const OutDevProviderSharedPtr& rOutDev ) :
                mpOutDev( rOutDev.get() ? &(rOutDev->getOutDev()) : NULL ),
                mbMappingWasEnabled( mpOutDev ? mpOutDev->IsMapModeEnabled() : false ),
                mnAntiAliasing( mpOutDev ? mpOutDev->GetAntialiasing() : 0 )
            {
                init();
        	}
 
            ~OutDevStateKeeper()
            {
                if( mpOutDev )
                {
                    mpOutDev->EnableMapMode( mbMappingWasEnabled );
                    mpOutDev->SetAntialiasing( mnAntiAliasing );
                    
                    mpOutDev->Pop();
                }
        	}

        private:
            void init()
            {
                if( mpOutDev )
                {
                    mpOutDev->Push();
                    mpOutDev->EnableMapMode(sal_False);
                    mpOutDev->SetAntialiasing( ANTIALIASING_ENABLE_B2DDRAW );
                }
            }

            OutputDevice*	    mpOutDev;
            const bool		    mbMappingWasEnabled;
            const sal_uInt16    mnAntiAliasing;
        };

        ::Point mapRealPoint2D( const ::com::sun::star::geometry::RealPoint2D& 	rPoint,
                                const ::com::sun::star::rendering::ViewState& 	rViewState,
                                const ::com::sun::star::rendering::RenderState&	rRenderState );

        ::PolyPolygon mapPolyPolygon( const ::basegfx::B2DPolyPolygon& 							rPoly,
                                      const ::com::sun::star::rendering::ViewState& 	rViewState,
                                      const ::com::sun::star::rendering::RenderState&	rRenderState );

        enum ModulationMode
        {
            MODULATE_NONE,
            MODULATE_WITH_DEVICECOLOR
        };

        ::BitmapEx transformBitmap( const BitmapEx& 									rBitmap,
                                    const ::basegfx::B2DHomMatrix& 						rTransform,
                                    const ::com::sun::star::uno::Sequence< double >&	rDeviceColor,
                                    ModulationMode										eModulationMode );

	}
}

#endif /* _VCLCANVAS_TOOLS_HXX */
