.SUFFIXES:
.PHONY: tests

# <summary>
# Path Variables
# </summary>
LOCAL   := $(shell pwd)
CARBON  := $(LOCAL)/../..

# <summary>
# Build scripts.
# </summary>
include $(CARBON)/Scripts/config.mk
include $(CARBON)/Scripts/functions.mk
include $(CARBON)/Scripts/rules.mk

# <summary>
# Compile flags
# </summary>
CFLAGS += -I.
CFLAGS += -I./include
CFLAGS += -I./source
CFLAGS += -I$(Includes)/drm
CFLAGS += -DDEBUG
#CFLAGS += -std=gnu99
CFLAGS += -Wall
#CFLAGS += -O3
CFLAGS += -g

# <summary>
# Linking flags
# </summary>
LDFLAGS += -ldl
LDFLAGS += -lstdc++
#LDFLAGS += -lm

# <summary>
# Source Objects
# </summary>
OBJS += source/device.o
OBJS += source/display.o
OBJS += source/resource.o
OBJS += source/cursor.o
OBJS += source/plane.o
OBJS += source/surface.o
OBJS += source/private.o
OBJS += source/driver.o
OBJS += source/arch/arch-x86.o
OBJS += source/view.o
OBJS += source/prime.o
OBJS += source/properties.o

# <summary>
# Target (all)
# </summary>
all: $(OBJS)
	$(call link_static,libgraphics.a,$(OBJS))
	$(call link_shared,libgraphics.so,$(OBJS))

# <summary>
# Cleaning
# </summary>
clean:
	@echo "\tCleaning libgraphics"
	@rm -f libgraphics.so
	@rm -f libgraphics.a
	@rm -f $(OBJS)

