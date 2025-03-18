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
	SrcID = 0
	for edge in edges:
	    G.add_edge(edge[0], edge[1])
	BFS_Ans = nx.shortest_path_length(G, source=SrcID, target=None, weight=None)
	for j in range(vnum):
		if j not in BFS_Ans.keys():
			BFS_Ans[j] = float('inf')
	BFS_Ans = sorted(BFS_Ans.items(), key=lambda d: d[0])

	out_path = f"./BFS_golden_out/BFS_golden_out_{i}"
	with open(out_path,'w') as f:
		for t in BFS_Ans:
			f.write(str(t[1]) +  '\n')

