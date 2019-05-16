/*
 * findaddress.cpp
 *
 *  Created on: 16 maj 2019
 *      Author: Mattias Larsson Sköld
 */

#include <iostream>
//#include <sys/types.h>
#include <netdb.h>
//#include <sys/socket.h>
#include <stdlib.h>
//#include <unistd.h>
#include <string.h> // For memset
//#include <netinet/in.h>
#include <arpa/inet.h>

//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netdb.h>

using namespace std;

// Described here https://beej.us/guide/bgnet/html/multi/syscalls.html#getaddrinfo

int main(int argc, char **argv) {
	addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;//AF_INET; // only ip4
	hints.ai_socktype = SOCK_DGRAM;

	addrinfo *result;
	if (getaddrinfo("www.google.com", "80", &hints, &result)) {
		throw runtime_error("could not find address");
	}

	for (auto p = result; p; p = p->ai_next) {
		void *addr;
		string ipver;
		if (p->ai_family == AF_INET) {
			auto *ip4 = (sockaddr_in*)p->ai_addr;
			addr = &(ip4->sin_addr);
			ipver = "ip4";
		}
		else {
			auto *ip6 = (sockaddr_in6 *)p->ai_addr;
			addr = &(ip6->sin6_addr);
			ipver = "ip6";
		}
		char ipstr[INET6_ADDRSTRLEN];
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		cout << ipstr << endl;
	}

	freeaddrinfo(result);

	cout << "hej" << endl;
	return 0;
}




