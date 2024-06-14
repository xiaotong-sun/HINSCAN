# 仓库说明

本仓库是HINSCAN的相关代码。

## 可执行文件说明

可执行文件存放于：`./executable/`目录下

hinscan_no_half_oneHopNB: 没有存储每一个遍历过的结点的邻居（针对A-P-T中的A-P，P-T），这两个信息没有存储，其实效果比都存储要好一些，因此最终会采用这个方案。

hinscan_no_oneHopNB: 只存储了两跳邻居的信息，没有存储一跳邻居的信息，因此在获取一跳邻居的时候会产生大量的重复，效果没有上面的那个好，也没有都存储的效果好。

hinscan_no_hop_optimize: 没有进行任何优化，没有存储任何点的二跳邻居或一跳邻居，因此会有大量的重复操作，运行时间较长

## 改进方案

1. × 验证的时候，使用不同的order去验证点: 不太好用，按度排序之后会变慢，无论是从小到大还是从大到小。

2. 采样+set intersection instrution speedup （refer to above paper）

3. try to figure out whether high level pruning is possible

4. 根据pscan的信息，作为我们heurestic的值
