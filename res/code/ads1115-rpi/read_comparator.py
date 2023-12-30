from ads1115 import *

a = ADS1115()

while True:
    try:
        print(a.read_adc_comparator(0, 2000, 10000)) # Traditional mode
        # print(a.read_adc_comparator(0, 2000, 10000, traditional=False)) # Window mode
        # print(a.read_adc_comparator(0, 2000, 10000, active_low=False)) # Traditional mode with active high
    except KeyboardInterrupt:
        print("\nProgram Stopped")
        break
