from ads1115 import *

a = ADS1115()

while True:
    # Loop infinitely
    try:
        # Print readings into rows
        print(a.read_adc(0), end="\t")
        print(a.read_adc(1), end="\t")
        print(a.read_adc(2), end="\t")
        print(a.read_adc(3), end="\n")
    except KeyboardInterrupt:
        # Exit loop
        print("\nProgram Stopped")
        break
