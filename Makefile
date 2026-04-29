CXX = g++
CXXFLAGS = -O2 -Wall -Iinclude
LIBS = -lGL -lGLU -lglut
TARGET = ofet

SRCS = \
	src/main.cpp \
	src/stl_loader.cpp \
	src/camera.cpp \
	src/draw_utils.cpp \
	src/screenshot.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f ofet_*.png
