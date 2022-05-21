package demo;

public class NativeInterface {

    static {
        System.loadLibrary("native");
    }

    public static void helloWorld()
    {
        new NativeInterface().native_helloWorld();
    }

    private native void native_helloWorld();

}
