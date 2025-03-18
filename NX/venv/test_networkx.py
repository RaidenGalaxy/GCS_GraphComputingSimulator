import networkx as nx
import pydot
import matplotlib.pyplot as plt

(dot_graph,) = pydot.graph_from_dot_file("example.dot")

G = nx.nx_pydot.from_pydot(dot_graph)

print("Nodes:", G.nodes())
print("Edges:", G.edges())

nx.draw(G, with_labels=True, node_color='lightblue', node_size=2000, font_size=12, font_weight='bold')
plt.savefig('graph.png')
