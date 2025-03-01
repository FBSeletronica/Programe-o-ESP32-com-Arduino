import socket
import pyaudio
import numpy as np

# Configuração do socket UDP
UDP_IP = "0.0.0.0"  # Recebe de qualquer IP
UDP_PORT = 12345
BUFFER_SIZE = 2048  

# Configuração do áudio
SAMPLE_RATE = 16000
FORMAT = pyaudio.paInt16
CHANNELS = 1

# Inicializa o socket UDP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

# Ajusta o buffer de recepção do socket
sock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 8192)

print(f"📡 Aguardando áudio UDP na porta {UDP_PORT}...")

# Inicializa PyAudio para reprodução em tempo real
p = pyaudio.PyAudio()
stream = p.open(format=FORMAT, channels=CHANNELS, rate=SAMPLE_RATE, output=True)

try:
    while True:
        data, addr = sock.recvfrom(BUFFER_SIZE)  # Recebe pacotes do ESP32
        if data:
            audio_data = np.frombuffer(data, dtype=np.int16)  # Converte para NumPy
            stream.write(audio_data.tobytes())  # Reproduz o áudio
except KeyboardInterrupt:
    print("\nEncerrando...")
    sock.close()
    stream.stop_stream()
    stream.close()
    p.terminate()
