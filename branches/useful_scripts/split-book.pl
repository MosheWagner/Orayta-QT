#!/usr/bin/perl
# split a book to smaller files
=pos
submitted under GNU GPL 3 licence: http://www.gnu.org/copyleft/gpl.html

Auther: abe izar - izar00@gmail.com

usage:
first set $SPLIT_CHAR.
then pas file to split (a book from torat-emet) as first argument.
=cut

use strict;
use warnings;

use Encode;
use utf8;
binmode STDOUT, "utf8";


my $SPLIT_CHAR = '^'; # you SHOULD modify this line manually!
print "split by: \'$SPLIT_CHAR\'\n";
#exit;

my $infile = shift;
open IN, "<:encoding(utf8)", $infile or die "cant read input file $infile\n$!";

my $counter = 0;
$_="";
my $conf = <IN>; #first line holds conf info.
$_ = <IN> until m|^[\$]|; # books header is in the line begining with '$'
my $head = $_;
chomp $head;
print "head $head-\n";
my $subhead = "";
#my $line;
#while ($line = <IN> and $line =~ /^[^SPLIT_CHAR]/ ){
#	print $line ;
#}
my @nextBlock;
my $itr = 0;
while (<IN>){
	if ($_ =~ /^[\^\#\@\~\!$SPLIT_CHAR]\s*(.*)/){ #break if we got to the a line with heading
		print "no empty line\n";
		@nextBlock = $1;
		last;
	} elsif ($_=~ /^\s*$/){ #break if we got to the an empty line
		print "emptyline\n";
		last;
	} else {
		$subhead = $subhead.$_;
	}
	$itr++;
#	print $itr;
}

# replace hebrew chars with english chars.
sub replace {
	my $string = decode_utf8(shift) ;
	$string =~ tr/אבגדהוזחטיכךלמםנןסעפףצץקרשת /abgdhozhticclmmnnsuppzzkrst_/;
	$string =~ tr/[A-Z]/[a-z]/; #convert all to lowercase.
	$string =~ s/[^0-z_\-]//g; #removes nearly every char, execpt english letters, and diggits. is used to remove nikud.
#	$string =~ s/[ְֱ֑֖֛֣֤֥֦֧֪֚֭֮֒֓֔֕֗֘֙֜֝֞֟֠֡֨֩֫֬֯]//g; #alternitive nikud remover that doesnt work.
	return decode_utf8($string);
}

$infile =~ /(.*)\.\w{3}$/;
my $path = $1;
mkdir $path or warn "cant make dir $path, $!" unless (-e $path);

sub mkConf ($$){
	my ($txtFilename, $bookName) = @_;
	my $confFilename = $txtFilename;
	die "cant parse confname" unless ($confFilename =~ s/\.\w{3}$/\.conf/); # replace file extension with .conf
	open OUT, ">:encoding(utf8)", $confFilename or die "cant write to $confFilename\n $!";
	
	print OUT "DisplayName=".$bookName  ;
	
	my @confText = split /&/, $conf;
	foreach (@confText) {
		s/^(UniqueId=.*)/$1$counter/; #append counter to the uid.
		s/_nbsp;/&nbsp;/g; #fix.
		print OUT $_."\n";
	}
	 
	close OUT;
}

sub makeNewFile {
	$counter++;
	my $append = "0" x (3 - length($counter));
	my $newBookName = shift(@nextBlock);
	my $newFilename = $path.'/'.$append.$counter.replace($newBookName).".txt";
	chomp $newBookName;
	open OUT, ">:encoding(utf8)", $newFilename or die "cant write to $newFilename\n $!";
	my $newHead = ($head." - ". $newBookName);
	print OUT $newHead."\n";
	print OUT $subhead;
	print OUT "\n";
	print OUT @nextBlock;
	
	close OUT;
	
	#now make conf
	mkConf($newFilename, $newBookName);
}

while ($_ = <IN>){
	
#	if ($_ =~ /^\^(.*)/){
	if ($_ =~ /^$SPLIT_CHAR(.*)/){
		print "newfile\nregex:\n$1\n";
		makeNewFile() if defined $nextBlock[0];
		
		@nextBlock = $1 ; #reset nextblock with current line.
	} else {
		s/\@PicWidth.*$//; #remove line that gives us truble.
		push @nextBlock, $_;
	}
	
}
makeNewFile() if defined $nextBlock[0]; # create the last block. TODO- make this more elegent.

close IN;
print "done\n";

