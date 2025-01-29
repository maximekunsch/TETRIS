CXX = g++
CXXFLAGS = -std=c++17 -I include -I/opt/homebrew/Cellar/sfml@2/2.6.2/include
LDFLAGS = -L/opt/homebrew/Cellar/sfml@2/2.6.2/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = tetris

$(TARGET): $(OBJ)
	g++ $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	g++ $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)