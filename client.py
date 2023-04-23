#!/usr/bin/env python3

import random
# import time
import socket
import numpy as np


def main():

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    addr = ("127.0.0.1", 5000)

    # for ii in range(500000):
    while(1):
        num = random.randint(100000, 100000000000)
        num = np.uint64(num)
        s.sendto(num.tobytes(), addr)


if __name__ == "__main__":
    main()
