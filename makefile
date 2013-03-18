
SRC=$(shell ls isolation/*.cpp)
OBJ=$(patsubst %.cpp,%.o,$(SRC))

CXXFLAGS=-std=c++11 -stdlib=libc++

default: isolation

isolation: $(OBJ)
	$(CXX) $(CXXFLAGS) -o game $(OBJ)

clean:
	rm -f isolation/*.o game
