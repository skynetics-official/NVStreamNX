ifneq ($(BUILD),$(notdir $(CURDIR)))

export TOPDIR  := $(CURDIR)
export LIBNX   := $(DEVKITPRO)/libnx
export PORTLIBS := $(DEVKITPRO)/portlibs/switch
export PREFIX  := $(DEVKITDEV)/bin/a64-none-elf-

export CC      := $(PREFIX)gcc
export CXX     := $(PREFIX)g++
export AR      := $(PREFIX)gcc-ar

export ARCH    := -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE

export CFLAGS  := -g -Wall -O2 -ffunction-sections $(ARCH) -D__SWITCH__
export CXXFLAGS:= $(CFLAGS) -std=gnu++20 -fno-rtti -fno-exceptions

export INCLUDE := -I$(CURDIR)/include -I$(LIBNX)/include -I$(PORTLIBS)/include
export LIBDIRS := -L$(LIBNX)/lib -L$(PORTLIBS)/lib

export LIBS    := -lnx

export OUTPUT  := NVStreamNX
export SOURCES := src

export VPATH   := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export CFILES  := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
export CPPFILES:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))

export OFILES  := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o)

export BUILD   := build

.PHONY: clean all

all: $(OUTPUT).nro

$(OUTPUT).nro: $(OUTPUT).elf
	@elf2nro $< $@ --name="$(OUTPUT)" --author="NVStreamNX Team" --version="1.0.0"

$(OUTPUT).elf: $(OFILES)
	@echo linking $(notdir $@)
	@$(CXX) $(ARCH) -Wl,-Map,$(OUTPUT).map $^ $(LIBDIRS) $(LIBS) -o $@

%.o: %.cpp
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@rm -rf $(BUILD) *.elf *.nro *.map *.o

else

endif
