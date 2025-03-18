CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -O3

SRCS = Expression.cpp test_dif.cpp
OBJS = $(SRCS:.cpp=.o)
GTEST_DIR = third_party/googletest
TEST_EXEC = test_dif

$(GTEST_DIR)/CMakeLists.txt:
	@echo "Скачивание Google Test..."
	git clone https://github.com/google/googletest.git $(GTEST_DIR)
$(TEST_EXEC): Expression.o test_dif.o
	$(CC) $(CFLAGS) -o $@ $^ -lgtest -lgtest_main -pthread

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_EXEC)
	@echo "Запуск тестов..."
	./$(TEST_EXEC)


clean:
	rm -f $(OBJS) $(TEST_EXEC)

