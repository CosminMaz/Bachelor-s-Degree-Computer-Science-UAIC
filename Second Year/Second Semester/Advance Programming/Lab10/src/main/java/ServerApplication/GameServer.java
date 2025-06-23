package ServerApplication;

import lombok.Getter;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class GameServer {
    public static final int PORT = 8100;
    private final List<ClientThread> clients = new ArrayList<>();
    @Getter
    private final Map<Integer, Integer> perechi = new ConcurrentHashMap<>();

    @Getter
    private final ServerSocket serverSocket = new ServerSocket(PORT);

    public GameServer() throws IOException {

        try{
            int id = 0;
            while(true) {
                Socket socket = serverSocket.accept();
                ClientThread client = new ClientThread(socket, this, ++id);
                clients.add(client);
                client.start();
            }
        } catch (IOException e) {
            System.err.println("Opsie dopsie from GameServer: " + e);
        } finally {
            serverSocket.close();
        }
    }

    public void sendToPartner(int idClient1, String message) throws IOException {
        Integer idClient2 = perechi.get(idClient1);
        if (idClient2 == null || idClient2 == 0) {
            for (Map.Entry<Integer, Integer> entry : perechi.entrySet()) {
                if (entry.getValue() == idClient1) {
                    idClient2 = entry.getKey();
                    break;
                }
            }
        }

        if (idClient2 != null && idClient2 != 0) {
            for (ClientThread client : clients) {
                if(client.getIdClient() == idClient2) {
                    PrintWriter out = new PrintWriter(client.getSocket().getOutputStream());
                    out.println(idClient1 + " " + message);
                    out.flush();
                    break;
                }
            }
        }
    }

    public static void main(String [] args) throws IOException {
        GameServer server = new GameServer();
    }


}
