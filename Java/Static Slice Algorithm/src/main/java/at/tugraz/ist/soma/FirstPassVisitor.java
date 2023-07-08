package at.tugraz.ist.soma;

import at.tugraz.ist.soma.JavaParser.*;
import at.tugraz.ist.soma.statements.*;

import java.util.HashMap;
import java.util.LinkedHashMap;

/**
 * This class creates a {@link at.tugraz.ist.soma.statements.Statement} for each line of the input file.
 * This is done by using the auto generated visitor methods from the ANTLR-framework.
 * @author Stephan Frühwirt
 *
 */

/*
 * This code works.
 * I don't know why.
 * It's kinda magic...
 */

public class FirstPassVisitor extends JavaBaseVisitor<String>{
	
	HashMap<Integer, Statement> statements = new LinkedHashMap<>();
	
	@Override
	public String visitLocalVariableDeclaration(LocalVariableDeclarationContext ctx) {
		String type = visit(ctx.type());
		String declaration = visit(ctx.variableDeclarators());
		
		Statement s = new VariableDeclaration(ctx.getStart().getLine(), type + " " + declaration);
		statements.put(s.getLine(),s);
		
		return type + " " + declaration;
	}
	
	@Override
	public String visitType(TypeContext ctx) {
		return ctx.getText();
	}
	
	@Override
	public String visitVariableDeclarators(VariableDeclaratorsContext ctx) {
		return ctx.getText();
	}
	
	@Override
	public String visitStatementExpression(StatementExpressionContext ctx) {
		String expr = visit(ctx.expression());

		Statement s = new Assignment(ctx.getStart().getLine(), expr);
		statements.put(s.getLine(),s);
		
		return "";
	}
	
	@Override
	public String visitExpression(ExpressionContext ctx) {
		return ctx.getText();
	}
	
	@Override
	public String visitIfStmt(IfStmtContext ctx) {
		String expr = visit(ctx.parExpression());
		
		Statement s = new IfStatement(ctx.getStart().getLine(), "if" + expr);
		statements.put(s.getLine(),s);
		
		visit(ctx.statement());
		
		return "";
	}
	
	@Override
	public String visitParExpression(ParExpressionContext ctx) {
		return ctx.getText();
	}
	
	@Override
	public String visitElseStmt(ElseStmtContext ctx) {
		Statement s = new ElseStatement(ctx.getStart().getLine(), "else");
		statements.put(s.getLine(),s);
		
		visit(ctx.statement());
		
		return "";
	}
	
	@Override
	public String visitWhileStmt(WhileStmtContext ctx) {
		String expr = visit(ctx.parExpression());
		
		Statement s = new WhileLoop(ctx.getStart().getLine(), "while" + expr);
		statements.put(s.getLine(),s);
		
		visit(ctx.statement());
		
		return "";
	}
	
	@Override
	public String visitForStmt(ForStmtContext ctx) {
		String expr = visit(ctx.forControl());
		
		Statement s = new ForLoop(ctx.getStart().getLine(), "for(" + expr + ")");
		statements.put(s.getLine(),s);
		
		visit(ctx.statement());
		
		return "";
	}
	
	@Override
	public String visitForControl(ForControlContext ctx) {
		return ctx.getText();
	}

	public HashMap<Integer, Statement> getStatements() {
		return statements;
	}

	public void setStatements(HashMap<Integer, Statement> statements) {
		this.statements = statements;
	}
}
