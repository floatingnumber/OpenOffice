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
#include "precompiled_shell.hxx"
#include "internal/global.hxx"
#include "zipexcptn.hxx"

//------------------------------------------
/**
*/
RuntimeException::RuntimeException(int Error) : 
    m_Error(Error)
{
}

//------------------------------------------
/**
*/
RuntimeException::~RuntimeException() throw()
{
}

//------------------------------------------
/**
*/
int RuntimeException::GetErrorCode() const
{
	return m_Error;
}

//------------------------------------------
/**
*/
ZipException::ZipException(int Error) : 
	RuntimeException(Error)
{
}

//------------------------------------------
/**
*/
const char* ZipException::what() const throw()
{
	return 0;
}

//------------------------------------------
/**
*/
Win32Exception::Win32Exception(int Error) :
	RuntimeException(Error),
	m_MsgBuff(0)
{
}

//------------------------------------------
/**
*/
Win32Exception::~Win32Exception() throw()
{
#ifndef OS2
	if (m_MsgBuff)
		LocalFree(m_MsgBuff);
#endif
}

//------------------------------------------
/**
*/
const char* Win32Exception::what() const throw()
{
#ifdef OS2
	return "Win32Exception!";
#else
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetErrorCode(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &m_MsgBuff,
		0,
		NULL);
		
	return reinterpret_cast<char*>(m_MsgBuff);
#endif
}

//------------------------------------------
/**
*/
ZipContentMissException::ZipContentMissException(int Error) :
	ZipException(Error)
{
}

//------------------------------------------
/**
*/
AccessViolationException::AccessViolationException(int Error) :
	Win32Exception(Error)
{
}

//------------------------------------------
/**
*/
IOException::IOException(int Error) :
	Win32Exception(Error)
{
}
