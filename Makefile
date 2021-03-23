# make -k -C /Users/jvalenzu/Source/PatriciaTrie -f /Users/jvalenzu/Source/PatriciaTrie/Makefile

include Build/rules.mk

CPPFLAGS += -Wno-unused-variable

INCLUDES += -IExternal

CPPFLAGS += -Werror

# CPPFLAGS += -fprofile-generate
# LDFLAGS += -fprofile-instr-generate

LIBRARIES += -framework Cocoa

# C++ source code to object files
SRCS += PatriciaTrieDebug.cpp
SRCS += PatriciaTrie.cpp
SRCS += Main.cpp
SRCS += Util.cpp

SRCS += External/linenoise/linenoise.c

OBJS := $(foreach src,$(SRCS),$(call outName,$(src)))

all: PatriciaTrie

PatriciaTrie : $(OBJS) Makefile
	$(CXX) -o $@ $(OBJS) -I$(CURDIR) $(LDFLAGS) $(LIBRARIES)

# Patricia.pdf: Makefile Patricia.roff
# 	pic Patricia.roff | groff -rN4 -t -mmm - | ps2pdf - > Patricia.pdf

Patricia.dot: PatriciaTrie
	./PatriciaTrie --dot test_strings_r.txt > Patricia.dot

Patricia.png: Makefile Patricia.dot
	dot Patricia.dot -Tpng > $@

test : Patricia.png

$(foreach src,$(SRCS),$(eval $(call srcToObj,$(src))))

CLEAN += PatriciaTrie

include Build/rules2.mk
