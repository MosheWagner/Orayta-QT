#!/usr/bin/perl
# rename a file to match it's uid.

# usage:
# perl stripuid2.pl <path> 
# <path> may contain global symbols i.e. '~' or '*', and may be repeated. 

=pos
submitted under GNU GPL 3 licence: http://www.gnu.org/copyleft/gpl.html

Auther: abe izar - izar00@gmail.com
=cut

use strict;
use warnings;
use File::Find;

# *STDERR = *STDOUT;

my $warning= "\n<<<ERROR!!!>>>";


sub stripFileUid ($) {
	my $file = shift  ;
	warn "$warning proccesing $file not alowed\n" and return unless (-l $file and ! -d _); # test that file is text file and not folder.
	
	print "proccesing: $file\t";
	my $uid;
	# $_ = "";
	# {
		# $_ = <FH> until /UniqueId=(\d+)/ and $uid = $1 and last;
		# die "cant find uid\n";
	# }
	sub getuid {
		my $f = shift;
		#open file and search for uid in the file
		open (FH, "<" , $f) or warn "$warning cant read $f, $!" and return;
		for (<FH>) {
			/UniqueId=(\d+)/ and return $1;
		}
		close FH;
		return;
	}
	
	#here we get the find a uid:
	unless ($uid= getuid ($file)) {
		#if we haven't found a uid inside the file, try looking in it's conf.
		$file =~ m{(.+)(\.[^\.]+)$}i;
		my $conf = "$1.conf";
		$uid = getuid ($conf);
	}
	warn "$warning cant find uid\n" and return unless (defined $uid);
	
	$file =~ m{(.*/|)([^/]+)(\.txt)$}i or (warn "$warning cant find pattern, $!" and return);
	# print "$1\n$2\n";
	
	my $newname = join ("", ($1, $uid, $3));
	
	warn "$warning $newname exists\n" and return if -e $newname;
	rename ($file, $newname) or (warn "$warning error renaming, $!" and return);
	print  "new name: $newname\n";
}

my @path;
push @path, glob $_ foreach @ARGV; 
 
print "processing: @path\n";


File::Find::find (\&wanted, @path); #go through path and execute 'wanted' on overy file.

sub wanted {
	my $longName = $File::Find::name;
	return if ($longName =~ m|/\.|) ; #dont process hidden files.
	return if ($_ eq "."); #dont process this folder twice.
	stripFileUid ($longName) if /\.txt$/i; #process only files that end with ".txt"
	

}
print "done!\n";