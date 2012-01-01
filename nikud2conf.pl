#!/usr/bin/perl
# add "Nikud" to a given conf.
# example :
# perl nikud2conf.pl ~/Books/010_mdrs/10_mdrs_rbh/*.conf

=pos
submitted under GNU GPL 3 licence: http://www.gnu.org/copyleft/gpl.html

Auther: abe izar - izar00@gmail.com
=cut

use strict;
use warnings;

sub pr {
	my $file = shift;
	open FH, ">>:encoding(utf8)", $file or warn "cant open file $file $!" and return;
	print "file: $file...\t";
	print FH "\nNikud\n";
	print "done\n";
}

for (glob (shift)) {
	pr $_;
}
