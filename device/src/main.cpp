#define CAMERA_MODEL_AI_THINKER
#define CAMERA_MODULE_OV2640
#include "conf.h"
#include <WiFi.h>
#include "Arduino.h"
#include "fb_gfx.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "fd_forward.h"
#include <string.h>
camera_fb_t *fb;
#include "camera_pins.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <iot.h>

WiFiClientSecure secureClient = WiFiClientSecure();
PubSubClient mqttClient(secureClient);

bool get_data(void)
{
    fb = esp_camera_fb_get();
    if (!fb)
    {
        Serial.println("Failed to get image");
        return false;
    }
    else
    {
        return true;
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    if (get_data())
    {
        if (mqttClient.publish(AWS_IOT_PUB_TOPIC, fb->buf, fb->len))
        {
            Serial.println(String("Successfully posted ") + String(fb->len) + String(" bytes"));
        }
        else
        {
            Serial.println(String("Failed to post to MQTT"));
        }
        fb = NULL;
    }
}

void connect()
{
    while (!mqttClient.connected())
    {
        Serial.print("Attempting MQTT connection...");
        if (mqttClient.connect(DEVICE_NAME))
        {
            Serial.println(" Connected.");
            mqttClient.subscribe(AWS_IOT_SUB_TOPIC);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.setDebugOutput(true);

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 32;
    config.fb_count = 1;
    config.ledc_timer = LEDC_TIMER_0;

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }
    sensor_t *s = esp_camera_sensor_get();
    //initial sensors are flipped vertically and colors are a bit saturated
    if (s->id.PID == OV3660_PID)
    {
        s->set_vflip(s, 1);       //flip it back
        s->set_brightness(s, 1);  //up the blightness just a bit
        s->set_saturation(s, -2); //lower the saturation
    }
    s->set_framesize(s, FRAMESIZE_VGA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Attempting WiFi connection...");
        sleep(5);
    }
    Serial.print("Connected to WiFi with IP address ");
    Serial.println(WiFi.localIP());
    mqttClient.setServer(AWS_IOT_ENDPOINT, 8883);
    secureClient.setCACert(AWS_CERT_CA);
    secureClient.setCertificate(AWS_CERT_CRT);
    secureClient.setPrivateKey(AWS_CERT_PRIVATE);
    mqttClient.setCallback(callback);
    connect();
    delay(1500);
}

void loop()
{
    if (!mqttClient.connected()) {
        connect();
    }
    mqttClient.loop();
}
