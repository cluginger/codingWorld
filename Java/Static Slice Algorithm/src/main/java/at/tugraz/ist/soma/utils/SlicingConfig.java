package at.tugraz.ist.soma.utils;

public class SlicingConfig {
    String criterion;
    String path;
    String fileContent;



    boolean debug;

    public SlicingConfig(String criterion, String path, boolean debug) {
        this.criterion = criterion;
        this.path = path;
        this.debug = debug;
    }

    public SlicingConfig() {
    }

    public String getPath() {
        return path;
    }

    public void setPath(String path) {
        this.path = path;
    }

    public String getCriterion() {
        return criterion;
    }

    public void setCriterion(String criterion) {
        this.criterion = criterion;
    }

    public boolean isDebug() {
        return debug;
    }

    public String getFileContent() {
        return fileContent;
    }

    public void setFileContent(String fileContent) {
        this.fileContent = fileContent;
    }

    public void setDebug(boolean debug) {
        this.debug = debug;
    }

    public String getOutputFileName() {
        return path.replace("input", "output");
    }
}
