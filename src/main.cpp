#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "ssid";
const char *password = "password";

const char *sseServerUrl = "http://pocketbase_url:port/api/realtime";
// COLLECTION_ID_OR_NAME or COLLECTION_ID_OR_NAME/RECORD_ID 
// https://pocketbase.io/docs/api-realtime/
// Leave empty to unsubscribe from everything.
const char *subscriptions[2] = {"", ""};

void perform_post_auth(String clientId)
{
  // Perform a HTTP POST request

  HTTPClient http;
  http.begin(sseServerUrl);
  http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument json(1024);

  json["clientId"] = clientId;
  JsonArray subscriptionsJson = json.createNestedArray("subscriptions");
  for (int i = 0; i < 2; ++i) {
    subscriptionsJson.add(subscriptions[i]);
  }

  String requestBody;
  serializeJson(json, requestBody);
  http.POST(requestBody);
  http.end();
}

String midString(String str, String start, String finish)
{
  int locStart = str.indexOf(start);
  if (locStart == -1)
    return "";
  locStart += start.length();
  int locFinish = str.indexOf(finish, locStart);
  if (locFinish == -1)
    return "";
  return str.substring(locStart, locFinish);
}

void setup()
{
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  HTTPClient http;
  http.setReuse(true);
  http.addHeader("Accept", "text/event-stream");
  http.begin(sseServerUrl);

  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK)
  {
    Serial.println("Failed to get SSE data");
    return;
  }

  if (httpCode == HTTP_CODE_OK)
  {
    Serial.println("SUCCES to get SSE data");
    WiFiClient client = http.getStream();
    Serial.println("SUCCES WifiClient");
    while (client.connected())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\n');
        String clientId = midString(line, "\"clientId\":\"", "\"}");
        Serial.println(line);
        if (clientId != "")
        {
          perform_post_auth(clientId);
        }
      }
    }
  }
}