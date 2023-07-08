package at.tugraz.ist.soma.dd.common;

public interface DDMinAlgo {

    /**
     * Initializes the minimizing Algorithm
     *
     * @param input   the input to test
     * @param program instance of the program (v1 or v2)
     * @param logger  instance of the logger
     */
    void init(String input, Program program, Logger logger);

    /**
     * Minimizing Algorithm
     *
     * @param input String to minimize
     */
    void ddmin(String input);
}
