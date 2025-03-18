#pragma once

#ifndef _WIN32
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define INVALID_SOCKET (-1)
#define SOCKET int
#define SOCKET_ERROR (-1)
#define closesocket close
#endif

#include <time.h>

class NTPClient
{
public:
    static time_t GetCurrentTimeUTC(const char * ntpserver = NULL)
    {
#ifdef _WIN32
        WSADATA w;
        int remote_length;
#else
        socklen_t remote_length;
#endif
        SOCKET sd = INVALID_SOCKET;
        char ntp_buffer[48];
        time_t current_time = 0;
        struct hostent *hp;
        struct sockaddr_in remote;
        struct sockaddr_in local;
        char host_name[256];
        hostent * host = NULL;

        do {
#ifdef _WIN32
            if (WSAStartup(MAKEWORD(2, 2), &w) != 0)
                return 0;
#endif

            if ((sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
                break;

            memset((void *)&remote, '\0', sizeof(struct sockaddr_in));

            remote.sin_family = AF_INET;
            remote.sin_port = htons(123);

            if ((host = gethostbyname((ntpserver) ? ntpserver : "pool.ntp.org")) == NULL)
                break;

            remote.sin_addr.s_addr = *(u_long *)host->h_addr_list[0];

            memset((void *)&local, '\0', sizeof(local));

            local.sin_family = AF_INET;
            local.sin_port = htons(0);
            local.sin_addr.s_addr = htonl(INADDR_ANY);

            if (bind(sd, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR)
                break;

            memset(ntp_buffer, 0, sizeof(ntp_buffer));
            ntp_buffer[0] = 0x1B;

            remote_length = sizeof(struct sockaddr_in);
            
            if (sendto(sd, ntp_buffer, sizeof(ntp_buffer), 0, (struct sockaddr *)&remote, remote_length) == -1)
                break;

            fd_set fds;

            FD_ZERO(&fds);
            FD_SET(sd, &fds);

            timeval timeout;
            timeout.tv_sec = 10;
            timeout.tv_usec = 0;

            if (select(1, &fds, NULL, NULL, &timeout) != 1)
                break;

            if (recvfrom(sd, ntp_buffer, sizeof(ntp_buffer), 0, (struct sockaddr *)&remote, &remote_length) < 0)
                break;

            unsigned int int_part = *(unsigned int *)(ntp_buffer + 40);
            unsigned int fract_part = *(unsigned int *)(ntp_buffer + 44);

            int_part = SwapEndianness(int_part);
            
            current_time = (time_t)(int_part -= 2208988800U);
        }
        while (0);

        if (sd != INVALID_SOCKET)
            closesocket(sd);
        
#ifdef _WIN32
        WSACleanup();
#endif

        return current_time;
    }

     static unsigned int SwapEndianness(unsigned int x)
     {
        return (unsigned int)(((x & 0x000000ff) << 24) +
                        ((x & 0x0000ff00) << 8) +
                        ((x & 0x00ff0000) >> 8) +
                        ((x & 0xff000000) >> 24));
     }
};
