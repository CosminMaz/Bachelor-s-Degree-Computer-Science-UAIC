package ServerApplication;

import lombok.Getter;

import java.io.*;
import java.net.Socket;
import java.util.Map;
import java.util.Objects;

public class ClientThread extends Thread{
    @Getter
    private Socket socket = null;
    private final GameServer gameServer;
    @Getter
    private int idClient;
    public ClientThread(Socket socket, GameServer gameServer, int id) {
        this.socket = socket;
        this.gameServer = gameServer;
        this.idClient = id;
    }

    public void run(){
        try {
            label:
            while(true) {
                BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                String request = in.readLine();

                PrintWriter out = new PrintWriter(socket.getOutputStream());

                switch (request) {
                    case "stop":
                        out.println("stop");
                        out.flush();
                        this.gameServer.getServerSocket().close();
                        break label;
                    case "create a game":
                        out.println("you created a game, your color is red");
                        out.flush();
                        this.gameServer.getPerechi().put(this.idClient, 0); // daca e 0 nu e nimeni pereche deocamdata
                        break;
                    case "join a game":
                        out.println("you joined a game, your color is blue");
                        out.flush();
                        for (Map.Entry<Integer, Integer> entry : this.gameServer.getPerechi().entrySet()) {
                            if (entry.getValue() == 0) {
                                this.gameServer.getPerechi().put(entry.getKey(), this.idClient);
                                break;
                            }
                        }
                        break;
                    case "create a game AI":
                        out.println("you created a game with AI");
                        out.flush();
                        break;
                    default:
                        if(!this.gameServer.getPerechi().containsKey(idClient) && !this.gameServer.getPerechi().containsValue(idClient)) {
                            out.println("Nu esti intr-o partida!");
                            out.flush();
                        }else {
                            this.gameServer.sendToPartner(this.idClient, request);
                        }
                        break;
                }
            }
        } catch (IOException e){
            System.err.println("Oopsie doopesie from ClientThread: " + e);
        } finally {
            try {
                socket.close();

            } catch (IOException e) {
                System.err.println("Another oopsie doopesie from ClientThread: " + e);
            }

        }
    }
}
