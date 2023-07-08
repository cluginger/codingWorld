package at.tugraz.ist.soma.dd.common;

public class DDConfig {
    private Algorithm algorithm;
    private String input;
    private char failure;
    private char[] depending;

    public DDConfig() {
        algorithm = Algorithm.DIFF_DELTA_P1;
        input = null;
        failure = '0';
        depending = "".toCharArray();
    }

    public DDConfig(Algorithm algorithm, String input, char failure, char[] depending) {
        this.algorithm = algorithm;
        this.input = input;
        this.failure = failure;
        this.depending = depending;
    }

    public Algorithm getAlgorithm() {
        return algorithm;
    }

    public void setAlgorithm(Algorithm algorithm) {
        this.algorithm = algorithm;
    }

    public String getInput() {
        return input;
    }

    public void setInput(String input) {
        this.input = input;
    }

    public char getFailure() {
        return failure;
    }

    public void setFailure(char failure) {
        this.failure = failure;
    }

    public char[] getDepending() {
        return depending;
    }

    public void setDepending(char[] depending) {
        this.depending = depending;
    }

    public enum Algorithm {
        DIFF_DELTA_P1("DIFF_DELTA_P1"),
        DIFF_DELTA_P2("DIFF_DELTA_P2"),
        MIN_DELTA_P1("MIN_DELTA_P1"),
        MIN_DELTA_P2("MIN_DELTA_P2");

        private final String ddConfigString;

        Algorithm(String ddConfigString){
            this.ddConfigString = ddConfigString;
        }

        public String getDdConfigString(){
            return ddConfigString;
        }
    }
}