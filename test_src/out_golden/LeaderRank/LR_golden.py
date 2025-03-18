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
	def leaderrank(G):
	    # 节点数量
	    num_nodes = G.number_of_nodes()
	    # 添加背景节点
	    G.add_node("Leader")
	    for node in G.nodes():
	        if(node != "Leader"):
	            G.add_edge(node,"Leader")
	            G.add_edge("Leader",node)
	    LR = dict.fromkeys(G.nodes(), 1.0)
	    LR["Leader"] = 0.0
	    tempLR = dict.fromkeys(G.nodes(), 0.0)
	    tempLR["Leader"] = 0.0
	    error = 0.0
	    # 迭代计算
	    for _ in range(1000):
	        error = 0.0
	        for node in G.nodes():
	            tempLR[node] = 0.0        
	        for node in G.nodes():
	            for neighbor in G.neighbors(node):
	                tempLR[neighbor] += 1.0 /G.out_degree(node) * LR[node] 
	        # 检查是否收敛
	        for node in tempLR:
	            error = error + abs(tempLR[node] - LR[node])
	        if(error <= 0.00001):
	            break
	        for node in tempLR:
	            LR[node] = tempLR[node]
	    # 分配背景节点的LR值
	    avg = tempLR["Leader"] / num_nodes
	    for keys in tempLR.keys():
	        tempLR[keys] += avg
	    del tempLR["Leader"]
	    return tempLR
	LR = leaderrank(G)
	LRsum = sum(LR.values())
	for key in LR:
	    LR[key] = LR[key] / LRsum
	LR = sorted(LR.items(), key=lambda d: d[0])
	out_path = f"./LR_golden_out/LR_golden_out_{i}"
	with open(out_path,'w') as f:
		for t in LR:
			f.write(str(t[1]) + '\n')
