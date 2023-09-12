package com.sample.ch09;

public class GetClassTest {
    public static void main(String[] args) {
        
        String s = "Hello";
        Class c = s.GetClass();
        System.out.println(c.getName());
        System.out.println(String.class.getName());
        System.out.println(int[].class.getName());
        System.out.println(int[][].class.getName());
        System.out.println(String[].class.getName());
        System.out.println(String[][].class.getName());
        println(GetClassTest.class.getName());
        System.out.println(GetClassTest[][].class.getName());
        System.out.println(Object[][].class.getName());
        System.out.println(void.class.getName());
        println(int.class.getName());
        
    }

    public static void println(String s) {
    }
}