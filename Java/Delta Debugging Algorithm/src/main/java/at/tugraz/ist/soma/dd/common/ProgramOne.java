package at.tugraz.ist.soma.dd.common;

import java.util.Arrays;

public class ProgramOne implements Program {

    char mFailure;
    char[] mDepending;

    public ProgramOne(char failure) {
        mFailure = failure;
    }

    public ProgramOne(char failure, char[] depending) {
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
                if (Arrays.binarySearch(cf, t) >= 0) {
                    alldeb[i] = true;
                }
                i++;
            }

            if (!m_Array_contains(alldeb, true)) {
                r = Result.UNRES;
            } else if (m_Array_contains(alldeb, false)) {
                r = Result.UNRES;
            }
        }
        return r;
    }

    private boolean m_Array_contains(boolean[] a, boolean bool) {
        for (boolean e : a) {
            if (e == bool)
                return true;
        }
        return false;
    }

}
