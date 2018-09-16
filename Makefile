CC=clang++-6.0
CFLAGS=-std=gnu++2a -O3 -Wall -pthread
OUTPUT=-o main.o
GL= -lGL -lglfw -lGLEW
GTEST_DIR=$(CURDIR)/googletest/googletest
INCLUDES=-I$(CURDIR)/sml/include -I$(CURDIR)/di/include -I$(CURDIR)
TEST_INCLUDES=-I$(CURDIR)/googletest/googletest/include

build:
	$(CC) $(CFLAGS) $(GL) $(OUTPUT) main.cxx

shade.loader.spec:
	$(CC) $(CFLAGS) $(TFLAGS) $(GL) $(INCLUDES) $(TEST_INCLUDES) -o ShadeLoader/ShadeLoader.spec gtest-all.o ShadeLoader/ShadeLoader.spec.cxx

gtest:
	$(CC) -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc && \
	ar -rv libgtest.a gtest-all.o