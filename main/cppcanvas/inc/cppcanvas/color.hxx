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



#ifndef _CPPCANVAS_COLOR_HXX
#define _CPPCANVAS_COLOR_HXX

#include <com/sun/star/uno/Sequence.hxx>

#include <boost/shared_ptr.hpp>


/* Definition of Color class */

namespace cppcanvas
{
    class Color
    {
    public:
        /** Color in the sRGB color space, plus alpha channel

        	The four bytes of the sal_uInt32 are allocated as follows
        	to the color channels and alpha: 0xRRGGBBAA.
         */
        typedef sal_uInt32 IntSRGBA;

        virtual ~Color() {}

        virtual IntSRGBA 									getIntSRGBA( ::com::sun::star::uno::Sequence< double >& rDeviceColor ) const = 0;
        virtual ::com::sun::star::uno::Sequence< double > 	getDeviceColor( IntSRGBA aSRGBA ) const = 0;
    };

    typedef ::boost::shared_ptr< ::cppcanvas::Color > ColorSharedPtr;

    inline sal_uInt8 getRed( Color::IntSRGBA nCol ) 
    { 
        return static_cast<sal_uInt8>( (nCol&0xFF000000U) >> 24U ); 
    }

    inline sal_uInt8 getGreen( Color::IntSRGBA nCol ) 
    { 
        return static_cast<sal_uInt8>( (nCol&0x00FF0000U) >> 16U ); 
    }

    inline sal_uInt8 getBlue( Color::IntSRGBA nCol ) 
    { 
        return static_cast<sal_uInt8>( (nCol&0x0000FF00U) >> 8U ); 
    }

    inline sal_uInt8 getAlpha( Color::IntSRGBA nCol ) 
    { 
        return static_cast<sal_uInt8>( nCol&0x000000FFU ); 
    }

    inline Color::IntSRGBA makeColor( sal_uInt8 nRed, sal_uInt8 nGreen, sal_uInt8 nBlue, sal_uInt8 nAlpha ) 
    { 
        return (nRed << 24U)|(nGreen << 16U)|(nBlue << 8U)|(nAlpha);
    }
    
    inline sal_Int32 unMakeColor( sal_uInt8 nAlpha, sal_uInt8 nRed, sal_uInt8 nGreen, sal_uInt8 nBlue)
    {
        return (nAlpha << 24U)|(nRed << 16U)|(nGreen << 8U)|(nBlue);
    }

    inline sal_Int32 makeColorARGB( sal_uInt8 nAlpha, sal_uInt8 nRed, sal_uInt8 nGreen, sal_uInt8 nBlue)
    {
        return (nAlpha << 24U)|(nRed << 16U)|(nGreen << 8U)|(nBlue);
    }

}

#endif /* _CPPCANVAS_COLOR_HXX */
