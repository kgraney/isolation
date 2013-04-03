
SRC=$(shell ls isolation/*.cpp)
OBJ=$(patsubst %.cpp,%.o,$(SRC))

# Below for the MacOS clang C++ compiler
CXXFLAGS=-std=c++11 -stdlib=libc++
CXX=c++

# Below for g++ (must have C++11 support)
#CXXFLAGS=-std=c++11 -O3 -pthread
#CXX=g++

default: isolation

isolation: $(OBJ)
	$(CXX) $(CXXFLAGS) -o game $(OBJ)

clean:
	rm -f isolation/*.o game
