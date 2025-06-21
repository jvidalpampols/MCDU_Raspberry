
#line make
# g++ -Wall -std:c++17 mcdu_app.cpp mcdu_app -0 mcdu_app -lncursesw

# Nombre del ejecutable
TARGET = mcdu_app

# Archivo fuente
SRC = mcdu_app.cpp

# Compilador
CXX = g++

# Flags de compilación
CXXFLAGS = -Wall -std=c++17

# Bibliotecas necesarias
LIBS = -lncursesw

# Regla principal
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# Limpieza
clean:
	rm -f $(TARGET)





