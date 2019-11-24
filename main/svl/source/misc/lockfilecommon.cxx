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
#include "precompiled_svl.hxx"

#include <stdio.h>

#include <com/sun/star/ucb/XSimpleFileAccess.hpp>
#include <com/sun/star/ucb/XCommandEnvironment.hpp>
#include <com/sun/star/ucb/InsertCommandArgument.hpp>
#include <com/sun/star/ucb/NameClashException.hpp>
#include <com/sun/star/io/WrongFormatException.hpp>

#include <osl/time.h>
#include <osl/security.hxx>
#include <osl/socket.hxx>
#include <osl/file.hxx>

#include <rtl/string.hxx>
#include <rtl/ustring.hxx>
#include <rtl/strbuf.hxx>
#include <rtl/ustrbuf.hxx>

#include <comphelper/processfactory.hxx>

#include <tools/urlobj.hxx>
#include <unotools/bootstrap.hxx>

#include <ucbhelper/content.hxx>

#include <unotools/useroptions.hxx>

#include <svl/lockfilecommon.hxx>

using namespace ::com::sun::star;

namespace svt {

// ----------------------------------------------------------------------
LockFileCommon::LockFileCommon( const ::rtl::OUString& aOrigURL, const uno::Reference< lang::XMultiServiceFactory >& xFactory, const ::rtl::OUString& aPrefix )
: m_xFactory( xFactory )
{
    if ( !m_xFactory.is() )
        m_xFactory = ::comphelper::getProcessServiceFactory();

    INetURLObject aDocURL = ResolveLinks( INetURLObject( aOrigURL ) );

    ::rtl::OUString aShareURLString = aDocURL.GetPartBeforeLastName();
    aShareURLString += aPrefix;
    aShareURLString += aDocURL.GetName();
    aShareURLString += ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "#" ) );
    m_aURL = INetURLObject( aShareURLString ).GetMainURL( INetURLObject::NO_DECODE );
}

// ----------------------------------------------------------------------
LockFileCommon::~LockFileCommon()
{
}

// ----------------------------------------------------------------------
INetURLObject LockFileCommon::ResolveLinks( const INetURLObject& aDocURL )
{
    if ( aDocURL.HasError() )
        throw lang::IllegalArgumentException();

    ::rtl::OUString aURLToCheck = aDocURL.GetMainURL( INetURLObject::NO_DECODE );

    sal_Bool bNeedsChecking = sal_True;
    sal_Int32 nMaxLinkCount = 128;
    sal_Int32 nCount = 0;

    while( bNeedsChecking )
    {
        bNeedsChecking = sal_False;

        // do not allow too deep links
        if ( nCount++ >= nMaxLinkCount )
            throw io::IOException();

        // there is currently no UCB functionality to resolve the symbolic links;
        // since the lock files are used only for local file systems the osl functionality is used directly

        ::osl::FileStatus aStatus( FileStatusMask_Type | FileStatusMask_LinkTargetURL );
        ::osl::DirectoryItem aItem;
        if ( ::osl::FileBase::E_None == ::osl::DirectoryItem::get( aURLToCheck, aItem )
          && aItem.is() && ::osl::FileBase::E_None == aItem.getFileStatus( aStatus ) )
        {
            if ( aStatus.isValid( FileStatusMask_Type )
              && aStatus.isValid( FileStatusMask_LinkTargetURL )
              && aStatus.getFileType() == ::osl::FileStatus::Link )
            {
                aURLToCheck = aStatus.getLinkTargetURL();
                bNeedsChecking = sal_True;
            }
        }
    }

    return INetURLObject( aURLToCheck );
}

// ----------------------------------------------------------------------
uno::Sequence< uno::Sequence< ::rtl::OUString > > LockFileCommon::ParseList( const uno::Sequence< sal_Int8 >& aBuffer )
{
    sal_Int32 nCurPos = 0;
    sal_Int32 nCurEntry = 0;
    uno::Sequence< uno::Sequence< ::rtl::OUString > > aResult( 10 );

    while ( nCurPos < aBuffer.getLength() )
    {
        if ( nCurEntry >= aResult.getLength() )
            aResult.realloc( nCurEntry + 10 );
        aResult[nCurEntry] = ParseEntry( aBuffer, nCurPos );
        nCurEntry++;
    }

    aResult.realloc( nCurEntry );
    return aResult;
}

