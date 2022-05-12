# TimOPipes
Find paths through forest of pipes.

Input file format is a space delimited text file with 4 columns, one line per pipe

| column name | description
|-|-|
| source name | text, no spaces
| source type | "source"<br>"-" for none
destination name | text, no spaces
destination type | "discharge"<br>"-" for none

Sample input file

<pre>
source1 source n1 -
n1 - n2 -
n2 - n3 -
n3 - sink1 discharge
n2 - n4 -
n4 - sink2 discharge
source2 source n5 -
n6 - sink3 discharge
</pre>

