package com.sample.ch10;

public class ExceptionTest {
    public static void main(String[] args) {
        foo(args);
    }
    private static void foo(String[] args) {
        try {
            bar(args);
        } catch (NumberFormatException e) {
            System.out.println(e.getMessage());
        }
    }
    private static void bar(String[] args) {
        if (args.length == 0) {
            throw new IndexOutOfBoundsException("no args");
        }
        int i = Integer.parseInt(args[0]);
        System.out.println(i);
    }
}