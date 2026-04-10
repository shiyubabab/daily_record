##########################################################################
# File Name: build.sh
# Author: mlxh
# mail: mlxh_gto@163.com
# Created Time: Mon 06 Apr 2026 08:03:53 PM CST
#########################################################################
#!/bin/sh

gcc -Wall -Wextra -o main main.c -lm
gcc -Wall -Wextra -o train train.c -lm

