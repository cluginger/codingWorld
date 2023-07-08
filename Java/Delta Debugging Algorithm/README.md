To build your project you can use the command gradle build. To run the public test cases
gradle test is the way to go. If you want to add your own tests just implement them into the
DDTest.java File or execute them using gradle runProgram. Your implementation will be tested
with the same mechanism the public tests use in DDTest.java. So make sure your implementation
works with this tests.
The algorithm is one of the following:
• DIFF DELTA P1 using the isolation differences algorithm with Program1
• DIFF DELTA P2 using the isolation differences algorithm with Program2
• MIN DELTA P1 using the minimizing delta debugging algorithm with Program1 
• MIN DELTA P2 using the minimizing delta debugging algorithm with Program2
This is how a program call could look like:
gradle runProgram -PfileName=<pathToYourFile>/test1.txt
gradle runProgram -Palgo="MIN DELTA P1" -Pinput="012345678" -Pfail="3" -Pdeps="45"
