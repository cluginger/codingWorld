package at.tugraz.ist.soma.dd.common;

public class ConsoleLogger implements Logger {

  private static int step = 0;

  public ConsoleLogger() {
  }

  public void log(String testcase, String input, Result result) {
    String format = "|%5d |%14s | %15s |%9s |%n";
    step++;

    System.out.format(format, step, testcase, modify(input), result);
  }

  public void init() {
    step = 0;
    breakLine();
    System.out
        .println("| Step |   Test case   |       Input     |  Result  |");
    breakLine();
  }

  public void breakLine() {
    System.out
        .println("+---------------------------------------------------+");
  }

  public void error() {
    System.out
        .println("+----------------------ERROR------------------------+");
  }
}
