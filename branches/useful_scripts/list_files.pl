#!/usr/bin/perl
# searches for conf files and creates a sorted list of 'uid:bookname'.
# usage: perl files_list.pl DIRECTORY

# use 5.006;
use strict;
use warnings;
use File::Find;

my %hash;
sub getVals { # get the uid and bookname
	my $uid = \0; my $bookname1 = ""; my $bookname2 = "";
	
	my $filename = shift;
	# my $filename = "books/000_WELLCOME.conf";
	
	open FH, "$filename" or warn "cant open $filename" and return ; 
	while (<FH>) {
		 if (/^UniqueId=\s*(\d+)\s*$/) {$uid = $1} 
		 elsif (/^DisplayName=\s*(.+)$/) {$bookname1 = $1}
		 elsif (/^ForcedBookName=\s*(.+)$/) {$bookname2 = $1};
	}
	# close FH;
	
	warn "no uid found!" and return if ($uid == \0);
	# my $bookname = "$bookname2 <OR> $bookname1";
	my $bookname = "empty";
	if ($bookname2 =~ /^\s*$/) {
		$bookname = $bookname1 unless ($bookname1 =~ /^\s*$/) ;
	} else {
		$bookname = $bookname2;
	}
	
	# my $bookname = ($bookname2 =~ /^\s+$/) ? $bookname1 : $bookname2; #choose bookname2 only if it isn't empty.
	
	$hash{$uid} = $bookname;
	# print $bookname;
}



sub output {
	my @sortedKeys = sort {$a <=> $b} keys %hash;
	my @sorted = "";
	for (@sortedKeys) {
		push @sorted, "$_:$hash{$_}\n";
	}
	
	print "@sorted";
	# print "@sortedKeys";
}


my $path = shift || glob '~/'; #if no path given as argument search home dir.
print "searcing for confs in $path\n";

# $| =1;

File::Find::find (\&wanted, $path); #go through path and execute 'wanted' on overy file.

sub wanted {
	my $longName = $File::Find::name;
	if ($longName =~ /\.conf$/ and -T $_) {# file is a conf file and also text file.
		return if ($longName =~ m|/\.|) ; #dont process hidden files.
		# print "$_\n";
		getVals ($_);
		return;
	}
	else { return }
}
output();

