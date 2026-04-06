#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Llllemon";
const char* password = "starlight0726";

// 硬件配置
const int TOUCH_PIN = T0;       // 触摸感应引脚（ESP32 T0对应GPIO4）
WebServer server(80);

// 生成带AJAX实时刷新的HTML页面
String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ex09 实时传感器 Web 仪表盘</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      height: 100vh;
      margin: 0;
      background-color: #f0f0f0;
    }
    h1 { color: #333; font-size: 2.5em; margin-bottom: 30px; }
    .value-box {
      background-color: #fff;
      padding: 50px 80px;
      border-radius: 15px;
      box-shadow: 0 4px 15px rgba(0,0,0,0.1);
    }
    #sensorValue {
      font-size: 5em;
      font-weight: bold;
      color: #e74c3c;
    }
    .label {
      font-size: 1.5em;
      color: #666;
      margin-top: 10px;
    }
  </style>
</head>
<body>
  <h1>触摸传感器实时监控</h1>
  <div class="value-box">
    <div id="sensorValue">0</div>
    <div class="label">当前触摸值</div>
  </div>

  <script>
    // AJAX 定时拉取数据，实现实时刷新
    function updateValue() {
      fetch('/getValue')
        .then(response => response.text())
        .then(data => {
          document.getElementById('sensorValue').innerText = data;
        })
        .catch(err => console.error('拉取数据失败:', err));
    }

    // 每100ms刷新一次，实现实时跳动效果
    setInterval(updateValue, 100);
    // 页面加载时立即刷新一次
    updateValue();
  </script>
</body>
</html>
)rawliteral";
  return html;
}

// 根路由：返回HTML页面
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

// 数据接口：返回触摸传感器的实时模拟量值
void handleGetValue() {
  // 读取触摸传感器值
  uint16_t touchValue = touchRead(TOUCH_PIN);
  // 直接返回数值，供AJAX拉取
  server.send(200, "text/plain", String(touchValue));
}

void setup() {
  Serial.begin(115200);
  
  // 连接WiFi
  WiFi.begin(ssid, password);
  Serial.print("连接WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n连接成功");
  Serial.print("访问地址: http://");
  Serial.println(WiFi.localIP());

  // 注册路由
  server.on("/", handleRoot);
  server.on("/getValue", handleGetValue);
  server.begin();
}

void loop() {
  server.handleClient();
}
