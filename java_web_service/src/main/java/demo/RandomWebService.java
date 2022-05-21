package demo;

import org.springframework.boot.SpringApplication;

public class RandomWebService {

    public static void main(String[] args)
    {
        System.out.println("Starting RandomWebService.");
        SpringApplication.run(WebServiceRunner.class, args);
        System.out.println("Quitting program.");
    }

}
