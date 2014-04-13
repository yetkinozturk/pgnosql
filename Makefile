CC      = g++
CFLAGS  = -I/usr/local/include/
LDFLAGS = -lpqxx -lpq -laprutil-1 -lexpat -lapr-1 -llog4cxx -lconfig++

all: server.app

server.app: server.o loggingoption.o pgconnection.o configure.o pgnosqlserver.o
	@echo "================="
	@echo "Linking Sources"
	@echo "================="
	$(CC) --std=gnu++11 -o $@ $^ $(LDFLAGS)

server.o: loggingoption.cpp pgconnection.cpp configure.cpp pgnosqlserver.cpp server.cpp
	@echo "================="
	@echo "Compiling Sources"
	@echo "================="
	$(CC) --std=gnu++11 -c $(CFLAGS) $^ 

.PHONY: clean

clean:
	@echo "==========="
	@echo "Cleaning Up"
	@echo "==========="
	rm *.o
	rm server.app
