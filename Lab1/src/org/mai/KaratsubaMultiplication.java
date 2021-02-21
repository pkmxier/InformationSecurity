package org.mai;

import jdk.jshell.execution.LocalExecutionControl;

import javax.naming.Name;
import javax.naming.directory.InvalidAttributeIdentifierException;
import java.lang.reflect.Array;
import java.util.Arrays;
import java.util.concurrent.Callable;

public class KaratsubaMultiplication {
    private static int naiveLength = 2;

    public static int[] NaiveMultiplication(int[] lhs, int[] rhs) {
        int n = lhs.length, m = rhs.length;
        int[] result = new int[n + m];

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                result[i + j] += lhs[i] * rhs[j];
            }
        }

        return result;
    }

    private static int[] Karatsuba(int[] lhs, int[] rhs) {
        int n = lhs.length;
        int[] result = new int[n * 2];

        if (n <= naiveLength) {
            return NaiveMultiplication(lhs, rhs);
        }

        int k = n / 2;

        int[] lhsH = Arrays.stream(lhs).limit(k).toArray();
        int[] lhsL = Arrays.stream(lhs).skip(k).toArray();
        int[] rhsH = Arrays.stream(rhs).limit(k).toArray();
        int[] rhsL = Arrays.stream(rhs).skip(k).toArray();

        int[] poly1 = Karatsuba(lhsL, rhsL);
        int[] poly2 = Karatsuba(lhsH, rhsH);

        int[] lhsLH = new int[k];
        int[] rhsLH = new int[k];

        for (int i = 0; i < k; ++i) {
            lhsLH[i] = lhsL[i] + lhsH[i];
            rhsLH[i] = rhsL[i] + rhsH[i];
        }

        int[] poly3 = Karatsuba(lhsLH, rhsLH);

        for (int i = 0; i < n; ++i) {
            poly3[i] -= poly2[i] + poly1[i];
            result[i] = poly2[i];
        }

        for (int i = n; i < 2 * n; ++i) {
            result[i] = poly1[i - n];
        }

        for (int i = k; i < n + k; ++i) {
            result[i] += poly3[i - k];
        }

        return result;
    }

    public static int[] Calculate(int[] lhs, int[] rhs) {
        return Arrays.stream(Karatsuba(lhs, rhs)).limit(lhs.length + rhs.length - 1).toArray();
    }
}
