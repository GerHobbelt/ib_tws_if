#! /bin/bash
#
# import mongoose from google code (Mercurial) to git and treat the end result as a module.
#
# actions based on http://stackoverflow.com/questions/883452/git-interoperability-with-a-mercurial-repository
#

pushd ..
mkdir mongoose-hg
cd mongoose-hg

easy_install hg-git

hg clone https://mongoose.googlecode.com/hg/ mongoose
cd mongoose

# edit ~/.hgrc and add :
#
# [extensions]
# hgext.bookmarks =
# hggit =
#

hg bookmark -r default master

# edit: .hg/hgrc and add:
#
# [git]
# intree = true
#

hg pull
hg gexport

popd

