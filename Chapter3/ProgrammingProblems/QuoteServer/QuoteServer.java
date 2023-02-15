package QuoteServer;

import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Random;

public class QuoteServer {
    private static String quotes[] = new String[] {
            "Someday is not a day of the week.", "Hire character. Train skill.",
            "Your time is limited, so don't waste it living someone else's life.",
            "Sales are contingent upon the attitude of the salesman -- not the attitude of the prospect.",
            "Everyone lives by selling something.",
            "If you are not taking care of your customer, your competitor will.",
            "The golden rule for every businessman is this: Put yourself in your customer's place.",
            "If you cannot do great things, do small things in a great way.",
            "The best leaders are those most interested in surrounding themselves with assistants and associates smarter than they are. They are frank in admitting this and are willing to pay for such talents.",
            "Beware of monotony; it's the mother of all the deadly sins.",
            "Nothing is really work unless you would rather be doing something else.",
            "Without a customer, you don't have a business -- all you have is a hobby.",
            "To be most effective in sales today, it's imperative to drop your 'sales' mentality and start working with your prospects as if they've already hired you.",
            "Pretend that every single person you meet has a sign around his or her neck that says, 'Make me feel important.' Not only will you succeed in sales, you will succeed in life.",
            "It's not just about being better. It's about being different. You need to give people a reason to choose your business.",
            "Being good in business is the most fascinating kind of art. Making money is art and working is art and good business is the best art.",
            "Be patient with yourself. Self-growth is tender; it's holy ground. There's no greater investment.",
    };

    public static void main(String[] args) {
        try {
            Random random = new Random();
            ServerSocket sock = new ServerSocket(6017);
            while (true) {
                Socket client = sock.accept();
                PrintWriter pout = new PrintWriter(client.getOutputStream(), true);

                int randomIndex = random.nextInt(quotes.length);
                pout.println(quotes[randomIndex]);
                client.close();
            }
        } catch (Exception e) {
            System.err.println(e);
        }
    }
}