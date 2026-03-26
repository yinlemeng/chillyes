#define TOUCH_PIN 4
#define LED_PIN   2
#define THRESHOLD 500

// 1. 布尔型状态变量 (bool ledState)
bool ledState = false;
// 记录上一次的触摸状态，用于边缘检测
bool lastTouch = false; 

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState); // 初始化LED状态
}

void loop() {
  // 读取原始触摸值
  int touchValue = touchRead(TOUCH_PIN);
  // 将原始值转换为逻辑状态：小于阈值视为被触摸
  bool currentTouch = (touchValue < THRESHOLD);

  // 2. 边缘检测逻辑：判断“上一次未触摸，当前被触摸的瞬间”
  //    currentTouch && !lastTouch 表示状态从 LOW -> HIGH 的瞬间
  if (currentTouch && !lastTouch) {
    // 3. 翻转LED状态，实现自锁
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);

    // 4. 软件防抖逻辑：利用延时屏蔽抖动
    //    确保一次触摸只触发一次切换
    delay(200); 
  }

  // 更新上一次状态为当前状态，为下一次循环做准备
  lastTouch = currentTouch;
  
  // 轻微延时，稳定系统运行
  delay(50);
}