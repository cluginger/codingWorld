package at.tugraz.ist.soma.dd;


/*
 * Main class of the project. From here the different delta debugging
 * algorithms are called.
 */

import at.tugraz.ist.soma.dd.algorithms.DiffDeltaDebugger;
import at.tugraz.ist.soma.dd.algorithms.MinDeltaDebugger;
import at.tugraz.ist.soma.dd.common.*;

public class Main {

    /**
     * main method: parses the given input parameters and starts the chosen algorithm
     *
     */
    public static void main(String[] args)
    {
        DDConfig config = CmdParser.parseInput(args);

		if(config != null) {
		    Logger logger = new ConsoleLogger();
            logger.init();
            String input = config.getInput();
            Program program;

            switch(config.getAlgorithm())
            {
                case DIFF_DELTA_P1:
                    program = new ProgramOne(config.getFailure(), config.getDepending());

                    DDDiffAlgo algoDiff_v1 = new DiffDeltaDebugger();
                    algoDiff_v1.init(program, logger);
                    algoDiff_v1.dddiff("", input);
                    break;

                case DIFF_DELTA_P2:
                    program = new ProgramTwo(config.getFailure(), config.getDepending());

                    DDDiffAlgo algoDiff_v2 = new DiffDeltaDebugger();
                    algoDiff_v2.init(program, logger);
                    algoDiff_v2.dddiff("", input);
                    break;

                case MIN_DELTA_P1:
                    program = new ProgramOne(config.getFailure(), config.getDepending());

                    DDMinAlgo algoMin_v1 = new MinDeltaDebugger();
                    algoMin_v1.init(input, program, logger);
                    algoMin_v1.ddmin(input);
                    break;

                case MIN_DELTA_P2:
                    program = new ProgramTwo(config.getFailure(), config.getDepending());

                    DDMinAlgo algoMin_v2 = new MinDeltaDebugger();
                    algoMin_v2.init(input, program, logger);
                    algoMin_v2.ddmin(input);
                    break;
            }
		}
    }
 }
