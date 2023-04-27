#!/usr/bin/env python3

import random
import socket
import numpy as np
import time


# Delay functions
def no_delay():
    return 0


def random_delay():
    return random.random()

# Main


def main():

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    addr = ("127.0.0.1", 5000)

    delay_type = 'none'  # TODO set this with CLI
    delay_functions = {"none": no_delay, "random": random_delay}
    delay = delay_functions[delay_type]

    while True:
        try:
            time.sleep(delay())
            num = np.random.randint(100000, 100000000000, dtype=np.uint64)
            s.sendto(num.tobytes(), addr)
        except KeyboardInterrupt:
            print(" CTRL-C Received, exiting")
            break


if __name__ == "__main__":
    main()
