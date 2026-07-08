TARGET = voicecommand

SRCS = main.cpp \
       wav_reader.cpp \
       mfcc.cpp

OBJS = $(SRCS:.cpp=.o)

# Use variables from environment
CXX ?= g++
CC ?= gcc
CXXFLAGS ?= -Wall -O2
CPPFLAGS ?=
LDFLAGS ?=

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) -lsndfile

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
