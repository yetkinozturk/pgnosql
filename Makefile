CC      = g++
CFLAGS  = -I ./
LDFLAGS = -lpqxx -lpq -laprutil-1 -lexpat -lapr-1 -llog4cxx -lconfig++

all: server.app

server.app: server.o pgconnection.o configure.o pgnosqlserver.o
	@echo ""
	@echo "==============="
	@echo "Linking Objects"
	@echo "==============="
	@echo ""
	$(CC) --std=gnu++11 -Wall -O3 -o $@ $^ $(LDFLAGS)
	rm *.o

server.o: connect/pgconnection.cpp config/configure.cpp cmd/pgnosqlcommand.cpp server/pgnosqlserver.cpp server/server.cpp
	@echo ""
	@echo "================="
	@echo "Compiling Sources"
	@echo "================="
	@echo ""
	$(CC) --std=gnu++11 -Wall -O3 -c $(CFLAGS) $^ 

.PHONY: clean

clean:
	@echo ""
	@echo "==========="
	@echo "Cleaning Up"
	@echo "==========="
	@echo ""
	rm server.app
