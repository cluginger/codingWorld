package at.tugraz.ist.soma.dd.common;

import java.util.Arrays;

public class ProgramTwo implements Program {

    char mFailure;
    char[] mDepending;

    public ProgramTwo(char failure) {
        mFailure = failure;
    }

    public ProgramTwo(char failure, char[] depending) {
        mFailure = failure;
        mDepending = depending;
    }

    @Override
    public Result test(String c) {

        if (c.equals(""))
            return Result.UNRES;

        Result r = Result.EMPTY;

        boolean fail = Boolean.FALSE;

        char[] cf = c.toCharArray();
        Arrays.sort(cf);

        // FAIL OR PASS
        if (Arrays.binarySearch(cf, mFailure) >= 0) {
            fail = true;
        }

        int i = 0;

        if (fail) {
            r = Result.FAIL;
        } else {
            r = Result.PASS;
        }


        // DEPENDENCIES
        if (mDepending != null) {

            boolean[] alldeb = new boolean[mDepending.length];
            Arrays.fill(alldeb, Boolean.FALSE);

            for (char t : mDepending) {
                if (0 <= Arrays.binarySearch(cf, t)) {
                    alldeb[i] = true;
                }
                i++;
            }

            if (mArrayContains(alldeb, true) && mArrayContains(alldeb, false)) {
                r = Result.UNRES;
            }

        }
        return r;
    }

    private boolean mArrayContains(boolean[] a, boolean bool) {
        for (boolean e : a) {
            if (e == bool)
                return true;
        }
        return false;
    }

}
