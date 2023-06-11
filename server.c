#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFFER_SIZE 1024
const char* proposals[] = {
        "Cafe", "Restaurant", "Home", "Lecture", "Sports", "Archery", "Countryside", "Town", "Cinema",
        "Museum", "Theatre",
        "Aircraft", "Melbourne", "Berlin", "Paris", "Masterclass", "Park", "Concert"
};
const char *names[] = {"Wade", "Dave", "Seth", "Ivan", "Riley", "Gilbert", "Jorge", "Dan", "Brian", "Roberto", "Ramon",
                       "Miles", "Liam", "Nathaniel", "Ethan", "Lewis", "Milton", "Claude"};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <ip_address> <port>\n", argv[0]);
        return 1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    // Создание UDP сокета
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    // Привязка адреса и порта к сокету
    if (inet_pton(AF_INET, ip, &(server_address.sin_addr)) <= 0) {
        perror("Invalid address/Address not supported");
        return 1;
    }

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Binding failed");
        return 1;
    }

    printf("Server started. Waiting for connections...\n");

    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    // Ожидание получения данных от клиента
    int read_bytes = recvfrom(server_socket, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&client_address, &client_address_len);
    if (read_bytes > 0) {
        buffer[read_bytes] = '\0';
        int num_fans;
        sscanf(buffer, "%d", &num_fans);

        if (num_fans == 0) {
            printf("No invitations today.\n");
            char response[] = "No invitations today!";
            sendto(server_socket, response, strlen(response), 0, (struct sockaddr *)&client_address, client_address_len);
        } else {
            // Выбор случайного поклонника
            srand(time(NULL));
            int chosen_fan = rand() % num_fans;
            printf("Chosen fan: %s who invites to %s\n",names[chosen_fan], proposals[chosen_fan]);

            // Отправка ответа клиенту
            char response[] = "Choice has been made!";
            sendto(server_socket, response, strlen(response), 0, (struct sockaddr *)&client_address, client_address_len);
        }
    } else {
        printf("Error reading data from client.\n");
    }

    close(server_socket);
    return 0;
}
