package Haiku;

import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Random;

public class HaikuServer {
    private static String haikus[] = new String[] {
            "You and me alone\nMadness of\nworld locked\naway Peace\nand quiet reigns",
    };

    public static void main(String[] args) {
        try {
            Random random = new Random();
            ServerSocket sock = new ServerSocket(5575);
            while (true) {
                Socket client = sock.accept();
                PrintWriter pout = new PrintWriter(client.getOutputStream(), true);

                int randomIndex = random.nextInt(haikus.length);
                pout.println(haikus[randomIndex]);
                client.close();
            }
        } catch (Exception e) {
            System.err.println(e);
        }
    }
}