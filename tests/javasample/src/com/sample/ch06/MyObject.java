package com.sample.ch06;

public class MyObject {
    public static int staticVar;
    public int instanceVar;
    public static void main(String[] args) {
        int x = 32678; // ldc
        MyObject obj = new MyObject(); // new
        
        MyObject.staticVar = x; // putstatic
        x = MyObject.staticVar; // getstatic
        obj.instanceVar = x; // putfield
        x = obj.instanceVar; // getfield
        Object obj2 = obj; // implicit cast
        if (obj2 instanceof MyObject) { // instanceof
            MyObject obj3 = (MyObject) obj2; // check cast
            System.out.println(obj3.instanceVar);
        }
    }
}