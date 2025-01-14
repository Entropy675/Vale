ifneq ($(wildcard config.make),)
	include config.make
endif

ifndef OF_ROOT
	OF_ROOT=$(realpath ../../../OpenFrameworks)
endif

# Automatically use all available cores for compilation (leaving one in case your on a potato)
MAKEFLAGS += -j$(shell echo $$(($(nproc) - 1)))

include $(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk
USER_CFLAGS += -I./src/
