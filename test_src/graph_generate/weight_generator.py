import random
import argparse

# 设置命令行参数
parser = argparse.ArgumentParser(description="Generate n random numbers between a and b and write them to a file.")
parser.add_argument("nEdge", type=int, help="The number of edges.")
parser.add_argument("nGraph", type=int, help="The number of graphs.")
args = parser.parse_args()

a = 1
b = 10 
# 生成n个随机数
for i in range(args.nGraph):
    random_numbers = [random.uniform(a, b) for _ in range(args.nEdge)]

    # 写入到weight.txt
    path = f"./weight/weight_{i}"
    with open(path, "w") as file:
        for num in random_numbers:
            file.write(f"{num}\n")

