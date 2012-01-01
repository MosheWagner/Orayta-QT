#!/usr/bin/perl
# convert MS and any other newline char to linux newline char

use 5.006;
use strict;
use warnings;
use Encode;
my $file = shift;
$file = decode_utf8 $file;
die "<!> proccesing $file not alowed\n" unless (-W $file and (-T $file or -l $file));
open (FH, "<" , $file) or die "\ncant read $file, $!";

print STDERR "proccesing: $file\t";
my $i =0;
my @hold;
for (<FH>) {
	s/\s+$/\n/ and $i++;
	push @hold, $_;
}
close FH;
print STDERR "lines: $i\t";
open (FH, ">" , $file) or die "\ncant write to $file $!";
print FH @hold;
close FH;
print STDERR "done!\n";
