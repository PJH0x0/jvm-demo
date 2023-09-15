package com.sample.ch07;

public class ClinitTest {
    static int a = 10;
    static {
        int b = 10;
        b++;
        println(b);
    }
    public static void main(String[] args) {
        //println(a);
        //new ClinitTest();
        //a = 20;
        test();
    }
    public static void test() {

    }
    public static void println(int a) {
    }
}
