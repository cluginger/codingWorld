package at.tugraz.ist.soma.algorithms;

import at.tugraz.ist.soma.cfg.CFGNode;
import at.tugraz.ist.soma.statements.Statement;
import at.tugraz.ist.soma.utils.DataIO;
import at.tugraz.ist.soma.utils.SlicingConfig;
import at.tugraz.ist.soma.utils.Visualizer;

import java.util.*;

/**
 * This is the class where you should implement static slicing using a PDG.
 */
public class StaticSlicer {

	private final HashMap<Integer, Statement> statements;
	ArrayList<Integer> slice = new ArrayList<>();
	private SlicingConfig config;
	private	List<CFGNode> cfg;

	public StaticSlicer(SlicingConfig config, HashMap<Integer, Statement> statements, List<CFGNode> cfg){
		this.statements = statements;
		this.config = config;
		this.cfg = cfg;
	}

	/**
	 * calls all necessary methods for calculating the slice.
	 * You can use the {@link at.tugraz.ist.soma.utils.Visualizer#createPDGdotFile(HashMap, ArrayList)}
	 * to get a graphical representation of your graph!
	 */
	public void calculateSlice(){
		HashMap<Integer, Node> tree = generateControlDependencies();
		generateDataDependencies(tree);
		startTheSlicingParty(tree, config.getCriterion());

		Visualizer.createPDGdotFile(tree, slice, config, statements);
		DataIO.printSlice(slice);
	}

	/**
	 * generates the control dependencies of the PDG.
	 * @return the generated PDG
	 */
	private HashMap<Integer, Node> generateControlDependencies() {

		HashMap<Integer, Node>tree = new LinkedHashMap<Integer, Node>();

		for (CFGNode cfgNode : cfg) {

			Node nodeToAdd = new Node(cfgNode.getStatement());

			if (cfgNode.getStatement() == null) {
				tree.put(0, nodeToAdd);
				continue;
			}

			if (cfgNode.getInfluence().getStatement() == null) {
				//tree.get(0).appendControlDependency(nodeToAdd);
				nodeToAdd.appendControlDependency(tree.get(0));
			} else {
				//tree.get(cfgNode.getInfluence().getStatement().getLine()).appendControlDependency(nodeToAdd);
				nodeToAdd.appendControlDependency(tree.get(cfgNode.getInfluence().getStatement().getLine()));
			}

			tree.put(cfgNode.getStatement().getLine(), nodeToAdd);
		}

		return tree;
	}

	/**
	 * adds the data dependencies to the generated PDG.
	 * @param tree the PDG which was created in {@link at.tugraz.ist.soma.algorithms.StaticSlicer#generateControlDependencies()}.
	 */
	private void generateDataDependencies(HashMap<Integer, Node> tree) {

		for (CFGNode cfgNode : cfg) {

			if (cfgNode.getStatement() == null) {
				continue;
			}
			Node node = tree.get(cfgNode.getStatement().getLine());
			Set<Integer> passedLines = new HashSet<>();
			for (String referenced : cfgNode.getStatement().getRef()) {
				// self reference
				if (cfgNode.getStatement().getDef().contains(referenced)) {
					node.appendDataDependency(node);
				}
				passedLines.clear();
				for (CFGNode predecessor : cfgNode.getPredecessors()) {
					appendDataDependendies(tree, node, predecessor, referenced, passedLines);
				}
			}
		}
	}

	private void appendDataDependendies(HashMap<Integer, Node> tree, Node node, CFGNode predecessor, String referenced, Set<Integer> passedLines) {
		if (predecessor.getStatement() == null || passedLines.contains(predecessor.getStatement().getLine())) {
			return;
		}
		passedLines.add(predecessor.getStatement().getLine());
		if (predecessor.getStatement().getDef().contains(referenced)) {
			node.appendDataDependency(tree.get(predecessor.getStatement().getLine()));
		} else {
			// go to next level above
			for (CFGNode nextPredecessor : predecessor.getPredecessors()) {
				appendDataDependendies(tree, node, nextPredecessor, referenced, passedLines);
			}
		}
	}

	/**
	 * calculates the slice of a given PDG.
	 */
	private void startTheSlicingParty(HashMap<Integer, Node> tree, String criterion) {

		Set<Integer> sliceSet = new HashSet<>();
		ArrayList<String>criterionVariables = new ArrayList<>();

		criterion = criterion.replaceAll("[() {}]", "");
		String[] arr = criterion.split(",");
		int criterionLine = Integer.parseInt(arr[0]);

		for(int i = 1; i < arr.length; i++){
			criterionVariables.add(arr[i]);
		}


        for(String criterionVar: criterionVariables) {
        	Set<Integer> startLineNumbers = new HashSet<>();

        	CFGNode endNode = cfg.get(cfg.size() - 1);
        	Set<Integer> passedLines = new HashSet<>();
        	appendStartLine(endNode, criterionVar, startLineNumbers, criterionLine, passedLines);

			for (Integer startLineNumber : startLineNumbers) {
				Node node = tree.get(startLineNumber);
				appendRelevantNodes(sliceSet, node);
			}
        }
        slice.addAll(sliceSet);

		Collections.sort(slice);
	}

	private void appendStartLine(CFGNode node, String criterionVar, Set<Integer> startLineNumbers, int criterionLine, Set<Integer> passedLines) {
		if (node.getStatement() == null) {
			return;
		}
		passedLines.add(node.getStatement().getLine());
		if (node.getStatement().getDef().contains(criterionVar) && node.getStatement().getLine() < criterionLine) {
			startLineNumbers.add(node.getStatement().getLine());
		} else {

			for (CFGNode predecessor : node.getPredecessors()) {
				if (predecessor.getStatement() != null && !passedLines.contains(predecessor.getStatement().getLine()))
					appendStartLine(predecessor, criterionVar, startLineNumbers, criterionLine, passedLines);
			};
		}
	}


	private void appendRelevantNodes(Set<Integer> sliceSet, Node nodeToAdd) {
		if (nodeToAdd.getStatement() == null
			|| sliceSet.contains(nodeToAdd.getStatement().getLine())) {
			return;
		}
		sliceSet.add(nodeToAdd.getStatement().getLine());
		nodeToAdd.getAllDependencies().forEach((node) -> {
			if (node != nodeToAdd) {
				appendRelevantNodes(sliceSet, node);
			}
		});
	}

	/**
	 * checks if a set contains at least one variable of a second set.
	 */
	private boolean contains(HashSet<String> set1, HashSet<String> set2){
		for(String string : set1){
			if(set2.contains(string)){
				return true;
			}
		}
		return false;
	}

	public ArrayList<Integer> getSlice() {
		return slice;
	}

	public void setSlice(ArrayList<Integer> slice) {
		this.slice = slice;
	}
}
