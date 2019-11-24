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
#include "precompiled_canvas.hxx"

#include <canvas/debug.hxx>
#include <canvas/canvastools.hxx>
#include <toolkit/helper/vclunohelper.hxx>
#include <vcl/canvastools.hxx>
#include <basegfx/tools/canvastools.hxx>
#include <vcl/dibtools.hxx>

#include "spritedevicehelper.hxx"
#include "spritecanvas.hxx"
#include "spritecanvashelper.hxx"
#include "canvasbitmap.hxx"

using namespace ::com::sun::star;

namespace vclcanvas
{
    SpriteDeviceHelper::SpriteDeviceHelper() :
        mpBackBuffer()
    {
    }

    void SpriteDeviceHelper::init( const OutDevProviderSharedPtr& pOutDev )
    {
        DeviceHelper::init(pOutDev);

        // setup back buffer
        OutputDevice& rOutDev( pOutDev->getOutDev() );
        mpBackBuffer.reset( new BackBuffer( rOutDev ));
        mpBackBuffer->setSize( rOutDev.GetOutputSizePixel() );

        // #i95645#
#if defined( QUARTZ )
        // use AA on VCLCanvas for Mac
        mpBackBuffer->getOutDev().SetAntialiasing( ANTIALIASING_ENABLE_B2DDRAW | mpBackBuffer->getOutDev().GetAntialiasing() );
#else
        // switch off AA for WIN32 and UNIX, the VCLCanvas does not look good with it and
        // is not required to do AA. It would need to be adapted to use it correctly 
        // (especially gradient painting). This will need extra work.
        mpBackBuffer->getOutDev().SetAntialiasing(mpBackBuffer->getOutDev().GetAntialiasing() & ~ANTIALIASING_ENABLE_B2DDRAW);
#endif
    }

    ::sal_Int32 SpriteDeviceHelper::createBuffers( ::sal_Int32 nBuffers )
    {
        (void)nBuffers;

        // TODO(F3): implement XBufferStrategy interface. For now, we
        // _always_ will have exactly one backbuffer
        return 1;
    }

    void SpriteDeviceHelper::destroyBuffers()
    {
        // TODO(F3): implement XBufferStrategy interface. For now, we
        // _always_ will have exactly one backbuffer
    }

    ::sal_Bool SpriteDeviceHelper::showBuffer( bool, ::sal_Bool )
    {
        OSL_ENSURE(false,"Not supposed to be called, handled by SpriteCanvas");
        return sal_False;
    }

    ::sal_Bool SpriteDeviceHelper::switchBuffer( bool, ::sal_Bool )
    {
        OSL_ENSURE(false,"Not supposed to be called, handled by SpriteCanvas");
        return sal_False;
    }

    void SpriteDeviceHelper::disposing()
    {
        // release all references
        mpBackBuffer.reset();

        DeviceHelper::disposing();
    }
       
    uno::Any SpriteDeviceHelper::isAccelerated() const
    {
        return DeviceHelper::isAccelerated();
    }
  
    uno::Any SpriteDeviceHelper::getDeviceHandle() const
    {
        return DeviceHelper::getDeviceHandle();
    }
  
    uno::Any SpriteDeviceHelper::getSurfaceHandle() const
    {
        if( !mpBackBuffer )
            return uno::Any();

        return uno::makeAny( 
            reinterpret_cast< sal_Int64 >(&mpBackBuffer->getOutDev()) );
    }

    void SpriteDeviceHelper::notifySizeUpdate( const awt::Rectangle& rBounds )
    {
        if( mpBackBuffer )
            mpBackBuffer->setSize( ::Size(rBounds.Width, 
                                          rBounds.Height) );
    }

    void SpriteDeviceHelper::dumpScreenContent() const
    {
        DeviceHelper::dumpScreenContent();

        static sal_uInt32 nFilePostfixCount(0);

        if( mpBackBuffer )
        {
            String aFilename( String::CreateFromAscii("dbg_backbuffer") );
            aFilename += String::CreateFromInt32(nFilePostfixCount);
            aFilename += String::CreateFromAscii(".bmp");
                
            SvFileStream aStream( aFilename, STREAM_STD_READWRITE );
                
            const ::Point aEmptyPoint;
            mpBackBuffer->getOutDev().EnableMapMode( sal_False );
            mpBackBuffer->getOutDev().SetAntialiasing( ANTIALIASING_ENABLE_B2DDRAW );
            WriteDIB(mpBackBuffer->getOutDev().GetBitmap(aEmptyPoint, mpBackBuffer->getOutDev().GetOutputSizePixel()), aStream, false, true);
        }

        ++nFilePostfixCount;
    }
    
}
