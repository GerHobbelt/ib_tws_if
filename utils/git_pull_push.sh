#! /bin/bash

pushd $(dirname $0)
cd ..

case x$1 in
x-f | x/f )
	shift
	for f in $( git submodule foreach --quiet $* pwd ) ; do
		pushd .
		echo processing PATH/SUBMODULE: $f
		cd $f
		#git pull --all
		#git push --all
		popd
	done
	;;
	
* )
	for f in $( find . -name '.git' ) ; do
		pushd .
		f = $( echo $f | sed -e 's#/?\.git$##' )
		echo processing PATH/SUBMODULE: $f
		cd $f
		$*
		#git pull --all
		#git push --all
		popd
	done
	;;
esac

popd



