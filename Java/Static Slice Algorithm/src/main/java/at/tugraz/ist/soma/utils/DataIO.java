package at.tugraz.ist.soma.utils;

import java.util.ArrayList;

/**
 * This class handles the parsing of the console arguments and the output of the end result (slice).
 * @author Stephan Frühwirt
 */
public class DataIO {

	/**
	 * parses the console arguments. There should be exactly two arguments:
	 * the path to an input (.java) file and a slicing criterion.
	 */
	public static SlicingConfig parseInput(String[] args){
		if(args.length < 2){
			System.err.println("Error: Please specify an input file and a slicing criterion, e.g. \"(13,{z})\"");
			System.exit(-1);
		}

		boolean debug = false;

		if(args.length == 3 && args[2].equals("debug")){
			debug = true;
		}
		
		return new SlicingConfig(args[0], args[1], debug);
	}
	
	/**
	 * prints the slice to the console. The formatting is done for you!
	 */
	public static void printSlice(ArrayList<Integer> slice){
		System.out.println(slice);
	}

}
