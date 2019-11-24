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



#include "jvmaccess/virtualmachine.hxx"

#include <iostream>

// Modify source/virtualmachine.cxx before running this test, by letting the
// AttachGuard ctor directly throw a CreationExcepiton (without even setting the
// m_xMachine member).

int main()
{
    try
    {
        jvmaccess::VirtualMachine::AttachGuard aGuard(0);
    }
    catch (jvmaccess::VirtualMachine::AttachGuard::CreationException &)
    {
        std::cout << "Exception caught" << std::endl;
    }
}
