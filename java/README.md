sources.txt contains the paths of the source code files in the project (not including the unit tests).

## Compiling the Project and Running the MWE
This can be compiled by running:

    javac -d out @sources.txt
This will place all class files in the directory `out`.

To run the MWE:

    java -cp out DagMWE
