#! /bin/bash
#
# import mongoose from google code (Mercurial) to git 
#

pushd $(dirname $0)

tmpar=$(mktemp).txt

cat > ${tmpar} <<EOT
#
# each line lists a mongoose clone we're going to pull
# 
# empty lines and lines starting with a # dash are ignored.
#
				
mongoose    	= mongoose
mongoose-wiki  	= wiki.mongoose
lrabinovich 	= lrabinovich-bind-to-zero-port 
rtems		= dufault-rtems-support
teamYASSL	= teamyassl-conf
andyrahn-async	= andyrahn-async
andyrahn-dyn	= andyrahn-dynamic-client
martinblais	= martinblais-fixes
zhenglei	= zhenglei-cond-wait-fix
ken-threads 	= ken-threads-ondemand
mattjhealy	= mattjhealy-mongoose
goodtaste	= goodtaste-uncompletedhttpbody
gjhurlbu	= gjhurlbu-custom-memory-mongoose
mkovak		= mkovax-1
markw-thread	= markw-thread-data
arnoudvdC	= arnoutvandecappelle-multiplereads
amoltambe	= amoltambe-m
bencollins	= collinsben-mg
ericscouten2	= ericscouten-mongoose2
ericscouten	= ericscouten-mongoose
martinlamb	= martinlamb-no-dl
klevernet	= hackerklevernet-mongoose-fixes
aysabtu		= aysabtu-certchain
martinlamb-ssl	= martinlamb-ssl-options
martinlamb-desc	= martinlamb-ssl-descriptive-errors
jabinyan	= jabinyan-range-put
timma666	= timma666-mg-get-cookie-fix
rickard-cgi	= rickard-fixes
colinleitner	= colinleitner-dav
mevdschee90	= mevdschee-issue-90
lenniaraki	= lenniearaki-mp4
ken-threads	= ken-threads-ondemand
tekert		= tekert-keepalive-fix
bel2125         = bel2125-mongoose
visionofarun    = visionofarun-mongoose
jmikhail-memfix = jmikhail-memfix
vovodroid-req   = vovodroid-request-complete-event
barrossmar-ssi  = barrossmar-ssi-echo
morgandel-file  = morgandel-enablefileserving
jigsaw-sqlite   = jigsaw-mongoose
gerhobbelt-gh   = gerhobbelt-github-mongoose

EOT

cat ${tmpar} | while read line ; do
	line=$(echo ${line} | sed -e 's/^#.*//' -e 's/\s+//')
	dst=${line%=*}
	src=${line#*=}
	if test -z "${dst}" -o -z "${src}" ; then
		continue;
	fi
	#echo "Importing ${src} to ${dst}:"
	./mongoose_hg2git.sh  ${src} ${dst}
done

popd

