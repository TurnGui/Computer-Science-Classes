// ── Thread de rede (Guião 1 — extends Thread) ────────────────────
// Esta thread corre em paralelo com o draw() do Processing.
// Ambas acedem ao mesmo estado (jogadores, estadoAtual, etc.)
// pelo que toda a escrita é protegida pelo ReentrantLock (Guião 3)
class ConnectionServer extends Thread {
  Socket socket;
  ClienteJogo app;

  public ConnectionServer(Socket socket, ClienteJogo app) {
    this.socket = socket; //a ligação TCP ao servidor Erlang
    this.app    = app; //referência ao ClienteJogo principal
  }

  public void run() { //método que corre quando se chama ouvinte.start()
    try {
      BufferedReader in = new BufferedReader(
        new InputStreamReader(socket.getInputStream()) //canal de leitura do socket TCP
      );
      String mensagem;
      while ((mensagem = in.readLine()) != null) { //bloqueia até chegar uma linha completa terminada em \n
        System.out.println("Servidor: " + mensagem);
        processarMensagem(mensagem.trim()); //remove espaços e \n residuais
      }
    } catch (Exception e) { //Quando o servidor fecha a ligação, devolve null e o while termina
      System.out.println("Conexao fechada: " + e.getMessage());
      // Adquire o lock para escrever na variável partilhada
      app.lock.lock();
      try { app.mensagemErro = "Ligacao perdida com o servidor."; }
      finally { app.lock.unlock(); }
    }
  }

  // ── Parser de mensagens ───────────────────────────────────────
  // Toda a escrita no estado partilhado é feita dentro do lock
  // para garantir exclusão mútua com o draw() — Guião 2 e 3
  void processarMensagem(String msg) {
    if (msg.isEmpty()) return;
    String[] partes = msg.split("#", -1); //Divide a mensagem pelo # e adquire o lock antes de qualquer escrita
    String tipo = partes[0];

    // Adquire o lock antes de qualquer escrita no estado partilhado
    app.lock.lock();
    try {
      switch (tipo) {
        
        //Muda o estadoAtual conforme necessário e sinaliza com signalAll()
        case "OK": //login, registo, unregister
          if (partes.length >= 2) {
            if (partes[1].equals("LOGIN")) {
              app.mensagemSucesso = "Login bem-sucedido!";
              app.estadoAtual     = 1;
              // Sinaliza o draw() que o estado mudou (Guião 5 — Condition)
              app.estadoMudou.signalAll();
            } else if (partes[1].equals("REGISTER")) {
              app.mensagemSucesso = "Registo feito! Podes fazer login.";
            } else if (partes[1].equals("UNREGISTER")) {
              app.mensagemSucesso = "Registo cancelado.";
              app.username = "";
              app.password = "";
              app.estadoAtual = 0;
              app.estadoMudou.signalAll();
            }
          }
          break;

        case "ERR":
          app.mensagemErro = partes.length >= 2 ? partes[1] : "Erro desconhecido.";
          break;

        case "LOBBY":
          // LOBBY#OK — confirmação de entrada na fila
          break;
          //top scores
        case "TOP":
          // TOP#user1:pts|user2:pts|...
          // Escrita na lista partilhada protegida pelo lock
          if (partes.length >= 2 && !partes[1].equals("vazio")) {
            app.topScores.clear();
            for (String entrada : partes[1].split("\\|")) {
              String[] kv = entrada.split(":");
              if (kv.length == 2)
                app.topScores.add(new String[]{kv[0], kv[1]});
            }
          } else {
            app.topScores.clear();
          }
          break;

        case "GAME_START":
          // Limpa o estado anterior e muda para ecrã de jogo
          app.jogadores.clear();
          app.objetos.clear();
          app.tempoRestante = 120;
          app.naFila        = false;
          app.estadoAtual   = 2;
          app.estadoMudou.signalAll();
          break;

        case "STATE":
          // STATE#tempo#players#objects
          // Atualização do estado do jogo a cada tick do servidor
          if (partes.length >= 3) {
            try {
              app.tempoRestante = Integer.parseInt(partes[1]);

              // Parse jogadores
              app.jogadores.clear();
              for (String js : partes[2].split("\\|")) {
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

              // Parse objetos
              app.objetos.clear();
              if (partes.length >= 4 && !partes[3].equals("none")) {
                for (String os : partes[3].split("\\|")) {
                  String[] c = os.split(":");
                  if (c.length >= 5) {
                    int   id = Integer.parseInt(c[0]);
                    float ot = c[4].equals("v") ? 1.0f : 0.0f;
                    app.objetos.put(id, new float[]{
                      id,
                      Float.parseFloat(c[1]),
                      Float.parseFloat(c[2]),
                      Float.parseFloat(c[3]),
                      ot
                    });
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
          app.estadoAtual     = 3;
          app.estadoMudou.signalAll();
          break;

        default:
          break;
      }
    } finally {
      // finally garante libertação do lock mesmo com exceções — Guião 3
      app.lock.unlock();
    }
  }
}
