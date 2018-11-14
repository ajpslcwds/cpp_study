all:server client

server:server.o
	g++ -g -o server server.o
client:client.o
	g++ -g -o client client.o
clean:
	rm -rf *.o server client
	