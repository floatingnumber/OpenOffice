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



#ifndef _SV_ACCEL_H
#define _SV_ACCEL_H

#include <vcl/sv.h>
#ifndef _SV_KEYCOD_HXX
#include <vcl/keycod.hxx>
#endif

class Accelerator;

// ------------------
// - ImplAccelEntry -
// ------------------

class ImplAccelEntry
{
public:
	sal_uInt16			mnId;
	KeyCode 		maKeyCode;
	Accelerator*	mpAccel;
	Accelerator*	mpAutoAccel;
	sal_Bool			mbEnabled;
};

// -----------------
// - Hilfemethoden -
// -----------------

// in KEYCOD.CXX
void ImplGetKeyCode( KeyFuncType eFunc, sal_uInt16& rCode1, sal_uInt16& rCode2, sal_uInt16& rCode3, sal_uInt16& rCode4 );

#endif	// _SV_ACCEL_H
