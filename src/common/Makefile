###############################
#      user program           #
###############################
SRC_DIR = ..

ALL_SRCS := $(wildcard *.cc)
ALL_OBJS := $(ALL_SRCS:.cc=.o)

all:$(ALL_OBJS)

###############################
#      compiler setting       #
###############################

CXX = c++
CXXFLAGS += -I$(SRC_DIR)/include -g -W -std=c++0x
LDFLAGS := $(LDFLAGS) -lpthread

###############################
#      make all               #
###############################

$(ALL_OBJS): $(ALL_SRCS)
	$(CXX) -c $(LDFLAGS) $(CXXFLAGS) $< 

###############################
#      clean                  #
###############################

clean::
	rm -f *.o *.so
