import org.mai.*;

import java.lang.reflect.Array;
import java.util.Arrays;

public class Application {
    public static void main(String[] args) {
        // 1
        int m = 100;
        int[] primes = Primes.GetPrimes(m);
        System.out.println(String.format("1. Primes less than %d:\n\t%s", m, Arrays.toString(primes)));

        // 2
        int value = 2;
        int power = 100;
        int modulo = 17;
        System.out.println(String.format("2. (%d^%d) mod %d = %d",
                value, power, modulo, MoudloExponentiation.Calculate(value, power, modulo)));

        // 3
        int a = 736534654, b = 1993483958;
        ModularArithmetic arithmetic = new ModularArithmetic(modulo);
        System.out.println(String.format(
                "3. Modular arithmetic:\n" +
                "\t(%d + %d) mod %d = %d\n" +
                "\t(%d - %d) mod %d = %d\n" +
                "\t(%d * %d) mod %d = %d\n" +
                "\t(%d ^ %d) mod %d = %d\n",
                a, b, modulo, arithmetic.Sum(a, b),
                a, b, modulo, arithmetic.Sub(a, b),
                a, b, modulo, arithmetic.Mult(a, b),
                a, b, modulo, arithmetic.Exp(a, b)));


        // 5
        int[] polynom1 = {1, 0, 1, 0, -3, -3, 8, 2};
        int[] polynom2 = {3, 0, 5, 0, -4, -9, 21, 4};
        int[] polynomMultiplication = KaratsubaMultiplication.Calculate(polynom1, polynom2);

        System.out.println(String.format("5. Karatsuba multiplication: \n\t%s", Arrays.toString(polynomMultiplication)));
    }
}
