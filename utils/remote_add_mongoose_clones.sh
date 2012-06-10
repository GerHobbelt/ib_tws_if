#! /bin/bash
#
# scan mongoose-hg subdirs and add each as a remote repo to the master @ mongoose:
#

pushd $(dirname $0)
pushd ../mongoose-hg

for f in $(find ./ -maxdepth 1 -type d -not -name mongoose -not -name '.' | sed -e 's/\.\///' ) ; do
    echo $f;
    pushd ../mongoose
    git remote add $f ../mongoose-hg/$f
    popd
done
cd ../mongoose
git remote update

popd
popd

