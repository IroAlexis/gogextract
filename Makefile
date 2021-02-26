NAME := ${notdir ${realpath .}}
VERSION := 0.2

# Utils binaries
MKDIR := mkdir -p
RMDIR := rm -rf
ECHO := echo -e
TAR := tar --create --zstd
PKGCONF := pkg-config --silence-errors

# Prefix
SRC_PREFIX := src/
DEP_PREFIX := depends/
OBJ_PREFIX := obj/
BIN_PREFIX := ./

# Suffix
PKG_SUFFIX := .pkg
SRC_SUFFIX := .c
DEP_SUFFIX := .mk
OBJ_SUFFIX := .o
BIN_SUFFIX :=
TAR_SUFFIX := .tar.zst

PKG := libzip
SRC := ${wildcard ${SRC_PREFIX}*${SRC_SUFFIX}}
DEP := ${subst ${SRC_PREFIX},${DEP_PREFIX},${addsuffix ${DEP_SUFFIX},${basename ${SRC}}}}
OBJ := ${addprefix ${OBJ_PREFIX},${addsuffix ${OBJ_SUFFIX},${notdir ${basename ${SRC}}}}}
BIN := ${BIN_PREFIX}${NAME}${BIN_SUFFIX}

# Compilator
CC := gcc
#Flags
CFLAGS := -Wall -Wextra -pedantic ${shell ${PKGCONF} --cflags ${PKG}}
LDFLAGS := ${shell ${PKGCONF} --libs ${PKG}}

all: binary

binary: objects ${BIN}

objects: depends ${OBJ}

depends: ${DEP}

check: ${addsuffix ${PKG_SUFFIX},${PKG}}

${BIN}: ${OBJ}
	@${ECHO} "[-] info:: Link: $@"
	${MKDIR} ${@D}
	${CC} -o $@ ${LDFLAGS} $^

${OBJ_PREFIX}%${OBJ_SUFFIX}: ${SRC_PREFIX}%${SRC_SUFFIX}
	@${ECHO} "[-] info:: Generate: $*"
	${MKDIR} ${@D}
	${CC} -o $@ ${CFLAGS} -c $<

${DEP_PREFIX}%${DEP_SUFFIX}: ${SRC_PREFIX}%${SRC_SUFFIX}
	@${ECHO} "[-] info:: Dependance: $*"
	${MKDIR} ${@D}
	${CC} -o $@ ${CFLAGS} -MM -MT ${OBJ_PREFIX}$*${OBJ_SUFFIX} $<

%${PKG_SUFFIX}:
	@${ECHO} -n "Package \033[1m$*\033[0m: "
	@(${PKGCONF} --exists $* && ${ECHO} "\e[1;32mOK\e[0m") || (${ECHO} "\e[1;31mFAILED\e[0m" && false)

clean:
	${RMDIR} ${OBJ_PREFIX}
	${RMDIR} ${DEP_PREFIX}

distclean mrproper: clean
	${RM} ${BIN}
	${RM} *${TAR_SUFFIX}

package: ${BIN}
	${TAR} -f ${NAME}-${VERSION}${TAR_SUFFIX} legacy LICENSE README.md $^

archive: distclean
	${TAR} -f ${NAME}-${shell git rev-parse --short HEAD}${TAR_SUFFIX} --exclude={.git,.gitignore} *

BLU:
	@${ECHO} "NAME: ${NAME}"
	@${ECHO} "-----"
	@${ECHO} "CC: ${CC}"
	@${ECHO} "CFLAGS: ${CFLAGS}"
	@${ECHO} "LDFLAGS: ${LDFLAGS}"
	@${ECHO} "======="
	@${ECHO} "SOURCES: ${SRC}"
	@${ECHO} "DEPENDS: ${DEP}"
	@${ECHO} "OBJECTS: ${OBJ}"
	@${ECHO} "BIN: ${BIN}"
