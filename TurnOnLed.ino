#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

// Define el pin al que está conectado el LED
const int ledPin = 27;
bool ledState = LOW; // Estado inicial del LED

void setup() {
  Serial.begin(115200);

  const char* ssid = "MY_WIFI";
  const char* password = "password";

  Serial.println("Desconectamos antes de conectar el WiFi");
  WiFi.disconnect();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  // Conectamos el ESP a la red WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Intentamos conectarnos a la red
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Si logramos conectarnos mostramos la IP a la que nos conectamos
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Configuramos el pin del LED como salida
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState); // Establece el estado inicial del LED

  // Si entramos a la raíz mostramos el botón para cambiar el estado
  server.on("/", HTTP_GET, []() {
    String content = "<html>";
    content += "<head><title>My ESP32</title></head>";
    content += "<body>";
    content += "<button style='font-size: 30px; background: green; color: white;' onclick='toggleLED(\"on\")'>Encender</button>";
    content += "<button style='font-size: 30px; background: red; color: white;' onclick='toggleLED(\"off\")'>Apagar</button>";
    content += "<script>";
    content += "function toggleLED(option) {";
    content += "  var xhr = new XMLHttpRequest();";
    content += "  xhr.open('GET', '/toggle/' + option, true);";
    content += "  xhr.send();";
    content += "  }";
    content += "</script>";
    content += "</body>";
    content += "</html>";
    server.send(200, "text/html", content);
  });

  // Maneja la solicitud para cambiar el estado del LED
  server.on("/toggle/on", HTTP_GET, []() {
    ledState = HIGH;
    digitalWrite(ledPin, ledState);
    server.send(200, "text/plain", "Estado del LED cambiado a: " + String(ledState));
  });

  server.on("/toggle/off", HTTP_GET, []() {
    ledState = LOW;
    digitalWrite(ledPin, ledState);
    server.send(200, "text/plain", "Estado del LED cambiado a: " + String(ledState));
  });

  server.begin();
}

void loop() {
  // Maneja las solicitudes de la página web
  server.handleClient();
  delay(100);
}
