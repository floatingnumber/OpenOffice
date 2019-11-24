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



#ifndef _FRAMEWORK_SCRIPT_PROVIDER_PROVIDERCACHE_HXX_
#define _FRAMEWORK_SCRIPT_PROVIDER_PROVIDERCACHE_HXX_

#include <hash_map>
#include <osl/mutex.hxx>
#include <rtl/ustring.hxx>
#include <cppuhelper/implbase1.hxx>
#include <com/sun/star/uno/RuntimeException.hpp>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/lang/XSingleServiceFactory.hpp>
#include <com/sun/star/lang/XSingleComponentFactory.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>

#include <com/sun/star/frame/XModel.hpp>
#include <com/sun/star/script/provider/XScriptProvider.hpp>

#include "ScriptingContext.hxx"

namespace func_provider
{
// for simplification
#define css ::com::sun::star

//Typedefs
//=============================================================================

struct ProviderDetails
{
    //css::uno::Reference< css::lang::XSingleServiceFactory > factory;
    css::uno::Reference< css::lang::XSingleComponentFactory > factory;
    css::uno::Reference< css::script::provider::XScriptProvider > provider;
};
typedef ::std::hash_map < ::rtl::OUString, ProviderDetails , ::rtl::OUStringHash,
            ::std::equal_to< ::rtl::OUString > > ProviderDetails_hash;


class ProviderCache 
{

public:
     ProviderCache( const css::uno::Reference< css::uno::XComponentContext >& xContext, const css::uno::Sequence< css::uno::Any >& scriptContext )
        throw ( css::uno::RuntimeException );
     ProviderCache( const css::uno::Reference< css::uno::XComponentContext >& xContext, const css::uno::Sequence< css::uno::Any >& scriptContext,
        const css::uno::Sequence< ::rtl::OUString >& blackList )
        throw ( css::uno::RuntimeException );
    ~ProviderCache();
     css::uno::Reference< css::script::provider::XScriptProvider > 
         getProvider( const ::rtl::OUString& providerName );
     css::uno::Sequence < css::uno::Reference< css::script::provider::XScriptProvider > >
         getAllProviders() throw ( css::uno::RuntimeException );
private:
    void populateCache() 
        throw ( css::uno::RuntimeException );

   css::uno::Reference< css::script::provider::XScriptProvider >
        createProvider( ProviderDetails& details ) throw ( css::uno::RuntimeException );
    bool isInBlackList( const ::rtl::OUString& serviceName )
    {
        if ( m_sBlackList.getLength() > 0 )
        {
            for ( sal_Int32 index = 0; index < m_sBlackList.getLength(); index++ )
            {
                if ( m_sBlackList[ index ].equals( serviceName ) )
                {
                    return true;
                }
            }
        }
        return false;
    }
    css::uno::Sequence< ::rtl::OUString >  m_sBlackList; 
    ProviderDetails_hash  m_hProviderDetailsCache;
    osl::Mutex m_mutex;
    css::uno::Sequence< css::uno::Any >  m_Sctx; 
    css::uno::Reference< css::uno::XComponentContext > m_xContext;
    css::uno::Reference< css::lang::XMultiComponentFactory > m_xMgr;


};
} // func_provider
#endif
