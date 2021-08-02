#!/bin/bash
file_names=()
args=()
for f in inputFiles/*.tex 
do  
    base=$(basename -- $f) 
    file_names+=($base)
    input_files=($f)
    output_files=("outputFiles/"${base%.tex}".md")
    args+=("$input_files" "$output_files")
done
echo ${args[@]}
echo "Running Makefile"
make 

echo "Processing files ..."
 
./Latex2Markdown.out ${args[@]}
    
echo "Done"