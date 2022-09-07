#include <SPI.h>
#include <WiFi.h>
#include <SSLClient.h>
#include <ArduinoJson.h>
#include "trust_anchors.h"
#include "wifi_ssids.h"

WiFiClient wifi;
SSLClient client(wifi, TAs, TAs_NUM, A7);


class YandexVariable {
  public:
    String errors = "";
    YandexVariable(String Token) {
      errors = "";
      token = Token;
      reconnect();
    };

    void reconnect() {
      errors = "";
      if (!client.connect("cloud-api.yandex.net", 443)) {
        errors = "Connection error";
        return;
      }
    }

    void create(String Name, String Val) {
      errors = "";
      reconnect();
      create_dir(Name);
      delay(500);
      copy_var_file(Name, Val);
    };

    int SSL_error_code() {
      return client.SSL_OK;
    }

    String get(String Name) {
      errors = "";
      reconnect();
      client.println("GET /v1/disk/resources?path=%2Fvariables%2F" + Name + " HTTP/1.1");
      client.println("Host: cloud-api.yandex.net");
      client.println("Connection: keep-alive");
      client.println("Accept: application/json");
      client.println("Authorization: OAuth " + token);
      client.println();
      client.println();

      String data = read_data();

      if (data == "") {
        errors = "Request error";
        return "";
      }
      errors = data;
      int json_start = data.indexOf("{");
      String json = data.substring(json_start, data.length() - json_start);

      try {
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, json);
        return doc["_embedded"]["items"][0]["name"];
      } catch (int exc) {
        errors = "Invalid JSON";
        return "";
      }
    };

    void set(String Name, String Val) {
      errors = "";
      reconnect();
      client.println("POST /v1/disk/resources/move?from=%2Fvariables%2F" + Name + "%2F" + get(Name) + "&path=%2Fvariables%2F" + Name + "%2F" + Val + "&overwrite=true HTTP/1.1");
      client.println("Host: cloud-api.yandex.net");
      client.println("Connection: keep-alive");
      client.println("Content-Length: 0");
      client.println("Accept: application/json");
      client.println("Content-Type: application/json");
      client.println("Authorization: OAuth " + token);
      client.println();
      client.println();

      errors = read_data();
      if (errors == "") {
        errors = "Request error";
        return;
      }
    }
  private:
    String token;
    String read_data() {
      Serial.print("Waiting responce");
      while (!client.available()) {
        delay(500);
        Serial.print(".");
      }
      Serial.println();

      int len = client.available();
      int loop_len = len;
      String data = "";
      while (loop_len > 0) {
        byte buffer[len];
        client.read(buffer, len);

        char str[(sizeof buffer) + 1];
        memcpy(str, buffer, sizeof buffer);
        str[sizeof buffer] = 0;
        data += String(str);

        loop_len = client.available();
        len += loop_len;
      }
      return data;
    }
    void create_dir(String Name) {
      errors = "";
      client.println("PUT /v1/disk/resources?path=%2Fvariables%2F" + Name + " HTTP/1.1");
      client.println("Host: cloud-api.yandex.net");
      client.println("Connection: keep-alive");
      client.println("Content-Length: 0");
      client.println("Accept: application/json");
      client.println("Content-Type: application/json");
      client.println("Authorization: " + token);
      client.println();
      client.println();

      errors = read_data();
      if (errors == "") {
        errors = "Request error";
        return;
      }
    };
    void copy_var_file(String Name, String Val) {
      errors = "";
      client.println("POST /v1/disk/resources/copy?from=system%2Femptyfile&path=%2Fvariables%2F" + Name + "%2F" + Val + "&overwrite=true HTTP/1.1");
      client.println("Host: cloud-api.yandex.net");
      client.println("Connection: keep-alive");
      client.println("Content-Length: 0");
      client.println("Accept: application/json");
      client.println("Content-Type: application/json");
      client.println("Authorization: OAuth " + token);
      client.println();
      client.println();

      errors = read_data();
      if (errors == "") {
        errors = "Request error";
        return;
      }
    };
};



void WiFiConnect(const char* ssid, const char* password) {
  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
