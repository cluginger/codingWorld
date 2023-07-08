package at.tugraz.ist.soma.cfg;

import at.tugraz.ist.soma.statements.EndNode;
import at.tugraz.ist.soma.statements.Statement;
import at.tugraz.ist.soma.utils.SlicingConfig;
import at.tugraz.ist.soma.utils.Type;
import at.tugraz.ist.soma.utils.Visualizer;

import javax.swing.plaf.nimbus.State;
import java.util.*;

/**
 * This class creates a control flow graph using the list of {@link at.tugraz.ist.soma.statements.Statement}s.
 * @author Stephan Frühwirt
 */
public class CFGgenerator {
	private Deque<CFGNode> stack;
	private HashMap<Integer, Statement> statements;
	private SlicingConfig config;

	public CFGgenerator(HashMap<Integer, Statement> statements,SlicingConfig config){
		stack = new LinkedList<>();
		this.config = config;
		this.statements = statements;
	}
	
	/**
	 * generates the CFG. You can use the {@link at.tugraz.ist.soma.utils.Visualizer#createCFGdotFile(ArrayList)}
	 * to get a graphical representation of your graph!
	 * @return
	 */
	public ArrayList<CFGNode> generate(){
		ArrayList<CFGNode> cfg = new ArrayList<>();
		
		CFGNode startNode = new CFGNode();
		cfg.add(startNode);

		stack.push(startNode);
		int maxKey = Collections.max(statements.keySet()) + 1000;
		statements.put(maxKey, new EndNode(maxKey, "endnode"));

		// TODO: calculate the CFG
		//		hint: this will be the most difficult part of this exercise.
		//		Prepare a coffee supply...
		// if else while for

		for (Statement stmt : statements.values()) {
			CFGNode nodeToAdd = new CFGNode(stmt);

			ArrayList<CFGNode> predecessorList = new ArrayList<>();
			if (cfg.get(cfg.size() -1).getType() != Type.ELSE_STMT) {
				predecessorList.add(cfg.get(cfg.size() - 1));
			}

			// check if new line is out of scope of last stack element
			while (stack.getFirst().getStatement() != null &&
					stmt.getLine() > stack.getFirst().getStatement().getLastLine()) {
				switch (stack.getFirst().getType()) {
					case IF_STMT: {
						predecessorList.add(stack.getFirst());
						break;
					}
					case ELSE_STMT: {
						stack.getFirst().getPredecessors().forEach(predecessor ->
						{
							if (predecessor.getType() != Type.IF_STMT) {
								predecessorList.add(predecessor);
							}
						});
						break;
					}
					case LOOP_WHILE:
					case LOOP_FOR: {
						predecessorList.forEach(stack.getFirst()::appendPredecessor);
						predecessorList.clear();
						predecessorList.add(stack.getFirst());
						break;
					}
					default:
				}
				stack.pop();
			}

			nodeToAdd.influencedBy(stack.getFirst());

			// loop case
			if (stmt.getType() == Type.LOOP_WHILE || stmt.getType() == Type.LOOP_FOR || stmt.getType() == Type.IF_STMT || stmt.getType() == Type.ELSE_STMT) {
				stack.push(nodeToAdd);
			}

			if (cfg.get(cfg.size() - 1).getType() == Type.ELSE_STMT) {
				CFGNode outestIf = cfg.get(cfg.size() - 1).getPredecessors().get(0);

				for (CFGNode predecessor : cfg.get(cfg.size() - 1).getPredecessors()) {
					if (predecessor.getType() == Type.IF_STMT && predecessor.getStatement().getLine() < outestIf.getStatement().getLine()) {
						outestIf = predecessor;
					}
				}
				predecessorList.add(outestIf);

				cfg.remove(cfg.size() - 1);
			}

			predecessorList.forEach(nodeToAdd::appendPredecessor);

			cfg.add(nodeToAdd);
		}

		Visualizer.createCFGdotFile(cfg, config);
		return cfg;
	}
}
