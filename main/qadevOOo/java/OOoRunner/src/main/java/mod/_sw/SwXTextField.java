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



package mod._sw;

import java.io.PrintWriter;

import lib.StatusException;
import lib.TestCase;
import lib.TestEnvironment;
import lib.TestParameters;
import util.SOfficeFactory;

import com.sun.star.beans.XPropertySet;
import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.text.XDependentTextField;
import com.sun.star.text.XText;
import com.sun.star.text.XTextContent;
import com.sun.star.text.XTextCursor;
import com.sun.star.text.XTextDocument;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.XInterface;

/**
 *
 * initial description
 * @see com.sun.star.lang.XComponent
 * @see com.sun.star.text.TextContent
 * @see com.sun.star.text.XTextContent
 * @see com.sun.star.text.XTextField
 *
 */
public class SwXTextField extends TestCase {

    XTextDocument xTextDoc;

    /**
     * in general this method creates a testdocument
     *
     *  @param tParam    class which contains additional test parameters
     *  @param log        class to log the test state and result
     *
     *
     *  @see TestParameters
     *    @see PrintWriter
     *
     */
    protected void initialize( TestParameters tParam, PrintWriter log ) {
        SOfficeFactory SOF = SOfficeFactory.getFactory( (XMultiServiceFactory)tParam.getMSF() );

        try {
            log.println( "creating a textdocument" );
            xTextDoc = SOF.createTextDoc( null );
        } catch ( com.sun.star.uno.Exception e ) {
            // Some exception occured.FAILED
            e.printStackTrace( log );
            throw new StatusException( "Couldn't create document", e );
        }
    }

    /**
     * in general this method disposes the testenvironment and document
     *
     *  @param tParam    class which contains additional test parameters
     *  @param log        class to log the test state and result
     *
     *
     *  @see TestParameters
     *    @see PrintWriter
     *
     */
    protected void cleanup( TestParameters tParam, PrintWriter log ) {
        log.println( "    disposing xTextDoc " );
        util.DesktopTools.closeDoc(xTextDoc);
    }


    /**
     *    creating a Testenvironment for the interfaces to be tested
     *
     *  @param tParam    class which contains additional test parameters
     *  @param log        class to log the test state and result
     *
     *  @return    Status class
     *
     *  @see TestParameters
     *    @see PrintWriter
     */
    protected TestEnvironment createTestEnvironment(TestParameters tParam, PrintWriter log) {

        XInterface oObj = null;

        // creation of testobject here
        // first we write what we are intend to do to log file
        log.println( "creating a test environment" );

        Object instance = null;

          // create testobject here
        try {
            XMultiServiceFactory oDocMSF = (XMultiServiceFactory)
                UnoRuntime.queryInterface(XMultiServiceFactory.class, xTextDoc);

            Object FieldMaster = oDocMSF.createInstance
                ( "com.sun.star.text.FieldMaster.Database" );
            XPropertySet PFieldMaster = (XPropertySet) UnoRuntime.queryInterface
                (XPropertySet.class,(XInterface) FieldMaster);
            oObj = (XInterface) oDocMSF.createInstance
                ( "com.sun.star.text.TextField.Database" );

            instance = (XInterface) oDocMSF.createInstance
                ( "com.sun.star.text.TextField.DateTime" );

            XDependentTextField xTF = (XDependentTextField)
                UnoRuntime.queryInterface(XDependentTextField.class,oObj);

            PFieldMaster.setPropertyValue("DataBaseName","Address Book File");
            PFieldMaster.setPropertyValue("DataTableName","address");
            PFieldMaster.setPropertyValue("DataColumnName","FIRSTNAME");
            XText the_Text = xTextDoc.getText();
            XTextCursor the_Cursor = the_Text.createTextCursor();
            XTextContent the_Field = (XTextContent)
                             UnoRuntime.queryInterface(XTextContent.class,oObj);

            xTF.attachTextFieldMaster(PFieldMaster);
            the_Text.insertTextContent(the_Cursor,the_Field,false);
        }
        catch (Exception ex) {
            log.println("Couldn't create instance");
            ex.printStackTrace(log);
        }

        log.println( "creating a new environment for FieldMaster object" );
        TestEnvironment tEnv = new TestEnvironment( oObj );


        tEnv.addObjRelation("CONTENT", (XTextContent)
                        UnoRuntime.queryInterface(XTextContent.class,instance));
        tEnv.addObjRelation("RANGE", xTextDoc.getText().createTextCursor());

        return tEnv;
    } // finish method getTestEnvironment
}    // finish class SwXTextField

