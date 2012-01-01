#!/usr/bin/perl

=pos
submitted under GNU GPL 3 licence: http://www.gnu.org/copyleft/gpl.html

i used this script to transalate all files and folder in 'orayta-books' to english.
it creates a conf file for folders with the original hebrew name.
i also fixes the 'AddSource' field in book conf files to match the new format.
the translation itself is done in this row:
tr/אבגדהוזחטיכךלמםנןסעפףצץקרשת/abgdhozhticclmmnnsuppzzkrst/;
so if someone wants to now what was the original name he can use this to guess.

Auther: abe izar - izar00@gmail.com

=cut

use strict;
use warnings;
use utf8;
use Encode;
use File::Find;
#binmode STDIN, "utf8" ;

binmode STDOUT, "utf8" ;

# replace hebrew chars with english chars.
sub replace {
	my $string = decode_utf8(shift) ;
	$string =~ tr/אבגדהוזחטיכךלמםנןסעפףצץקרשת /abgdhozhticclmmnnsuppzzkrst_/;
#	$string =~ tr/[A-Z]/[a-z]/; #convert all to lowercase.
 	$string =~ s/\.TXT$/\.txt/;
	return decode_utf8($string);
}

sub mkconf ($$) {
	my $oldname = decode_utf8(shift);
	my $newname = decode_utf8(shift);
		my $confName = $newname. ".conf";
		open FH, ">:utf8", $confName or (warn "cant create conf for folder: $oldname" and return);
		$oldname =~ s/^\d+_//;
		$oldname =~ y/_/ /;
		print FH "BranchName=", $oldname, "\n";
		close FH;
	
}

# fix conf lines that have filenames, to match the new english version.
sub fixConf {
	my $file = decode_utf8(shift);
#	my $newname = decode_utf8(shift);
	
	open FH2, "<:utf8", $file or warn "cant read file: ". $file;
	my @holder = "";
	my $needFix = 0;
	while (<FH2>) {
		
		if (/^(AddSource )([^\:]+)(\:.*)$/) {
			$needFix = 1;
			$_ = $1. decode_utf8(replace ($2)). $3."\n" ;
		}
		push @holder, $_;
	}
	close FH2;
	
	if ($needFix) {
		open FH2, ">:utf8", $file or warn "cant write to file: ". $file;
		for (@holder) {
			print FH2 $_;
		}
		close FH2;
	}
	

}

sub fixName {
	my $oldname = decode_utf8(shift);
	my $newname =  decode_utf8(replace ($oldname));
	
	mkconf($oldname, $newname) if (-d $oldname); 
	if ($newname eq $oldname) {
		print "no need to rename $oldname\n" ;
	} else { 
		(warn "cant rename $oldname" and return) unless (-e $oldname and ! -e $newname);
		rename ($oldname, $newname) or warn "rename erorr, $oldname" and return;
		print "file: $oldname renamed as: $newname\n";
	}
	
	if ($newname =~ /\.conf$/) {
		fixConf($newname);
	}
	
}


sub test {
	my $test = "אבגדהוזחטיכךלמםנןסעפףצץקרשת";
	print replace ($test);
}

#my @files = glob (shift);
#recurse (@files);
#
#sub recurse {
#	for (@_) {
#		fixName ($_);
#	}
#}

my @path;
push @path, glob decode_utf8 ($_) foreach @ARGV; 

print "processing: @path\n";


File::Find::finddepth (\&wanted, @path); #go through path and execute 'wanted' on overy file.

sub wanted {
	my $longName = $File::Find::name;
	return if ($longName =~ m|/\.|) ; #dont process hidden files.
	return if ($_ eq ".");
	fixName ($_);
	

}
#test();