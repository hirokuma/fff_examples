BUILD_DIR:=build
TEST_BIN:=tst
FFF_BUILD_DIR:=fff/build

TEST_SRCS:=\
	test_main.cpp \
	test_func.cpp \
	test_func2.cpp

CFLAGS += -I$(FFF_BUILD_DIR)/_deps/googletest-src/googletest/include
#CFLAGS += -DFFF_GCC_FUNCTION_ATTRIBUTES="__attribute__((weak))"
LDFLAGS += -L$(FFF_BUILD_DIR)/lib -lgtest -lgtest_main

all: mk_fff
	mkdir -p $(BUILD_DIR)
	g++ $(CFLAGS) -o $(BUILD_DIR)/$(TEST_BIN) $(TEST_SRCS) $(LDFLAGS) -pthread

ifeq ($(wildcard $(FFF_BUILD_DIR)),)
mk_fff:
	mkdir -p $(FFF_BUILD_DIR)
	cd fff && cmake -B build -DFFF_GENERATE=ON -DFFF_UNIT_TESTING=ON && cmake --build build && cd ..
else
mk_fff:
	@echo
endif

test:
	$(BUILD_DIR)/$(TEST_BIN)

clean: 
	rm -rf $(FFF_BUILD_DIR) $(BUILD_DIR)
