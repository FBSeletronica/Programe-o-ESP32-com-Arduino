/*
* Exemplo de uso do Touchpad para acordar o ESP32 do Deep Sleep
* Este exemplo utiliza o pino T2 como entrada do touchpad.
* O ESP32 entra em modo Deep Sleep e acorda quando o touchpad é tocado.
* O número de inicializações é impresso no Serial Monitor.
* O código utiliza a biblioteca "touch" do ESP32.
* 
 * Autor: Fábio Souza
 * Data: 28/04/2025
*/

#define TOUCH_PIN T2          // Pino do Touch
#define TOUCH_THRESHOLD 150   // Limite de toque (ajustar conforme necessário)

RTC_DATA_ATTR int bootCount = 0;      // Variável RTC para contar o número de inicializações
volatile bool touchDetected = false;  // Variável para detectar toque

// Função para imprimir a razão do wakeup
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();  

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO (EXT0)"); break;
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL (EXT1)"); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default: Serial.printf("Wakeup not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

// Função chamada quando o toque é detectado
void gotTouchEvent() {
  touchDetected = true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  print_wakeup_reason();

  // Inicializa o touchpad
  touchAttachInterrupt(TOUCH_PIN, gotTouchEvent, TOUCH_THRESHOLD);

  // Configura wakeup por Touchpad
  esp_sleep_enable_touchpad_wakeup();

  Serial.println("Indo para Deep Sleep...");
  Serial.println("Toque no pino para acordar!");
  esp_deep_sleep_start();
}

void loop() {
  // Nunca chega aqui
}
