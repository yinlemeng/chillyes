const int ledPin1 = 2;
const int ledPin2 = 5;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void loop() {
  // 第一阶段：LED1变亮，LED2变暗
  for(int duty = 0; duty <= 255; duty++){
    analogWrite(ledPin1, duty);          // D2: 0→255
    analogWrite(ledPin2, 255 - duty);    // D5: 255→0
    delay(3);
    Serial.print("LED1: ");
    Serial.print(duty);
    Serial.print(" | LED2: ");
    Serial.println(255 - duty);
  }

  // 第二阶段：LED1变暗，LED2变亮
  for(int duty = 255; duty >= 0; duty--){
    analogWrite(ledPin1, duty);          // D2: 255→0
    analogWrite(ledPin2, 255 - duty);    // D5: 0→255
    delay(3);
    Serial.print("LED1: ");
    Serial.print(duty);
    Serial.print(" | LED2: ");
    Serial.println(255 - duty);
  }

  Serial.println("=== 循环完成 ===");
}