package com.sample.ch08;

public class BubbleSort {
    public static void main(String[] args) {
      int[] a = { 22, 84, 77, 11, 95, 9, 78, 101, 56, 36, 10, 25, 92, 3, 48 };
      bubbleSort(a);
      printArray(a);
    }
    public static void bubbleSort(int[] a) {
        int n = a.length;
        int temp = 0;
        for (int i = 0; i < n - 1; i++) { // 外层循环控制排序趟数
            for (int j = 0; j < n - 1 - i; j++) { // 内层循环控制每一趟排序多少次
                if (a[j] > a[j + 1]) { // 相邻元素两两对比
                    temp = a[j]; // 元素交换
                    a[j] = a[j + 1];
                    a[j + 1] = temp;
                }
            }
        }
    }
    public static void printArray(int[] a) {
        for (int i = 0; i < a.length; i++) { // 遍历数组
            System.out.println(a[i]);
        }
    }
}