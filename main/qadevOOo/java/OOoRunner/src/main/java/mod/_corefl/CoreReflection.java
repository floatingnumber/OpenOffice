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



package mod._corefl;

import java.io.PrintWriter;

import lib.StatusException;
import lib.TestCase;
import lib.TestEnvironment;
import lib.TestParameters;

import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.uno.XInterface;

/**
* Test for object which is represented by service
* <code>com.sun.star.reflection.CoreReflection</code>. <p>
* Object implements the following interfaces :
* <ul>
*  <li> <code>com::sun::star::reflection::XIdlReflection</code></li>
* </ul>
* @see com.sun.star.reflection.XIdlReflection
* @see ifc.reflection._XIdlReflection
*/
public class CoreReflection extends TestCase {

    /**
    * Does nothing.
    */
    protected void initialize ( TestParameters Param, PrintWriter log) {

    }

    /**
    * Creating a Testenvironment for the interfaces to be tested.
    * Creates <code>com.sun.star.reflection.CoreReflection</code> service.
    */
    public TestEnvironment createTestEnvironment(
        TestParameters Param, PrintWriter log) throws StatusException {
        XInterface oObj = null;
        Object oInterface = null;

        try {
            XMultiServiceFactory xMSF = (XMultiServiceFactory)Param.getMSF();
            oInterface = xMSF.createInstance
                ("com.sun.star.reflection.CoreReflection");
        }
        catch(com.sun.star.uno.Exception e) {
            e.printStackTrace(log);
            log.println("CoreReflection Service not available" );
        }

        oObj = (XInterface) oInterface;

        log.println( "    creating a new environment for CoreReflection object" );
        TestEnvironment tEnv = new TestEnvironment( oObj );

        return tEnv;
    } // finish method getTestEnvironment

}    // finish class CoreReflection

