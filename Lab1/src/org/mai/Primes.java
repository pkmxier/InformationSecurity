package org.mai;

import java.math.BigInteger;
import java.util.*;
import java.util.stream.IntStream;

public class Primes {
    public static int[] GetPrimes(int m) {
        boolean[] isPrime = new boolean[m];
        Arrays.fill(isPrime, true);

        isPrime[0] = isPrime[1] = false;

        for (int i = 2; i < m; ++i) {
            if (isPrime[i] && i * i < m) {
                for (int j = i * i; j < m; j += i) {
                    isPrime[j] = false;
                }
            }
        }

        return IntStream
                .range(0, isPrime.length)
                .filter(i -> isPrime[i])
                .toArray();
    }
}
