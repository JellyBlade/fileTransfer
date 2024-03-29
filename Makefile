PROJECT = fileTransfer
GTEST = ./test_$(PROJECT)

# Compilation command and flags
CXX=g++
CXXVERSION= -std=c++11
CXXFLAGS= $(CXXVERSION) -g -fprofile-arcs -ftest-coverage
LINKFLAGS= -lgtest -lgtest_main -lpthread -lz

# Directories
SRC_DIR = src
PROJECT_SRC_DIR = src/project
GTEST_DIR = test
SRC_INCLUDE = include
INCLUDE = -I ${SRC_INCLUDE}

# Tool variables
GCOV = gcov
LCOV = lcov
COVERAGE_RESULTS = results.coverage
COVERAGE_DIR = coverage

STATIC_ANALYSIS = cppcheck

STYLE_CHECK = cpplint.py

DOXY_DIR = docs/code

# Default goal, used by Atom for local compilation
.DEFAULT_GOAL := all

# default rule for compiling .cc to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean up all files that should NOT be submitted
.PHONY: clean
clean:
	rm -rf *~ $(SRC)/*.o $(GTEST_DIR)/output/*.dat \
	*.gcov *.gcda *.gcno \
	$(PROJECT) $(COVERAGE_RESULTS) \
	$(GTEST) $(MEMCHECK_RESULTS) $(COVERAGE_DIR)  \
	$(PROJECT)_* $(GTEST)

# compilation using the files in include, src, and test, but not src/project
$(GTEST): $(GTEST_DIR) $(SRC_DIR)
	$(CXX) $(CXXFLAGS) -o $(GTEST) $(INCLUDE) \
	$(GTEST_DIR)/*.cpp $(SRC_DIR)/*.cpp $(LINKFLAGS)

# compilation using the files in include, src, and src/project, but not test
all: sender receiver
sender: $(SRC_DIR) $(PROJECT_SRC_DIR)
	$(CXX) $(CXXVERSION) -o $(PROJECT)_sender $(INCLUDE) \
	$(SRC_DIR)/*.cpp $(PROJECT_SRC_DIR)/sender.cpp -lz

receiver: $(SRC_DIR) $(PROJECT_SRC_DIR)
	$(CXX) $(CXXVERSION) -o $(PROJECT)_receiver $(INCLUDE) \
	$(SRC_DIR)/*.cpp $(PROJECT_SRC_DIR)/receiver.cpp -lz -g

# To perform all tests
.PHONY: allTests
allTests: $(GTEST) memcheck coverage docs

.PHONY: memcheck
memcheck: $(GTEST)
	valgrind --tool=memcheck --leak-check=yes --error-exitcode=1 $(GTEST)

.PHONY: fullmemcheck
fullmemcheck: $(GTEST)
	valgrind --tool=memcheck --leak-check=full --error-exitcode=1 $(GTEST)

.PHONY: coverage
coverage: $(GTEST)
	$(GTEST)
	# Determine code coverage
	$(LCOV) --capture --gcov-tool $(GCOV) --directory . --output-file $(COVERAGE_RESULTS) --rc lcov_branch_coverage=1
	# Only show code coverage for the source code files (not library files)
	$(LCOV) --extract $(COVERAGE_RESULTS) */*/$(SRC_DIR)/* -o $(COVERAGE_RESULTS)
	#Generate the HTML reports
	genhtml $(COVERAGE_RESULTS) --output-directory $(COVERAGE_DIR)
	#Remove all of the generated files from gcov
	rm -f *.gcda *.gcno

.PHONY: docs
docs: ${SRC_INCLUDE}
	doxygen $(DOXY_DIR)/doxyfile
