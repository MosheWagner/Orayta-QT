#!/usr/bin/perl
# diff orayta and torat-emet

=pos
submitted under GNU GPL 3 licence: http://www.gnu.org/copyleft/gpl.html

Auther: abe izar - izar00@gmail.com

usage:
first set globals to match your paths, then pas the working directory as an argument or modify it manually too.
you are expected to have 4 directories which all have the same files with small differences.
i.e. you have a file named 1143.txt in all 4 dirs, which is actually a link to a file someware else.
then we will compare the 4 file and merge the best results into ory-new.
=cut

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

# globals:
my $MAIN_PATH = glob(shift || ".");
#my $ORY_NEW_DIR = "ory-343-lnk";
#my $ORY_OLD_DIR = "ory-190-lnk";
#my $ORY_BASE_DIR = "ory-09-lnk";
#my $TOM_DIR = "tom-lnk";
my $ORY_NEW_DIR = "new-lnk";
my $ORY_OLD_DIR = "old-lnk";
my $ORY_BASE_DIR = "base-lnk";
my $TOM_DIR = "tom-lnk";

# files who have more the bigDiff percent of difference are concluded very different.
use constant bigDiff => 100;

my %counters = (
	'total'=> 0,
	'same'=>0,
	'conflict'=>0,
	'bigDifference'=>0,
	'manual'=>0,
	'missing'=>0,
);

sub getFileText($){
	my $filename = shift;
	my @text;
	open FH, "<:encoding(utf8)", $filename or die "can't open file: $filename\n $!\n";
	while (<FH>){
		push @text, $_;
	}
	close FH;
	return @text;
}

