package com.sample.ch08;
public class ArrayTest {
    public static void main(String[] args) {
        int[] a = new int[10];
        long[] l = new long[100];
        float[] f = new float[1000];
        double[] d = new double[10000];
        boolean[] b = new boolean[10000];
        char[] c = new char[10000];
        ArrayTest[] s = new ArrayTest[100];
        int[][][] mutiArray = new int[3][4][5];
        int length1 = l.length;
        int length2 = mutiArray.length;
        System.out.println(length1);
        System.out.println(length2);
    }
}
