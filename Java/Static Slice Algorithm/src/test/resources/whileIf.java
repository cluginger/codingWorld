class Public2 {
    public static void main() {
        int x = 5;
        int z  = 0;
        int y = 2;

        while (z < 4)
        {
            while (z < 4)
            {
                if (y < 10)
                {
                    y = 11;
                    z = z + 1;
                }
                else
                {
                    y = 4;
                    x = z + 2;
                }
            }
        }
        z =  y;
        x = y;
    }
}