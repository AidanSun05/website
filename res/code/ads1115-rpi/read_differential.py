from ads1115 import *

a = ADS1115()

while True:
    try:
        print(a.read_adc_differential(DIFF_0_1))
    except KeyboardInterrupt:
        print("\nProgram Stopped")
        break
