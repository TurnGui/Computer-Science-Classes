class ConnectionServer extends Thread {
  Socket socket;
  ClienteJogo app;

  public ConnectionServer(Socket socket, ClienteJogo app) {
    this.socket = socket;
    this.app    = app;
  }

  public void run() {
    try {
      BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
      String mensagem;
      while ((mensagem = in.readLine()) != null) {
        System.out.println("Servidor: " + mensagem);
        processarMensagem(mensagem.trim());
      }
    } catch (Exception e) {
      System.out.println("Conexao fechada: " + e.getMessage());
      app.mensagemErro = "Ligacao perdida com o servidor.";
    }
  }

  void processarMensagem(String msg) {
    if (msg.isEmpty()) return;
    String[] partes = msg.split("#", -1);
    String tipo = partes[0];

    switch (tipo) {

      case "OK":
        if (partes.length >= 2) {
          if (partes[1].equals("LOGIN")) {
            app.mensagemSucesso = "Login bem-sucedido!";
            app.estadoAtual = 1;
          } else if (partes[1].equals("REGISTER")) {
            app.mensagemSucesso = "Registo feito! Podes fazer login.";
          } else if (partes[1].equals("UNREGISTER")) {
            app.mensagemSucesso = "Registo cancelado.";
            app.username = "";
            app.password = "";
          }
        }
        break;

      case "ERR":
        app.mensagemErro = partes.length >= 2 ? partes[1] : "Erro desconhecido.";
        break;

      case "LOBBY":
        break;

      case "TOP":
        // TOP#user1:pts|user2:pts|...
        if (partes.length >= 2 && !partes[1].equals("vazio")) {
          app.topScores.clear();
          for (String entrada : partes[1].split("\\|")) {
            String[] kv = entrada.split(":");
            if (kv.length == 2) app.topScores.add(new String[]{kv[0], kv[1]});
          }
        } else {
          app.topScores.clear();
        }
        break;

      case "GAME_START":
        app.jogadores.clear();
        app.objetos.clear();
        app.tempoRestante = 120;
        app.naFila        = false;
        app.estadoAtual   = 2;
        break;

      case "STATE":
        // STATE#tempo#players#objects
        if (partes.length >= 3) {
          try {
            app.tempoRestante = Integer.parseInt(partes[1]);

            // Parse jogadores
            String[] jogsStr = partes[2].split("\\|");
            app.jogadores.clear();
            for (String js : jogsStr) {
              String[] c = js.split(":");
              if (c.length >= 6) {
                app.jogadores.put(c[0], new float[]{
                  Float.parseFloat(c[1]),
                  Float.parseFloat(c[2]),
                  Float.parseFloat(c[3]),
                  Float.parseFloat(c[4]),
                  Float.parseFloat(c[5])
                });
              }
            }

            // Parse objetos (parte 4, pode ser "none")
            app.objetos.clear();
            if (partes.length >= 4 && !partes[3].equals("none")) {
              for (String os : partes[3].split("\\|")) {
                String[] c = os.split(":");
                if (c.length >= 5) {
                  int id    = Integer.parseInt(c[0]);
                  float ox  = Float.parseFloat(c[1]);
                  float oy  = Float.parseFloat(c[2]);
                  float om  = Float.parseFloat(c[3]);
                  float ot  = c[4].equals("v") ? 1.0f : 0.0f;
                  app.objetos.put(id, new float[]{id, ox, oy, om, ot});
                }
              }
            }
          } catch (Exception e) {
            System.out.println("Erro a parsear STATE: " + e.getMessage());
          }
        }
        break;

      case "GAME_OVER":
        app.mensagemFimJogo = partes.length >= 2 ? partes[1] : "Fim de jogo!";
        app.estadoAtual = 3;
        break;

      default:
        break;
    }
  }
}
