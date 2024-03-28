#include <Arduino.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <WiFi.h>

#define DHT_SENSOR_PIN 4  // ESP32 pin GPIO4 connected to DHT22 sensor
#define DHT_SENSOR_TYPE DHT22

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// 初始化OLED屏幕，选择一个合适的构造函数和字体
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

void connectToWiFi(const char *ssid, const char *password) {
  Serial.println("Connecting to WiFi...");
  //设置ESP32工作模式为无线终端模式
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}


void setup() {
  Serial.begin(9600);

  connectToWiFi("wifi", "password");

  dht_sensor.begin();  // initialize the DHT sensor

  u8g2.begin();
  u8g2.enableUTF8Print();  // enable UTF8 support for the Arduino print() function
}

void loop() {
  // 读取湿度和温度值
  float humi = dht_sensor.readHumidity();
  float tempC = dht_sensor.readTemperature();

  // 检查读取值是否成功
  if (isnan(humi) || isnan(tempC)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    // 在串口监视器中显示读取值
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%  |  ");
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.println("°C");

    // 在OLED屏幕上显示读取值
    u8g2.clearBuffer();  // 清除内部图形缓冲区
    u8g2.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2.setFontDirection(0);
    u8g2.setCursor(0, 16);
    u8g2.print("湿度: ");
    u8g2.print(humi);
    u8g2.print(" %");
    u8g2.setCursor(0, 36);
    u8g2.print("温度: ");
    u8g2.print(tempC);
    u8g2.print(" C");
    u8g2.sendBuffer();  // 将缓冲区的内容传输到屏幕
  }

  delay(2000);  // 等待2秒
}
