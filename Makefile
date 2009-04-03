include mak/UserDefine.mak

SUB_DIRS = osal util runtime platform compiler
PROJECT_ROOT_DIR=$(shell pwd)
export PROJECT_ROOT_DIR

all:
	@make build_subdir

clean:
	@make clean_subdir

install:
	@make install_subdir

include mak/Rules.mak
