package com.sample.ch09;

public class ObjectTest {
    public static void main(String[] args) {
        ObjectTest ot = new ObjectTest();
        System.out.println(ot.toString());
        System.out.println(ot);
        System.out.println(Integer.toHexString(ot.HashCode()));
        ObjectTest ot2 = new ObjectTest();
        System.out.println(ot.equals(ot2));
        System.out.println(ot == ot2);
        System.out.println(ot.equals(ot));
    }
}