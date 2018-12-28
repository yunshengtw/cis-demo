CIS Demo
--------

Prerequisites
-------------
1. A Linux or MacOS computer with packages numpy, matplotlib installed
2. A CIS demo board
3. A router connecting to the host and the CIS demo board

Execution Steps
---------------
1. Turn on both the router and the CIS demo board
2. Go to the `cis-demo` directory
3. Uplaod the firmware and hardware to the CIS demo board with `make upload-cis-driver`
   and `make upload-bit`
4. Find out the host ip with `ifconfig`
5. Execute `./recv.py` on the host
6. Log in to the CIS demo board with `ssh root@10.1.1.11`
7. Execute `./cis-driver <host ip>` on the CIS demo board
