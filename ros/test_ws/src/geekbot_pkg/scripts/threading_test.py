#!/usr/bin/env python
import threading


def test():
	print("hello")


if __name__ == '__main__':
	t = threading.Timer(2.0, test)
	t.start()

