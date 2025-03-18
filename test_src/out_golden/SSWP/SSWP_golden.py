import networkx as nx
import heapq
import numpy as np
import argparse
def sswp(G, source):
    # 初始化所有节点的最宽路径宽度为负无穷
    width = {node: float('-inf') for node in G.nodes}
    width[source] = float('inf')  # 源节点的宽度是无限大
    
    # 使用优先队列（最大堆），通过宽度从大到小访问节点
    pq = [(-width[source], source)]  # 负值表示最大堆
    
    # 记录从源节点到各节点的最宽路径
    paths = {node: [] for node in G.nodes}
    paths[source] = [source]
    
    while pq:
        # 获取当前路径宽度最大的节点
        current_width, u = heapq.heappop(pq)
        current_width = -current_width
        
        for v, attr in G[u].items():
            edge_weight = attr['weight']
            # 计算当前路径的宽度
            new_width = min(current_width, edge_weight)
            
            # 如果找到一条更宽的路径，就更新路径
            if new_width > width[v]:
                width[v] = new_width
                paths[v] = paths[u] + [v]
                heapq.heappush(pq, (-new_width, v))
    
    return width, paths

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
	# 计算从节点 1 到所有其他节点的最宽路径
	width, paths = sswp(G, SrcID)
	out_path = f"./SSWP_golden_out/SSWP_golden_out_{i}"
	with open(out_path,'w') as f:
	    for target, path in paths.items():
	        f.write(str(width[target])+ '\n')	