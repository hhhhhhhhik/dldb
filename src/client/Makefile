###############################
#      user program           #
###############################
TARGETS = client

PROTOS_PATH = ../protos
SRC_DIR = ..

###############################
#      compiler setting       #
###############################

CXX = g++

CPPFLAGS += -I/usr/local/include -I$(SRC_DIR)/include
CXXFLAGS += -std=c++11
LDFLAGS += -L/usr/local/lib -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lpthread -ldl

PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

###############################
#      make all               #
###############################
vpath %.proto $(PROTOS_PATH)

all:$(TARGETS)

$(TARGETS): dldb.pb.o dldb.grpc.pb.o client.o $(SRC_DIR)/common/ThreadPool.o
	$(CXX) $^ $(LDFLAGS) -o $@

%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<
	cp *.grpc.pb.h $(SRC_DIR)/include

%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<
	cp *.pb.h $(SRC_DIR)/include

###############################
#      clean                  #
###############################

clean::
	rm -f $(TARGETS)
	rm -f *.o *.so *.pb.cc *.pb.h
	rm -f $(SRC_DIR)/include/*.pb.h
