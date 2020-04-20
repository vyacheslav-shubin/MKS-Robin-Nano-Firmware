#!/usr/bin/perl
use XML::XPath;
use XML::XPath::XMLParser;

my $xp = XML::XPath->new(filename => 'EWARM/mksRobinNano.ewp');
    
if (($ARGV[0] eq "src") or (not $ARGV[0]))  {
	my $nodeset = $xp->find('//file[not(excluded[configuration="mksRobinNano"])]/name/text()'); 
    foreach my $node ($nodeset->get_nodelist) {
		my $f=XML::XPath::XMLParser::as_string($node);
		$f =~ s/\$PROJ_DIR\$\\..\\//;
		$f =~ s/\\/\//g;
		if (-e $f) {
			print "$f\n"; 
		}
	}
} elsif ($ARGV[0] eq "define")  {
	my $nodeset = $xp->find('//settings/data/option[name="CCDefines"]/state/text()');
    foreach my $node ($nodeset->get_nodelist) {
		my $f=XML::XPath::XMLParser::as_string($node);
        print "$f\n"; 
	}	
} elsif ($ARGV[0] eq "include")  {
	my $nodeset = $xp->find('//settings/data/option[name="CCIncludePath2"]/state/text()');
    foreach my $node ($nodeset->get_nodelist) {
		my $f=XML::XPath::XMLParser::as_string($node);
		$f =~ s/\$PROJ_DIR\$\\..\\//;
		$f =~ s/\$PROJ_DIR\$\/..\///;
		$f =~ s/\\/\//g;
        print "$f\n"; 
	}	
} elsif ($ARGV[0] eq "lib")  {
	my $nodeset = $xp->find('//settings/data/option[name="IlinkAdditionalLibs"]/state/text()');
    foreach my $node ($nodeset->get_nodelist) {
		my $f=XML::XPath::XMLParser::as_string($node);
		$f =~ s/\$PROJ_DIR\$\\..\\//;
		$f =~ s/\$PROJ_DIR\$\/..\///;
		$f =~ s/\\/\//g;
        print "$f\n"; 
	}	
}
	
