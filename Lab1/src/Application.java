import org.mai.*;

import java.util.Arrays;

public class Application {
    public static void main(String[] args) {
        int m = 100;
        int[] primes = Primes.GetPrimes(m);
        System.out.println(String.format("Primes less than %d:\n\t%s", m, Arrays.toString(primes)));

        int value = 2;
        int power = 100;
        int modulo = 5;
        System.out.print(String.format("(%d^%d) mod %d = %d",
                value, power, modulo, MoudloExponentiation.Calculate(value, power, modulo)));


    }
}
