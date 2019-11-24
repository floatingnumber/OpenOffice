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


#ifndef _NEWHDL_HXX
#define _NEWHDL_HXX

#include <tools/link.hxx>

#include "errhdl.hxx"		// hier kommt auch swtypes rein, was ich brauche

// ----------------------------------------------------------------------------

extern void SW_NEW_HDL();

class SwNewHdl
{
		friend void SW_NEW_HDL();
private:
		char*			pWarnMem;
		char*			pExceptMem;
		Link 			aWarnLnk;
		Link			aErrLnk;
public:
		void	SetWarnLnk( const Link &rLink );
		void	SetErrLnk( const Link &rLink );

		sal_Bool	TryWarnMem();

		sal_Bool 	Flush_WarnMem();
		sal_Bool 	Flush_ExceptMem();

		SwNewHdl();
		~SwNewHdl();
};

/******************************************************************************
 *	INLINE
 ******************************************************************************/
inline void SwNewHdl::SetWarnLnk( const Link &rLink )
{
	aWarnLnk = rLink;
}

inline void SwNewHdl::SetErrLnk( const Link &rLink )
{
	aErrLnk = rLink;
}

#endif

