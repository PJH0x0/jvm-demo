package com.sample.ch09;

import java.util.ArrayList;
import java.util.List;
public class BoxTest {
    public static void main(String[] args) {
        List<Integer> list = new ArrayList<>();
        list.add(1);
        list.add(2);
        list.add(3);
        for (int x : list) {
            System.out.println(x);
        }
    }
}