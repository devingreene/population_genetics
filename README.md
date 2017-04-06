-- vim: set tw=70: --
[ Note:  This description only applies to the Wright-Fisher part of
the program.  There is a logistic model `carrying capacity' type
version which can be compile using CC.Makefile.  At some point I will
write a description of it. ]

This is a basic population genetic model which takes into account fitness
differentials, migration, mutation, and recombination.  Each simulation begins
from an initial state consisting of a set of islands with migration rates
between them, and a set of haplotypes on each island, with mutation,
recombination, and fitness differentials taken into account.  Each island
population is effectively subject to a Wright-Fisher process, in addition to
changes in the haplotype distribution due to migrations to and from other
islands.  The parameters used in the simulations are read from the following
data files: 

census: Contains initial haplotype distribution on each island 
`("census") as well as fitnesses of each haplotypes 
mut: 	Mutational rates amongst haplotypes 
recom: 	Recombination rates amongst haplotypes 
mig: 	Migration rates amongst islands   

Note that migration may be set to be a periodic phenomenon via the argument
`mperiod=<num>' (see code for details).  The argument is optional.  If not
present, it will default to '1`, and migration will occur every generation. 

To compile the program as described above, do the following on a command line:

make -f WF.Makefile

The main executable file is execWF, but you will need to set up the
parameters using the following binaries:

settle
makeMut
makeMig
makeRecom

These are described here in turn:

`Settle' allows the user to create, populate, and assign fitnesses in
an island model.  With the argument `create', the user is prompted to
enter the number of haplotypes and islands.  With the argument
`populate', one is prompted to enter the island # followed by the
distribution of haplotypes on that island.  With the argument
`fitness', the user is prompted to enter the fitnesses of the
haplotype.  The default fitness is 1.  The default number of
haplotypes on a particular island is 0.  Whether created or modified,
the data is stored in the file `census'.  Which can be examined using
the executable readParameters

`makeMut' creates a mutation matrix data file name `mut'.  Keyword `morgan'
should be followed by number of loci and per-locus mutation rate
produces correct matrix, where haplotypes are treated as bit strings
and ordered numerically as if they were base 2 numbers. Keyword
`flat' prompts for entry of mutation rates 1->2, 1->3, ..., 2->1,
2->2, ...,g n->1, ... Keyword `symmetric' prompts for entry of
mutation rate 1->2,g 1->3, ...,2->3, ..., n-1->n.

	`makeMig' creates the data file `mig' containing the parameters for inter-island
	migration.  There are three keywords:
	
	`array': Must be followed dim1 dim2.  Forms a dim1 by dim2 array of
	islands.  Adjacent islands (where diagonal neighbors are considered
	adjacent) exchange migrants at equal rates.
	
	`torus': Like array, but migration "wraps around"
	
	`flat': Starting with a set number of islands and a default migration
	rate between any pair of islands, program then prompts user to
	enter whatever migration rates for whatever pair of islands
	she see fit.  Note that this is asymmetric.  The rate `1->2'
	may differ from `2->1'
	
	`symmetric': Like `flat', but migration rates are constrained to be
	symmetric between islands in a pair.


`makeRecome' creates a recombination matrix data file name `recom'.
The following is a description of the various keywords:

	`morgan' :
	
	Keyword `morgan' should be followed by:
	
	1. (Optional) The the keyword `linkage' followed by <num of loci>
	linkage parameters, expressed as numbers between 0 and 0.5 (0 is full
	linkage, 0.5 is no linkage).
	
	2. The number of loci and a recombination rate `R`.  Note that
	recombination rate is a bit non-conventional here.  With R==0, there
	is no recombination -- the haplotypes effective clone themselves.
	With R==1, haplotypes randomly pair, and the progeny are determined by
	a coin flip at each locus on each parent.  Intermediate values 0 < R <
	1 are equivalent to proportion R of the population mating with
	recombination as just described, and proportion 1-R cloning.


