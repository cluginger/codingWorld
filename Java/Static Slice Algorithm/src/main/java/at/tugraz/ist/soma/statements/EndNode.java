package at.tugraz.ist.soma.statements;

import at.tugraz.ist.soma.utils.Type;

public class EndNode extends Statement {

    public EndNode(int line, String text) {
        super(line, text);
        type = Type.ENDNODE;
    }

    @Override
    public void setLastLine(int lastLine) {
        this.lastLine = lastLine;
    }

    @Override
    public int getLastLine() {
        return lastLine;
    }
}
