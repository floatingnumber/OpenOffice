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


#ifndef _I18N_TRANSLITERATION_TRANSLITERATION_NUMERIC_H_
#define _I18N_TRANSLITERATION_TRANSLITERATION_NUMERIC_H_

#include <transliteration_commonclass.hxx>

namespace com { namespace sun { namespace star { namespace i18n {

class transliteration_Numeric : public transliteration_commonclass {
public:
        virtual ::rtl::OUString SAL_CALL
        transliterate( const ::rtl::OUString& inStr, sal_Int32 startPos, sal_Int32 nCount, ::com::sun::star::uno::Sequence< sal_Int32 >& offset )
        throw(::com::sun::star::uno::RuntimeException);

        virtual sal_Unicode SAL_CALL
        transliterateChar2Char( sal_Unicode inChar)
        throw(com::sun::star::i18n::MultipleCharsOutputException,
                com::sun::star::uno::RuntimeException);

        // Methods which are shared.
        virtual sal_Int16 SAL_CALL getType(  ) throw(::com::sun::star::uno::RuntimeException);
    
        virtual ::rtl::OUString SAL_CALL
        folding( const ::rtl::OUString& inStr, sal_Int32 startPos, sal_Int32 nCount, ::com::sun::star::uno::Sequence< sal_Int32 >& offset )
        throw(::com::sun::star::uno::RuntimeException);

        virtual sal_Bool SAL_CALL
        equals( const ::rtl::OUString& str1, sal_Int32 pos1, sal_Int32 nCount1, sal_Int32& nMatch1, const ::rtl::OUString& str2, sal_Int32 pos2, sal_Int32 nCount2, sal_Int32& nMatch2 )
        throw(::com::sun::star::uno::RuntimeException);

        virtual ::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL 
        transliterateRange( const ::rtl::OUString& str1, const ::rtl::OUString& str2 )
        throw(::com::sun::star::uno::RuntimeException);
protected:
        sal_Int16 nNativeNumberMode;
        sal_Int16 tableSize;
        sal_Unicode* table;
        sal_Bool recycleSymbol;
private:
        rtl::OUString SAL_CALL 
        transliterateBullet( const rtl::OUString& inStr, sal_Int32 startPos, sal_Int32 nCount,
        com::sun::star::uno::Sequence< sal_Int32 >& offset ) 
        throw(com::sun::star::uno::RuntimeException);
};

} } } }

#endif // _I18N_TRANSLITERATION_TRANSLITERATION_NUMERIC_H_
