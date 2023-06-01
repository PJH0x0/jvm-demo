package com.sample.ch09;

public class GetClassTest {
    public static void main(String[] args) {
        
        String s = "Hello";
        Class c = s.getClass();
        println(c.getName());
        println(String.class.getName());
        println(int[].class.getName());
        println(int[][].class.getName());
        println(String[].class.getName());
        println(String[][].class.getName());
        println(GetClassTest.class.getName());
        println(GetClassTest[][].class.getName());
        println(Object[][].class.getName());
        println(void.class.getName());
        println(int.class.getName());
        
    }

    public static void println(String s) {
    }
}