# get file text but ignore lines that we don't care about such as line which start white '//'
sub getFileTextIgnoreUnwanted($){
	my $filename = shift;
	my @text;
	open FH, "<:encoding(utf8)", $filename or die "can't open file: $filename\n $!\n";
	while (<FH>){
		 if( m|^//| or m|^&|  ){} # ignore these lines
		 else {
		 	s/\s+$//; #ignore trailing whitespace and empty newlines
		 	push @text, $_; #put the cleaned text in the output list.
		 }
	}
	close FH;
	return @text;
}

sub writeListToFile (\@$) {
	my ($arrayRef, $filename) = @_;
	open FH, ">:encoding(utf8)", $filename or die "can't open file: $filename\n $!\n";
	die "can't write to file: $filename" unless -w $filename;
	
	print FH $_ foreach @{$arrayRef};	
	
}

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

    if(@{$conflict[0]} || @{$conflict[1]}) {
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
	

	 my @merged3 = merge3(\@baseText,\@myText,\@yourText,
	 		{ 
               CONFLICT => sub ($$$) { 
               	 print LOG "conflict, selecting 1. 1:\n@{$_[0]}\n2:\n@{$_[1]}\nbase:\n@{$_[2]}\n";
               	 $counters{'conflict'}++;
				#TODO : create a method to merge single lines which have a conflict. see below this subroutine
				return @{$_[0]}; # select line a to output. for some reason line a is the tom version and not ours.
            	}
           }
           );
      
      writeListToFile(@merged3, $outFile);
       
#           print "alg-mrege:>>>>>>>>>\n@merged3"."\n";
#           print "@$merged"."\n";
    
}


sub diffPercent ($$){
	print "1\n";
	my $file1 = shift;
	my $file2 = shift;
	my @seq1 = getFileTextIgnoreUnwanted($file1);
	print "2\n";
	my @seq2 = getFileTextIgnoreUnwanted($file2);
	print "3\n";
	my $diff = Algorithm::Diff->new( \@seq1, \@seq2 );
#	my $diff = Text::Diff->diff()
	print "4\n";
	
	# get all chars that are differnet in both files:
	my $chars = "";
    while(  $diff->Next()  ) {
        next   if  $diff->Same();
        
        $chars = $chars.$_  for  $diff->Items(1);
		$chars = $chars.$_   for  $diff->Items(2);
    }
	
	print "5\n";
	return 0 if $chars eq "";
	my $diffLen = length($chars);
	return 0 if $diffLen == 0;
	
	#get length of our original files:
	my $file1Len =0;
	my $file2Len =0;
	$file1Len += length($_) for @seq1;
	$file2Len += length($_) for @seq2;
	
	#now calculate the difference:
	my $percent = $diffLen *100 / ($file1Len + $file2Len);
#	print "diff len: $diffLen file1 len: $file1Len file2 len: $file2Len percent: $percent\n";
#	print "same\n" if $percent == 0;
	
	return $percent;
}

#sub equal($$){
#	#TODO: make this method more efficiat.
#	my $file1 = shift;
#	my $file2 = shift;
#	if (diffPercent($file1, $file2) == 0){
#		return 1;
#	} else {
#		return 0;
#	}
#	
#}

sub equal ($$){
	print "0\n";
	my ($file1, $file2) = @_;
#	$file1 = [getFileTextIgnoreUnwanted($file1)];
#	$file2 = [getFileTextIgnoreUnwanted($file2)];
##	for(my $i; $i < 100; i++) {
#	for my $i (0 .. $#{$file1}) {
#		return 0 unless (${$file1}[$i] eq ${$file2}[$i]);
#	}
	open FH1, $file1 or die $!;
	open FH2, $file2 or die $!;
	my $size1 = (stat FH1)[7];
	my $size2 = (stat FH2)[7];
	close FH1;
	close FH2;
	
	($size1 == $size2)? return 1 : return 0;	
#	return 1;
}



sub chooseLines(@){
#	print "\n";
	print $_ for @_;
}

# gat text from one file and write it to another
sub writeFrom($$){
	my ($from, $to) = (shift, shift);
	my @text = getFileText($from);
	open FH, ">:encoding(utf8)", $to or die "can't open file: $to\n $!\n";
	print FH $_ for @text;
	close FH; 
}

#deprecated.
sub myDiff(\@\@\@\@){ 
	my $oryNew_r = shift ;
	my $tom_r = shift;
	my $oryOld_r = shift;
	my $oryBase_r = shift || \0;
	
	my @oryNew = @{$oryNew_r};
	my @tom = @{$tom_r} ;
	my @oryOld = @{$oryOld_r};
	my @oryBase = @{$oryBase_r} || \0;
	
	my $mainDiff = Algorithm::Diff->new( \@oryNew, \@tom );
	my $oldDiff = Algorithm::Diff->new( \@oryNew, \@oryOld );
	my $baseDiff = Algorithm::Diff->new( \@oryNew, \@oryBase );
	
	# $mainDiff->Base( 1 );   # Return line numbers, not indices
	    while(my $pos = $mainDiff->Next()  ) {
	    	#set the location in other diffs:
#	    	$oldDiff->Reset($pos);
#			$baseDiff->Reset($pos+1);
			print "hunk pos: $pos\n";
#			my $linePos =  @{$mainDiff->[1]};
			my $hunkBeginA = $mainDiff->Get("Min1");
			my $hunkEndA = $mainDiff->Get("Max1");
			my $hunkBeginB = $mainDiff->Get("Min2");
			my $hunkEndB = $mainDiff->Get("Max2");
#			print "line pos: ".$linePos ."\n";
#			print "tom line: " . $tom[$pos] . "\n";
#			print "oryNew line: " . $oryNew[$pos]. "\n";
#			print "oryOld line: " . $oryOld[$pos]. "\n";		
#			print "oryBase line: " . $oryBase[$pos]. "\n";
			
	    	my $sep = "\n";
	        if  (my @lines = $mainDiff->Same()) { 			#lines are the same in both files
	        	chooseLines(@lines) ;
	#        	next;
	#        } elsif(  ! $mainDiff->Items(2)  ) { 			#line a has additions to line b.
	#            printf "%d,%dd%d\n",
	#                $mainDiff->Get(qw( Min1 Max1 Max2 ));
	#        } elsif(  ! $mainDiff->Items(1)  ) { 			#line b has additions to line a.
	#            printf "%da%d,%d\n",
	#                $mainDiff->Get(qw( Max1 Min2 Max2 ));
	        } else { 									#both lines have additions opon each other.
	#            $sep = "---\n";
	#            printf "%d,%dc%d,%d\n",
	#                $mainDiff->Get(qw( Min1 Max1 Min2 Max2 ));
				print "ne\n";
#					if ($baseDiff->Same()){
#						print "tom line: " . $tom[$pos];
#					}
				print "tom lines: " . @tom[$hunkBeginB..$hunkEndB];
				print "ory-new line: " . @oryNew[$hunkBeginA..$hunkEndA];
				
				
					
	        }
	    	            printf "a: begin-%d, end-%d\nb: begin-%d, end-%d\n",
	       	         $mainDiff->Get(qw( Min1 Max1 Min2 Max2 ));
	#        print "< $_"   for  $mainDiff->Items(1);
	#        print $sep;
	#        print "> $_"   for  $mainDiff->Items(2);
	    }
}



sub manualDiff(@){
	$counters{'manual'}++;
#	my @fileList = ($oryNewFilename, $tomFilename, $oryOldFilename, $oryBaseFilename);
	my @fileList = @_;
	print LOG "doing manual diff\n";
	
	my ($oryNewFilename, $tomFilename, $oryOldFilename, $oryBaseFilename) = @fileList;
	# lets test if ory-base exist. if not all we need to do is merge the three remaninig files.
	if (! -e $oryBaseFilename) {
		print LOG "base not found, will ignore base\n";
		myMerge($oryNewFilename, $oryOldFilename, $tomFilename, $oryNewFilename);
	}
	elsif (diffPercent($oryNewFilename,$oryBaseFilename) > bigDiff){
		print LOG "base is very different from new, will ignore base\n";
		myMerge($oryNewFilename, $oryOldFilename, $tomFilename, $oryNewFilename);
	} else {
		#first we will merge old into tmpfile, and then we will merge tmp into new.
		print LOG "gradual 4-way merge\n";
		my $tmpfile = "/tmp/ory-diff-tmpfile.txt";
		unlink $tmpfile if (-e $tmpfile);
		myMerge($tomFilename, $oryBaseFilename, $oryOldFilename, $tmpfile);
		myMerge($oryNewFilename, $oryOldFilename,$tmpfile, $oryNewFilename);
		unlink ($tmpfile) or die "cant remove tmpfile, $!";
		
	}
	
}

sub processFile($){
	my $filename = shift;
	my $oryNewFilename = "$MAIN_PATH/$ORY_NEW_DIR/$filename" ;
	my $tomFilename = "$MAIN_PATH/$TOM_DIR/$filename";
	my $oryOldFilename = "$MAIN_PATH/$ORY_OLD_DIR/$filename";
	my $oryBaseFilename = "$MAIN_PATH/$ORY_BASE_DIR/$filename";
	
	$counters{'total'}++;
	
	print LOG "\n\nprocessing file: $oryNewFilename\n";
	
	my @fileList = ($oryNewFilename, $tomFilename, $oryOldFilename, $oryBaseFilename);
	
#	my @oryNew = getFileText($oryNewFilename) ;
#	my @tom = getFileText($tomFilename) ;
#	my @oryOld = getFileText($oryOldFilename) ;
#	my @oryBase = getFileText($oryBaseFilename) ;

	# if file is too large, we should skip it:
	open FH1, $oryNewFilename or die $!;
	my $size = (stat FH1)[7];
	close FH1;
	if ($size > 10000000){
		$counters{'larg'}++;
		warn "$oryNewFilename too big, skipping"; 
		return ;
	}
	
	
	# we are done with this set of files, remove the links.
	sub end (@){
		my @files = shift; # this was originaly a bug but i decided to leave it. only first file gets opporated.
		foreach my $file (@files) {
#    		unlink($file);
			rename ($file, "$file.old") or warn "can't rename $file, $!";
		}
	}

	eval {	
		if (! -e $tomFilename) {
			# torat-emet doesnt seem to have this file, skip it.
			print LOG "tom doesnt exist, skipping\n";
			$counters{'missing'}++;
	#		end(@fileList);
		} else {
#			my $percent;
#			print "percent: $percent\n";
			if (equal ($oryNewFilename, $tomFilename)){
				# our file is the same as in torat-emet, no need to do anything.
				print LOG "new and tom are the same, skipping\n";
				$counters{'same'}++;
	#		end(@fileList);
			} elsif ((my $percent = diffPercent($oryNewFilename, $tomFilename)) > bigDiff){
				# our file is very different from torat-emet, merging is unwize.
				$counters{'bigDifference'}++;
				print LOG "new and tom are very different ($percent), skipping\n";
			} elsif ((! -e $oryOldFilename) || (equal($oryNewFilename, $oryOldFilename))) { 
				# no older version by us, or our current version is the same as our older version, we must test where the change came from:
					print LOG "no old or old same as new, diciding what to do...\n";
				if ((! -e $oryBaseFilename) || (equal($oryOldFilename, $oryBaseFilename))){ 
					# we dont have an older version, or the old and the oldest versions are the same. this means that the change was made by torat-emet and not by us.
					print LOG "no base and old or no diff in our virsions, selecting tom\n";
					# choose the version from torat-emet:
					writeFrom($tomFilename,$oryNewFilename );
		#			end(@fileList);
				}else{
					# the file was changed also by us and also by torat emet. we must do a line-by-line compare.	
					manualDiff(@fileList);
				}
			} else {
			# the file was changed also by us and also by torat emet. we must do a line-by-line compare.	
			manualDiff(@fileList);
			}
		}
	} or do {
		warn "somthing went wrong processing file: $oryNewFilename\n$@" and return;
	};
#	end(@fileList);
	
		

}


my $searchPath = $MAIN_PATH ."/". $ORY_NEW_DIR;
print LOG "processing dir: $searchPath\n";
File::Find::find (\&wanted, $searchPath); #go through path and execute 'wanted' on overy file.

sub wanted {
	my $longName = $File::Find::name;
	return if ($longName =~ m|/\.|) ; #dont process hidden files.
	return if ($_ eq ".");
#	return if /(1582|1778).txt/i; #this file gives me trouble.
	processFile ($_) if /\.txt$/i; # process only files who have .txt extension
	

}

print LOG "$_ " for %counters;
print LOG "\ndone!\n";