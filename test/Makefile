SRCS != ls *.cc
PROGS = $(SRCS:R)

CXX = clang++-17

#CXXFLAGS += -g -Wno-deprecated
CXXFLAGS += -O3 -Wno-deprecated

CXXFLAGS += -std=c++23
CXXFLAGS += -I ../
CXXFLAGS += -I props

# Run the tests.
all: $(PROGS)
	@for i in $(PROGS); do echo "Running" $$i; ./$$i; done
