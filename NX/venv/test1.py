import networkx as nx
import pydot
import matplotlib.pyplot as plt

(dot_graph,) = pydot.graph_from_dot_file("example.dot")

G = nx.nx_pydot.from_pydot(dot_graph)

print("Nodes:", G.nodes())
print("Edges:", G.edges())

def analyze_graph(G):
    node_attributes = {node: G.nodes[node] for node in G.nodes()}
    print("\nNode Attributes:", node_attributes)

    edge_attributes = {edge: G[edge[0]][edge[1]] for edge in G.edges()}
    print("\nEdge Attributes:", edge_attributes)

    predecessors = {node: list(G.predecessors(node)) for node in G.nodes()}
    successors = {node: list(G.successors(node)) for node in G.nodes()}
    print("\nPredecessors:", predecessors)
    print("\nSuccessors:", successors)

    in_degrees = dict(G.in_degree())
    out_degrees = dict(G.out_degree())
    print("\nIn-degrees:", in_degrees)
    print("\nOut-degrees:", out_degrees)

    if nx.is_directed_acyclic_graph(G):
        topo_sort = list(nx.topological_sort(G))
        print("\nTopological Sort (Execution Order):", topo_sort)
    else:
        print("\nThe graph is not a DAG, so topological sort is not possible.")

    if nx.is_directed_acyclic_graph(G):
        longest_path_length = nx.dag_longest_path_length(G)
        longest_path = nx.dag_longest_path(G)
        print("\nLongest Path (Critical Path):", longest_path)
        print("Longest Path Length:", longest_path_length)
    else:
        print("\nThe graph is not a DAG, so critical path analysis is not possible.")

    return node_attributes, edge_attributes, predecessors, successors, in_degrees, out_degrees, topo_sort, longest_path

node_attributes, edge_attributes, predecessors, successors, in_degrees, out_degrees, topo_sort, longest_path = analyze_graph(G)

def visualize_graph(G):
    nx.draw(G, with_labels=True, node_color='lightblue', node_size=2000, font_size=12, font_weight='bold')
    plt.savefig('graph1.png')

visualize_graph(G)
