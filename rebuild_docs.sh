#!/bin/bash
daisysp_dir=./

# Generate MD

## daisysp
echo "creating daisysp markdown files. . . "
for header in $daisysp_dir/modules/*.h; do
    fname=${header##*/}
    fname=${fname%.h}
    ex=$daisysp_dir/examples/$fname/ex_$fname.cpp
    outfile=$daisysp_dir/doc/$fname.md
    echo "    "$fname
    python makedoc.py $header > $outfile
    # add example to the end of each doc.
    echo $'\n## Example\n' >> $outfile
    if [ -f "$ex" ]; then
        echo $'```C++\n' >> $outfile
        cat $ex >> $outfile
        echo $'\n```' >> $outfile
    else
        echo "No example Provided for $fname"
        echo "No example Provided" >> $outfile
    fi
done
echo "done."


# Generate PDFs

mkdir -p doc
## daisysp
pdf_name=doc/daisysp_reference.pdf
cat resources/preamble.md > temp.md
echo "\pagebreak" >> temp.md
for file in $daisysp_dir/doc/*.md; do
    cat $file >> temp.md
    echo "\pagebreak" >> temp.md
done
echo "Generating documenation for " $pdf_name
#pandoc temp.md -s --template=./resources/template.tex --pdf-engine=xelatex --toc -o $pdf_name
pandoc temp.md -s --template=./resources/template.tex --toc -o $pdf_name
echo "Created $pdf_name"
# remove temp file.
rm temp.md
echo "done."
echo "Creating Style Guide PDF"
pdf_name=doc/style_guide.pdf
pandoc ./resources/Style-Guide.md  -s --template=./resources/template.tex --toc -o $pdf_name
echo "finished."


