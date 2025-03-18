import numpy as np
from collections import defaultdict

import random
import argparse

# 设置命令行参数
parser = argparse.ArgumentParser()
parser.add_argument("nVertex", type=int, help="The number of vertex")
parser.add_argument("nGraph", type=int, help="The number of graphs.")
args = parser.parse_args()
vnum = args.nVertex

for i in range(args.nGraph):
	g_path = f"../graph_generate/PaRMAT_out/PaRMAT_out_{i}"
	data = np.genfromtxt(g_path,dtype=[int,int])
	edges = data.tolist()
	maxn = max(max(edge) for edge in edges)
	csr_dict = {key: [] for key in range(maxn + 1)}
	for j in range(len(edges)):
	    csr_dict[edges[j][0]].append(edges[j][1])	
ofs = 0	
csr_ofs = [0]
csr_edges = []
for value in csr_dict.values():
    ofs += len(value)
    csr_ofs.append(ofs)
    for dst in value:
        csr_edges.append(dst)

while(len(csr_ofs) <= vnum):
	    csr_ofs.append(ofs)
ofs_path = f"./ofs/ofs_{i}"
with open(ofs_path,"w") as f:
	    for ofs in csr_ofs:
	        f.write(f'{ofs}\n')	
edges_path = f"./edges/edges_{i}"
with open(edges_path,"w") as f:
	    for edge in csr_edges:
	        f.write(f'{edge}\n') 