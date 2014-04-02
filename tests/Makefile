.SUFFIXES:

# <summary>
# Path Variables
# </summary>
LOCAL    := $(shell pwd)
GLOBAL   := $(LOCAL)/../../..
MESA     := $(GLOBAL)/External/Mesa-9.0
GALLIUM  := $(MESA)/src/gallium

# <summary>
# Compiling flags.
# </summary>
include $(GLOBAL)/Scripts/config.mk
include $(GLOBAL)/Scripts/functions.mk
include $(GLOBAL)/Scripts/rules.mk

# <summary>
# 
# </summary>
define test

  $(call msg,"Test",$1)

  @$(CC) $(CFLAGS) $(LDFLAGS) -o $1 $2

endef


# <summary>
# Compile flags
# </summary>
CFLAGS += -I.
CFLAGS += -I./../include
CFLAGS += -I./../../libcairo
CFLAGS += -I$(GALLIUM)/include
CFLAGS += -I$(GALLIUM)/auxiliary
CFLAGS += -msse2

# <summary>
# Linking flags
# </summary>
LDFLAGS += -ldl
LDFLAGS += -lsettings
LDFLAGS += -lgraphics
LDFLAGS += -lsystem
#LDFLAGS += -lfreetype
#LDFLAGS += -lcairo
#LDFLAGS += -lpng

# <summary>
# Target (all)
# </summary>
all: $(OBJS)
#	$(call test,test1,test1.c)
#	$(call test,test2,test2.c)
#	$(call test,test3,test3.c)
	$(call test,font,font.c)
	$(call test,display,display.c)
	$(call test,planes,planes.c)
	$(call test,view1,view1.c)
	$(call test,view2,view2.c)
	$(call test,view3,view3.c)

# <summary>
# Cleaning
# </summary>
clean:
	@echo "\tCleaning tests"
	@rm -f test1
	@rm -f test2
	@rm -f test3
	@rm -f test4
	@rm -f device
	@rm -f surface
	@rm -f matrix
	@rm -f cursor
	@rm -f tgsi


