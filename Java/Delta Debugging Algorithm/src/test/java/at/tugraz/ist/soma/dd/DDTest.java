package at.tugraz.ist.soma.dd;

import at.tugraz.ist.soma.dd.algorithms.DiffDeltaDebugger;
import at.tugraz.ist.soma.dd.algorithms.MinDeltaDebugger;
import at.tugraz.ist.soma.dd.common.*;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.net.URI;
import java.net.URISyntaxException;
import java.nio.file.Paths;

public class DDTest {

    @Test
    public void testDDDiffProgramOne() throws FileNotFoundException, URISyntaxException {

        URI uriJson = ClassLoader.getSystemResource("json/publicDiffDD.ref.json").toURI();
        String jsonPath = Paths.get(uriJson).toString();

        TestLogger logger = TestLogger.loadTestcase(new FileReader(jsonPath));
        DDConfig config = new DDConfig(DDConfig.Algorithm.DIFF_DELTA_P1,"12345678", '6', "37".toCharArray());
        Program program = new ProgramOne(config.getFailure(), config.getDepending());

        DDDiffAlgo algoDiff_v1 = new DiffDeltaDebugger();
        algoDiff_v1.init(program, logger);
        algoDiff_v1.dddiff("", config.getInput());

        Assertions.assertTrue(logger.compare());
    }

    @Test
    public void testDDMinProgramTwo() throws URISyntaxException, FileNotFoundException {
        URI uriJson = ClassLoader.getSystemResource("json/publicMinDD.ref.json").toURI();
        String jsonPath = Paths.get(uriJson).toString();


        TestLogger logger = TestLogger.loadTestcase(new FileReader(jsonPath));
        DDConfig config = new DDConfig(DDConfig.Algorithm.MIN_DELTA_P2,"12345678", '5', "36".toCharArray());
        Program program = new ProgramTwo(config.getFailure(), config.getDepending());

        MinDeltaDebugger algoMin = new MinDeltaDebugger();
        algoMin.init(config.getInput(), program, logger);
        algoMin.ddmin(config.getInput());

        Assertions.assertTrue(logger.compare());
    }

    @Test
    public void testDDDiffProgramTwoOwnTestcase() throws URISyntaxException, FileNotFoundException {

        URI uriJson = ClassLoader.getSystemResource("json/ownDiffDD.ref.json").toURI();
        String jsonPath = Paths.get(uriJson).toString();

        TestLogger logger = TestLogger.loadTestcase(new FileReader(jsonPath));
        DDConfig config = new DDConfig(DDConfig.Algorithm.DIFF_DELTA_P2,"12A34V567DD8WQ", 'V', "67".toCharArray());
        Program program = new ProgramOne(config.getFailure(), config.getDepending());

        DDDiffAlgo algoDiff_v1 = new DiffDeltaDebugger();
        algoDiff_v1.init(program, logger);
        algoDiff_v1.dddiff("", config.getInput());

        Assertions.assertTrue(logger.compare());
    }

    @Test
    public void testDDMinProgramTwoOwnTestcase() throws URISyntaxException, FileNotFoundException {
        URI uriJson = ClassLoader.getSystemResource("json/ownMinDD.ref.json").toURI();
        String jsonPath = Paths.get(uriJson).toString();


        TestLogger logger = TestLogger.loadTestcase(new FileReader(jsonPath));
        DDConfig config = new DDConfig(DDConfig.Algorithm.MIN_DELTA_P1,"A31TEO4", 'E', "31".toCharArray());
        Program program = new ProgramTwo(config.getFailure(), config.getDepending());

        MinDeltaDebugger algoMin = new MinDeltaDebugger();
        algoMin.init(config.getInput(), program, logger);
        algoMin.ddmin(config.getInput());

        Assertions.assertTrue(logger.compare());
    }

}
