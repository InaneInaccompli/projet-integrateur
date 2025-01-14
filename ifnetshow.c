#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s -n <ip_address> -i <interface_name>\n", argv[0]);
        fprintf(stderr, "       %s -n <ip_address> -a\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *ip_address = NULL;
    char *interface = "all";

    // Analyse des arguments de la ligne de commande
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-n") == 0) {
            ip_address = argv[++i];
        } else if (strcmp(argv[i], "-i") == 0) {
            interface = argv[++i];
        }
    }

    if (ip_address == NULL) {
        fprintf(stderr, "L'adresse IP est manquante.\n");
        exit(EXIT_FAILURE);
    }

    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Créer le socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip_address);
    server_address.sin_port = htons(PORT);

    // Connecter le client au serveur
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Erreur lors de la connexion au serveur");
        exit(EXIT_FAILURE);
    }

    printf("Connecté au serveur\n");

    // Envoyer des données au serveur
    send(client_socket, interface, strlen(interface), 0);

    // Recevoir la réponse du serveur
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received <= 0) {
        perror("Erreur lors de la réception des données");
    }

    printf("Serveur: %s", buffer);

    // Fermer le socket
    close(client_socket);

    return 0;
}
