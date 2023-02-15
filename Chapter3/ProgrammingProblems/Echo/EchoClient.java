package Echo;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class EchoClient {
    public static void main(String[] args) {
        try {
            Socket sock = new Socket("127.0.0.1", 5576);
            InputStream in = sock.getInputStream();
            BufferedReader bin = new BufferedReader(new InputStreamReader(in));
            PrintWriter writer = new PrintWriter(sock.getOutputStream(), true);
            writer.write("Hello world!\n");
            writer.flush();

            String line = bin.readLine();
            System.out.println(line);
            sock.close();
        } catch (

        IOException e) {
            System.err.println(e);
        }
    }

}
