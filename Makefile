# Detect OS
UNAME_S := $(shell uname -s)

# Default is MacOS makefile
PLATFORM_MAKEFILE := Makefile.macos

ifeq ($(UNAME_S), Linux)
    PLATFORM_MAKEFILE := Makefile.linux
endif

THREADS_USED_FOR_BUILD := 8

# Passes through the makefile target to specific makefile with -f
all:
	$(MAKE) -f $(PLATFORM_MAKEFILE) -j$(THREADS_USED_FOR_BUILD) all

clean:
	$(MAKE) -f $(PLATFORM_MAKEFILE) clean

clean-tests:
	$(MAKE) -f $(PLATFORM_MAKEFILE) clean-tests

run-tests:
	$(MAKE) -f $(PLATFORM_MAKEFILE) run-tests

list:
	$(MAKE) -f $(PLATFORM_MAKEFILE) list

.PHONY: all clean clean-tests run-tests list
