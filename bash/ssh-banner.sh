#!/bin/bash
strings /usr/sbin/sshd | tr , '\n' | egrep 'OpenSSH[-_][0-9](\.[0-9])*(p[0-9])?$' | head -n 1
