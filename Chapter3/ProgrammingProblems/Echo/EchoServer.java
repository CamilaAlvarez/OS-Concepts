package Echo;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class EchoServer {
    public static void main(String[] args) {
        try {
            ServerSocket sock = new ServerSocket(5576);
            // Fixed size buffer
            byte[] buffer = new byte[4096];
            while (true) {
                Socket client = sock.accept();
                InputStream clientStream = client.getInputStream();
                OutputStream outputClientStream = client.getOutputStream();
                while (true) {
                    int readChars = clientStream.read(buffer);
                    if (readChars < 0)
                        break;
                    outputClientStream.write(buffer);
                    outputClientStream.flush();

                }
                client.close();
            }
        } catch (Exception e) {
            System.err.println(e);
        }
    }
}