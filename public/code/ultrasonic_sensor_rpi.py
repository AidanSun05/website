import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

# GPIO Pin constants
TRIG_PIN = 27
ECHO_PIN = 22
RED_PIN = 26
GREEN_PIN = 20

# Set GPIO as input/output
GPIO.setup(TRIG_PIN, GPIO.OUT)
GPIO.setup(ECHO_PIN, GPIO.IN)
GPIO.setup(RED_PIN, GPIO.OUT)
GPIO.setup(GREEN_PIN, GPIO.OUT)

def get_distance_cm():
    # Send the 10us Pulse
    GPIO.output(TRIG_PIN, True)
    time.sleep(0.00001)
    GPIO.output(TRIG_PIN, False)

    start_time, stop_time = 0, 0

    # Get the time when echo pin is high (pulse started)
    while GPIO.input(ECHO_PIN) == 0:
        start_time = time.time()

    # Get the time when echo pin is low (pulse ended)
    while GPIO.input(ECHO_PIN) == 1:
        stop_time = time.time()

    # Time difference to find pulse length
    elapsed_time = stop_time - start_time

    # Multiply with the sonic speed (34300 cm/s) and divide by 2
    distance = (elapsed_time * 34300) / 2

    return distance

while True:
    try:
        cm = get_distance_cm()

        if cm < 20:
            # Less than 15cm, red LED on and green LED off
            GPIO.output(RED_PIN, True)
            GPIO.output(GREEN_PIN, False)
        else:
            # Greater than 15cm, red LED off and green LED on
            GPIO.output(RED_PIN, False)
            GPIO.output(GREEN_PIN, True)

        print("Distance: %.1f cm" % cm)
        time.sleep(0.2)

    except KeyboardInterrupt:
        break # CTRL+C to end program

print("Program Stopped")
GPIO.cleanup() # Reset all GPIO
