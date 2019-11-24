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
#ifndef _IPDF_PNGHELPER_HXX
#define _IPDF_PNGHELPER_HXX

#include "sal/types.h"
#include "pdfioutdev_gpl.hxx"


namespace pdfi
{
    class PngHelper
    {
        static sal_uInt32 crc_table[ 256 ];
        static bool bCRCTableInit;
        
        static void initCRCTable();
        static void appendFileHeader( OutputBuffer& o_rOutputBuf );
        static size_t startChunk( const char* pChunkName, OutputBuffer& o_rOut );
        static void endChunk( size_t nStart, OutputBuffer& o_rOut );
     
        static void set( sal_uInt32 i_nValue, OutputBuffer& o_rOutputBuf, size_t i_nIndex );
        static void append( sal_uInt32 i_nValue, OutputBuffer& o_rOutputBuf )
        {
            size_t nCur = o_rOutputBuf.size();
            o_rOutputBuf.insert( o_rOutputBuf.end(), 4, (Output_t)0 );
            set( i_nValue, o_rOutputBuf, nCur );
        }
        
        static void appendIHDR( OutputBuffer& o_rOutputBuf, int width, int height, int depth, int colortype );
        static void appendIEND( OutputBuffer& o_rOutputBuf );
        
    public:
        static void updateCRC( sal_uInt32& io_rCRC, const sal_uInt8* i_pBuf, size_t i_nLen );
        static sal_uInt32 getCRC( const sal_uInt8* i_pBuf, size_t i_nLen );
        
        // deflates the passed buff i_pBuf and appends it to the output vector
        // returns the number of bytes added to the output
        static sal_uInt32 deflateBuffer( const Output_t* i_pBuf, size_t i_nLen, OutputBuffer& o_rOut );
        
        static void createPng( OutputBuffer&     o_rOutputBuf,
                               Stream*           str,
                               int               width,
                               int               height,
                               GfxRGB&           zeroColor,
                               GfxRGB&           oneColor,
                               bool              bIsMask
                               );
        static void createPng( OutputBuffer& o_rOutputBuf,
                               Stream* str,
                               int width, int height, GfxImageColorMap* colorMap,
                               Stream* maskStr,
                               int maskWidth, int maskHeight, GfxImageColorMap* maskColorMap );

        // for one bit masks
        static void createPng( OutputBuffer& o_rOutputBuf,
                               Stream* str,
                               int width, int height, GfxImageColorMap* colorMap,
                               Stream* maskStr,
                               int maskWidth, int maskHeight, bool maskInvert );

    };
}

#endif
