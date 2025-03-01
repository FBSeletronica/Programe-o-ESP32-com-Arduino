
#intale as bibliotecas necessárias
# pip install pyserial numpy

import serial
import wave
import numpy as np

# Configuração da porta serial (ajuste conforme necessário)
SERIAL_PORT = "COM6"  # No Windows, use COMx. No Linux/Mac, algo como "/dev/ttyUSB0"
BAUD_RATE = 115200
SAMPLE_RATE = 16000  # Deve ser o mesmo usado no ESP32
DURATION = 10  # Tempo de gravação em segundos
OUTPUT_FILE = "output.wav"

# Inicializa a comunicação serial
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

# Abre o arquivo WAV para escrita
wavefile = wave.open(OUTPUT_FILE, "wb")
wavefile.setnchannels(1)  # Mono
wavefile.setsampwidth(2)  # 16 bits
wavefile.setframerate(SAMPLE_RATE)

print(f"Gravando áudio por {DURATION} segundos...")

# Captura os dados do ESP32
audio_data = []
bytes_to_read = SAMPLE_RATE * 2 * DURATION  # 2 bytes por amostra (16 bits)
while len(audio_data) < bytes_to_read:
    data = ser.read(1024)  # Lê em blocos de 1024 bytes
    if data:
        audio_data.extend(data)

# Fecha a porta serial
ser.close()

# Converte os dados para um formato correto e salva no WAV
audio_array = np.frombuffer(bytearray(audio_data), dtype=np.int16)
wavefile.writeframes(audio_array.tobytes())
wavefile.close()

print(f"Áudio salvo como {OUTPUT_FILE}")
