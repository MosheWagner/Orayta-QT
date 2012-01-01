package myPkg;

use utf8;
# use 5.006;
use strict;
use warnings;
use Encode;

our $example;
sub getFiles ( $ ) {
	my @files;
	if (!defined @ARGV) {
		warn "no input file, using example\n";
		my $decoded_input = Encode::decode_utf8 (shift);
		# warn "$decoded_input\n";
		@files = glob ($decoded_input);
		$example =1;
		# warn "files: @files\n @_\n";
		}
	 else {
		 @files = glob(shift @ARGV);
		 $example = 0;
	 }
	# print "@files \n";
	# @files = sort {hebrew2num (decode  ("utf8", $a)) <=> hebrew2num (decode  ("utf8", $b))} @files;
	# print "\n<>\n@files \n";
	return @files;
}



our $location = {};
my %symbol = (
		1 => '!',
		2 => '~',
		3 => '@',
		4 => '#',
		5 => '^',
		6 => '$',
		);
sub manipulate ($$$) { 		
=pos
	this subrutine recives a header name, the header itself and the symbol for that level.
	it has several perposes: 
		1. determine if this header is the same as the previous, and if so leaves the subrutine.
		2. prints the header with the symbol.
		3. keeps track of all headers in order, so that we can print a table of conetents when we are done.
=cut
	my ($key, $place, $level) = @_;
	if (!defined $location->{$key}) {
		$location->{$key} = [$place,];
	} else {
		my $last = pop @{$location->{$key}};
		if ($place eq $last) {
			push @{$location->{$key}}, $last;
			err1 ("$key $place");
			return 0;
		}
		push @{$location->{$key}}, $last, $place;
	}
	
	print "$symbol{$level} $key $place\n";
}

sub manipulate2 { 		
=pos
	this subrutine recives a header name, the header itself and the symbol for that level.
	it has several perposes: 
		1. determine if this header is the same as the previous, and if so leaves the subrutine.
		2. prints the header with the symbol.
		3. keeps track of all headers in order, so that we can print a table of conetents when we are done.
=cut
	my ($place, $level, $text) = @_;
	$text = "" unless $text;
	if (!defined $location->{$level}) {
		$location->{$level} = [$place,];
	} else {
		my $last = pop @{$location->{$level}};
		if ($place eq $last) {
			push @{$location->{$level}}, $last;
			err1 ("$text $place, level: $level");
			return 0;
		}
		push @{$location->{$level}}, $last, $place;
	}
	
	print "$symbol{$level} $text$place\n";
}

sub err1 {
	print STDERR "repeted head: $_[0]\n";
}

sub toc {
	my $fh = *STDERR;
	print $fh "\n-----Table of Contents:------\n";
	for (keys %{$location}) {
		my $key = $_;
		for (@{$location->{$_}}) {
			print $fh "$key $_, "		
		}
		print $fh "\n";	
	}
}
sub clearToc {
	$location = {};
}


my %values = (
א => 1, ב => 2, ג => 3, ד => 4, ה => 5, ו => 6, ז => 7, ח => 8, ט=> 9, י => 10,
כ => 20, ל => 30, מ => 40, נ => 50, ס => 60, ע => 70, פ => 80, צ => 90, ק => 100,
ר => 200, ש => 300, ת => 400,
a => 1, b => 2, c => 3, d => 4,
);


sub hebrew2num ($) {
	my $a = shift;
	# print $a, "\n";
	my @num = split (//, $a);
	my $result =0;
	for my $key (@num) {
		$result += $values{$key} unless (!defined $values{$key}) ;
	}
	# print "$_ $values{$_}\t" for @num;
	# print "\n";
	# return ("request: ", @num, " result: ", $result) ;
	return ($result);
}

=pod

=head2 mkConf
	
	should create a simple conf file for orayta.
	
=cut
my $uid_num= 4004;
sub uid {
	# my $num= (rand (10e10) % (10e8+1)) + 3000;
	eval {warn "using base uniqe id as: $uid_num";};
	$uid_num++;
	return $uid_num;
	
}

sub clearFilename ( $ ) {
	$_[0] =~ /(.*)\.[^\.\/]+$/;
	my $res = $1 || $_[0];
	return $res;
}
		


sub mkConf ($$$) {
	my ($filename, $head, $source) = @_;
	my $conf = join ("", (clearFilename ($filename), ".conf"));
	(warn "$conf exists, skipping conf creation" and return) if (-e $conf);
	open FH, ">:utf8", "$conf" or die $!;
	# select FH;
	print STDERR "creating configureations file: $conf\n";
	my $dname = ${$location->{$head}}[$#{$location->{$head}}] || "";
	print FH "DisplayName=", $head, " ", $dname, "\n";
	print FH "TextSource=", $source, "\n";
	print FH "UniqueId=", uid, "\n";
	close FH;
}

sub log ( $ ) {
	my $file = shift or (warn "no log" and return);
	if (-e $file) {
		warn "$file exists, no loging\n";
		return;
	}
	open LOGFILE, ">:utf8", $file or (warn "cant create logfile $file, $!" and return);
	*STDERR = *LOGFILE;
}
