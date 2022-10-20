TARGET 				:= main.exe
CXXFLAGS 			:= -std=c++11 -O0 #-Werror -Wpedantic -Wall -pedantic
CXX					:= g++
CXX_SOURCE    		= $(CXX) $(CXXFLAGS) -c $(1) -o $(2) &&
CXX_OBJECT    		= $(CXX) $(CXXFLAGS) -o main.exe $(OBJECT_FILES)
MAKE_LIB			= ar -rcs $(1) $(OBJECT_FILES)

SOURCE_EXTENSION	:= .cpp
SOURCES_DIR 		:= source/
BUILD_DIR       	:= build/
PARSE_DIR			= $(wildcard $(1)*$(SOURCE_EXTENSION)) $(foreach d,$(dir $(wildcard $(1)*/.)),$(call PARSE_DIR,$(d)))
SOURCE_FILES  		= $(call PARSE_DIR,$(SOURCES_DIR))
OBJECT_FILES    	= $(addprefix $(BUILD_DIR),$(notdir $(subst $(SOURCE_EXTENSION),.o,$(SOURCE_FILES))))
TO_OBJECT       	= $(addprefix $(BUILD_DIR),$(notdir $(subst $(SOURCE_EXTENSION),.o,$(1))))
COMPILE         	= $(foreach d, $(SOURCE_FILES),$(call CXX_SOURCE,$(d),$(call TO_OBJECT,$(d)))) @echo

all: pre-compile compile link clear

pre-compile :
	@del /f /s /q "build\"
	@rmdir build
	@mkdir build

compile :
# $(call COMPILE,$(SOURCES_DIR))
# Compile and link DECore library
	$(call COMPILE, source/DECore)
	$(call MAKE_LIB, libs/DECore.a)

link: compile
# $(MAKE_LIB)
# $(CXX_OBJECT)

clear: $(TARGET)
	@del $(subst /,\,$(OBJECT_FILES))