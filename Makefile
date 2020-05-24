all:
	g++ client.cpp -o client -Wall -fsanitize=address,undefined -O3
	g++ server.cpp -o server -Wall -fsanitize=address,undefined -O3

clean:
	rm client
	rm server
