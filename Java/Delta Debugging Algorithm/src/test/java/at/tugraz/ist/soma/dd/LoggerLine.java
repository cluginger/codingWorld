package at.tugraz.ist.soma.dd;

import at.tugraz.ist.soma.dd.common.Result;

public class LoggerLine {
    LineType lineType;
    private Result result;
    private String input;
    private String testcase;
    private int step;

    public LoggerLine(LineType type){
        lineType = type;
    }

    public LoggerLine(int step, LineType type, String testcase, String input, Result result){
        lineType = type;
        this.testcase = testcase;
        this.input = input;
        this.result = result;
        this.step = step;
    }

    public LineType getLineType() {
        return lineType;
    }

    public void setLineType(LineType lineType) {
        this.lineType = lineType;
    }

    public Result getResult() {
        return result;
    }

    public void setResult(Result result) {
        this.result = result;
    }

    public String getInput() {
        return input;
    }

    public void setInput(String input) {
        this.input = input;
    }

    public String getTestcase() {
        return testcase;
    }

    public void setTestcase(String testcase) {
        this.testcase = testcase;
    }

    public int getStep() {
        return step;
    }

    public void setStep(int step) {
        this.step = step;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        LoggerLine that = (LoggerLine) o;

        if (step != that.step) return false;
        if (lineType != that.lineType) return false;
        if (result != that.result) return false;
        if (input != null ? !input.equals(that.input) : that.input != null) return false;
        return testcase != null ? testcase.equals(that.testcase) : that.testcase == null;
    }

    @Override
    public int hashCode() {
        int result1 = lineType.hashCode();
        result1 = 31 * result1 + (result != null ? result.hashCode() : 0);
        result1 = 31 * result1 + (input != null ? input.hashCode() : 0);
        result1 = 31 * result1 + (testcase != null ? testcase.hashCode() : 0);
        result1 = 31 * result1 + step;
        return result1;
    }

    enum LineType{
        ERROR,LOG,BREAK
    }

}
