
INC_G_DIR	= ../../AgentHandler/include
INC_I_DIR	= ../include

OUT_DIR	= .

CC              = g++

INCS		= -I${INC_G_DIR} -I${INC_I_DIR} 

SRCS		=	MarlinCdmInterface.cpp \
				MarlinCdmEngine.cpp \
				CdmSessionManager.cpp

OBJS		= ${SRCS:%.cpp=$(OUT_DIR)/%.o}

CFLAGS += $(ARCH_CFLAGS)
CFLAGS += -c -Wall

$(OBJS):$(SRCS)
	${CC} ${CFLAGS} ${INCS} ${SRCS}

clean:
	\rm -f ${OBJS}


#
# 2015 - Copyright Marlin Trust Management Organization
#
