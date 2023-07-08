The framework uses Gradle as its build tool.
You could use gradle build to compile your sources and gradle test to run the unit tests. To
execute your program, the task gradle runProgram along with the needed parameters is a possible
option e.g.
gradle runProgram -PfileName="path-to-file" -Pcriterion="(12, {z})" -Pdebug="true"
Do not change the build.gradle.kts file or add a build.gradle file to the framework folder.
The slicing criterion is a pair (i, V ) where i is a line number in the program and V is the set of
variables of interest. The static slice for the slicing criterion should be calculated with the help of the
provided statement classes.
