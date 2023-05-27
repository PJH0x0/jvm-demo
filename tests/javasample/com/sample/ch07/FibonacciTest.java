package com.sample.ch07;

class FibonacciTest {
    public static void main(String[] args) {
        FibonacciTest fibonacciTest = new FibonacciTest();
        long x = fibonacciTest.fibonacci(20);
        println(x);
    }
    public static void println(long x){}
    public static long fibonacci(int n) {
        if (n <= 1) return n;
        return fibonacci(n-1) + fibonacci(n-2);
    }
}