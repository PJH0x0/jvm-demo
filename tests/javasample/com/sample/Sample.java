package com.sample;
public class Sample {
    public static final boolean FLAG = true;
    public static final byte BYTE = 123;
    public static final char X = 'X';
    public static final short SHORT = 12345;
    public static final int INT = 123456789;
    public static final long LONG = 12345678901L;
    public static final float PI = 3.14f;
    public static final double E = 2.71828;
    public static final String str = "pengjinghui";
    private int a = -1;
    String b = "hello world";
    protected float c = 3.14f;
    public interface Test {
        public void method(String s);
    }
    public static void main(String[] args) {
        Test test = (s)->System.out.println(s + "world");
        test.method("Hello ");
        float a = 1.0f - 0.9f;
        float b = 0.9f - 0.8f;
        System.out.println("a>>>" + a);
        System.out.println("b>>>" + b);
        if (a == b) {
            System.out.println("True");   
        } else {
            System.out.println("false");
        }
        Float c = Float.valueOf(1.0f - 0.9f);
        Float d = Float.valueOf(0.9f - 0.8f);
        System.out.println("c>>>" + c);
        System.out.println("d>>>" + d);
        if (c == d) {
            System.out.println("True");
        } else {
            System.out.println("False");
        }
        String param = null;
        switch(param) {
            case "null":
                System.out.println("null");
                break;
            default:
                System.out.println("default");
        }
        System.out.println("Hello world");
    }
}