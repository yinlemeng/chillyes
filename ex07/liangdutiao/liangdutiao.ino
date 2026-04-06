#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Llllemon";
const char* password = "starlight0726";

const int LED_PIN = 2; // D2 常见为 GPIO2
WebServer server(80);

int currentBrightness = 128; // 亮度 0~255

String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ex07 PWM 调光器</title>
</head>
<body style="font-family:Arial; text-align:center; margin-top:50px;">
  <h1>Web 无极调光器 (PWM)</h1>
  <p>当前亮度：<b><span id="val">)rawliteral" + String(currentBrightness) + R"rawliteral(</span></b></p>

  <input type="range" min="0" max="255" value=")rawliteral" + String(currentBrightness) + R"rawliteral(" 
     style="width:300px; height:25px;"
     oninput="update(this.value)">

  <script>
    function update(v) {
      document.getElementById("val").innerText = v;
      fetch("/set?b="+v);
    }
  </script>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

void handleSetBrightness() {
  if (server.hasArg("b")) {
    currentBrightness = server.arg("b").toInt();
    currentBrightness = constrain(currentBrightness, 0, 255);
    analogWrite(LED_PIN, currentBrightness); // 最兼容的PWM写法
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("连接WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功");
  Serial.print("访问地址: http://");
  Serial.println(WiFi.localIP());

  // 注册路由
  server.on("/", handleRoot);
  server.on("/set", handleSetBrightness);
  server.begin();
}

// 这里只保留一个 loop() 函数！删除了重复的那个
void loop() {
  server.handleClient();
}