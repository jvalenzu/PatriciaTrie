# can't define any targets in here, just macros

empty:=
slash=/
underscore=_
slash_to_underscore = $(subst $(slash),$(underscore),$1)
space:=$(empty) $(empty)
first_dot = $(subst $(space),.,$(wordlist 1, 2,$(subst ., ,$(1))))

CPPFLAGS += -Wno-parentheses
CPPFLAGS += -Wno-deprecated-declarations
CPPFLAGS += -Wc++11-extensions

ifeq ($(BUILD_TYPE),Release)
CPPFLAGS += -O2 -g
CPPFLAGS += -DNDEBUG=1
else
CPPFLAGS += -fstack-protector-strong
CPPFLAGS += -g -O0
CPPFLAGS += -D_DEBUG=1
endif

CXXFLAGS += -std=c++11 -stdlib=libc++ 

LDFLAGS += -Wc++11-extensions

# C++ generated code with dependencies
outName = obj/$(basename $(call slash_to_underscore,$1)).o
depName = obj/$(basename $(call slash_to_underscore,$1)).d

define srcToObj

ifeq (x$(filter %.cpp, $1),x)
$(call outName,$1) : obj/dummy $1 $(call depName,$1) Makefile
	$(CC) $(CPPFLAGS) $(CCFLAGS) -MMD $(INCLUDES) -I$(CURDIR) -c -o $(call outName,$1) $1
else
$(call outName,$1) : obj/dummy $1 $(call depName,$1) Makefile
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD $(INCLUDES) -I$(CURDIR) -c -o $(call outName,$1) $1
endif

$(call depName,$1) : $1

# depname
-include $(call depName,$1)

CLEAN += $(call depName,$1)
CLEAN += $(call outName,$1)
endef

# Shader code with dependencies
shaderOutName = obj/Shader/$(notdir $1)
shaderDepName = obj/Shader/$(notdir $1).d

# jiv fixme: should extract from cpp pass
SHADER_DEPS := Render/Shaders/shader.h Render/Shaders/light.h

define shaderSrcToObj
$(call shaderOutName,$1) : obj/dummy obj/Shader/dummy $1 $(call shaderDepName,$1) Makefile Build/rules.mk $(SHADER_DEPS)
#	cpp -MMD -IRender/Shaders -MF$(call shaderDepName,$1) $1 $(call shaderOutName,$1)
	-Build/Binary/mcpp -P -@old -MMD -IRender/Shaders -MF $(call shaderDepName,$1) $1 $(call shaderOutName,$1)

$(call shaderDepName,$1) : $1 

# depname
-include $(call shaderDepName,$1)

CLEAN += $(call shaderDepName,$1)
CLEAN += $(call shaderOutName,$1)
endef
