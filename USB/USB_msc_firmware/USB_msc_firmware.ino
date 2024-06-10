#include "USB.h"              // Inclui a biblioteca para suporte a USB
#include "FirmwareMSC.h"      // Inclui a biblioteca para atualização de firmware via MSC (Mass Storage Class)

// Instancia um objeto da classe FirmwareMSC para gerenciar atualizações de firmware
FirmwareMSC MSC_Update;

// Função de callback para tratar eventos USB e MSC
static void usbEventCallback(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
  // Verifica se o evento pertence à base de eventos do Arduino USB
  if (event_base == ARDUINO_USB_EVENTS) {
    arduino_usb_event_data_t *data = (arduino_usb_event_data_t *)event_data;
    switch (event_id) {
      case ARDUINO_USB_STARTED_EVENT: // Evento quando o USB é conectado
        Serial.println("USB PLUGGED"); 
        break;
      case ARDUINO_USB_STOPPED_EVENT: // Evento quando o USB é desconectado
        Serial.println("USB UNPLUGGED"); 
        break;
      case ARDUINO_USB_SUSPEND_EVENT: // Evento quando o USB é suspenso
        Serial.printf("USB SUSPENDED: remote_wakeup_en: %u\n", data->suspend.remote_wakeup_en); 
        break;
      case ARDUINO_USB_RESUME_EVENT:  // Evento quando o USB é retomado
        Serial.println("USB RESUMED"); 
        break;
      default: 
        break;
    }
  // Verifica se o evento pertence à base de eventos do firmware MSC
  } else if (event_base == ARDUINO_FIRMWARE_MSC_EVENTS) {
    arduino_firmware_msc_event_data_t *data = (arduino_firmware_msc_event_data_t *)event_data;
    switch (event_id) {
      case ARDUINO_FIRMWARE_MSC_START_EVENT: // Evento quando a atualização MSC inicia
        Serial.println("MSC Update Start"); 
        break;
      case ARDUINO_FIRMWARE_MSC_WRITE_EVENT: // Evento quando há escrita durante a atualização MSC
        Serial.printf("MSC Update Write %u bytes at offset %u\n", data->write.size, data->write.offset);
        Serial.print(".");
        break;
      case ARDUINO_FIRMWARE_MSC_END_EVENT:   // Evento quando a atualização MSC termina
        Serial.printf("\nMSC Update End: %u bytes\n", data->end.size); 
        break;
      case ARDUINO_FIRMWARE_MSC_ERROR_EVENT: // Evento quando ocorre um erro na atualização MSC
        Serial.printf("MSC Update ERROR! Progress: %u bytes\n", data->error.size); 
        break;
      case ARDUINO_FIRMWARE_MSC_POWER_EVENT: // Evento de alteração de condição de energia durante a atualização MSC
        Serial.printf("MSC Update Power: power: %u, start: %u, eject: %u", data->power.power_condition, data->power.start, data->power.load_eject);
        break;
      default: 
        break;
    }
  }
}

void setup() {
  Serial.begin(115200);         // Inicializa a comunicação serial com a taxa de baud de 115200
  Serial.setDebugOutput(true);  // Habilita a saída de depuração serial

  USB.onEvent(usbEventCallback); // Define a função de callback para eventos USB
  MSC_Update.onEvent(usbEventCallback); // Define a função de callback para eventos MSC
  MSC_Update.begin();           // Inicia o gerenciamento de atualização MSC
  USB.begin();                  // Inicia o gerenciamento USB
}

void loop() {
  // Loop principal vazio
}
