#!/bin/env python

import ctypes
from ctypes.util import find_library
libc = ctypes.CDLL(find_library("libc"))
libc.printf(b"TEST: %d %s\n", 1234, b'mujstring')

