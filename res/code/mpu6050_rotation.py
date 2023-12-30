from time import time, sleep
import smbus

bus = smbus.SMBus(1)
previous, current, elapsed = 0, 0, 0
x, y, z = 0, 0, 0

# MPU6050 registers
PWR_MGMT_1  = 0x6B
SMPLRT_DIV  = 0x19
CONFIG      = 0x1A
GYRO_CONFIG = 0x1B
INT_ENABLE  = 0x38
ACCEL_X     = 0x3B
ACCEL_Y     = 0x3D
ACCEL_Z     = 0x3F
GYRO_X      = 0x43
GYRO_Y      = 0x45
GYRO_Z      = 0x47

device_address = 0x68 # MPU6050 address

def init():
    # Write to sample rate register
    bus.write_byte_data(device_address, SMPLRT_DIV, 7)
    # Write to power management register
    bus.write_byte_data(device_address, PWR_MGMT_1, 1)
    # Write to Configuration register
    bus.write_byte_data(device_address, CONFIG, 0)
    # Write to Gyro configuration register
    bus.write_byte_data(device_address, GYRO_CONFIG, 24)
    # Write to interrupt enable register
    bus.write_byte_data(device_address, INT_ENABLE, 1)

def read_data(addr):
    # Get elapsed time for calculating gyro angle
    global current, elapsed
    previous = current
    current = time()
    elapsed = current - previous

    # Accel and Gyro value are 16-bit
    high = bus.read_byte_data(device_address, addr)
    low = bus.read_byte_data(device_address, addr + 1)

    # Concatenate higher and lower value
    value = (high << 8) | low

    # Get signed value from sensor
    if value > 32768: value -= 65536
    return value

def scale_gyro(val):
    return val / 131.0

def scale_accel(val):
    return val / 16384.0

def get_rotation():
    x_ang = scale_gyro(read_data(GYRO_X)) # Read gyro data (degrees per second)
    y_ang = scale_gyro(read_data(GYRO_Y))
    z_ang = scale_gyro(read_data(GYRO_Z))

    global x, y, z
    x += round(x_ang * elapsed * 1000) # Degrees/second multiplied by seconds = degrees
    y += round(y_ang * elapsed * 1000) # Round values to prevent drift
    z += round(z_ang * elapsed * 1000)

init()
while True:
    try:
        get_rotation()
        print("X: %d Y: %d Z: %d" % (x, y, z))
        sleep(0.2)
    except KeyboardInterrupt:
        print("Stopped")
        break
