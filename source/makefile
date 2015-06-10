MAKE        = make
MAKE_DIRS   = ./CDM/src \
              ./AgentHandler/src 

CC          = g++
AR          = ar

TARGET		= libMarlinCdm.a

OBJS		= ./CDM/src/CdmSessionManager.o \
              ./CDM/src/MarlinCdmEngine.o \
              ./CDM/src/MarlinCdmInterface.o \
              ./AgentHandler/src/MarlinAgentHandler.o 

compile:
	@for subdir in $(MAKE_DIRS) ; do \
		(cd $$subdir && $(MAKE)) ;\
	done
	${AR} -r ${TARGET} ${OBJS}

clean:
	@for subdir in $(MAKE_DIRS) ; do \
		(cd $$subdir && $(MAKE) clean) ;\
	done
	@rm -f ${TARGET}


#
# 2015 - Copyright Marlin Trust Management Organization
#
