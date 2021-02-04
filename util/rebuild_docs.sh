#!/bin/bash
daisysp_dir=./

# create folder
mkdir -p doc

# Generate doxygen HTML
echo "Creating doxygen documentation"
doxygen Doxyfile > /dev/null

# Generate reference manual PDF
reference_manual_pdf_name=doc/daisysp_reference.pdf
doxygen_latex_dir=doc/latex
doxygen_latex_outfile=doc/latex/refman.pdf
if ! [ -x "$(command -v pdflatex)" ]; then
    echo 'Warning: no reference manual will be created. Please install pdflatex.' >&2
else
    echo "Generating reference manual PDF"
    # remember current directory
    cwd=$(pwd)
    cd $doxygen_latex_dir
    make pdf > /dev/null
    # go back and move the complete pdf file
    cd $cwd
    mv $doxygen_latex_outfile $reference_manual_pdf_name
fi

# Generate Style Guide
echo "Creating Style Guide PDF"
pdf_name=doc/style_guide.pdf
pandoc ./resources/Style-Guide.md  -s --template=./resources/template.tex --toc -o $pdf_name

echo "finished."
