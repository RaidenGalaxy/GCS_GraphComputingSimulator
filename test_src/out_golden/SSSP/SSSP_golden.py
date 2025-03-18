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

	weight_path = f"../../graph_generate/weight/weight_{i}"
	weight = np.genfromtxt(weight_path,dtype=float)
	edges = data.tolist()
	SrcID = 0

	for edge,w in zip(edges,weight):
	    G.add_edge(edge[0], edge[1],weight=w)

	SSSP_Ans = nx.shortest_path_length(G, source=SrcID, target=None, weight='weight')
	for j in range(vnum):
		if j not in SSSP_Ans.keys():
			SSSP_Ans[j] = float('inf')

	SSSP_Ans = sorted(SSSP_Ans.items(), key=lambda d: d[0])

	out_path = f"./SSSP_golden_out/SSSP_golden_out_{i}"
	with open(out_path,'w') as f:
		for t in SSSP_Ans:
			f.write(str(t[1]) +  '\n')