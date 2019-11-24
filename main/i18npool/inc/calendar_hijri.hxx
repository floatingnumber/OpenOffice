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


#ifndef _I18N_CALENDAR_HIJRI_HXX_
#define _I18N_CALENDAR_HIJRI_HXX_

#include "calendar_gregorian.hxx"


//	----------------------------------------------------
//	class Calendar_hijri
//	----------------------------------------------------

namespace com { namespace sun { namespace star { namespace i18n {

class Calendar_hijri : public Calendar_gregorian
{
public:

	// Constructors
	Calendar_hijri();

protected:
	void mapToGregorian() throw(com::sun::star::uno::RuntimeException);
	void mapFromGregorian() throw(com::sun::star::uno::RuntimeException);

	// radians per degree (pi/180)
	static const double RadPerDeg;

	// Synodic Period (mean time between 2 successive new moon: 29d, 12 hr, 44min, 3sec
	static const double SynPeriod;

	static const double SynMonth;			// Solar days in a year/SynPeriod

	// Julian day on Jan 1, 1900
	static const double jd1900;

	// Reference point: September 1984 25d 3h 10m UT. == 1405 Hijri == 1048 Synodial month from 1900
	static const sal_Int32 SynRef;
	static const sal_Int32 GregRef;

	// Local time (Saudi Arabia)
	static const double SA_TimeZone;		// Time Zone

	// Period between 1.30pm - 6:30pm
	static const double EveningPeriod;
	
	// "Leap" years
	static const sal_Int32 LeapYear[];

private:
	double NewMoon(sal_Int32 n);
	void getHijri(sal_Int32 *day, sal_Int32 *month, sal_Int32 *year);
	void ToGregorian(sal_Int32 *day, sal_Int32 *month, sal_Int32 *year);
	void getGregorianDay(sal_Int32 jd, sal_Int32 *pnDay, sal_Int32 *pnMonth, sal_Int32 *pnYear);
	double getJulianDay(sal_Int32 day, sal_Int32 month, sal_Int32 year);
};

} } } }

#endif
