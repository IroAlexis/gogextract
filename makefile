NAME := ${notdir ${realpath .}}
VERSION := 0.1

CC := gcc
CFLAGS := -Wall -Wextra -pedantic
LDFLAGS := -lzip

MKDIR := mkdir -p
RMDIR := rm -rf
ECHO := echo -e

SRC_PREFIX := ./
SRC_SUFFIX := .c
SRC := ${wildcard ${SRC_PREFIX}*${SRC_SUFFIX}}

DEP_PREFIX := depends/
DEP_SUFFIX := .mk
DEP := ${subst ${SRC_PREFIX},${DEP_PREFIX},${addsuffix ${DEP_SUFFIX},${basename ${SRC}}}}

BIN_PREFIX := ./
BIN_SUFFIX :=
BIN := ${BIN_PREFIX}${NAME}${BIN_SUFFIX}

OBJ_PREFIX := obj/
OBJ_SUFFIX := .o
OBJ := ${addprefix ${OBJ_PREFIX},${addsuffix ${OBJ_SUFFIX},${notdir ${basename ${SRC}}}}}

TAR_PREFIX := ../
TAR_SUFFIX := .tar.gz
TAR := ${TAR_PREFIX}${NAME}-${VERSION}${TAR_SUFFIX}

all: ${DEP} ${BIN}

${BIN}: ${OBJ}
	${MKDIR} ${@D}
	${CC} -o $@ ${LDFLAGS} $^

${OBJ_PREFIX}%${OBJ_SUFFIX}: ${SRC_PREFIX}%${SRC_SUFFIX}
	${MKDIR} ${@D}
	${CC} -o $@ ${CFLAGS} -c $<


${DEP_PREFIX}%${DEP_SUFFIX}: ${SRC_PREFIX}%${SRC_SUFFIX}
	@${ECHO} "Dependance: $*"
	${MKDIR} ${@D}
	${CC} -o $@ ${CFLAGS} -MM -MT ${OBJ_PREFIX}$*${OBJ_SUFFIX} $<


clean:
	${RMDIR} ${OBJ_PREFIX}

distclean mrproper: clean
	${RMDIR} ${DEP_PREFIX}
	${RM} ${BIN}

archive: distclean
	 tar -cz -f ${TAR} --exclude ".git" ../${NAME}

BLU:
	@${ECHO} "CC: ${CC}"
	@${ECHO} "CFLAGS: ${CFLAGS}"
	@${ECHO} "LDFLAGS: ${LDFLAGS}"
	@${ECHO} "======="
	@${ECHO} "SOURCES: ${SRC}"
	@${ECHO} "OBJECTS: ${OBJ}"
	@${ECHO} "BIN: ${BIN}"
