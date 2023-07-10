#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>

#define DHTPIN 23
#define DHTTYPE DHT11

const char *ssid = "networkname";
const char *password = "networkpassword";


WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);


void handleRoot() {
  char msg[2000];

  snprintf(msg, sizeof(msg),
           "<html>\
  <head>\
    <meta http-equiv='refresh' content='4'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>\
    <title>ESP32 DHT Server</title>\
    <style>\
    body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f4f4f4; }\
    .container { max-width: 400px; margin: 0 auto; background-color: #fff; border-radius: 8px; padding: 20px; box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1); }\
    h1 { text-align: center; font-size: 24px; margin-bottom: 30px; }\
    .reading { display: flex; align-items: center; margin-bottom: 20px; }\
    .icon { font-size: 32px; color: #ca3517; margin-right: 10px; }\
    .label { flex-grow: 1; font-size: 18px; font-weight: bold; }\
    .value { font-size: 24px; }\
    .unit { font-size: 18px; }\
    </style>\
  </head>\
  <body>\
    <div class='container'>\
      <h1>ESP32 DHT Server</h1>\
      <div class='reading'>\
        <i class='fas fa-thermometer-half icon'></i>\
        <div>\
          <div class='label'>Temperature</div>\
          <div class='value'>%.2f</div>\
          <div class='unit'>&deg;C</div>\
        </div>\
      </div>\
      <div class='reading'>\
        <i class='fas fa-tint icon'></i>\
        <div>\
          <div class='label'>Humidity</div>\
          <div class='value'>%.2f</div>\
          <div class='unit'>&percnt;</div>\
        </div>\
      </div>\
    </div>\
  </body>\
</html>",
           readDHTTemperature(), readDHTHumidity()
          );
  server.send(200, "text/html", msg);
}


void setup(void) {

  Serial.begin(115200);
  dht.begin();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);
}


float readDHTTemperature() {
  // Read temperature as Celsius
  float t = dht.readTemperature();
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(t);
    return t;
  }
}

float readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(h);
    return h;
  }
}





