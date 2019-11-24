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



package mod._cnt;

import java.io.PrintWriter;

import lib.StatusException;
import lib.TestCase;
import lib.TestEnvironment;
import lib.TestParameters;

import com.sun.star.container.XIndexContainer;
import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.XInterface;

/**
* Test for object which is represented by service
* <code>com.sun.star.cnt.CntUnoDataContainer</code>. <p>
* Object implements the following interfaces :
* <ul>
*  <li> <code>com::sun::star::io::XActiveDataSink</code></li>
*  <li> <code>com::sun::star::container::XIndexContainer</code></li>
*  <li> <code>com::sun::star::ucb::XDataContainer</code></li>
*  <li> <code>com::sun::star::container::XIndexAccess</code></li>
*  <li> <code>com::sun::star::container::XElementAccess</code></li>
*  <li> <code>com::sun::star::container::XIndexReplace</code></li>
* </ul>
* This object test <b> is NOT </b> designed to be run in several
* threads concurently.
* @see com.sun.star.io.XActiveDataSink
* @see com.sun.star.container.XIndexContainer
* @see com.sun.star.ucb.XDataContainer
* @see com.sun.star.container.XIndexAccess
* @see com.sun.star.container.XElementAccess
* @see com.sun.star.container.XIndexReplace
* @see ifc.io._XActiveDataSink
* @see ifc.container._XIndexContainer
* @see ifc.ucb._XDataContainer
* @see ifc.container._XIndexAccess
* @see ifc.container._XElementAccess
* @see ifc.container._XIndexReplace
*/
public class CntUnoDataContainer extends TestCase {

    /**
    * Creating a Testenvironment for the interfaces to be tested.
    * Creates an instance of the service
    * <code>com.sun.star.cnt.CntUnoDataContainer</code> and adds
    * to it one <code>CntUnoDataContainer</code> instance. <p>
    *     Object relations created :
    * <ul>
    *  <li> <code>'INSTANCE1'</code> for
    *      {@link ifc.container._XIndexContainer},
    *      {@link ifc.container._XIndexReplace} : for this object
    *      elements contained in it are <code>CntUnoDataContainer</code>
    *      instances. (only one relation for one interface thread is now
    *      passed).</li>
    *  <li> <code>'InputStream'</code> for
    *      {@link ifc.io._XActiveDataSink} : a stream to set and get.
    *       </li>
    * </ul>
    */
    protected TestEnvironment createTestEnvironment(
                                    TestParameters Param, PrintWriter log) {
        XInterface oObj = null;
        Object oInterface = null;
        XMultiServiceFactory xMSF = (XMultiServiceFactory)Param.getMSF();
        Object relationContainer = null ;
        Object xIn = null;

        try {
            oInterface = xMSF.createInstance
                ( "com.sun.star.ucb.DataContainer" );

            // adding one child container
            XIndexContainer xIC = (XIndexContainer) UnoRuntime.queryInterface
                (XIndexContainer.class, oInterface) ;

            Object child = xMSF.createInstance
                ( "com.sun.star.ucb.DataContainer" );
            xIC.insertByIndex(0, child) ;

            relationContainer = xMSF.createInstance
                ( "com.sun.star.ucb.DataContainer" );

            xIn = xMSF.createInstance
                ( "com.sun.star.io.DataInputStream" );
        }
        catch( com.sun.star.uno.Exception e ) {
            log.println("Can't create an object." );
            throw new StatusException( "Can't create an object", e );
        }

        oObj = (XInterface) oInterface;

        TestEnvironment tEnv = new TestEnvironment( oObj );

        tEnv.addObjRelation("INSTANCE1", relationContainer) ;
        tEnv.addObjRelation("InputStream", xIn) ;

        return tEnv;
    } // finish method getTestEnvironment

}    // finish class BreakIterator

