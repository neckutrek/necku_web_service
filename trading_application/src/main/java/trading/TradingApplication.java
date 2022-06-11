package trading;

import org.springframework.boot.SpringApplication;

public class TradingApplication {

    public static void main(String[] args)
    {
        System.out.println("Starting TradingApplication");

        System.loadLibrary("market_connection");
        System.out.println("Dynamic libraries loaded successfully");

        SpringApplication.run(SpringBootRunner.class, args);

        System.out.println("Quitting program.");
    }

}
