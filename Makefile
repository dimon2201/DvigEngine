TARGET 				:= testlib.a
CXXFLAGS 			:= -std=c++11 #-Werror -Wpedantic -Wall -pedantic
CXX					:= g++
GXX    				= $(CXX) $(CXXFLAGS) -c $(1) -o $(2) &&

SOURCE_EXTENSION	:= .cpp
SOURCES_DIR 		:= source/
BUILD_DIR       	:= build/
PARSE_DIR			= $(wildcard $(1)*$(SOURCE_EXTENSION)) $(foreach d,$(dir $(wildcard $(1)*/.)),$(call PARSE_DIR,$(d)))
SOURCE_FILES  		= $(call PARSE_DIR,$(SOURCES_DIR))
OBJECT_FILES    	= $(addprefix $(BUILD_DIR),$(notdir $(subst $(SOURCE_EXTENSION),.o,$(SOURCE_FILES))))
TO_OBJECT       	= $(addprefix $(BUILD_DIR),$(notdir $(subst $(SOURCE_EXTENSION),.o,$(1))))
COMPILE         	= $(foreach d, $(SOURCE_FILES),$(call GXX,$(d),$(call TO_OBJECT,$(d)))) @echo

all: pre-compile compile link clear

pre-compile :
	@del /f /s /q "build\"
	@rmdir build
	@mkdir build

compile :
	$(call COMPILE,$(SOURCES_DIR))

link: compile
	$(CXX) $(CXXFLAGS) -o main.exe $(OBJECT_FILES)
# ar -crs $(TARGET) $(OBJECT_FILES)

clear: $(TARGET)
	@del $(subst /,\,$(OBJECT_FILES))