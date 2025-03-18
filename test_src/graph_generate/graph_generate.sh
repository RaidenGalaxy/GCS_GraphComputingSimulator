#$1: nVertices
#$2: nEdges
#$3: nGraph

echo "generating graph ..."
for i in $(seq 0 $(($3-1)));
do
    ./PaRMAT -nVertices $1 -nEdges $2 -output "./PaRMAT_out/PaRMAT_out_$i" -threads 4 -sorted -noEdgeToSelf -noDuplicateEdges
done

echo "generating graph ... done"

echo "generating edge weight ..."

python3 weight_generator.py $2 $3

echo "generating edge weight ... done"