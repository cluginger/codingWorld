package at.tugraz.ist.soma.utils;

import at.tugraz.ist.soma.FirstPassVisitor;
import at.tugraz.ist.soma.JavaLexer;
import at.tugraz.ist.soma.JavaParser;
import at.tugraz.ist.soma.SecondPassVisitor;
import at.tugraz.ist.soma.statements.Statement;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.ParseTree;

import java.io.IOException;
import java.util.HashMap;

public class StatementProcessor {

    private HashMap<Integer, Statement> statements;
    private SlicingConfig config;

    public StatementProcessor(SlicingConfig config) {
        this.config = config;
    }

    public HashMap<Integer, Statement> processStatements() throws IOException {
        String fileContent = config.getFileContent();
        CharStream input;
        if(fileContent == null) {
            input = CharStreams.fromFileName(config.getPath());
        }
        else{
            input = CharStreams.fromString(fileContent);
        }
        JavaLexer lexer = new JavaLexer(input);
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        JavaParser parser = new JavaParser(tokens);

        ParseTree tree = parser.compilationUnit();

        FirstPassVisitor visitor = new FirstPassVisitor();
        visitor.visit(tree);

        SecondPassVisitor visitor2 = new SecondPassVisitor(visitor.getStatements());
        visitor2.visit(tree);

        return visitor2.getStatements();
    }
}
