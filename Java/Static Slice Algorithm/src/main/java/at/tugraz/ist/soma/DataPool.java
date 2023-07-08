package at.tugraz.ist.soma;


import at.tugraz.ist.soma.cfg.CFGNode;
import at.tugraz.ist.soma.statements.Statement;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;

/**
 * This class holds a list of all statements, the information about the file name, the slicing criterion
 * and the generated control flow graph.
 * @author Stephan Frühwirt
 */

public class DataPool {

	private static DataPool instance = null;
	private HashMap<Integer, Statement> statements;
	private ArrayList<CFGNode> controlFlowGraph;

	
	private DataPool(){
		// LinkedHashMap to keep ordering
		statements = new LinkedHashMap<Integer, Statement>();
	}
	
	/**
	 * returns one instance of the singleton class.
	 */
	public static DataPool getInstance(){
		if(instance == null){
			instance = new DataPool();
		}
		return instance;
	}
	
	public void addStatement(Statement stmt){
		statements.put(stmt.getLine(), stmt);
	}

	/**
	 * returns a statement at a specific position of the list.
	 */
	public Statement getStatement(int index){
		if(statements.containsKey(index)) {
			return statements.get(index);
		}
		return null;
	}
	
	/**
	 * returns the hashmap of statements.
	 */
	public HashMap<Integer, Statement> getStatements() {
		return statements;
	}

	public void updateStatement(int index, Statement stmt){
		statements.put(index, stmt);
	}

	/**
	 * returns the control flow graph.
	 */
	public ArrayList<CFGNode> getControlFlowGraph() {
		return controlFlowGraph;
	}

	public void setControlFlowGraph(ArrayList<CFGNode> controlFlowGraph) {
		this.controlFlowGraph = controlFlowGraph;
	}

	/**
	 * calls the {@link at.tugraz.ist.soma.statements.Statement#toString()} method of each statement.
	 */
	public void printStatements(){
		statements.forEach((k,v) -> System.out.print(v.toString()));
	}
	
}
