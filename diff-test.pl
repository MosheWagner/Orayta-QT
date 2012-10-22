#!/usr/bin/perl
# see hew diffs work

use strict;
use warnings;
#use Text::Diff;
#use Text::Diff3;
#use Algorithm::Diff;
#use Algorithm::Merge;
use File::Find;
use Encode;
binmode STDOUT, "utf8";
*LOG = *STDOUT; 

#my $MAIN_PATH = shift || ".";
#my $ORY_NEW_DIR = "ory-new";
#my $ORY_OLD_DIR = "ory-old";
#my $ORY_BASE_DIR = "ory-base";
#my $TOM_DIR = "tom";

sub getFileText($){
	my $filename = shift;
	my @text;
	open FH, "<:encoding(utf8)", $filename or die "can't open file: $filename\n $!\n";
	while (<FH>){
		push @text, $_;
	}
	return @text;
}

sub writeListToFile (\@$) {
	my ($arrayRef, $filename) = @_;
	open FH, ">:encoding(utf8)", $filename or die "can't open file: $filename\n $!\n";
	die "can't write to file: $filename" unless -w $filename;
	
	print FH $_ foreach @{$arrayRef};	
	
}

# get file text but ignore lines that we don't care about such as m|^//|
sub getFileTextIgnoreUnwanted($){
	my $filename = shift;
	my @text;
	open FH, "<:encoding(utf8)", $filename or die "can't open file: $filename\n $!\n";
	while (<FH>){
		 if( m|^//| or m|^&|  ){}
		 else {
		 	s/\s+$//; #ignore trailing whitespace and empty newlines
		 	push @text, $_;
		 }
	}
	close FH;
	return @text;
}

#my $file1 = "/home/avi/orayta/compare/ory-343-lnk-test/ne.txt";
#my $file1 = "/home/avi/orayta/compare/ory-343-lnk-test/ne3.txt";
#my $file2 = "/home/avi/orayta/compare/tom-lnk-test/ne3.txt";
#my $file2 = "/home/avi/orayta/compare/ory-343-lnk-test/ne.txt";
#my $file3 = "/home/avi/orayta/compare/ory-base-test/ne3.txt";

my $file1 = "/home/avi/orayta/compare/ory-343-lnk-test/test.txt";
my $file2 = "/home/avi/orayta/compare/tom-lnk-test/test.txt";
my $file3 = "/home/avi/orayta/compare/ory-base-test/test.txt";

my @seq1 = getFileText($file1);
my @seq2 = getFileText($file2);

#modified copy of Text::Diff3::merge
sub merge3 {
    my $pivot             = shift;                                  # array ref
    my $doca              = shift;                                  # array ref
    my $docb              = shift;                                  # array ref
    my $callbacks         = shift || {};
    my $keyGen            = shift;

    my $conflictCallback  = $callbacks -> {'CONFLICT'} || sub ($$$) { (
        qq{<!-- ------ BASE -------------- -->\n},
        (@{$_[2]}),
        qq{<!-- ------- A ---------------- -->\n},
        (@{$_[0]}),
         qq{<!-- ------ B ---------------- -->\n},
        (@{$_[1]}),
        qq{<!-- ------  END  CONFLICT ------ -->\n},
    ) };

    my $diff = Algorithm::Merge::diff3($pivot, $doca, $docb, $keyGen, @_);

#    print Data::Dumper -> Dump([$diff]), "\n";

    my @ret;

    my @conflict = ( [], [] ,[] );

    foreach my $h (@{$diff}) {
        my $i = 0;
        #print "op: ", $h -> [0];
        if($h -> [0] eq 'c') { # conflict
            push @{$conflict[0]}, $h -> [2] if defined $h -> [2];
            push @{$conflict[1]}, $h -> [3] if defined $h -> [3];
            push @{$conflict[2]}, $h -> [1] if defined $h -> [1]; #add the original line as third param
        }
        else {
            if(@{$conflict[0]} || @{$conflict[1]}) {
                push @ret, &$conflictCallback(@conflict);
                @conflict = ( [], [], [] );
            }
            if($h -> [0] eq 'u') { # unchanged
                push @ret, $h -> [2] || $h -> [3];
            }
            elsif($h -> [0] eq 'o') { # added
                push @ret, $h -> [2] if defined $h -> [2];
            }
            elsif($h -> [0] eq 'l') { # added by left
                push @ret, $h -> [2] if defined $h -> [2];
            }
            elsif($h -> [0] eq 'r') { # added by right
                push @ret, $h -> [3] if defined $h -> [3];
            }
        }
        #print " : ", join(" ", @ret), " [$$h[1],$$h[2],$$h[3]]\n";
    }

    if(@{$conflict[0]} || @{$conflict[1]} ) {
        push @ret, &$conflictCallback(@conflict);
    }

    if(wantarray) {
        return @ret;
    }
    return \@ret;
}


# merge 3 files into the 4'th file
sub myMerge($$$$){
	my ($myFile, $baseFile, $yourFile, $outFile)  = @_;
	my @myText = getFileText($myFile);
	my @baseText = getFileText($baseFile);
	my @yourText = getFileText($yourFile);
	
#	my $merge = Text::Diff3::merge(\@myText,\@baseText,\@yourText);
#	if ($merge->{conflict}) {
#        print STDERR "conflict\n";
#    }
#    for my $line (@{$merge->{body}}) {
#        print "$line\n";
#    }
    
#    my @merged = Algorithm::Merge::merge(\@baseText,\@myText,\@yourText,{ 
#               CONFLICT => sub ($$) {  
#				print "conflict, 1:\n@{$_[0]}\n2:\n@{$_[1]}\n";
##				@{$diffs{$right}};
#               }
#           });
#
#
#           
#           print "alg-mrege:>>>>>>>>>\n@merged"."\n";
##           print "@$merged"."\n";

	 my @merged3 = merge3(\@baseText,\@myText,\@yourText,
	 		{ 
               CONFLICT => sub ($$$) { 
               	 print LOG "conflict, selecting 2. 1:\n@{$_[0]}\n2:\n@{$_[1]}\nbase:\n@{$_[2]}\n";
               	 
				#TODO : create a method to merge single lines which have a conflict. see below this subroutine
				return @{$_[1]}; # select line b to output.
            	}
           }
           );
      
      writeListToFile(@merged3, $outFile);
       
#           print "alg-mrege:>>>>>>>>>\n@merged3"."\n";
#           print "@$merged"."\n";
    
}

			# the folowing is the previos efort to do a merge on confilcted lines :
#					my @line1 = @{$_[0]};
#					my @line2 = @{$_[1]};
#					my @line3 = @{$_[2]};
#					
#					my @lineA;
#					foreach my $line (@line1) {
#					 push @lineA, split $line  
#					};
#					my @lineB;
#					foreach my $line (@line2) {
#					 push @lineB, split $line  
#					};					
#					my @lineO;
#					foreach my $line (@line3) {
#					 push @lineO, split $line  
#					};
#					
#					
#					my @linemerge = merge3(\@lineO,\@lineA,\@lineB,{ 
#		               CONFLICT => sub ($$$) {  return @{$_[1]}; # select line b to output.
#							}
#		            	});
#				return @linemerge; # try doing another merge on the conflict

sub diffPercent ($$){
	my $file1 = shift;
	my $file2 = shift;
	my @seq1 = getFileTextIgnoreUnwanted($file1);
	my @seq2 = getFileTextIgnoreUnwanted($file2);
	my $diff = Algorithm::Diff->new( \@seq1, \@seq2 );
#	$diff = shift;
#	$diff->Base( 1 );   # Return line numbers, not indices
	my $chars = "";
    while(  $diff->Next()  ) {
        next   if  $diff->Same();
#        my $sep = '';
#        if(  ! $diff->Items(2)  ) {
#            printf "%d,%dd%d\n",
#                $diff->Get(qw( Min1 Max1 Max2 ));
#        } elsif(  ! $diff->Items(1)  ) {
#            printf "%da%d,%d\n",
#                $diff->Get(qw( Max1 Min2 Max2 ));
#        } else {
#            $sep = "---\n";
#            printf "%d,%dc%d,%d\n",
#                $diff->Get(qw( Min1 Max1 Min2 Max2 ));
#        }
		
		
        $chars = $chars.$_  for  $diff->Items(1);
#        print $sep;
		$chars = $chars.$_   for  $diff->Items(2);
    }
#    return $chars;
	my $diffLen = length($chars);
	my $file1Len =0;
	my $file2Len =0;
	$file1Len += length($_) for @seq1;
	$file2Len += length($_) for @seq2;
	my $percent = $diffLen *100 / ($file1Len + $file2Len);
#	print "diff len: $diffLen file1 len: $file1Len file2 len: $file2Len percent: $percent\n";
#	print "same\n" if $percent == 0;
	
	return $percent;
}

use constant bigDiff => 75;
sub manualDiff(@){
#	my @fileList = ($oryNewFilename, $tomFilename, $oryOldFilename, $oryBaseFilename);
	my @fileList = @_;
	my ($oryNewFilename, $tomFilename, $oryOldFilename, $oryBaseFilename) = @fileList;
	# lets test if ory-base exist. if not all we need to do is merge the three remaninig files.
	if ((! -e $oryBaseFilename) || (diffPercent($oryNewFilename,$oryBaseFilename) > bigDiff)){
		myMerge($oryNewFilename, $oryOldFilename, $tomFilename, $oryNewFilename);
	} else{
		#first we will merge old into tmpfile, and then we will merge tmp into new.
		my $tmpfile = glob("~/orayta/compare/tmpfile.txt");
		myMerge($tomFilename, $oryBaseFilename, $oryOldFilename, $tmpfile);
		myMerge($oryNewFilename, $oryOldFilename,$tmpfile, $oryNewFilename);
		
	}
	
}

#my $MAIN_PATH = shift || ".";
my $MAIN_PATH = glob("~/orayta/compare");
my $ORY_NEW_DIR = "ory-new";
my $ORY_OLD_DIR = "ory-old";
my $ORY_BASE_DIR = "ory-base";
my $TOM_DIR = "tom-new";

my $filename ="1531.txt";
my $oryNewFilename = "$MAIN_PATH/$ORY_NEW_DIR/$filename" ;
	my $tomFilename = "$MAIN_PATH/$TOM_DIR/$filename";
	my $oryOldFilename = "$MAIN_PATH/$ORY_OLD_DIR/$filename";
	my $oryBaseFilename = "$MAIN_PATH/$ORY_BASE_DIR/$filename";
my @fileList = ($oryNewFilename, $tomFilename, $oryOldFilename, $oryBaseFilename);
#manualDiff( @fileList);
myMerge($file1, $file3, $file2, "tmp.txt");




