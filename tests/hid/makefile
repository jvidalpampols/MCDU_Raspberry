# Makefile profesional para mcdu_app en Raspberry Pi

#g++ -O2 -Wall -o mcdu_template mcdu_template.cpp -lncursesw -lhidapi-hidraw
#g++ -Wall -Wextra -O2 -o mcdu_template mcdu_template.cpp -lncursesw -lhidapi-hidraw
# Makefile correcto para mcdu_template.cpp

#TARGET = hid_wakeup_read
#SRC = hid_wakeup_read.cpp
# Archivos fuente (puedes agregar más luego)
TARGET = hid_read
SRC = hid_read.cpp hidTools.cpp 

# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -Wextra -O2

# Directorios donde buscar headers (*.h)
#INCLUDES = -I. -I/mnt/mcdu/test/hid
INCLUDES = -I/mnt/mcdu/test/hid

# Librerías necesarias
LDFLAGS = -lncursesw -lhidapi-hidraw

#$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

all:
	
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)


