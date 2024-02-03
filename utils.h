#ifndef UTILS_H
#define UTILS_H

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REQUEST_SIZE 1024
#define MAX_RESPONSE_SIZE 10240

WSADATA *initialize_winsock(int major_version, int minor_version) {
    WSADATA *wsa = (WSADATA *)malloc(sizeof(WSADATA));
    if (WSAStartup(MAKEWORD(major_version, minor_version), wsa) != 0) {
        perror("Failed to initialize Winsock");
        exit(EXIT_FAILURE);
    }
    return wsa;
}

SOCKET initialize_sever_socket() {
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    return server_socket;
}

struct sockaddr_in populate_server_address(unsigned short int port) {
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    return server_address;
}

void bind_address_to_socket(struct sockaddr_in server_addr, SOCKET server_socket) {
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
}

void listen_to_connections(SOCKET server_socket) {
    if (listen(server_socket, 10) == SOCKET_ERROR) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }
}

FILE *send_response_to_client(SOCKET client_socket, char status[], FILE *file) {
    char response[MAX_RESPONSE_SIZE];
    sprintf(response, "%s", status);
    send(client_socket, response, strlen(response), 0);

    size_t bytesRead;
    while ((bytesRead = fread(response, 1, sizeof(response), file)) > 0) {
        send(client_socket, response, bytesRead, 0);
    }
}

#endif