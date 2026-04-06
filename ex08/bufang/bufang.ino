#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Llllemon";
const char* password = "starlight0726";

// 硬件配置
const int LED_PIN = 2;          // 报警LED
const int TOUCH_PIN = T0;       // 触摸引脚（T0对应GPIO4，如果不同请修改）

WebServer server(80);

// 全局变量：使用 volatile 保证中断中能正常访问
volatile int systemState = 0; // 0=未布防, 1=已布防, 2=报警锁定
unsigned long lastBlink = 0;
const long blinkInterval = 150; // 报警闪烁速度

// 生成HTML页面
String makePage() {
  String stateText;
  if (systemState == 0) stateText = "未布防";
  else if (systemState == 1) stateText = "已布防 (等待触发...)";
  else stateText = "🔴 报警锁定！";

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ex08 安防报警器</title>
  <style>
    body { font-family:Arial; text-align:center; margin-top:50px; background:#f5f5f5; }
    h1 { color:#333; }
    .state { font-size:1.5em; margin:20px 0; }
    .state b { color:#e74c3c; font-size:1.8em; }
    button { padding:15px 30px; font-size:18px; margin:0 10px; border:none; border-radius:5px; cursor:pointer; }
    .arm { background:#2ecc71; color:white; }
    .disarm { background:#e74c3c; color:white; }
  </style>
</head>
<body>
  <h1>物联网安防报警器</h1>
  <div class="state">当前状态：<b>)rawliteral" + stateText + R"rawliteral(</b></div>
  
  <a href="/arm"><button class="arm">布防 (Arm)</button></a>
  <a href="/disarm"><button class="disarm">撤防 (Disarm)</button></a>
</body>
</html>
)rawliteral";
  return html;
}

// 根路由
void handleRoot() { server.send(200, "text/html; charset=UTF-8", makePage()); }

// 布防
void handleArm() {
    // 只有在未布防状态下才能布防
    if (systemState == 0) {
        systemState = 1;
        digitalWrite(LED_PIN, LOW); // 布防成功，LED熄灭
    }
    server.sendHeader("Location", "/");
    server.send(303);
}

// 撤防
void handleDisarm() {
    systemState = 0; // 重置为未布防状态
    digitalWrite(LED_PIN, LOW); // 立即关灯，解除报警
    server.sendHeader("Location", "/");
    server.send(303);
}

// 触摸中断回调：正确写法
void IRAM_ATTR onTouch() {
    // 关键逻辑：只有在【已布防】状态下，触摸才会触发【报警锁定】
    if (systemState == 1) {
        systemState = 2; // 切换到报警锁定状态
    }
}

void setup() {
    Serial.begin(115200);
    
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    // 初始化触摸中断
    touchAttachInterrupt(TOUCH_PIN, onTouch, 40);

    // WiFi 连接
    WiFi.begin(ssid, password);
    Serial.print("连接WiFi");
    while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
    Serial.println("\n连接成功");
    Serial.print("访问: http://");
    Serial.println(WiFi.localIP());

    // 注册路由
    server.on("/", handleRoot);
    server.on("/arm", handleArm);
    server.on("/disarm", handleDisarm);
    server.begin();
}

void loop() {
    server.handleClient();
    // 1. systemState == 2 (报警锁定) 才会闪烁
    // 2. 只要 systemState != 2，LED 保持熄灭 (符合未布防/已布防无反应的要求)
    if (systemState == 2) {
        unsigned long now = millis();
        if (now - lastBlink >= blinkInterval) {
            lastBlink = now;
            digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        }
    } else {
        digitalWrite(LED_PIN, LOW); // 非报警状态，LED 熄灭
    }
}