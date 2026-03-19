const int ONBOARD_LED_PIN = 2;
const long interval = 1000;
unsigned long previousMillis = 0;
int ledState = LOW;

void setup() {
  pinMode(ONBOARD_LED_PIN, OUTPUT);
  // 新增：初始化串口，用于调试
  Serial.begin(115200);
  Serial.println("ex02: 1Hz LED Blink Started");
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(ONBOARD_LED_PIN, ledState);
    // 新增：打印当前LED状态
    Serial.print("LED State: ");
    Serial.println(ledState == HIGH ? "ON" : "OFF");
  }
}