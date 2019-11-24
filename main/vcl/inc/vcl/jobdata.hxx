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



#ifndef _PSPRINT_JOBDATA_HXX_
#define _PSPRINT_JOBDATA_HXX_

#include "vcl/ppdparser.hxx"

namespace psp {

namespace orientation {
enum type {
    Portrait,
    Landscape
};
}

struct VCL_DLLPUBLIC JobData
{
    int                     m_nCopies;
    int                     m_nLeftMarginAdjust;
    int                     m_nRightMarginAdjust;
    int                     m_nTopMarginAdjust;
    int                     m_nBottomMarginAdjust;
    // user overrides for PPD
    int                     m_nColorDepth;
    int                     m_nPSLevel;     // 0: no override, else languaglevel to use
    int                     m_nColorDevice; // 0: no override, -1 grey scale, +1 color
    int                     m_nPDFDevice;   // 0: PostScript, 1: PDF
    orientation::type       m_eOrientation;
    ::rtl::OUString         m_aPrinterName;
    const PPDParser*        m_pParser;
    PPDContext              m_aContext;

    JobData() :
            m_nCopies( 1 ),
            m_nLeftMarginAdjust( 0 ),
            m_nRightMarginAdjust( 0 ),
            m_nTopMarginAdjust( 0 ),
            m_nBottomMarginAdjust( 0 ),
            m_nColorDepth( 24 ),
            m_nPSLevel( 0 ),
            m_nColorDevice( 0 ),
            m_nPDFDevice( 0 ),
            m_eOrientation( orientation::Portrait ),
            m_pParser( NULL ) {}

    JobData& operator=(const psp::JobData& rRight);

    JobData( const JobData& rData ) { *this = rData; }
    
    void setCollate( bool bCollate );
    bool setPaper( int nWidth, int nHeight ); // dimensions in pt
    bool setPaperBin( int nPaperBin ); // dimensions in pt

    // creates a new buffer using new
    // it is up to the user to delete it again
    bool getStreamBuffer( void*& pData, int& bytes );
    static bool constructFromStreamBuffer( void* pData, int bytes, JobData& rJobData );
};

} // namespace


#endif // PSPRINT_JOBDATA_HXX
