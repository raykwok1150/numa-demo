# How to build
`gcc numa_demo.c -lnuma -o numa_demo -g`

# Run with numactl
`numactl --membind=3 ./numa_demo`

# log
```bash
(base) [root@localhost test]# ./numa_demo 
┌─────────────────────────────────────┐
 max num node: 7 
 current memory mask: 0xFF 
 node: 0, size: 65431MB, free: 34679MB 
 node: 1, size: 65535MB, free: 55223MB 
 node: 2, size: 65535MB, free: 63989MB 
 node: 3, size: 65535MB, free: 63528MB 
 node: 4, size: 65535MB, free: 62701MB 
 node: 5, size: 65535MB, free: 63163MB 
 node: 6, size: 65535MB, free: 63968MB 
 node: 7, size: 65535MB, free: 61769MB 
└─────────────────────────────────────┘
new memory mask: 0x2 
┌─────────────────────────────────────┐
 max num node: 7 
 current memory mask: 0xFF 
 node: 0, size: 65431MB, free: 29528MB 
 node: 1, size: 65535MB, free: 44983MB 
 node: 2, size: 65535MB, free: 63989MB 
 node: 3, size: 65535MB, free: 63528MB 
 node: 4, size: 65535MB, free: 62700MB 
 node: 5, size: 65535MB, free: 63163MB 
 node: 6, size: 65535MB, free: 63968MB 
 node: 7, size: 65535MB, free: 61769MB 
└─────────────────────────────────────┘
(base) [root@localhost test]# 
```
We can see that 5GB memory be allocted on node 3, and 10GB memory be allocted on node 1.
