import networkx as nx
import numpy as np
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("nVertex", type=int, help="The number of vertex")
parser.add_argument("nGraph", type=int, help="The number of graphs.")
args = parser.parse_args()
vnum = args.nVertex

for i in range(args.nGraph):
	# 创建有向图
	G = nx.DiGraph()
	for j in range(vnum):
		G.add_node(j) 
	# 有向图之间边的关系
	g_path = f"../../graph_generate/PaRMAT_out/PaRMAT_out_{i}"
	data = np.genfromtxt(g_path,dtype=[int,int])

	edges = data.tolist()

	for edge in edges:
	    G.add_edge(edge[0], edge[1])
	PR_Ans = nx.pagerank(G, alpha=0.85)
	PR_Ans = sorted(PR_Ans.items(), key=lambda d: d[0])
	out_path = f"./PR_golden_out/PR_golden_out_{i}"
	with open(out_path,'w') as f:
		for t in PR_Ans:
			f.write(str(t[1]) + '\n')