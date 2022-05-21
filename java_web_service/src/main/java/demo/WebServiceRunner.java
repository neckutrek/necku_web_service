package demo;

import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class WebServiceRunner implements CommandLineRunner {

    @Override
    public void run(String... args)  {
        main2(args);
    }

    public static void main2(String[] args)
    {
        System.out.println("Hello from inside Runner");
        while (true) {
            NativeInterface.helloWorld();
        }
    }
}
