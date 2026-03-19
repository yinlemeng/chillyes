// 定义板载LED引脚（ESP32通常是2，Arduino Uno是13，根据你的开发板修改）
const int ONBOARD_LED_PIN = 2;

// 计时变量
unsigned long previousMillis = 0;  // 记录上一次状态翻转的时间
const long interval = 500;         // 闪烁间隔（500ms，1Hz = 1000ms周期）
bool ledState = LOW;               // LED当前状态

void setup() {
  pinMode(ONBOARD_LED_PIN, OUTPUT);  // 设置LED引脚为输出模式
  Serial.begin(115200);             // 可选：初始化串口，用于调试
  Serial.println("1Hz LED 闪烁启动！");
}

void loop() {
  unsigned long currentMillis = millis();  // 获取当前系统运行时间（毫秒）

  // 判断是否达到闪烁间隔
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // 更新上一次翻转时间
    ledState = !ledState;             // 翻转LED状态（亮 ↔ 灭）
    digitalWrite(ONBOARD_LED_PIN, ledState);  // 控制LED
    
    // 可选：串口输出状态
    Serial.print("LED 状态：");
    Serial.println(ledState ? "亮" : "灭");
  }
}