NAME := ${notdir ${realpath .}}
VERSION := 0.0.1

CC := gcc
CFLAGS := -Wall -Wextra
LDFLAGS :=

MKDIR := mkdir -p
RMDIR := rm -rf
ECHO := echo -e

SRC_PREFIX := ./
SRC_SUFFIX := .c
SRC := ${wildcard ${SRC_PREFIX}*${SRC_SUFFIX}}

BIN_PREFIX := ./
BIN_SUFFIX :=
BIN := ${BIN_PREFIX}${NAME}${BIN_SUFFIX}

OBJ_PREFIX := obj/
OBJ_SUFFIX := .o
OBJ := ${addprefix ${OBJ_PREFIX},${addsuffix ${OBJ_SUFFIX},${notdir ${basename ${SRC}}}}}

TAR_PREFIX := ../
TAR_SUFFIX := .tar.gz
TAR := ${TAR_PREFIX}${NAME}-${VERSION}${TAR_SUFFIX}

all: ${BIN}

${BIN}: ${OBJ}
	${MKDIR} ${@D}
	${CC} -o $@ ${LDFLAGS} $^

${OBJ_PREFIX}%${OBJ_SUFFIX}: ${SRC_PREFIX}%${SRC_SUFFIX}
	${MKDIR} ${@D}
	${CC} -o $@ ${CFLAGS} -c $<

clean:
	${RMDIR} ${OBJ_PREFIX}

mrproper: clean
	${RM} ${BIN}

archive: mrproper
	 tar -cz -f ${TAR} --exclude ".git" ../${NAME}

BLU:
	@${ECHO} "CC: ${CC}"
	@${ECHO} "CFLAGS: ${CFLAGS}"
	@${ECHO} "LDFLAGS: ${LDFLAGS}"
	@${ECHO} "======="
	@${ECHO} "SOURCES: ${SRC}"
	@${ECHO} "OBJECTS: ${OBJ}"
	@${ECHO} "BIN: ${BIN}"
