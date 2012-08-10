#! /bin/bash

pushd $(dirname $0)       2> /dev/null
cd ..

getopts ":fh" opt
#echo opt+arg = "$opt$OPTARG"
case "$opt$OPTARG" in
"?" )
	echo --- pull/push every git repo in this directory tree ---
	#echo full - args: $@
	for f in $( find . -maxdepth 3 -name '.git' ) ; do
		pushd .       2> /dev/null
		f=$( echo $f | sed -e 's#/\?\.git$##' )
		echo processing PATH/SUBMODULE: $f
		cd $f
		#echo $@
		$@
		git pull --all
		git push --all
		git push --tags
		popd         2> /dev/null
	done
	;;
	
f )
	echo --- pull/push the git repo and its submodules ---
	for (( i=OPTIND; i > 1; i-- )) do
		shift
	done
	#echo args: $@
	for f in $( git submodule foreach --recursive --quiet pwd ) ; do
		pushd .       2> /dev/null
		echo processing PATH/SUBMODULE: $f
		cd $f
		#echo $@
		$@
		git pull --all
		git push --all
		git push --tags
		popd         2> /dev/null
	done
	git pull --all
	git push --all
	git push --tags
	;;
	
* )
	cat <<EOT
$0 [-f] [args]	

pull & push all git repositories in the current path.

-f       : only pull/push this git repository and and the git submodules.
<no opt> : pull/push ANY git repository find in the current directory tree.

When further commandline [args] are specified, those are treated as a command
and executed for each directory containing a git repository. E.g.:

  #0 git commit -a
  
will execute a 'git commit -a' for every git repository.

EOT
	;;
esac


popd       2> /dev/null



