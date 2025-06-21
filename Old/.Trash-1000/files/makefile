# Makefile profesional para mcdu_app en Raspberry Pi

#g++ -O2 -Wall -o mcdu_template mcdu_template.cpp -lncursesw -lhidapi-hidraw
#g++ -Wall -Wextra -O2 -o mcdu_template mcdu_template.cpp -lncursesw -lhidapi-hidraw
# Makefile correcto para mcdu_template.cpp

TARGET = hid_wakeup_read
SRC = hid_wakeup_read.cpp

CXX = g++
CXXFLAGS = -Wall -Wextra -O2
LDFLAGS = -lncursesw -lhidapi-hidraw

all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)


