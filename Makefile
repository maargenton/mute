TARGET_EXEC ?= $(notdir $(PWD))
MKDIR_P ?= mkdir -p

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src ./include
TEST_DIRS ?= ./test

BIN_SUFFIX :=
SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

TEST_SRCS := $(shell find $(TEST_DIRS) -name test_*.cpp)
TEST_OBJS := $(TEST_SRCS:%.cpp=$(BUILD_DIR)/%.test.o)
TEST_BINS := $(TEST_OBJS:%.test.o=%.test)
TEST_OUTPUTS := $(TEST_BINS:%.test=%.test.output)
TEST_DEPS := $(TEST_OBJS:.o=.d)
.PRECIOUS: $(TEST_OBJS) $(TEST_BINS)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -std=c++14 -Wall -O0 -g


# ----------------------------------------------------------------------------
# default rule
# ----------------------------------------------------------------------------

.PHONY: all
all: test build


# ----------------------------------------------------------------------------
# build rules
# ----------------------------------------------------------------------------

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Archive
$(BUILD_DIR)/$(TARGET_EXEC).a: $(OBJS)
	ar -rcs $@ $<

# Linker
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) -g $(OBJS) -o $@ $(LDFLAGS)

.PHONY: build
build: $(BUILD_DIR)/$(TARGET_EXEC)

.PHONY: run
run: $(BUILD_DIR)/$(TARGET_EXEC)
	@$(BUILD_DIR)/$(TARGET_EXEC)

.DEFAULT: $(BUILD_DIR)/$(TARGET_EXEC)

# ----------------------------------------------------------------------------
# mute test targets
# ----------------------------------------------------------------------------

# c++ source
$(BUILD_DIR)/%.test.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -include mute/mute_runner_stdout.h -c $< -o $@

%.test: %.test.o $(BUILD_DIR)/$(TARGET_EXEC).a
	$(CXX) -g -o $@ $^ $(LDFLAGS)

%.test.output: %.test
	$< |tee $@

.PHONY: test
test: $(TEST_OUTPUTS)


# ----------------------------------------------------------------------------
# clean target
# ----------------------------------------------------------------------------

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS) $(TEST_DEPS)
