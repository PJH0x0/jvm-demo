package com.sample.ch09;

import java.util.Arrays;

public class CloneTest implements Cloneable {
    public char[] data = {'a', 'b', 'c'};
    @Override
    public CloneTest clone() throws CloneNotSupportedException {
        return (CloneTest) super.clone();
    }
    public static void main(String[] args) {
        CloneTest obj1 = new CloneTest();
        try {
            CloneTest obj2 = obj1.clone();
            System.out.println(obj1 == obj2);
            System.out.println(obj1.data == obj2.data);
        } catch (CloneNotSupportedException e) {
            e.printStackTrace();
        }
        
        char[] originChar = {'a', 'b', 'c'};
        char[] copyChar = originChar.clone();
        System.out.println(copyChar[0]);
        System.out.println(originChar == copyChar);
        System.out.println(new String(copyChar));
        int[] originInt = {1, 2, 3};
        int[] copyInt = originInt.Clone();
        System.out.println(Arrays.toString(copyInt));
    }
}