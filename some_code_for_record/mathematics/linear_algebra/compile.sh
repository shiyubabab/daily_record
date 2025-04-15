##########################################################################
# File Name: complic.sh
# Author: mlxh
# mail: mlxh_gto@163.com
# Created Time: Tue 15 Apr 2025 05:12:59 PM CST
#########################################################################
#!/bin/bash
PATH=/home/edison/bin:/home/edison/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/work/tools/gcc-3.4.5-glibc-2.3.6/bin
export PATH

g++ -I /usr/include/eigen3 least_squared.cpp
