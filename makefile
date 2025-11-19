CXX = g++
CXXFLAGS = -std=c++17 -g -O2 -Wall -Wextra -IPhase-1/include
SRC_DIR = Phase-1/src
OBJ_DIR = Phase-1/build
TEST_DIR = Phase-1/tests

OBJS = $(OBJ_DIR)/Graph.o \
        $(OBJ_DIR)/KNN.o \
        $(OBJ_DIR)/ShortestPath.o \
        $(OBJ_DIR)/QueryProcessor.o \
        $(OBJ_DIR)/SampleDriver.o

TARGET = phase1

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo "Build complete!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


run-test1: $(TARGET)
	./phase1 $(TEST_DIR)/test1/graph.json $(TEST_DIR)/test1/queries.json $(TEST_DIR)/test1/output.json

run-test2: $(TARGET)
	./phase1 $(TEST_DIR)/test2/graph.json $(TEST_DIR)/test2/queries.json $(TEST_DIR)/test2/output.json

compare-test1:
	@diff -u <(jq --sort-keys . $(TEST_DIR)/test1/expected_output.json) <(jq --sort-keys . $(TEST_DIR)/test1/output.json) || true

compare-test2:
	@diff -u <(jq --sort-keys . $(TEST_DIR)/test2/expected_output.json) <(jq --sort-keys . $(TEST_DIR)/test2/output.json) || true

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Cleaned build files!"

.PHONY: all clean run-test1 run-test2 compare-test1 compare-test2
