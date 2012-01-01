#!/usr/bin/perl
# convert a file from MS hebrew encoding to utf-8
=pos
submitted under GNU GPL 3 licence: http://www.gnu.org/copyleft/gpl.html

Auther: abe izar - izar00@gmail.com
=cut


use strict;
use warnings;
use Encode;
use File::Find;


sub recode ( $ ) {
	my $file = shift;
	$file = decode_utf8 $file;
	#(warn "<!> proccesing $file not alowed\n" and return) unless (-T $file and -W _);
	#warn "<!> proccesing $file not alowed it is not Text (or maybe wrong encoding)\n" and return if (! -T $file);
	warn "<!> proccesing $file not alowed it is not writeable\n" and return if (! -W $file);
	open (FH, "<:encoding(windows-1255)" , $file) or (warn "\ncant read $file, $!" and return);

	print "proccesing: $file\t";
	my $i =0;
	my @hold;
	for (<FH>) {
		s/\s+$/\n/ or s/\n/\n/;
		push @hold, $_;
		$i++;
	}
	close FH;
	print "lines: $i\t";
	open (FH, ">:encoding(utf8)" , $file) or (warn "\ncant write to $file $!" and return);
	print FH @hold;
	close FH;
	print "done!\n";
}

# this was the old method before we used recursion-
#my @files = glob (shift);
#for (@files) {
#	recode ($_);
#}

my @path = glob (shift); 
print "processing: @path\n";


File::Find::finddepth (\&wanted, @path); #go through path and execute 'wanted' on overy file.

sub wanted {
	my $longName = $File::Find::name;
	return if ($longName =~ m|/\.|) ; #dont process hidden files.
	return if ($_ eq ".");
	recode ($_);# if /txt$/i;
	

}
