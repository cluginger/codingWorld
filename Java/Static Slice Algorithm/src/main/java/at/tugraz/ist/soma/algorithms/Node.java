package at.tugraz.ist.soma.algorithms;

import java.util.ArrayList;
import java.util.HashMap;

import at.tugraz.ist.soma.statements.Statement;

/**
 * This class represents one node of the PDG.
 * @author Stephan Frühwirt
 */
public class Node {

	private Statement statement;
	private HashMap<Integer, Node> controlDependencies;
	private HashMap<Integer, Node> dataDependencies;
	
	/**
	 * use this constructor if you want to create the ENTRY node.
	 * You can recognize the ENTRY node via the <i>statement</i>
	 * variable which should always be <b>null</b>.
	 */
	public Node(){
		controlDependencies = new HashMap<Integer, Node>();
		dataDependencies = new HashMap<Integer, Node>();
		statement = null;
	}
	
	/**
	 * use this constructor if you want to create a new node.
	 * If you want to create the ENTRY node you should use the {@link at.tugraz.ist.soma.algorithms.Node#Node()} constructor.
	 */
	public Node(Statement statement){
		this.statement = statement;
		controlDependencies = new HashMap<Integer, Node>();
		dataDependencies = new HashMap<Integer, Node>();
	}
	
	/**
	 * appends a control dependency to this node, i.e. a link between this node and a given node.
	 * <br><strong>HINT:</strong> think about in which direction you should save the linkages!
	 */
	public void appendControlDependency(Node node){
		if(node != null) {
			if (node.getStatement() == null) {
				controlDependencies.put(0, node);
			} else {
				controlDependencies.put(node.getStatement().getLine(), node);
			}
		}
	}
	
	/**
	 * returns the control dependencies of this node.
	 */
	public HashMap<Integer, Node> getControlDependencies() {
		return controlDependencies;
	}
	
	/**
	 * appends a data dependency to this node, i.e. a link between this node and a given node.
	 * <br><strong>HINT:</strong> think about in which direction you should save the linkages!
	 */
	public void appendDataDependency(Node node){
		if(node != null){
			dataDependencies.put(node.getStatement().getLine(), node);			
		}
	}
	
	/**
	 * returns the data dependencies of this node.
	 */
	public HashMap<Integer, Node> getDataDependencies() {
		return dataDependencies;
	}
	
	/**
	 * returns an array list of all dependencies (control + data).
	 * Might be useful when you calculate the slice.
	 */
	public ArrayList<Node> getAllDependencies(){
		ArrayList<Node> all = new ArrayList<>();
		
		controlDependencies.forEach((k,v) -> all.add(v));
		dataDependencies.forEach((k,v) -> all.add(v));
		return all;
	}
	
	/**
	 * returns the {@link at.tugraz.ist.soma.statements.Statement} of a node.
	 */
	public Statement getStatement(){
		return statement;
	}
}
