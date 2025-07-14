#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <DNSServer.h>

// Hotspot Settings (Open Network)
const char* ssid = "FREE WIFI PEMKOT SURABAYA";  // SSID tanpa password

AsyncWebServer server(80);  // Web server
DNSServer dnsServer;       // DNS server untuk captive portal

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Gagal mount SPIFFS!");
    return;
  }

  // Start Hotspot
  WiFi.softAP(ssid);
  Serial.println("Hotspot terbuka aktif!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Configure DNS for captive portal
  dnsServer.start(53, "*", WiFi.softAPIP());  // Redirect semua domain ke IP AP

  // Main HTML endpoint
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = R"(
<!DOCTYPE html>
<html>
<head>
  <title>WiFi Login</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body { 
      font-family: Arial, sans-serif; 
      text-align: center; 
      margin: 0;
      padding: 20px;
      background: #f0f0f0;
    }
    .login-box {
      background: white;
      padding: 25px;
      border-radius: 10px;
      display: inline-block;
      box-shadow: 0 0 15px rgba(0,0,0,0.1);
      max-width: 400px;
      width: 90%;
      margin-top: 30px;
    }
    button { 
      padding: 12px 30px; 
      font-size: 16px; 
      background: #4CAF50; 
      color: white; 
      border: none; 
      border-radius: 5px;
      cursor: pointer;
      margin: 10px 0;
      width: 100%;
    }
    button:hover {
      background: #45a049;
    }
    #loading {
      display: none;
      margin: 15px 0;
      color: #666;
    }
    .modal {
      display: none;
      position: fixed;
      z-index: 100;
      left: 0;
      top: 0;
      width: 100%;
      height: 100%;
      background-color: rgba(0,0,0,0.7);
    }
    .modal-content {
      background-color: #fefefe;
      margin: 20% auto;
      padding: 20px;
      border-radius: 10px;
      width: 80%;
      max-width: 400px;
      box-shadow: 0 4px 20px rgba(0,0,0,0.2);
      animation: modalopen 0.3s;
    }
    @keyframes modalopen {
      from {opacity: 0; transform: translateY(-50px);}
      to {opacity: 1; transform: translateY(0);}
    }
    .close-btn {
      color: #aaa;
      float: right;
      font-size: 24px;
      font-weight: bold;
      cursor: pointer;
    }
    .close-btn:hover {
      color: #333;
    }
    .logo {
      max-width: 150px;
      margin-bottom: 20px;
    }
  </style>
</head>
<body>
  <div class='login-box'>
    <img src="/logo.png" class="logo" alt="Pemkot Surabaya">
    <h2>Selamat Datang di WiFi Publik</h2>
    <p>Klik tombol <strong>CONNECT</strong> untuk mengakses internet gratis</p>
    <button onclick='playSound()' id='connectBtn'>CONNECT</button>
    <div id='loading'>Memproses koneksi...</div>
  </div>

  <!-- Modal -->
  <div id="myModal" class="modal">
    <div class="modal-content">
      <span class="close-btn" onclick='closeModal()'>&times;</span>
      <h2>✅ Koneksi Berhasil</h2>
      <p>Totol mana ada yang gratis di dunia ini</p>
      <p><small>(created by AA)</small></p>
    </div>
  </div>

  <script>
    const modal = document.getElementById('myModal');
    
    function playSound() {
      const btn = document.getElementById('connectBtn');
      const loading = document.getElementById('loading');
      
      btn.disabled = true;
      loading.style.display = 'block';
      
      var audio = new Audio('/sound.wav');
      
      audio.addEventListener('canplaythrough', function() {
        audio.play();
        
        audio.addEventListener('ended', function() {
          showModal();
          btn.disabled = false;
          loading.style.display = 'none';
        });
      });
      
      audio.addEventListener('error', function() {
        showModal();
        btn.disabled = false;
        loading.style.display = 'none';
      });
    }

    function showModal() {
      modal.style.display = 'block';
    }

    function closeModal() {
      modal.style.display = 'none';
    }

    window.onclick = function(event) {
      if (event.target == modal) {
        closeModal();
      }
    }
  </script>
</body>
</html>
)";
    request->send(200, "text/html", html);
  });

  // Serve file suara dari SPIFFS
  server.on("/sound.wav", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/sound.wav", "audio/wav");
  });

  // Serve file logo dari SPIFFS
  server.on("/logo.png", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/logo.png", "image/png");
  });



  // Endpoint untuk captive portal detection
  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request) {  // Android
    request->redirect("http://" + WiFi.softAPIP().toString());
  });
  
  server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request) {  // Apple
    request->redirect("http://" + WiFi.softAPIP().toString());
  });
  
  server.on("/ncsi.txt", HTTP_GET, [](AsyncWebServerRequest *request) {  // Windows
    request->send(200, "text/plain", "Microsoft NCSI");
  });
  
  server.on("/library/test/success.html", HTTP_GET, [](AsyncWebServerRequest *request) {  // MacOS
    request->send(200, "text/plain", "Success");
  });



  server.begin();
}

void loop() {
  dnsServer.processNextRequest();  // Handle DNS requests
}