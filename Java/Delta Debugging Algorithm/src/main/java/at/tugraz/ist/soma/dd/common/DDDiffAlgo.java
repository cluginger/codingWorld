package at.tugraz.ist.soma.dd.common;

public interface DDDiffAlgo {

    /**
     * Initializes the minimizing Algorithm
     *
     * @param program
     * @param logger
     */
    void init(Program program, Logger logger);

    /**
     * Minimizing Algorithm
     *
     * @param cs
     * @param cf
     */
    void dddiff(String cs, String cf);

}
