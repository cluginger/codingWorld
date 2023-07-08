package at.tugraz.ist.soma;

import at.tugraz.ist.soma.JavaParser.*;
import at.tugraz.ist.soma.statements.Statement;

import java.util.HashMap;

/**
 * This class adds information (line number, last line etc.) to a given {@link at.tugraz.ist.soma.statements.Statement}.
 * This is done by using the auto generated visitor methods from the ANTLR-framework.
 * @author Stephan Frühwirt
 */

public class SecondPassVisitor extends JavaBaseVisitor<String>{

	private HashMap<Integer,Statement> statements;
	private Statement statement = null;

	public SecondPassVisitor(HashMap<Integer,Statement> statements){
		this.statements = statements;
	}

	public HashMap<Integer, Statement> getStatements() {
		return statements;
	}

	public void setStatements(HashMap<Integer, Statement> statements) {
		this.statements = statements;
	}

	@Override
	public String visitVariableDeclarator(VariableDeclaratorContext ctx) {
		int line = ctx.getStart().getLine();
		statement = statements.getOrDefault(line, null);
		
		String left = ctx.variableDeclaratorId().getText();
		visit(ctx.variableInitializer());
		
		statement.addDef(left);
		statements.put(line, statement);
		
		return "";
	}
	
	@Override
	public String visitPrimary(PrimaryContext ctx) {
		if(ctx.varName != null){
			statement.addRef(ctx.varName.getText());
		}
		
		visitChildren(ctx);
		return ctx.getText();
	}
	
	@Override
	public String visitStatementExpression(StatementExpressionContext ctx) {
		int line = ctx.getStart().getLine();
		statement = statements.getOrDefault(line, null);
		
		visit(ctx.expression());
		
		statements.put(line, statement);
		
		return "";
	}
	
	@Override
	public String visitWhileStmt(WhileStmtContext ctx) {
		int line = ctx.getStart().getLine();
		statement = statements.getOrDefault(line, null);
		statement.setLastLine(ctx.getStop().getLine());
		
		visit(ctx.parExpression());
		statements.put(line, statement);
		visit(ctx.statement());
		
		return "";
	}
	
	@Override
	public String visitForStmt(ForStmtContext ctx) {
		int line = ctx.getStart().getLine();
		statement = statements.getOrDefault(line, null);
		statement.setLastLine(ctx.getStop().getLine());

		visit(ctx.forControl());
		statements.put(line, statement);
		visit(ctx.statement());
		
		return "";
	}
	
	@Override
	public String visitIfStmt(IfStmtContext ctx) {
		int line = ctx.getStart().getLine();
		statement = statements.getOrDefault(line, null) ;
		statement.setLastLine(ctx.getStop().getLine());
		
		visit(ctx.parExpression());
		statements.put(line, statement);
		visit(ctx.statement());
		

		return "";
	}
	
	@Override
	public String visitElseStmt(ElseStmtContext ctx) {
		int line = ctx.getStart().getLine();
		statement = statements.getOrDefault(line, null);
		statement.setLastLine(ctx.getStop().getLine());

		statements.put(line, statement);
		visit(ctx.statement());
		
		return "";
	}
	
	@Override
	public String visitExpression(ExpressionContext ctx) {

		if(ctx.unary != null){
			statement.addDef(ctx.unary.getText());
			statement.addRef(ctx.unary.getText());
			return "";
		}
		if(ctx.lhs != null){
			visit(ctx.rhs);
			statement.addDef(ctx.lhs.getText());
			return "";
		}
		visitChildren(ctx);
		return "";
	}
}
