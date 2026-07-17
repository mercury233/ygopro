#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32

#include <WinSock2.h>
#include <ws2tcpip.h>

#else // _WIN32

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define SD_BOTH 2
#define SOCKET int
#define closesocket close
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SOCKADDR_IN sockaddr_in
#define SOCKADDR sockaddr
#define SOCKET_ERRNO() (errno)

#endif // _WIN32

#endif // SOCKET_H
