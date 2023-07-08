package at.tugraz.ist.soma.dd;

import at.tugraz.ist.soma.dd.common.Logger;
import at.tugraz.ist.soma.dd.common.Result;
import com.google.gson.Gson;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class TestLogger implements Logger {

    private List<LoggerLine> lines;
    private List<LoggerLine> refLines;
    private int step = 0;

    private TestLogger(LoggerLine[] refLines){
        this.refLines = Arrays.asList(refLines);
        this.lines = new ArrayList<>(refLines.length+1);
    }

    @Override
    public void init() {
        step = 0;
    }

    @Override
    public void log(String testcase, String input, Result result) {
        lines.add(new LoggerLine(++step,LoggerLine.LineType.LOG, testcase, modify(input), result));
    }

    @Override
    public void breakLine() {
        lines.add(new LoggerLine(LoggerLine.LineType.BREAK));
    }

    @Override
    public void error() {
        lines.add(new LoggerLine(LoggerLine.LineType.ERROR));
    }

    public static TestLogger loadTestcase(FileReader fileReader) throws FileNotFoundException {
        Gson g = new Gson();
        LoggerLine[] refLines = g.fromJson(fileReader, LoggerLine[].class);
        return new TestLogger(refLines);
    }

    public boolean compare(){
        if(refLines.size() != lines.size()) {
            System.err.println("Amount of lines unequal");
            return false;
        }
        for(int i = 0; i < refLines.size(); ++i){
            if (!refLines.get(i).equals(lines.get(i))) {
                System.err.println("Error in line "+ i);
                return false;
            }
        }
        return true;
    }
}
