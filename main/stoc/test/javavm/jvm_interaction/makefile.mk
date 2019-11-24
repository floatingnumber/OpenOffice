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
TARGET= jvminteraction
PRJNAME=	stoc
TARGET6=	$(TARGET)
TARGETTYPE=	CUI
LIBTARGET=	NO
ENABLE_EXCEPTIONS=TRUE
NO_BSYMBOLIC=	TRUE

UNOUCRDEP=	$(SOLARBINDIR)$/udkapi.rdb
UNOUCRRDB=	$(SOLARBINDIR)$/udkapi.rdb
UNOUCROUT=	$(OUT)$/inc$
INCPRE+=	$(OUT)$/inc$



# --- Settings -----------------------------------------------------
.INCLUDE :  settings.mk

# --- Application 6 - testjavavm ------------------------------------

UNOTYPES= com.sun.star.lang.XMultiComponentFactory \
	com.sun.star.uno.XWeak	\
	com.sun.star.java.XJavaVM	\
	com.sun.star.java.XJavaThreadRegister_11	\
	com.sun.star.java.JavaNotConfiguredException \
	com.sun.star.java.MissingJavaRuntimeException \
	com.sun.star.java.JavaDisabledException \
	com.sun.star.java.JavaVMCreationFailureException \
	com.sun.star.registry.XSimpleRegistry \
	com.sun.star.lang.XComponent \
	com.sun.star.registry.XImplementationRegistration \
	com.sun.star.lang.XSingleServiceFactory \
	com.sun.star.uno.TypeClass \
	com.sun.star.lang.XMultiServiceFactory \
	com.sun.star.uno.XCurrentContext \
	com.sun.star.task.XInteractionHandler \
	com.sun.star.task.XInteractionRequest \
	com.sun.star.task.XInteractionContinuation \
	com.sun.star.task.XInteractionAbort \
	com.sun.star.task.XInteractionRetry \

	
APP6TARGET= 	$(TARGET6)
APP6OBJS  = 	$(OBJ)$/interactionhandler.obj 
APP6STDLIBS= \
		$(CPPULIB) 		\
		$(CPPUHELPERLIB) 	\
		$(SALHELPERLIB) 	\
		$(SALLIB)	

# --- Target ------------------------------------------------

.INCLUDE :	target.mk