// ----------------------------------------------------------------------
uno::Sequence< ::rtl::OUString > LockFileCommon::ParseEntry( const uno::Sequence< sal_Int8 >& aBuffer, sal_Int32& io_nCurPos )
{
    uno::Sequence< ::rtl::OUString > aResult( LOCKFILE_ENTRYSIZE );

    for ( int nInd = 0; nInd < LOCKFILE_ENTRYSIZE; nInd++ )
    {
        aResult[nInd] = ParseName( aBuffer, io_nCurPos );
        if ( io_nCurPos >= aBuffer.getLength()
          || ( nInd < LOCKFILE_ENTRYSIZE - 1 && aBuffer[io_nCurPos++] != ',' )
          || ( nInd == LOCKFILE_ENTRYSIZE - 1 && aBuffer[io_nCurPos++] != ';' ) )
            throw io::WrongFormatException();
    }

    return aResult;
}

// ----------------------------------------------------------------------
::rtl::OUString LockFileCommon::ParseName( const uno::Sequence< sal_Int8 >& aBuffer, sal_Int32& io_nCurPos )
{
    ::rtl::OStringBuffer aResult;
    sal_Bool bHaveName = sal_False;
    sal_Bool bEscape = sal_False;

    while( !bHaveName )
    {
        if ( io_nCurPos >= aBuffer.getLength() )
            throw io::WrongFormatException();

        if ( bEscape )
        {
            if ( aBuffer[io_nCurPos] == ',' || aBuffer[io_nCurPos] == ';' || aBuffer[io_nCurPos] == '\\' )
                aResult.append( (sal_Char)aBuffer[io_nCurPos] );
            else
                throw io::WrongFormatException();

            bEscape = sal_False;
            io_nCurPos++;
        }
        else if ( aBuffer[io_nCurPos] == ',' || aBuffer[io_nCurPos] == ';' )
            bHaveName = sal_True;
        else
        {
            if ( aBuffer[io_nCurPos] == '\\' )
                bEscape = sal_True;
            else
                aResult.append( (sal_Char)aBuffer[io_nCurPos] );

            io_nCurPos++;
        }
    }

    return ::rtl::OStringToOUString( aResult.makeStringAndClear(), RTL_TEXTENCODING_UTF8 );
}

// ----------------------------------------------------------------------
::rtl::OUString LockFileCommon::EscapeCharacters( const ::rtl::OUString& aSource )
{
    ::rtl::OUStringBuffer aBuffer;
    const sal_Unicode* pStr = aSource.getStr();
    for ( sal_Int32 nInd = 0; nInd < aSource.getLength() && pStr[nInd] != 0; nInd++ )
    {
        if ( pStr[nInd] == '\\' || pStr[nInd] == ';' || pStr[nInd] == ',' )
            aBuffer.append( (sal_Unicode)'\\' );
        aBuffer.append( pStr[nInd] );
    }

    return aBuffer.makeStringAndClear();
}

// ----------------------------------------------------------------------
::rtl::OUString LockFileCommon::GetOOOUserName()
{
    SvtUserOptions aUserOpt;
    ::rtl::OUString aName = aUserOpt.GetFirstName();
    if ( aName.getLength() )
        aName += ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( " " ) );
    aName += aUserOpt.GetLastName();

    return aName;
}

// ----------------------------------------------------------------------
::rtl::OUString LockFileCommon::GetCurrentLocalTime()
{
    ::rtl::OUString aTime;

    TimeValue aSysTime;
    if ( osl_getSystemTime( &aSysTime ) )
    {
        TimeValue aLocTime;
        if ( osl_getLocalTimeFromSystemTime( &aSysTime, &aLocTime ) )
        {
            oslDateTime aDateTime;
            if ( osl_getDateTimeFromTimeValue( &aLocTime, &aDateTime ) )
            {
                char pDateTime[20];
                sprintf( pDateTime, "%02d.%02d.%4d %02d:%02d", aDateTime.Day, aDateTime.Month, aDateTime.Year, aDateTime.Hours, aDateTime.Minutes );
                aTime = ::rtl::OUString::createFromAscii( pDateTime );
            }
        }
    }

    return aTime;
}

// ----------------------------------------------------------------------
uno::Sequence< ::rtl::OUString > LockFileCommon::GenerateOwnEntry()
{
    uno::Sequence< ::rtl::OUString > aResult( LOCKFILE_ENTRYSIZE );

    aResult[LOCKFILE_OOOUSERNAME_ID] = GetOOOUserName();

    ::osl::Security aSecurity;
    aSecurity.getUserName( aResult[LOCKFILE_SYSUSERNAME_ID] );

    aResult[LOCKFILE_LOCALHOST_ID] = ::osl::SocketAddr::getLocalHostname();

    aResult[LOCKFILE_EDITTIME_ID] = GetCurrentLocalTime();

    ::utl::Bootstrap::locateUserInstallation( aResult[LOCKFILE_USERURL_ID] );


    return aResult;
}

} // namespace svt

