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



package complex.dataPilot;

import com.sun.star.sheet.XDataPilotTable;
import com.sun.star.table.CellAddress;
import com.sun.star.table.CellRangeAddress;
import com.sun.star.table.XCell;
// import lib.Status;
//import lib.StatusException;
import lib.TestParameters;
// import share.LogWriter;

/**
* Testing <code>com.sun.star.sheet.XDataPilotTable</code>
* interface methods :
* <ul>
*  <li><code> getOutputRange()</code></li>
*  <li><code> refresh()</code></li>
* </ul> <p>
* This test needs the following object relations :
* <ul>
*  <li> <code>'OUTPUTRANGE'</code> (of type <code>CellAddress</code>):
*   to check value returned by method <code>getOutputRange()</code> </li>
*  <li> <code>'CELLFORCHANGE'</code> (of type <code>XCell</code>):
*   to check the method refresh(value of this cell will be changed)</li>
*  <li> <code>'CELLFORCHECK'</code> (of type <code>XCell</code>):
* to check the method refresh (value of this cell must be changed after refresh
* call) </li><ul> <p>
* @see com.sun.star.sheet.XDataPilotTable
* @see com.sun.star.table.CellAddress
*/
public class _XDataPilotTable {

    public XDataPilotTable oObj = null;
    XCell xCellForChange = null;
    XCell xCellForCheck = null;
    CellAddress OutputRange = null;
    int changeValue = 0;
    
    /**
     * The test parameters
     */
    private TestParameters param = null;

    /**
     * The log writer
     */
    // private LogWriter log = null;
    
    /**
     * Constructor: gets the object to test, a logger and the test parameters
     * @param xObj The test object

     * @param param The test parameters
     */
    public _XDataPilotTable(XDataPilotTable xObj/*,
                                    LogWriter log*/, TestParameters param) {
        oObj = xObj;
        // this.log = log;
        this.param = param;
    }

    public boolean before() {
        xCellForChange = (XCell)param.get("CELLFORCHANGE");
        xCellForCheck = (XCell)param.get("CELLFORCHECK");
        OutputRange = (CellAddress)param.get("OUTPUTRANGE");
        changeValue = ((Integer)param.get("CHANGEVALUE")).intValue();

        if (xCellForChange == null || OutputRange == null ||
                xCellForCheck == null) {
            System.out.println("Relation not found");
            return false;
        }
        return true;
    }
    /**
    * Test calls the method and checks returned value using value obtained by
    * object relation <code>'OUTPUTRANGE'</code>. <p>
    * Has <b> OK </b> status if values are equal. <p>
     * @return
     */
    public boolean _getOutputRange(){
        boolean bResult = true;
        CellRangeAddress objRange = oObj.getOutputRange();
        bResult &= OutputRange.Sheet == objRange.Sheet;
        bResult &= OutputRange.Row == objRange.StartRow;
        bResult &= OutputRange.Column == objRange.StartColumn;
        return bResult;
    }

    /**
    * Test sets new value of the cell obtained by object relation
    * 'CELLFORCHANGE', and checks value of the cell obtained by object
    * relation 'CELLFORCHECK'.<p>
    * Has <b>OK</b> status if value of the cell obtained by object relation
    * 'CELLFORCHECK' is changed. <p>
     * @return
     */
    public boolean _refresh(){
        xCellForChange.setValue(changeValue);
        double oldData = xCellForCheck.getValue();
        oObj.refresh();
        double newData = xCellForCheck.getValue();
        System.out.println("Old data:" + oldData + "; new data:" + newData);

        return oldData != newData;
    }
}

