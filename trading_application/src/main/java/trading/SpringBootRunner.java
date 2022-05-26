package trading;

import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class SpringBootRunner implements CommandLineRunner {

    @Override
    public void run(String... args)  {
        execute(args);
    }

    private static void execute(String[] args)
    {
        MarketChannel.connect();
    }
}
