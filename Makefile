CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lm

TARGET = calculator.exe
TEST_TARGET = test_suite.exe

SRCS = main.c calculator.c history.c
TEST_SRCS = test_suite.c calculator.c history.c

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(SRCS) calculator.h
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

$(TEST_TARGET): $(TEST_SRCS) calculator.h
	$(CC) $(CFLAGS) $(TEST_SRCS) -o $(TEST_TARGET) $(LDFLAGS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

gui: $(TARGET)
	python app_server.py

clean:
	rm -f $(TARGET) $(TEST_TARGET) *.o

.PHONY: all test gui clean

