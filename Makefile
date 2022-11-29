# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
TARGET_EXEC := COARA

CC  := gcc
CXX := g++
AS  := as


#SRC_DIRS := src external/imgui external/imgui/backends external/imgui/misc/cpp external/imgui/misc/fonts external/imgui/misc/freetype external/stb 

#SRC_DIR_OPENCV := /opt/homebrew/include/opencv4/opencv2
SRC_DIR_EXT  := external 
SRC_DIR_MY   := src 
#SRC_DIRS     := $(shell find $(SRC_DIR_EXT) -type d) $(SRC_DIR_MY)
SRC_DIRS     := $(SRC_DIR_MY) #$(SRC_DIR_OPENCV)


$(info #####===========##########============##########===========##### )
$(info #####===========##########============##########===========##### )
$(info #####===========#####                      #####===========##### )
$(info #####                        START                         ##### )
$(info #####===========#####                      #####===========##### )
$(info #####===========#####                      #####===========##### )
$(info #####===========#####                      #####===========##### )
$(info )
$(info )
$(info )
$(info #####===========#####                      #####===========##### )
$(info #####===========#####       COMPILING      #####===========##### )
$(info #####===========#####                      #####===========##### )
$(info )
$(info ****  SOURCE DIRECTORIES ARE $(SRC_DIRS) )
$(info )
$(info )
$(info )



HEADER_DIRS := header



BUILD_DIR := build
BUILD_PATH := $(shell pwd)/$(BUILD_DIR)

INC_FLAGS := -I$(HEADER_DIRS) -I/usr/local/include -I/usr/local/include/opencv4
#INC_FLAGS := -I$(HEADER_DIRS) -I/usr/local/include -I/usr/local/Cellar/opencv/4.6.0_1/include
#INC_FLAGS := -I$(HEADER_DIRS) -Iexternal/imgui -Iexternal/stb -I/usr/local/include


LIBS_FLAGS_OPENCV := -lopencv_core -lopencv_videoio -lopencv_calib3d -lopencv_core -lopencv_dnn -lopencv_features2d -lopencv_flann -lopencv_gapi -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_video


LIB_FLAGS := -lstdc++ -lGLEW -lglfw3 $(LIBS_FLAGS_OPENCV) -framework Cocoa -framework OpenGL -framework IOKit 
#LIB_FLAGS := -lstdc++ -lGLEW -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
#LIB_FLAGS := -lGLEW -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

LIBPATH_FLAGS := -L/usr/local/lib/


# SETTING ENVIRONMENT PATH FOR COMPILING
#export PATH := $(shell pwd)./external/imgui/lib:$(PATH)



# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. Make will incorrectly expand these otherwise.
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
$(info ##################### )
$(info ****  SOURCE FILES ARE $(SRCS) )
$(info )
$(info )
$(info )

# String substitution for every C/C++ file.
# As an example, hello.cpp turns into ./build/hello.cpp.o
ASMS := $(SRCS:%=$(BUILD_DIR)/%.S)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
$(info ##################### )
$(info ****  ASMS FILES ARE $(ASMS) )
$(info ##################### )
$(info ****  OBJS FILES ARE $(OBJS) )
$(info )
$(info )
$(info )




# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
#DEPS := $(OBJS:.o=.d)



# Every folder in ./src will need to be passed to GCC so that it can find header files
# INC_DIRS := $(shell find $(SRC_DIRS) -type d)

# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
#INC_FLAGS := $(addprefix -I,$(INC_DIRS))
#LDFLAGS := -L/opt/homebrew/lib -lSDL2



# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
#CPPFLAGS := $(INC_FLAGS) -MMD -MP



.PRECIOUS: $(BUILD_DIR)/%.c.S $(BUILD_DIR)/%.cpp.S $(BUILD_DIR)/%.c.o $(BUILD_DIR)/%.cpp.o



# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(info )
	$(info )
	$(info )
	$(info #####===========#####                     #####===========##### )
	$(info #####===========#####       LINKING       #####===========##### )
	$(info #####===========#####                     #####===========##### )
	$(CXX) -g $(OBJS) $(LIBPATH_FLAGS) $(LIB_FLAGS) -o $@ 
	$(info )
	$(info )
	$(info )
	$(info )
	$(info )
	$(info )




# Build step for C source
$(BUILD_DIR)/%.c.o: $(BUILD_DIR)/%.c.S
	$(info )
	$(info ===========       COMPILING --> OBJS      =========== )
	$(info FILE NAME -->    $@)
	mkdir -p $(dir $@)
	$(AS) -o $@ $<


# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: $(BUILD_DIR)/%.cpp.S
	$(info )
	$(info ===========       COMPILING --> OBJS      =========== )
	$(info FILE NAME -->    $@)
	mkdir -p $(dir $@)
	$(AS) -o $@ $<





# Build step for C source
$(BUILD_DIR)/%.c.S: %.c
	$(info )
	$(info ===========       COMPILING --> ASMS      =========== )
	$(info FILE NAME -->    $@)
	mkdir -p $(dir $@)
	$(CC) -g $(INC_FLAGS) -S $< -o $@


# Build step for C++ source
$(BUILD_DIR)/%.cpp.S: %.cpp
	$(info )
	$(info ===========       COMPILING --> ASMS      =========== )
	$(info FILE NAME -->    $@)
	mkdir -p $(dir $@)
	$(CXX) -g -std=c++11 $(INC_FLAGS) -S $< -o $@





.PHONY: run
run:
	cd $(BUILD_PATH) ; \
	$(BUILD_PATH)/$(TARGET_EXEC)
	


.PHONY: clean
clean:
	rm -r $(BUILD_DIR)



# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
#-include $(DEPS)