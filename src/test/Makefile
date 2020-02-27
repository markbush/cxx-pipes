CXXFLAGS += -std=c++17

TESTS = *.h
SOURCE = runner.cpp
OBJECTS = $(SOURCE:%.cpp=%.o)
RUNNER = runner

DEFAULT: test

test: $(RUNNER)
	./$(RUNNER)

$(RUNNER): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(RUNNER) $(OBJECTS)

$(SOURCE): $(TESTS) ../pipes/pipes.h
	cxxtestgen --error-printer -o $(SOURCE) $(TESTS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<
