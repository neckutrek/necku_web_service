package trading;

public class MarketChannel {

    public static void connect()
    {
        new MarketChannelImpl().connect();
    }

}
