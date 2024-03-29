import serial
import time

# Defina a variável global
mensagem_serial = 'Teste'

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)  # ajuste a porta conforme necessário
intervalo = 3

for i in range(intervalo, 0, -1):
    print(f"cadastrando em {i}")
    time.sleep(1)
print("ID Cadastrado")

# Use a variável global no lugar de 'Hello World'
ser.write(mensagem_serial.encode('utf-8'))
time.sleep(1)

ser.close()