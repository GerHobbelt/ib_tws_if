#! /bin/bash
#
# import mongoose from google code (Mercurial) to git 
#

tmpar=$(mktemp).txt

cat > ${tmpar} <<EOT
#
# each line lists a mongoose clone we're going to pull
# 
# empty lines and lines starting with a # dash are ignored.
#
				
mongoose    	= https://mongoose.googlecode.com/hg/
lrabinovich 	= https://lrabinovich-bind-to-zero-port.googlecode.com/hg/ 
rtems		= https://dufault-rtems-support.googlecode.com/hg/
teamYASSL	= https://teamyassl-conf.googlecode.com/hg/
andyrahn-async	= https://andyrahn-async.googlecode.com/hg/
andyrahn-dyn	= https://andyrahn-dynamic-client.googlecode.com/hg/
martinblais	= https://martinblais-fixes.googlecode.com/hg/
zhenglei	= https://zhenglei-cond-wait-fix.googlecode.com/hg/
ken-threads 	= https://ken-threads-ondemand.googlecode.com/hg/
mattjhealy	= https://mattjhealy-mongoose.googlecode.com/hg/
goodtaste	= https://goodtaste-uncompletedhttpbody.googlecode.com/hg/
gjhurlbu	= https://gjhurlbu-custom-memory-mongoose.googlecode.com/hg/
mkovak		= https://mkovax-1.googlecode.com/hg/
markw-thread	= https://markw-thread-data.googlecode.com/hg/
arnoudvdC	= https://arnoutvandecappelle-multiplereads.googlecode.com/hg/
amoltambe	= https://amoltambe-m.googlecode.com/hg/
bencollins	= https://collinsben-mg.googlecode.com/hg/
ericscouten2	= https://ericscouten-mongoose2.googlecode.com/hg/
ericscouten	= https://ericscouten-mongoose.googlecode.com/hg/
martinlamb	= https://martinlamb-no-dl.googlecode.com/hg/
klevernet	= https://hackerklevernet-mongoose-fixes.googlecode.com/hg/
aysabtu		= https://aysabtu-certchain.googlecode.com/hg/
martinlamb-ssl	= https://martinlamb-ssl-options.googlecode.com/hg/
martinlamb-desc	= https://martinlamb-ssl-descriptive-errors.googlecode.com/hg/
jabinyan	= https://jabinyan-range-put.googlecode.com/hg/


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


