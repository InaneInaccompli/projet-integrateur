#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void executeCommand(int client_socket, const char *command) {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    
    // Ouvrir un processus pour exécuter la commande
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Erreur lors de l'exécution de la commande");
        exit(EXIT_FAILURE);
    }

    // Lire la sortie de la commande et envoyer au client
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        send(client_socket, buffer, strlen(buffer), 0);
    }

    // Fermer le flux
    pclose(fp);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE];

    // Créer le socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Lier le socket à l'adresse et au port
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Erreur lors de la liaison du socket");
        exit(EXIT_FAILURE);
    }

    // Mettre le serveur en mode écoute
    if (listen(server_socket, 5) == -1) {
        perror("Erreur lors de la mise en écoute du socket");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexions...\n");

    // Lire et afficher les données reçues du client
    while (1) {
        // Accepter la connexion d'un client
        socklen_t client_address_len = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket == -1) {
            perror("Erreur lors de l'acceptation de la connexion");
            exit(EXIT_FAILURE);
        }

        printf("Client connecté\n");

        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            perror("Erreur lors de la réception des données");
            break;
        }

        if (strcmp(buffer, "all") == 0) {
            // Exécuter la commande et envoyer la sortie au client
            executeCommand(client_socket, "ifshow -a");
        } else if (strcmp(buffer, "lo") == 0) {
            // Exécuter la commande et envoyer la sortie au client
            executeCommand(client_socket, "ifshow -i lo");
        }else if (strcmp(buffer, "eth0") == 0) {
            // Exécuter la commande et envoyer la sortie au client
            executeCommand(client_socket, "ifshow -i eth0");
        }else if (strcmp(buffer, "eth1") == 0) {
            // Exécuter la commande et envoyer la sortie au client
            executeCommand(client_socket, "ifshow -i eth1");
        }else if (strcmp(buffer, "eth2") == 0) {
            // Exécuter la commande et envoyer la sortie au client
            executeCommand(client_socket, "ifshow -i eth2");
        }else if (strcmp(buffer, "eth3") == 0) {
            // Exécuter la commande et envoyer la sortie au client
            executeCommand(client_socket, "ifshow -i eth3");
        }else if (strcmp(buffer, "eth4") == 0) {
            // Exécuter la commande et envoyer la sortie au client
            executeCommand(client_socket, "ifshow -i eth4");
        }else if (strcmp(buffer, "eth5") == 0) {
            // Exécuter la commande et envoyer la sortie au client
            executeCommand(client_socket, "ifshow -i eth5");
        }

  
        // Fermer les sockets
        close(client_socket);
    }

    // Fermer les sockets
    close(server_socket);

    return 0;
}
