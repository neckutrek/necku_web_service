package trading;

import org.springframework.boot.SpringApplication;

public class TradingApplication {

    public static void main(String[] args)
    {
        System.out.println("Starting RandomWebService.");
        SpringApplication.run(SpringBootRunner.class, args);
        System.out.println("Quitting program.");
    }

}
