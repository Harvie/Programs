#!/bin/sh
ssh-keygen -P '' -f ssh-a.key >/dev/null
ssh-keygen -P '' -f ssh-b.key >/dev/null
md5sum ssh-?.key ssh-?.key.pub
rm ssh-?.key ssh-?.key.pub
