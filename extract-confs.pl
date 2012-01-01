#!/usr/bin/perl
# should get confs from tom file and create our-style conf.

# usage : perl extract-confs.pl file1 [file2...]

=pos
submitted under GNU GPL 3 licence: http://www.gnu.org/copyleft/gpl.html

Auther: abe izar - izar00@gmail.com
=cut

use strict;
use warnings;
use lib qw(./);
#use myPkg;
use Encode;
use utf8;

#my @files = myPkg::getFiles ($tmp);
my @files;
push @files, glob $_ foreach @ARGV; 


sub mkConf($$$) {
	my ($txtFilename, $bookName, $confText) = @_;
	my $confFilename = $txtFilename;
	die "cant parse confname" unless ($confFilename =~ s/\.\w{3}$/\.conf/); # replace file extension with .conf
	open OUT, ">:encoding(utf8)", $confFilename or die "cant write to $confFilename\n $!";
	
	print OUT "DisplayName=".$bookName  ;
	$bookName =~ /^([^-]+?)\s+\-/;
	my $name4tree = $1 || $bookName;
	
	my @text = split /&/, $confText;
	foreach (@text) {
#		s/^(UniqueId=.*)/$1$counter/; #append counter to the uid.
		s/^(ForcedBookName=).*$/$1$name4tree/; #fix for books which get there tree display name wrong.
		s/_nbsp;/&nbsp;/g; #another fix. 
		print OUT $_."\n";
	}
	 
	close OUT;
}

foreach my $iter (@files) {
#	print "$iter\n";
	my $file = $iter;
	open INFILE, "<:encoding(utf8)", $file or die "error: $!" ;
	print "\nprocessing file: $file\n";
	
	my $confText = <INFILE>;
	$_ = "";
	$_ = <INFILE> until m|^\s*\$\s*(.+)$|; #run until we find heading that starts with '$';
	
	(my $head = $1) or (print STDERR "cant find header, skipping\n" and next);
	
	
#	myPkg::mkConf( $file, $head, $source);
	mkConf($file, $head, $confText);
	# myPkg::toc();
	# myPkg::clearToc;
	
} 
print "done!\n";
