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
#include "precompiled_extensions.hxx"

#include "ldapaccess.hxx"
#include "ldapuserprofilebe.hxx"
#include <osl/file.hxx>
#include <osl/module.hxx>
#include <osl/process.h>
#include <rtl/ustrbuf.hxx>
#include <rtl/byteseq.h>

#ifndef INCLUDED_RTL_INSTANCE_HXX_
#include <rtl/instance.hxx>
#endif
#include <com/sun/star/beans/NamedValue.hpp>
#include <com/sun/star/beans/Optional.hpp>
#include <osl/security.hxx>

//==============================================================================
namespace extensions { namespace config { namespace ldap {

LdapUserProfileBe::LdapUserProfileBe( const uno::Reference<uno::XComponentContext>& xContext)
: LdapProfileMutexHolder(),
  BackendBase(mMutex)
{
    LdapDefinition aDefinition;
    rtl::OUString loggedOnUser;

    // This whole rigmarole is to prevent an infinite recursion where reading
    // the configuration for the backend would create another instance of the
    // backend, which would try and read the configuration which would...
    {
        osl::Mutex & aInitMutex = rtl::Static< osl::Mutex, LdapUserProfileBe >::get();
        osl::MutexGuard aInitGuard(aInitMutex);
    
        static bool bReentrantCall; // = false
        OSL_ENSURE(!bReentrantCall, "configuration: Ldap Backend constructor called reentrantly - probably a registration error.");

        if (!bReentrantCall)
        {
            try
            {
                bReentrantCall = true ;
                if (!readLdapConfiguration(
                        css::uno::Reference< css::lang::XMultiServiceFactory >(
                            xContext->getServiceManager(),
                            css::uno::UNO_QUERY_THROW),
                        &aDefinition, &loggedOnUser))
                {
                    throw css::uno::RuntimeException(
                        rtl::OUString::createFromAscii("LdapUserProfileBe- LDAP not configured"),           
                        NULL); 	
                }
                    
                bReentrantCall = false ;
            }
            catch (...)
            {
                bReentrantCall = false;
                throw;
            }
        }
    }

    LdapConnection connection;
    connection.loadModule();
    connection.connectSimple(aDefinition);
    connection.getUserProfile(loggedOnUser, &data_);
}
//------------------------------------------------------------------------------
LdapUserProfileBe::~LdapUserProfileBe() 
{
}
//------------------------------------------------------------------------------

bool LdapUserProfileBe::readLdapConfiguration(
    css::uno::Reference< css::lang::XMultiServiceFactory > const & factory,
    LdapDefinition * definition, rtl::OUString * loggedOnUser)
{
    OSL_ASSERT(factory.is() && definition != 0 && loggedOnUser != 0);
    const rtl::OUString kConfigurationProviderService( RTL_CONSTASCII_USTRINGPARAM("com.sun.star.configuration.ConfigurationProvider")) ;
    const rtl::OUString kReadOnlyViewService( RTL_CONSTASCII_USTRINGPARAM("com.sun.star.configuration.ConfigurationAccess")) ;
    const rtl::OUString kComponent( RTL_CONSTASCII_USTRINGPARAM("org.openoffice.LDAP/UserDirectory"));
    const rtl::OUString kServerDefiniton(RTL_CONSTASCII_USTRINGPARAM ("ServerDefinition"));
    const rtl::OUString kServer(RTL_CONSTASCII_USTRINGPARAM ("Server"));
    const rtl::OUString kPort(RTL_CONSTASCII_USTRINGPARAM("Port"));
    const rtl::OUString kBaseDN(RTL_CONSTASCII_USTRINGPARAM("BaseDN"));
    const rtl::OUString kUser(RTL_CONSTASCII_USTRINGPARAM("SearchUser"));
    const rtl::OUString kPassword(RTL_CONSTASCII_USTRINGPARAM("SearchPassword"));
    const rtl::OUString kUserObjectClass(RTL_CONSTASCII_USTRINGPARAM("UserObjectClass"));
    const rtl::OUString kUserUniqueAttr(RTL_CONSTASCII_USTRINGPARAM("UserUniqueAttribute"));

	uno::Reference< XInterface > xIface;
    try
    {
        uno::Reference< lang::XMultiServiceFactory > xCfgProvider(
                                                        factory->createInstance(kConfigurationProviderService), 
                                                        uno::UNO_QUERY);
	    OSL_ENSURE(xCfgProvider.is(),"LdapUserProfileBe: could not create the configuration provider");
	    if (!xCfgProvider.is())
            return false;

        css::beans::NamedValue aPath(rtl::OUString::createFromAscii("nodepath"), uno::makeAny(kComponent) );

        uno::Sequence< uno::Any > aArgs(1);
        aArgs[0] <<=  aPath;

        xIface = xCfgProvider->createInstanceWithArguments(kReadOnlyViewService, aArgs);
	
        uno::Reference<container::XNameAccess > xAccess(xIface, uno::UNO_QUERY_THROW);
        xAccess->getByName(kServerDefiniton) >>= xIface; 

        uno::Reference<container::XNameAccess > xChildAccess(xIface, uno::UNO_QUERY_THROW);
       
        if (!getLdapStringParam(xChildAccess, kServer, definition->mServer))
            return false;
        if (!getLdapStringParam(xChildAccess, kBaseDN, definition->mBaseDN))
            return false;

        definition->mPort=0;
        xChildAccess->getByName(kPort) >>= definition->mPort ;
	    if (definition->mPort == 0)
		    return false;

        if (!getLdapStringParam(xAccess, kUserObjectClass, definition->mUserObjectClass))
            return false;
        if (!getLdapStringParam(xAccess, kUserUniqueAttr, definition->mUserUniqueAttr))
            return false;

        getLdapStringParam(xAccess, kUser, definition->mAnonUser);
        getLdapStringParam(xAccess, kPassword, definition->mAnonCredentials);
    }
    catch (uno::Exception & e)
    {
        OSL_TRACE("LdapUserProfileBackend: access to configuration data failed: %s",
                rtl::OUStringToOString( e.Message, RTL_TEXTENCODING_ASCII_US ).getStr() );
        return false;
    }
    
    osl::Security aSecurityContext;
	if (!aSecurityContext.getUserName(*loggedOnUser))
		OSL_TRACE("LdapUserProfileBackend - could not get Logged on user from system");
	
    sal_Int32 nIndex = loggedOnUser->indexOf('/');
	if (nIndex > 0)
		*loggedOnUser = loggedOnUser->copy(nIndex+1);
	
    //Remember to remove
    OSL_TRACE("Logged on user is %s", rtl::OUStringToOString(*loggedOnUser,RTL_TEXTENCODING_ASCII_US).getStr());

    return true;
}

//------------------------------------------------------------------------------
bool LdapUserProfileBe::getLdapStringParam(
	uno::Reference<container::XNameAccess>& xAccess,
	const rtl::OUString& aLdapSetting,
	rtl::OString& aServerParameter)
{
    rtl::OUString sParam;
    xAccess->getByName(aLdapSetting) >>= sParam;
    aServerParameter = rtl::OUStringToOString(sParam, RTL_TEXTENCODING_ASCII_US);
    
    return aServerParameter.getLength() != 0;
}
//------------------------------------------------------------------------------
void LdapUserProfileBe::setPropertyValue(
    rtl::OUString const &, css::uno::Any const &)
    throw (
        css::beans::UnknownPropertyException, css::beans::PropertyVetoException,
        css::lang::IllegalArgumentException, css::lang::WrappedTargetException,
        css::uno::RuntimeException)
{
    throw css::lang::IllegalArgumentException(
        rtl::OUString(
            RTL_CONSTASCII_USTRINGPARAM("setPropertyValue not supported")),
        static_cast< cppu::OWeakObject * >(this), -1);
}

css::uno::Any LdapUserProfileBe::getPropertyValue(
    rtl::OUString const & PropertyName)
    throw (
        css::beans::UnknownPropertyException, css::lang::WrappedTargetException,
        css::uno::RuntimeException)
{
    for (sal_Int32 i = 0;;) {
        sal_Int32 j = PropertyName.indexOf(',', i);
        if (j == -1) {
            j = PropertyName.getLength();
        }
        if (j == i) {
            throw css::beans::UnknownPropertyException(
                PropertyName, static_cast< cppu::OWeakObject * >(this));
        }
        LdapData::iterator k(data_.find(PropertyName.copy(i, j - i)));
        if (k != data_.end()) {
            return css::uno::makeAny(
                css::beans::Optional< css::uno::Any >(
                    true, css::uno::makeAny(k->second)));
        }
        if (j == PropertyName.getLength()) {
            break;
        }
        i = j + 1;
    }
    return css::uno::makeAny(css::beans::Optional< css::uno::Any >());
}

//------------------------------------------------------------------------------
rtl::OUString SAL_CALL LdapUserProfileBe::getLdapUserProfileBeName(void) {
	return rtl::OUString::createFromAscii("com.sun.star.comp.configuration.backend.LdapUserProfileBe") ;
}
//------------------------------------------------------------------------------

rtl::OUString SAL_CALL LdapUserProfileBe::getImplementationName(void) 
    throw (uno::RuntimeException) 
{
    return getLdapUserProfileBeName() ;
}
//------------------------------------------------------------------------------

uno::Sequence<rtl::OUString> SAL_CALL LdapUserProfileBe::getLdapUserProfileBeServiceNames(void) 
{
    uno::Sequence<rtl::OUString> aServices(1) ;
    aServices[0] = rtl::OUString( RTL_CONSTASCII_USTRINGPARAM("com.sun.star.configuration.backend.LdapUserProfileBe")) ;
    return aServices ;
}
//------------------------------------------------------------------------------

sal_Bool SAL_CALL LdapUserProfileBe::supportsService(const rtl::OUString& aServiceName) 
    throw (uno::RuntimeException) 
{
    uno::Sequence< rtl::OUString > const svc = getLdapUserProfileBeServiceNames();

	for(sal_Int32 i = 0; i < svc.getLength(); ++i )
		if(svc[i] == aServiceName)
			return true;
	return false;
}

//------------------------------------------------------------------------------

uno::Sequence<rtl::OUString> 
SAL_CALL LdapUserProfileBe::getSupportedServiceNames(void) 
    throw (uno::RuntimeException) 
{
    return getLdapUserProfileBeServiceNames() ;
}
// ---------------------------------------------------------------------------------------
}}}
// ---------------------------------------------------------------------------------------


