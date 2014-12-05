Evaluate link parition
=========================
This program evaluate a link partition of a graph according to different quality functions.
This program process only undirected unweighted edges.
The quality function that can be computed are:

* Partition density.
"Link communities reveal multiscale complexity in networks"
Y.-Y. Ahn, J.P. Bagrow, S. Lehmann
* Linegraphe Modularity.
"Line Graphs, Link Partitions and Overlapping Communities"
T.S.Evans, R.Lambiotte
* Expected Nodes.
"Expected Nodes: a quality function for the detection of link communities"
N. Gaumont, F. Queyroi, C. Magnien, M. Latapy

Author
-------
* Author : Gaumont Noé
* Email    : noe.gaumont@lip6.fr
* Location : Paris, France

####Version 1.0
Disclaimer
-----------
If you find a bug, please send a bug report to noe.gaumont@lip6.fr
including if necessary the input file and the parameters that caused the bug.
You can also send me any comment or suggestion about the program.

Compilation
------------

This program needs boost to be installed.
To install boost:
* On debian : sudo apt-get install libboost-all-dev
* On archlinux : sudo yaourt -S boost
* See  http://www.boost.org

Once boost installed, just type:
```sh
make evaluation
```
If you don't want to bother with boost.
Use the care achive "builder.bin":

```
./builder.bin # this creates builder/
./builder/re-execute.sh # this compile everything
mv builder/rootfs/home/gaumont/CodingPlace/linkStream/evaluation .
```


After the compilation, you are ready to use the program.
See the Usage section.

### Usage
Here are typical usages :

```sh
./evaluation -i INPUTFILE -q 1,2,3,4
```
This evaluates the graph stored in INPUTFILE with the quality functions 1,2,3 and 4.

If you don't like ID, you can also use names:
```sh
./evaluation -i INPUTFILE -q ExpectedNodes,PartionDensity,E1,E2,E3
```

Of course, you can use only one quality function:
```sh
./evaluation -i INPUTFILE -q ExpectedNodes
```

#### INPUTFILE

The input file should contain a list of undirected unweighted links with a community.
A links with a community should be in the format: Source, Target, Community
For a triangle with 2 communities:
```
1 2 1
1 3 1
2 3 2
...

```
The entry could be space or tab separated.


You can generate toy example entry file. See Examples generation.

### Arguments

With the option q, you can provide which quality function has to be tested.
The quality functions has to be provided as a comma separated list of their id numbers or names.
Available quality function are:


* Parition Density introduced by Ahn et al. ID=, name=ParitionDensity
* The first version of the quality function introduce by Evans et al. . id= , name=E1
* The second version of the quality function introduce by Evans et al. . id= , name=E2
* The third version of the quality function introduce by Evans et al. . id= , name=E3
* Expected Nodes Id= , name= LinkExpectation

Please refer to the according paper to find their definition.


Examples generation
----------------------
In the folder Example, there is a script to generate toy examples.
The script creates a complete graph of given size with a link partition.
To create the link partition, the script need 2 parameters.
The first one is a number of nodes and the second is the number of groups (1,2 or 3).
Please refere to the paper "ExpectedNodes :[..]"  to have more information.
```sh
./1cliqueGeneration.sh 100 15 3 > example1.txt
```

License
--------

MIT
