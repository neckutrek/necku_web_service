package trading;

public class MarketChannelImpl
{
    static {
        System.loadLibrary("market_connection");
    }

    public native void connect();
}

