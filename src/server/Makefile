###############################
#      user program           #
###############################
TARGETS = ldb_server

PROTOS_PATH = ../protos
LEVELDB_PATH = /root/work/leveldb

###############################
#      compiler setting       #
###############################

CXX = g++

CPPFLAGS += -I/usr/local/include -I$(LEVELDB_PATH)/include -pthread
CXXFLAGS += -std=c++11
LDFLAGS += -L/usr/local/lib -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lpthread -ldl -L$(LEVELDB_PATH)/lib -lleveldb

PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

###############################
#      make all               #
###############################
vpath %.proto $(PROTOS_PATH)

all:$(TARGETS)

$(TARGETS): ldb.pb.o ldb.grpc.pb.o LdbServiceImpl.o server.o
	$(CXX) $^ $(LDFLAGS) -o $@

%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<

###############################
#      clean                  #
###############################

clean::
	rm -f $(TARGETS)
	rm -f *.o *.so *.pb.cc *.pb.h
