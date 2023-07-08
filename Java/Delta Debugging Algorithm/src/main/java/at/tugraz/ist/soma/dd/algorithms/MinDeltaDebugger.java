package at.tugraz.ist.soma.dd.algorithms;

import at.tugraz.ist.soma.dd.common.*;

import java.util.Vector;



public class MinDeltaDebugger implements DDMinAlgo {


    private String input;
    private Program program;
    private Logger logger;
    private ConsoleLogger consoleLogger;

	@Override
	public void init(String input, Program program, Logger logger) {
	    this.input = input;
	    this.program = program;
	    this.logger = logger;
	    consoleLogger = new ConsoleLogger();
	    consoleLogger.init();
	}

	@Override
	public void ddmin(String input) {
	    String solution = ddmin2(input, 1);
    }

    private String ddmin2(String delta, int splits) {

	    Vector<String> subsets = getSubsets(delta, splits);

        Result result;
        int counter = 1;
        String loggerOutput = "";

        // Step 2
	    for (String subset : subsets) {
	        result = program.test(subset);

            if (splits != 1) {
                loggerOutput = Logger.DELTA + counter;
            }
            logger.log(loggerOutput, subset, result);
            consoleLogger.log(loggerOutput, subset, result);
            counter++;

            if (result == Result.FAIL) {
                if (subset.length() == 1) {
                    logger.breakLine();
                    consoleLogger.breakLine();
                    return subset;
                }
                logger.breakLine();
                consoleLogger.breakLine();
                return ddmin2(subset,2);
            }
        }

	    // Step 3
        counter = 1;
	    for (String subset : subsets) {
            String complement = delta;
            for (char c : subset.toCharArray()) {
                complement = complement.replace(String.valueOf(c), "");
            }
            result = program.test(complement);

            if (splits != 1) {
                loggerOutput = Logger.NABLA + counter;
            }
            logger.log(loggerOutput, complement, result);
            consoleLogger.log(loggerOutput, complement, result);
            counter++;

            if (result == Result.FAIL) {
                logger.breakLine();
                consoleLogger.breakLine();
                return ddmin2(complement, Math.max(splits - 1, 2));
            }
        }

        // Step 4
        if (delta.length() > splits) {
            logger.breakLine();
            consoleLogger.breakLine();
            return ddmin2(delta, Math.min(delta.length(), 2 * splits));
        }

        return delta;
    }

	/**
   * This method splits a string into numberSets subsets.
   * @param input Input to split
   * @param numberSets Number of Sets to receive
   * @return Subsets
   */
  private Vector<String> getSubsets(String input, int numberSets) {
    int rest = input.length() % numberSets;
    int blockSize = input.length() / numberSets;

    Vector<String> sets = new Vector<String>();
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
            (start + blockSize < input.length() ? start + blockSize : input
                .length()));
        start += blockSize;
      }
      sets.add(subString);

    }
    return sets;
  }
}
