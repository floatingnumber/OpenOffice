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



package mod._sc;

import java.io.PrintWriter;

import lib.StatusException;
import lib.TestCase;
import lib.TestEnvironment;
import lib.TestParameters;
import util.SOfficeFactory;

import com.sun.star.container.XEnumerationAccess;
import com.sun.star.container.XIndexAccess;
import com.sun.star.frame.XController;
import com.sun.star.frame.XModel;
import com.sun.star.lang.XComponent;
import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.sheet.XSpreadsheetDocument;
import com.sun.star.uno.AnyConverter;
import com.sun.star.uno.Type;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.XInterface;

public class ScIndexEnumeration_SpreadsheetViewPanesEnumeration extends TestCase {
    private static XSpreadsheetDocument xSpreadsheetDoc;
    private static SOfficeFactory SOF;
    private static XInterface oObj;

    /**
    * Creates Spreadsheet document.
    */
    public void initialize( TestParameters Param, PrintWriter log ) {
        // get a soffice factory object
        SOF = SOfficeFactory.getFactory( (XMultiServiceFactory)Param.getMSF());

        try {
            log.println("creating a spreadsheetdocument");
            xSpreadsheetDoc = SOF.createCalcDoc(null);
        } catch (com.sun.star.uno.Exception e) {
            e.printStackTrace( log );
            throw new StatusException( "Couldn't create document ", e );
        }
    }

    /**
    * Disposes Spreadsheet document.
    */
    protected void cleanup( TestParameters tParam, PrintWriter log ) {
        log.println("disposing xSpreadsheetDocument");
        XComponent oComp = (XComponent)
            UnoRuntime.queryInterface(XComponent.class, xSpreadsheetDoc);
        util.DesktopTools.closeDoc(oComp);
    }

    protected TestEnvironment createTestEnvironment(TestParameters Param, PrintWriter log) {

        XModel xm = (XModel)
            UnoRuntime.queryInterface(XModel.class, xSpreadsheetDoc);
        XController xc = xm.getCurrentController();
        XIndexAccess xIA = (XIndexAccess)
            UnoRuntime.queryInterface(XIndexAccess.class, xc);
        try {
            oObj = (XInterface) AnyConverter.toObject(
                        new Type(XInterface.class),xIA.getByIndex(0));
        } catch (com.sun.star.lang.WrappedTargetException e) {
            e.printStackTrace(log);
            throw new StatusException("Couldn't get by index", e);
        } catch (com.sun.star.lang.IndexOutOfBoundsException e) {
            e.printStackTrace(log);
            throw new StatusException("Couldn't get by index", e);
        } catch (com.sun.star.lang.IllegalArgumentException e) {
            e.printStackTrace(log);
            throw new StatusException("Couldn't get by index", e);
        }

        XEnumerationAccess ea = (XEnumerationAccess)
                    UnoRuntime.queryInterface(XEnumerationAccess.class,xIA);

        oObj = ea.createEnumeration();

        log.println("ImplementationName: "+util.utils.getImplName(oObj));
        // creating test environment
        TestEnvironment tEnv = new TestEnvironment( oObj );

        tEnv.addObjRelation("ENUM",ea);

        return tEnv;
    }
}

