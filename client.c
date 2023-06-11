#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <ip_address> <port> <num_fans>\n", argv[0]);
        return 1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int numFans = atoi(argv[3]);
    // Создание UDP сокета
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    // Устанавливаем семейство адресов для сервера
    server_address.sin_family = AF_INET;
    // Устанавливаем порт для сервера.
    server_address.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &(server_address.sin_addr)) <= 0) {
        perror("Invalid address/Address not supported");
        return 1;
    }

    // Отправка числа поклонников на сервер
    char numFansStr[10];
    sprintf(numFansStr, "%d", numFans);
    sendto(client_socket, numFansStr, strlen(numFansStr), 0, (struct sockaddr *)&server_address, sizeof(server_address));

    char buffer[BUFFER_SIZE];
    socklen_t server_address_len = sizeof(server_address);
    int read_bytes = recvfrom(client_socket, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&server_address, &server_address_len);
    if (read_bytes > 0) {
        buffer[read_bytes] = '\0';
        printf("Response from server: %s\n", buffer);
    } else {
        printf("No response from server.\n");
    }

    close(client_socket);
    return 0;
}
