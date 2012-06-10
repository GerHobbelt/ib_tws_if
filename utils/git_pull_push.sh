#! /bin/bash

pushd $(dirname $0)
cd ..

for f in $( git submodule foreach --quiet $* pwd ) ; do
    pushd .
    echo processing PATH/SUBMODULE: $f
    cd $f
    git pull --all
    git push --all
    popd
done

popd



