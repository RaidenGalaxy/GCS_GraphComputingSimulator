#$1: nVertices
#$2: nEdges
#$3: nGraph


cd /home/xiaoxuxin/graph/test_src/graph_generate
./graph_generate.sh $1 $2 $3

cd /home/xiaoxuxin/graph/test_src/out_golden
./out_golden.sh $1 $3

cd /home/xiaoxuxin/graph/test_src/test_generate
python3 test_generate.py $1 $3