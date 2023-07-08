package at.tugraz.ist.soma;

import at.tugraz.ist.soma.algorithms.StaticSlicer;
import at.tugraz.ist.soma.cfg.CFGNode;
import at.tugraz.ist.soma.cfg.CFGgenerator;
import at.tugraz.ist.soma.statements.Statement;
import at.tugraz.ist.soma.utils.DataIO;
import at.tugraz.ist.soma.utils.SlicingConfig;
import at.tugraz.ist.soma.utils.StatementProcessor;

import java.util.HashMap;
import java.util.List;

/**
 * This is the main class. The slicing algorithm can be chosen in this class.
 * In this assignment you have to implement static slicing using a PDG.
 * @author Stephan Frühwirt
 */

public class Main {

	public static void main(String[] args) throws Exception {
		SlicingConfig config = DataIO.parseInput(args);

		StatementProcessor statementProcessor = new StatementProcessor(config);
		HashMap<Integer, Statement> statements = statementProcessor.processStatements();

		CFGgenerator cfgGenerator = new CFGgenerator(statements, config);
		List<CFGNode> cfg = cfgGenerator.generate();

		
		StaticSlicer slicer = new StaticSlicer(config,statements, cfg);
		slicer.calculateSlice();

		DataIO.printSlice(slicer.getSlice());
	}

}
