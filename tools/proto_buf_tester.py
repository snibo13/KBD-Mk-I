import sys
import serial

sys.path.append("../")
import messages.status_pb2


status = messages.status_pb2.Test()
status.data = "Saluton Mondo"

ser = serial.Serial("/dev/ttyACM0")

ser.write(status.SerializeToString())
