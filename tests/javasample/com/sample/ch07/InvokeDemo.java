package com.sample.ch07;

public class InvokeDemo implements Runnable{

    public static void main(String[] args) {
        new InvokeDemo().test();
    }

    public void test() {
      InvokeDemo.staticMethod();//invokestatic
      InvokeDemo demo = new InvokeDemo();//new, invokespecial
      demo.privateMethod();//invokespecial
      demo.run();//invokevirtual
      super.equals(null);//invokespecial
      ((Runnable)demo).run();//invokeinterface
    }

    public static void staticMethod() {
        
    }
    private void privateMethod() {
      int b = 0;
      int a = 1;
      int c = a + b;
      if (a > c) {
        a++;
      } else {
        run();
      }
    }
    @Override
    public void run() {
      int a = 0;
      a++;
    }
}