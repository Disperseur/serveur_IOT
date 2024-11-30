#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[1024];

    // Création de la socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Erreur lors de la création de la socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Liaison de la socket au port
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur lors du bind");
        close(server_fd);
        return -1;
    }

    // Mise en écoute
    if (listen(server_fd, 3) < 0) {
        perror("Erreur lors de l'écoute");
        close(server_fd);
        return -1;
    }

    printf("Serveur en écoute sur le port %d...\n", PORT);

    // Boucle principale pour gérer plusieurs clients
    while (1) {
        printf("En attente de connexion...\n");

        // Acceptation d'une connexion client
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("Erreur lors de l'acceptation");
            continue; // Passe au client suivant
        }

        printf("Client connecté depuis %s:%d\n",
            inet_ntoa(client_addr.sin_addr),
            ntohs(client_addr.sin_port));

        // Communication avec le client
        while (1) {
            int bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                printf("Message reçu : %s\n", buffer);

                // Répondre au client (facultatif)
                // char response[] = "Message reçu\n";
                // send(client_fd, response, strlen(response), 0);
            } else if (bytes_read == 0) {
                printf("Client déconnecté\n");
                break; // Sort de la boucle de communication
            } else {
                perror("Erreur lors de la réception");
                break; // Sort de la boucle de communication
            }
        }

        sleep(1); //delai apparemment necessaire pour eviter le plantage (la carte qui se deconnecte en meme temps que elle envoie les donnees)
        // Ferme la connexion avec le client
        close(client_fd);
        printf("Connexion fermée.\n\n");
    }

    // Ferme le serveur (n'atteint jamais cette ligne en boucle infinie)
    close(server_fd);
    return 0;
}
