
// A nice instruction: https://www.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html
// Also check man pages for recv socket ect

#include <iostream>
#include <string.h> // for memset
#include <sys/socket.h>
#include <netinet/in.h> // for sockaddr_in
#include <array>
#include <unistd.h> // for close ()

using namespace std;

struct sockaddr_in;

const int port = 1113;

// Check man for getaddrinfo for information on how to get adress from a string (like google.com)

int main(int argc, char **argv) {
	// the sockaddr_in is a special version of sockaddr
	// for IP addresses (in comparison to eg linux addresses etc)
	sockaddr_in thisAddress;
	sockaddr_in otherAddress;

	std::array<char, 1024> buffer;

	int sock;

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		throw runtime_error("cannot create socket");
	}

	thisAddress = {};
	
	memset(&thisAddress, 0, sizeof(thisAddress));
	thisAddress.sin_family = AF_INET;
	thisAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	thisAddress.sin_port = htons(port);

	// INADDR_ANY tells sockets that we dont care what ip addres we are using
	// in case you are both connected to ethernet and wifi for example
	// htonl converts form the computers format to internet format for example ports and ip-addresses
//	thisAddress = {AF_INET, htonl(INADDR_ANY), htons(port)};

	if (bind(sock, (sockaddr*)&thisAddress, sizeof(thisAddress)) < 0) {
		throw runtime_error("bind failed");
	}

	bool running = true;
	int receivedSize;
	while (running) {
		cout << "waiting on port " << port << endl;
		// An application can use select, poll or epoll to se if there is more data available
		unsigned int otherAddressLength = sizeof(otherAddress);
		receivedSize = recvfrom(sock, &buffer.front(), buffer.size(), 0, (sockaddr *) &otherAddress, &otherAddressLength);

		// Receive from any without getting the address
//		receivedSize = recv(sock, &buffer.front(), buffer.size(), 0);

		// Without flags it is also possible to call read(sock, ...)
		cout << "received " << receivedSize << " bytes" << endl;
		for (int i = 0; i < receivedSize; ++i) {
			cout.put(buffer[i]);
		}
	}



	// To send: sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&remaddr, addrlen)

	close(sock);

	return 0;
}

