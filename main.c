#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define PORT 8080

#define STATUS_OK "HTTP/1.1 200 OK\r\n\r\n"
#define STATUS_NOT_FOUND "HTTP/1.1 404 NOT FOUND\r\n\r\n"

#define NOT_FOUND_FILE_PATH "responses/404.html"

void handle_client(SOCKET client_socket) {
    char buffer[MAX_REQUEST_SIZE];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    
    if (bytes_received == SOCKET_ERROR) {
        perror("Error receiving data from client");
        closesocket(client_socket);
    }

    char method[10], path[255];
    sscanf(buffer, "%s %s", method, path);

    FILE* file = fopen(path + 1, "r");

    if (file == NULL) {
        FILE* not_found_file = fopen(NOT_FOUND_FILE_PATH, "r");
        send_response_to_client(client_socket, STATUS_NOT_FOUND, not_found_file);
    } else{
        send_response_to_client(client_socket, STATUS_OK, file);
    }

    fclose(file);
    closesocket(client_socket);
}

void accept_and_handle_connections(SOCKET server_socket) {
    SOCKET client_socket;
    struct sockaddr_in client_addr;
    int addr_len = sizeof(client_addr);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket == INVALID_SOCKET) {
            perror("Error accepting connection");
            continue;
        }

        handle_client(client_socket);
    }
}

int main() {
    WSADATA *wsa = initialize_winsock(2, 2);

    SOCKET server_socket = initialize_sever_socket();
    struct sockaddr_in server_addr = populate_server_address(PORT);
    bind_address_to_socket(server_addr, server_socket);

    listen_to_connections(server_socket);

    printf("Server listening on port %d...\n", PORT);

    accept_and_handle_connections(server_socket);

    closesocket(server_socket);

    WSACleanup();
    free(wsa);

    return 0;
}
