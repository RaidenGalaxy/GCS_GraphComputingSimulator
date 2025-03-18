#$1: nVertices
#$2: nGraph


echo "generating golden output ..."

find . -type f -name "*.py" | while read py_file; do
    dir=$(dirname "$py_file")
    cd "$dir"
    python3 "$(basename "$py_file")" $1 $2
    cd - > /dev/null
done

echo "generating golden output ... done"
