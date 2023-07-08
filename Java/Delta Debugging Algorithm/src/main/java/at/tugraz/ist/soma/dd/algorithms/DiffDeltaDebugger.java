package at.tugraz.ist.soma.dd.algorithms;

import at.tugraz.ist.soma.dd.common.*;

import java.util.HashSet;
import java.util.Set;
import java.util.Vector;

public class DiffDeltaDebugger implements DDDiffAlgo {

    private Program program;

    private Logger logger;

    private Logger consoleLogger;

    @Override
    public void init(Program program, Logger logger) {
        this.program = program;
        this.logger = logger;
        this.consoleLogger = new ConsoleLogger();
    }

    @Override
    public void dddiff(String cs, String cf) {

        Result result = program.test(cf);
        logger.log("cf", cf, result);
        consoleLogger.log("cf", cf, result);

        result = program.test(cs);
        logger.log("cs", cs, result);
        consoleLogger.log("cs", cs, result);

        String failure;
        failure = dddiff2(cs, cf, 2);
        System.out.println("The input causing the failure is: " + failure);
    }

    private String dddiff2(String cs, String cf, int splits) {
        int counter = 1;
        Result result;

        String delta = getDifferentChars(cs, cf);
        Vector<String> differenceSubsets = getSubsets(delta, splits);

        logger.breakLine();
        consoleLogger.breakLine();

        if (delta.length() == 1) {
            return delta;
        }

        // step 2
        for (String differenceSubset : differenceSubsets) {
            String union = getUnion(cs, differenceSubset);
            result = program.test(union);
            String logOutput = "cs " + Logger.UNION + " " + Logger.DELTA + counter;
            logger.log(logOutput, union, result);
            consoleLogger.log(logOutput, union, result);
            if (result == Result.FAIL) {
                return dddiff2(cs, union, 2);
            }
            counter++;
        }

        // step 3
        counter = 1;
        for (String differenceSubset : differenceSubsets) {
            String difference = getDifference(cf, differenceSubset);
            result = program.test(difference);
            String logOutput = "cf " + Logger.WITHOUT + " " + Logger.DELTA + counter;
            logger.log(logOutput, difference, result);
            consoleLogger.log(logOutput, difference, result);
            if (result == Result.PASS) {
                return dddiff2(difference, cf, 2);
            }
            counter++;
        }

        // step 4
        counter = 1;
        for (String differenceSubset : differenceSubsets) {
            String union = getUnion(cs, differenceSubset);
            result = program.test(union);
            String logOutput = "cs " + Logger.UNION + " " + Logger.DELTA + counter;
            logger.log(logOutput, union, result);
            consoleLogger.log(logOutput, union, result);
            if (result == Result.PASS) {
                return dddiff2(union, cf, Math.max(splits - 1, 2));
            }
            counter++;
        }

        // step 5
        counter = 1;
        for (String differenceSubset : differenceSubsets) {
            String difference = getDifference(cf, differenceSubset);
            result = program.test(difference);
            String logOutput = "cf " + Logger.WITHOUT + " " + Logger.DELTA + counter;

            logger.log(logOutput, difference, result);
            consoleLogger.log(logOutput, difference, result);
            if (result == Result.FAIL) {
                return dddiff2(cs, difference, Math.max(splits - 1, 2));
            }
            counter++;
        }

        // step 6
        if (delta.length() > splits) {
            return dddiff2(cs, cf, Math.min(delta.length(), 2 * splits));
        }

        // step 7 --> bug found
        // hope it is logged somewhere else
        logger.breakLine();
        consoleLogger.breakLine();
        return delta;
    }

    /**
     * This method splits a string into numberSets subsets.
     *
     * @param input      Input to split
     * @param numberSets Number of Sets to receive
     * @return Subsets
     */
    private Vector<String> getSubsets(String input, int numberSets) {
        int rest = input.length() % numberSets;
        int blockSize = input.length() / numberSets;

        Vector<String> sets = new Vector<>();
        int start = 0;
        while (start < input.length()) {
            String subString;
            if (rest > 0) {
                subString = input.substring(start, start + blockSize + 1);
                rest--;
                start += blockSize + 1;
            } else {
                subString = input.substring(
                        start,
                        (Math.min(start + blockSize, input.length())));
                start += blockSize;
            }
            sets.add(subString);

        }
        return sets;
    }

    private String getDifferentChars(String input1, String input2) {
        Set<String> differences = new HashSet<>();

        for (char c : input1.toCharArray()) {
            if (!input2.contains(String.valueOf(c))) {
                differences.add(String.valueOf(c));
            }
        }

        for (char c : input2.toCharArray()) {
            if (!input1.contains(String.valueOf(c))) {
                differences.add(String.valueOf(c));
            }
        }

        StringBuilder differencesBuilder = new StringBuilder();

        differences.forEach(differencesBuilder::append);

        return differencesBuilder.toString();
    }

    private String getDifference(String input1, String input2) {
        StringBuilder differencesBuilder = new StringBuilder();

        for (char c : input1.toCharArray()) {
            if (!input2.contains(String.valueOf(c))) {
                differencesBuilder.append(c);
            }
        }

        return differencesBuilder.toString();
    }

    private String getUnion(String input1, String input2) {
        StringBuilder resultBuilder = new StringBuilder(input1);

        for (char c : input2.toCharArray()) {
            if (!input1.contains(String.valueOf(c))) {
                resultBuilder.append(c);
            }
        }
        return resultBuilder.toString();
    }
}
