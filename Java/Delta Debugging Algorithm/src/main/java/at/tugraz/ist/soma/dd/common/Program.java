package at.tugraz.ist.soma.dd.common;


public interface Program {

    /**
     * Takes an input string and returns the corresponding result
     *
     * @param input Input string to test
     * @return Result (PASS, FAIL, UNRES, EMPTY)
     */
    Result test(String input);
}

