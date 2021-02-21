package org.mai;

public class ModularArithmetic {
    private int modulo;

    public ModularArithmetic(int modulo) {
        this.modulo = modulo;
    }

    public int Sum(int lhs, int rhs) {
        return (lhs % modulo + rhs % modulo) % modulo;
    }

    public int Sub(int lhs, int rhs) {
        return (lhs % modulo - rhs % modulo) % modulo;
    }

    public int Mult(int lhs, int rhs) {
        return ((lhs % modulo) * (rhs % modulo)) % modulo;
    }

    public int Exp(int value, int power) {
        return MoudloExponentiation.Calculate(value, power, modulo);
    }
}
