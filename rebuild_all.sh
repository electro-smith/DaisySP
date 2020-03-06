#!/bin/bash
echo "Rebuilding Everything. . ."
echo "Building DaisySP"
make clean | grep "warningr\|error"
make | grep "warningr\|error"
echo "Done."
./rebuild_examples.sh
./rebuild_docs.sh
