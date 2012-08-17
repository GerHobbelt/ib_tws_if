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

aktiniya-xcap   = aktiniya-xcap-server
amoltambe       = amoltambe-m
andyrahn-async  = andyrahn-async
andyrahn-dyn    = andyrahn-dynamic-client
arnoudvdC       = arnoutvandecappelle-multiplereads
aysabtu         = aysabtu-certchain
barrossmar-ssi  = barrossmar-ssi-echo
bel2125         = bel2125-mongoose
bencollins      = collinsben-mg
colinleitner    = colinleitner-dav
ericscouten     = ericscouten-mongoose
ericscouten2    = ericscouten-mongoose2
gjhurlbu        = gjhurlbu-custom-memory-mongoose
goodtaste       = goodtaste-uncompletedhttpbody
jabinyan        = jabinyan-range-put
jigsaw-sqlite   = jigsaw-mongoose
jmikhail-memfix = jmikhail-memfix
klevernet       = hackerklevernet-mongoose-fixes
lenniaraki      = lenniearaki-mp4
lrabinovich     = lrabinovich-bind-to-zero-port
markw-thread    = markw-thread-data
martinblais     = martinblais-fixes
martinlamb      = martinlamb-no-dl
martinlamb-desc = martinlamb-ssl-descriptive-errors
martinlamb-ssl  = martinlamb-ssl-options
mattjhealy      = mattjhealy-mongoose
mevdschee90     = mevdschee-issue-90
mkovak          = mkovax-1
morgandel-file  = morgandel-enablefileserving
rickard-cgi     = rickard-fixes
rtems           = dufault-rtems-support
teamYASSL       = teamyassl-conf
tekert          = tekert-keepalive-fix
timma666        = timma666-mg-get-cookie-fix
visionofarun    = visionofarun-mongoose
vovodroid-glerr = vovodroid-get-last-error
vovodroid-req   = vovodroid-request-complete-event
zhenglei        = zhenglei-cond-wait-fix

gerhobbelt-gh   = gerhobbelt-github-mongoose
gerhobbelt-360  = gerhobbelt-mongoose-issue-360

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

