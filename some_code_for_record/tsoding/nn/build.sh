##########################################################################
# File Name: build.sh
# Author: mlxh
# mail: mlxh_gto@163.com
# Created Time: Wed 08 Apr 2026 01:34:38 PM CST
#########################################################################

#!/bin/sh

gcc -Wall -Wextra -o nn main.c -lm
gcc -Wall -Wextra -o cnn cnn.c -lm
