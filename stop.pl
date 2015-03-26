#! /usr/local/bin/perl -w

use strict;
use warnings;

sub find_proc_id{
	my $target = shift // die "No parameters";
	my @a = `ps -ef | grep $target`;
	#print @a;
	#print "\n\n";
	my $pat = qr/\.\/$target\b/;
	my $pid;
	for(@a){
		chomp;
		#print "Yes: $_\n" if $_ =~ /$pat/;
		if(/$pat/){
			/^([\w\d]+)\s+([\w\d]+)/;
			#print $2, "\n";
			$pid = $2;
			last;
		}
	}
	$pid;
}

my @kills = qw/dir
	world/;
for(@kills){
	my $pid = find_proc_id($_);
	if($pid){
		system "kill $pid";
		print "Error killing $pid" if $?;
	}
}


