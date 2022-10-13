  # the compiler: gcc for C program, define as g++ for C++
  CXX = g++

  # compiler flags:
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
  CFLAGS  = -g -Wall

  # the build target executable:
  TARGET = ExtendibleHashing

  # output
  OUTPUT = ex

  all: $(TARGET)

  $(TARGET): $(TARGET).cpp
	$(CXX) $(CFLAGS) -o $(OUTPUT).o $(TARGET).cpp

  clean:
	$(RM) $(OUTPUT).o