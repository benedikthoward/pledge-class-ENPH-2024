#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>

#include "main.h"
#include "ws.h"
// #include "bluepill_uart.h"
#include "crane.h"
#include "pinout.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
const char* ssid = "iPhone von Benedikt";
const char* password = "gabi1234";

// function declaration
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

void wsSetup() {
    // Serial.println("Connecting to WiFi...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "WebSocket server running.");
    });
    server.begin();

    // Serial.println("Servers started!"); 
    // Serial.println("IP: " + WiFi.localIP().toString());
    // Serial.println("ESP1 MAC: " + WiFi.macAddress());
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        ws.textAll("WebSocket client connected");
    } else if (type == WS_EVT_DISCONNECT) {
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            data[len] = 0; // Null-terminate the data
            // bluepillUartSend(String((char*)data));
            ws.textAll("WS received: " + String((char*)data));
            String msg = String((char*)data);
            if(msg == "stop") {
                run = false;
            } else if(msg== "start") {
                run = true;
            } else if (msg.substring(0, 2).equalsIgnoreCase("RF")) {
                int dist = msg.substring(2).toInt();
                craneSetDirectionRB(1);
                moveR = dist;
                ws.textAll("Set MoveR to " + String(dist));
            } else if (msg.substring(0, 2).equalsIgnoreCase("RB")) {
                int dist = msg.substring(2).toInt();
                craneSetDirectionRB(-1);
                moveR = -dist;
                ws.textAll("Set MoveR to " + String(-dist));
            } else if (msg.substring(0, 2).equalsIgnoreCase("YD")) {
                craneSetDirectionYB(1);
                moveY = 1;
                ws.textAll("Set MoveY to " + String(moveY));
            } else if (msg.substring(0, 2).equalsIgnoreCase("YU")) {
                craneSetDirectionYB(-1);
                moveY = -1;
                ws.textAll("Set MoveY to " + String(moveY));
            } else if (msg.substring(0, 3).equalsIgnoreCase("ZCW")) {
                int dist = msg.substring(3).toInt();
                craneSetDirectionZB(1);
                moveZ = dist;
                ws.textAll("Set ZCW to " + String(dist));
            } else if (msg.substring(0, 4).equalsIgnoreCase("ZCCW")) {
                int dist = msg.substring(4).toInt();
                craneSetDirectionZB(-1);
                moveZ = -dist;
                ws.textAll("Set ZCCW to " + String(-dist));
            // } else if (msg.substring(0, 3).equalsIgnoreCase("VON")) {
            //     int power = msg.substring(3).toInt();
            //     vacuumOnB(power);
            //     wsSend("Vacuum on");
            // } else if (msg.substring(0, 4).equalsIgnoreCase("VOFF")) {
            //     vacuumOffB();
            //     wsSend("Vacuum off");
            // }
            }
            ws.textAll("Parsed " + String((char*)data));
        }
    }
}


void wsSend(String message) {
    ws.textAll(message);
}