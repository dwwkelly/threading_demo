#!/usr/bin/env python3


import threading
import queue
import socket
import numpy as np

q = queue.Queue()


def largest_prime_factor(n):
    factor = n
    i = 2
    while i * i <= factor:
        if factor % i:
            i += 1
        else:
            factor = factor // i
    return factor


def Process():

    while True:
        data = q.get()
        largest_prime_factor(data)


def main():

    sock = socket.socket(socket.AF_INET,  # Internet
                         socket.SOCK_DGRAM)
    sock.bind(("127.0.0.1", 5000))

    n_threads = 6
    for ii in range(n_threads):
        threading.Thread(target=Process).start()

    while True:
        data, addr = sock.recvfrom(64)
        num = np.frombuffer(data, dtype=np.uint64)
        q.put(num)


if __name__ == "__main__":
    main()
