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



PRJ=..

PRJNAME=sal
TARGET=saltest
TARGETTYPE=CUI
LIBTARGET=NO
USE_LDUMP2=TRUE
ENABLE_EXCEPTIONS=TRUE
#LDUMP2=LDUMP3


# --- Settings -----------------------------------------------------
.INCLUDE :  settings.mk
CFLAGS+= $(LFS_CFLAGS)
CXXFLAGS+= $(LFS_CFLAGS)
# ------------------------------------------------------------------

.IF "$(GUI)"=="WNT"
BOOTSTRAPSCRIPT=bootstrap.bat
BOOTSTRAPINI=testbootstrap.ini
MY_SCRIPTCAT=cat
.ELSE
BOOTSTRAPSCRIPT=bootstrap
BOOTSTRAPINI=testbootstraprc
MY_SCRIPTCAT=tr -d "\015" <
.ENDIF

APP2OBJS = $(OBJ)$/testbootstrap.obj

OBJFILES= \
	$(APP2OBJS) 

APP2TARGET = testbootstrap
APP2STDLIBS = $(SALLIB)

APP3OBJS = $(OBJ)$/test_salmain.obj
APP3TARGET = test_salmain
APP3STDLIBS = $(SALLIB)

APP4OBJS = $(OBJ)$/test_salmainwithargs.obj
APP4TARGET = test_salmainwithargs
APP4STDLIBS = $(SALLIB)


# --- Targets ------------------------------------------------------
.IF "$(depend)" == ""
ALL : ALLTAR \
	  $(BIN)$/$(BOOTSTRAPSCRIPT) \
	  $(BIN)$/$(BOOTSTRAPINI)    \
	  $(BIN)$/bootstraptest.ini  \
	  $(BIN)$/$(APP2TARGET).bin  \
	  $(BIN)$/$(APP2TARGET).Bin  \
	  $(BIN)$/$(APP2TARGET).Exe  \
	  $(BIN)$/bootstrap.pl  


.IF "$(GUI)"=="UNX"
ALL:  $(BIN)$/$(APP2TARGET).exe \
	  $(BIN)$/inirc \
	  $(BIN)$/defaultrc

$(BIN)$/$(APP2TARGET).exe : $(APP2TARGETN)
	cp $(APP2TARGETN) $@

$(BIN)$/inirc:
		echo "CUSTOMINIVALUE=auxaux"   > $@
		echo "INHERITED_OVERWRITTEN_VALUE=inherited_overwritten_value" >> $@

$(BIN)$/defaultrc:
		echo "Default=defaultValue" > $@

.ELSE

ALL:  $(BIN)$/ini.ini		\
	  $(BIN)$/default.ini

$(BIN)$/ini.ini:
		echo CUSTOMINIVALUE=auxaux     > $@
		echo INHERITED_OVERWRITTEN_VALUE=inherited_overwritten_value   >> $@


$(BIN)$/default.ini:
		echo Default=defaultValue > $@

.ENDIF

$(BIN)$/bootstrap.pl:
	cp bootstrap.pl $@

$(BIN)$/$(APP2TARGET).bin : $(APP2TARGETN)
	cp $(APP2TARGETN) $@

$(BIN)$/$(APP2TARGET).Bin : $(APP2TARGETN)
	cp $(APP2TARGETN) $@

$(BIN)$/$(APP2TARGET).Exe : $(APP2TARGETN)
	cp $(APP2TARGETN) $@

.ELSE
ALL: 	ALLDEP
.ENDIF

.INCLUDE :  target.mk


$(BIN)$/$(BOOTSTRAPSCRIPT) : $(BOOTSTRAPSCRIPT)
	$(MY_SCRIPTCAT) $(BOOTSTRAPSCRIPT) > $@
.IF "$(GUI)"!="WNT"
	chmod ug+x $@
.ENDIF

$(BIN)$/$(BOOTSTRAPINI) : $(APP2TARGET).ini
	$(MY_SCRIPTCAT) $(APP2TARGET).ini > $@	

$(BIN)$/bootstraptest.ini : bootstraptest.ini
	$(MY_SCRIPTCAT) bootstraptest.ini > $@	


# --- SO2-Filter-Datei ---
$(MISC)$/tsl$(DLLPOSTFIX).flt:
	@echo ------------------------------
	@echo Making: $@
	@echo WEP>$@
	@echo LIBMAIN>>$@
	@echo LibMain>>$@
