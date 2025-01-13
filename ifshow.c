#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void display_interface_info(const char *interface_name) {
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) {
            continue;
        }

        if (interface_name == NULL || strcmp(interface_name, ifa->ifa_name) == 0) {
            printf("Interface: %s\n", ifa->ifa_name);

            if (ifa->ifa_addr->sa_family == AF_INET) {
                struct sockaddr_in *sa = (struct sockaddr_in *) ifa->ifa_addr;
                char ip_address[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &sa->sin_addr, ip_address, sizeof(ip_address));
                printf("IPv4 Address: %s\n", ip_address);
            } else if (ifa->ifa_addr->sa_family == AF_INET6) {
                struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *) ifa->ifa_addr;
                char ip6_address[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &sa6->sin6_addr, ip6_address, sizeof(ip6_address));
                printf("IPv6 Address: %s\n", ip6_address);
            }
        }
    }

    freeifaddrs(ifaddr);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // if no option is provided, display all interfaces and their addresses
        display_interface_info(NULL);
    } else if (argc == 3 && strcmp(argv[1], "-i") == 0) {
        // if -i option is provided, display addresses for the specified interface
        display_interface_info(argv[2]);
    } else if (argc == 2 && strcmp(argv[1], "-a") == 0) {
        // if -a option is provided, display names and addresses for all interfaces
        display_interface_info(NULL);
    } else {
        // incorrect usage, provide usage information
        fprintf(stderr, "Usage: %s [-i interface_name] [-a]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}
