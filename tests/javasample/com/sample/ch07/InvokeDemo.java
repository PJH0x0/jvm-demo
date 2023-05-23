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
        
    }
    @Override
    public void run() {
      
    }
}