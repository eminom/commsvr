#! /usr/bin/perl -w
#It works on mac
# Due to the ps format

use strict;
use warnings;

sub find_proc_id{
	my $target = shift // die "No parameters";
	my @a = `ps -ef | grep $target`;
	print @a;
	#print "\n\n";
	my $pat = qr/$target\b/;
	my $pid;
	for(@a){
		chomp;
		#print "Yes: $_\n" if $_ =~ /$pat/;
		if(/$pat/){
			/^\s+([\w\d]+)\s+([\w\d]+)/;
			print $2, "\n";
			$pid = $2;
			last;
		}
	}
	$pid;
}

my @kills = qw{ 
	./out/dir
	./out/world
};

for(@kills){
	my $pid = find_proc_id($_);
	if($pid){
		system "Kill $pid";
		print "Error killing $pid\n" if $?;
		print "Killing $pid ...\n" if not $?;
	}else{
		print "Not found for $_\n";
	}
}


