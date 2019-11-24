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



#include <salhelper/dynload.hxx>
#include <rtl/ustrbuf.hxx>

namespace salhelper
{     

typedef	void*	(SAL_CALL *ApiInitFunction) (void);

ORealDynamicLoader::ORealDynamicLoader(ORealDynamicLoader ** ppSetToZeroInDestructor_,
				       const rtl::OUString& moduleName,
				       const rtl::OUString& initFunction,
				       void* pApi,
				       oslModule pModule)
	: m_pApi(pApi)
	, m_refCount(1)
	, m_pModule(pModule)
	, m_strModuleName(moduleName)
	, m_strInitFunction(initFunction)
	, ppSetToZeroInDestructor( ppSetToZeroInDestructor_ )
{
}

ORealDynamicLoader* ORealDynamicLoader::newInstance(ORealDynamicLoader ** ppSetToZeroInDestructor,
								  const rtl::OUString& moduleName,
								  const rtl::OUString& initFunction)
{
	ApiInitFunction initFunc;
	oslModule pModule = osl_loadModule(moduleName.pData, SAL_LOADMODULE_DEFAULT);	
	
	if ( !pModule )
	{
		return NULL;
	}
	
	initFunc = (ApiInitFunction)osl_getFunctionSymbol(
        pModule, initFunction.pData);

	if ( !initFunc )
	{
		osl_unloadModule(pModule);
		return NULL;
	}

	return(new ORealDynamicLoader(ppSetToZeroInDestructor, moduleName, 
								 initFunction,
								 initFunc(),
								 pModule));
}

ORealDynamicLoader::~ORealDynamicLoader()
{
	// set the address to zero
	if( ppSetToZeroInDestructor )
		*ppSetToZeroInDestructor = 0;

	if (m_pModule) 
	{
		osl_unloadModule(m_pModule);	
		m_pModule = NULL;
	}
}	

sal_uInt32 ORealDynamicLoader::acquire()
{ 
	return ++m_refCount; 
}

sal_uInt32 ORealDynamicLoader::release()
{ 
	sal_uInt32 nRet = --m_refCount; 
	if( nRet == 0 )
		delete this;
	return nRet; 
}


void* ORealDynamicLoader::getApi() const
{ 
	return m_pApi;		
}

} // namespace salhelper

