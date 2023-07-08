package at.tugraz.ist.soma.dd.common;

import java.util.Arrays;

public interface Logger {

    String DELTA = "\u0394";
    String NABLA = "\u2207";
    String UNION = "\u222A";
    String WITHOUT = "/";

    /**
     * Initialises the logger
     */
    void init();

    /**
     * Prints a result-line
     *
     * @param testcase Name of the Testcase (Δ1, Δ2, cf, cs, cf / Δ1, ...)
     * @param input    Input to test
     * @param result   Result of the test (PASS, FAIL, UNRES, EMPTY)
     */
    void log(String testcase, String input, Result result);

    /**
     * Prints a line (to show the depth of the Algorithm)
     */
    void breakLine();

    /**
     * Prints an error-line
     */
    void error();

    default String modify(String input) {
        String output = "";
        char[] out = input.toCharArray();
        Arrays.sort(out);

        for (int index = 0; index < out.length; index++)
            output += out[index];

        return output;
    }
}
