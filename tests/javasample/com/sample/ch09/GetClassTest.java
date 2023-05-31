package com.sample.ch09;

public class GetClassTest {
    public static void main(String[] args) {
        String s = "Hello";
        Class c = s.getClass();
        println(c.getName());
    }

    public static void println(String s) {
    }
}