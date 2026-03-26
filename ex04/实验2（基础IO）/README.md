#define TOUCH_PIN 4
#define LED_PIN   2
#define THRESHOLD 600  // 阈值调大一点，更容易触发

bool ledState = false;

// 中断服务函数
void gotTouch() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(LED_PIN, OUTPUT);
  // 上电先亮一下，证明LED是好的
  digitalWrite(LED_PIN, HIGH);
  delay(300);
  digitalWrite(LED_PIN, LOW);

  // 绑定触摸中断
  touchAttachInterrupt(TOUCH_PIN, gotTouch, THRESHOLD);
}

void loop() {
  delay(100);
}
