#**************************************************************
#  
#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
#  
#**************************************************************



PRJ=..$/..$/..

PRJNAME=filter
TARGET=expm
DEPTARGET=vexpm


# --- Settings -----------------------------------------------------------

.INCLUDE :	settings.mk

# --- Allgemein ----------------------------------------------------------

.IF "$(editdebug)"!="" || "$(EDITDEBUG)"!=""
CDEFS+= -DEDITDEBUG
.ENDIF
.IF "$(L10N_framework)"==""
SLOFILES =	$(SLO)$/expm.obj

# ==========================================================================

SHL1TARGET= 	exp$(DLLPOSTFIX)
SHL1IMPLIB= 	expm
SHL1STDLIBS=	$(VCLLIB) $(TOOLSLIB) $(SALLIB) $(SVTOOLLIB)
.IF "$(GUI)" == "OS2"
SHL1STDLIBS+=   $(CPPULIB)
.ENDIF
SHL1LIBS=		$(SLB)$/expm.lib

.IF "$(GUI)" != "UNX"
.IF "$(COM)" != "GCC"
SHL1OBJS=		$(SLO)$/expm.obj
.ENDIF
.ENDIF

SHL1VERSIONMAP=exports.map
SHL1DEF=		$(MISC)$/$(SHL1TARGET).def
DEF1NAME=$(SHL1TARGET)
.ENDIF
# ==========================================================================

.INCLUDE :	target.mk
