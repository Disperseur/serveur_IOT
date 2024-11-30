#include <WiFiNINA.h>
#include "ArduinoLowPower.h"
#define LOWPOWER_SLEEPTIME 5000 //ms

// Paramètres WiFi
char ssid[] = "iphoneA";       // Nom du réseau WiFi
char pass[] = "iphone69";      // Mot de passe WiFi

// Adresse du serveur
char server[] = "172.20.10.4"; // Adresse IP du PC
int port = 8080;               // Port du serveur

int compteur = 0;

WiFiClient client;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  Serial.println("Programme démarré.");
}

void loop() {
  // Connexion au WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connexion au WiFi...");
    WiFi.disconnect(); // Réinitialise toute connexion précédente
    WiFi.begin(ssid, pass);
    WiFi.lowPowerMode();

    // Attente de connexion
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 10) {
      delay(1000);
      retries++;
      Serial.println("Connexion WiFi en cours...");
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi connecté !");
      Serial.print("Adresse IP Arduino : ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("Échec de la connexion WiFi. Nouvelle tentative au prochain cycle.");
      // delay(5000); // Attente avant de réessayer
      LowPower.sleep(LOWPOWER_SLEEPTIME);
      return; // Redémarre la boucle loop()
    }
  }

  // Connexion au serveur
  Serial.println("Tentative de connexion au serveur...");
  if (client.connect(server, port)) {
    Serial.println("Connecté au serveur !");

    // Envoi d'un message au serveur
    String message = "Ceci est un message de l'Arduino: ";
    client.print(message + String(compteur));
    Serial.println("Message envoyé : " + message);

    compteur++;

    // Déconnexion du serveur
    delay(500);
    client.stop();
    Serial.println("Déconnecté du serveur.");
  } else {
    Serial.println("Impossible de se connecter au serveur.");
  }

  delay(1000); // tempo pour le serveur distant
  
  // Déconnexion du WiFi
  Serial.println("Déconnexion du WiFi...");
  WiFi.disconnect();
  while (WiFi.status() == WL_CONNECTED) {
    delay(100); // Petite attente pour s'assurer de la déconnexion
  }
  Serial.println("WiFi déconnecté.");

  // Pause avant de recommencer
  // delay(5000);
  WiFi.end();
  LowPower.sleep(LOWPOWER_SLEEPTIME);
}
