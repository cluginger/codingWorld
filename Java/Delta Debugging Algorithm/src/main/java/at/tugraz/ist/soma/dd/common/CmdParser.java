package at.tugraz.ist.soma.dd.common;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class CmdParser {


    /**
     * parses the console arguments.
     * There are two ways:
     * -> one argument:    * the path to an input-parameter (.txt) file
     * -> four arguments:  * the algorithm (DIFF_DELTA, MIN_DELTA), the input string, the failure character,
     * the dependency characters
     */

    public static DDConfig parseInput(String[] args) {
        DDConfig config = new DDConfig();

        switch (args.length) {
            case 1:
                try {
                    FileReader fr = new FileReader(args[0]);
                    BufferedReader br = new BufferedReader(fr);

                    String algorithm = br.readLine();
                    String input = br.readLine();
                    String failure = br.readLine();
                    String dependency = br.readLine();

                    br.close();

                    if (algorithm == null || input == null || failure == null) {
                        System.err.println("Wrong file format: should be: <algorithm>");
                        System.err.println("                              <input>");
                        System.err.println("                              <failure>");
                        System.err.println("                          or: <algorithm>");
                        System.err.println("                              <input>");
                        System.err.println("                              <failure>");
                        System.err.println("                              <dependency>");
                        return null;
                    }

                    return saveInput(config, algorithm, input, failure, dependency == null ? "" : dependency);
                } catch (IOException e) {
                    System.err.println(e);
                    return null;
                }

            case 4:
                return saveInput(config, args[0], args[1], args[2], args[3]);

            default:
                System.err.println("Usage: java -jar Minimizer_<your group number>[.jar] <input-file>");
                System.err.println("       java -jar Minimizer_<your group number>[.jar] <Algorithm> <InputString> <Failure> <Dependency>");
                System.err.println("                 e.g. " + DDConfig.Algorithm.DIFF_DELTA_P1.getDdConfigString() + " \"012345678\" \"3\" \"14\"");
                return null;
        }
    }


    /**
     * Saves the given input-parameters for further usage in the algorithm
     */
    public static DDConfig saveInput(DDConfig config, String algorithm, String input, String failure, String dependency) {

        boolean algoSet = false;
        for (DDConfig.Algorithm algo : DDConfig.Algorithm.values()) {
            if (algorithm.equals(algo.getDdConfigString())) {
                config.setAlgorithm(algo);
                algoSet = true;
            }
        }

        if (!algoSet) {
            StringBuilder sb = new StringBuilder();
            sb.append("Usage: Algorithm must be one of the following: ");
            boolean first = true;

            for (DDConfig.Algorithm algo : DDConfig.Algorithm.values()) {
                if (first) {
                    sb.append("{'");
                    sb.append(algo.getDdConfigString());
                    first = false;
                } else {
                    sb.append(algo.getDdConfigString());
                    sb.append("', '");
                }
            }

            sb.append("'}");

            System.err.println(sb.toString());
            return null;
        }

        config.setInput(input);
        config.setFailure(failure.charAt(0));
        config.setDepending(dependency.length() == 0 ? null : dependency.toCharArray());
        return config;
    }
}