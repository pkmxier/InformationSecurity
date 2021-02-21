package org.mai;

public class MoudloExponentiation {
    public static int Calculate(int value, int power, int modulo) {
        if (power == 0) {
            return 1;
        }

        int current =  Calculate(value, power / 2, modulo);

        return power % 2 == 0 ?
                (current * current) % modulo :
                (value * current * current) % modulo;
    }
}
