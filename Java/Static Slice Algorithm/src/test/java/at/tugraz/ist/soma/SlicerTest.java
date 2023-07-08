package at.tugraz.ist.soma;


import at.tugraz.ist.soma.algorithms.StaticSlicer;
import at.tugraz.ist.soma.cfg.CFGNode;
import at.tugraz.ist.soma.cfg.CFGgenerator;
import at.tugraz.ist.soma.statements.Statement;
import at.tugraz.ist.soma.utils.SlicingConfig;
import at.tugraz.ist.soma.utils.StatementProcessor;
import com.google.gson.Gson;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;

import java.io.FileReader;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class SlicerTest {

    @BeforeEach
    public void setUp() throws Exception {

    }

    @ParameterizedTest
    @ValueSource(strings = {"public", "public1", "public2", "if", "ifelse", "while", "for", "forforwhile", "ass2_2a"})
    public void public1Test(String input) throws IOException, URISyntaxException {
        Gson g = new Gson();

        URI uriJson = ClassLoader.getSystemResource(input+".json").toURI();
        URI uriJava = ClassLoader.getSystemResource(input+".java").toURI();

        String jsonPath = Paths.get(uriJson).toString();
        String javaPath = Paths.get(uriJava).toString();

        Testcase tc = g.fromJson(new FileReader(jsonPath), Testcase.class);
        SlicingConfig config = new SlicingConfig(tc.getCriterion(), javaPath, true);

        StatementProcessor statementProcessor = new StatementProcessor(config);
        HashMap<Integer, Statement> statements = statementProcessor.processStatements();

        CFGgenerator cfgGenerator = new CFGgenerator(statements, config);
        List<CFGNode> cfg = cfgGenerator.generate();


        StaticSlicer slicer = new StaticSlicer(config,statements, cfg);
        slicer.calculateSlice();

        ArrayList<Integer> slice = slicer.getSlice();

        Assertions.assertArrayEquals(slice.toArray(), tc.getResult().toArray());
    }
}
