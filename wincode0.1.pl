#!/usr/bin/perl
# convert a utf-8 file to MS hebrew incoding

use 5.006;
use strict;
use warnings;
use Encode;

sub recode ( $ ) {
	my $file = shift;
	$file = decode_utf8 $file;
	(warn "<!> proccesing $file not alowed\n" and return) unless (-T $file and -W _);
	open (FH, "<:utf8" , $file) or (warn "\ncant read $file, $!" and return);

	print STDERR "proccesing: $file\t";
	my $i =0;
	my @hold;
	for (<FH>) {
		s/\s+$/\r\n/ or s/\n/\r\n/;
		push @hold, $_;
		$i++;
	}
	close FH;
	print STDERR "lines: $i\t";
	open (FH, ">:encoding(cp1255)" , $file) or (warn "\ncant write to $file $!" and return);
	print FH @hold;
	close FH;
	print STDERR "done!\n";
}

my @files = glob (shift);
for (@files) {
	recode ($_);
}
