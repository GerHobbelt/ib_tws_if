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

# copy the config file to edit to tempfile; create an empty file to process when the config file does not exist yet.
# Done this way so gawk can write the result to the config file location.
tmpscript=`mktemp`
cat > ${tmpscript}.awk  <<EOT
#
# edit ~/.hgrc and add :
#
# [extensions]
# hgext.bookmarks =
# hggit =
#
BEGIN {
	has_ext = 0;
}

/extensions/	{
	has_ext++;
}

/hggit/		{
	has_ext++;
}

END {
	switch (has_ext)
	{
	case 0:
	case 1:
		printf("\n[extensions]\n");
		printf("hgext.bookmarks =\n");
		printf("hggit =\n");
		break;

	default:
		break;
	}
}

	{
	printf("%s\n", \$0);
}

EOT
cat ~/.hgrc > ${tmpscript}.cfg
gawk -f ${tmpscript}.awk ${tmpscript}.cfg > ~/.hgrc


hg bookmark -r default master


cat > ${tmpscript}.awk  <<EOT
#
# edit: .hg/hgrc and add:
#
# [git]
# intree = true
#
BEGIN {
	has_git = 0;
}

/git/	{
	has_git++;
}

/intree/	{
	if (has_git) has_git++;
}

END	{
	switch (has_git)
	{
	case 0:
		printf("\n[git]\n");
	case 1:
		printf("intree = true\n");
		break;

	default:
		break;
	}
}

	{
	printf("%s\n", \$0);
}

EOT
cat .hg/hgrc > ${tmpscript}.cfg
gawk -f ${tmpscript}.awk ${tmpscript}.cfg > .hg/hgrc


hg pull
hg gexport

popd

