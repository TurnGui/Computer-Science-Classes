import java.net.Socket;
import java.io.PrintWriter;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;

// ── Configuração ─────────────────────────────────────────────────
final String SERVER_HOST = "127.0.0.1";
final int    SERVER_PORT = 8080;
final int    WORLD_W     = 800;
final int    WORLD_H     = 600;

// ── Estado global ─────────────────────────────────────────────────
// 0=Login  1=Lobby  2=Jogo  3=FimJogo
int estadoAtual = 0;

// ── Dados do utilizador ───────────────────────────────────────────
String username        = "";
String password        = "";
int    campoAtivo      = 0;
String mensagemErro    = "";
String mensagemSucesso = "";
String mensagemFimJogo = "";

// ── Lobby ─────────────────────────────────────────────────────────
boolean naFila = false;
ArrayList<String[]> topScores = new ArrayList<String[]>();

// ── Estado do jogo ────────────────────────────────────────────────
// Jogador: float[] {x, y, massa, angulo, score}
ConcurrentHashMap<String, float[]> jogadores = new ConcurrentHashMap<String, float[]>();
// Objetos: float[] {id, x, y, massa, tipo}  tipo: 0=comestivel 1=venenoso
ConcurrentHashMap<Integer, float[]> objetos = new ConcurrentHashMap<Integer, float[]>();
int tempoRestante = 120;

// ── Teclas premidas ───────────────────────────────────────────────
boolean keyLeft    = false;
boolean keyRight   = false;
boolean keyForward = false;

// ── Ligação ao servidor ───────────────────────────────────────────
PrintWriter serverOut = null;

// ─────────────────────────────────────────────────────────────────
void setup() {
  size(800, 600);
  surface.setTitle("Jogo Concorrente");
}

void draw() {
  switch (estadoAtual) {
    case 0: desenharMenu();      break;
    case 1: desenharLobby();     break;
    case 2: desenharJogo();      break;
    case 3: desenharFimJogo();   break;
  }
}

// ── Enviar login/registo ──────────────────────────────────────────
void enviarDados(String tipo) {
  mensagemErro    = "";
  mensagemSucesso = "";

  if (username.isEmpty() || password.length() < 3) {
    mensagemErro = "Preenche os campos! (password min. 3 caracteres)";
    return;
  }

  try {
    Socket socket = new Socket(SERVER_HOST, SERVER_PORT);
    serverOut = new PrintWriter(socket.getOutputStream(), true);

    String comando;
    if      (tipo.equals("LOGIN"))      comando = "login#"      + username + "#" + password;
    else if (tipo.equals("UNREGISTER")) comando = "unregister#" + username + "#" + password;
    else                                 comando = "register#"   + username + "#" + password;

    serverOut.println(comando);

    ConnectionServer ouvinte = new ConnectionServer(socket, this);
    ouvinte.start();

  } catch (Exception e) {
    mensagemErro = "Erro: Servidor offline.";
  }
}

// ── Enviar comando genérico ───────────────────────────────────────
void enviarComando(String comando) {
  if (serverOut != null) serverOut.println(comando);
}

// ── Entrar/sair da fila ───────────────────────────────────────────
void entrarNaFila() {
  naFila = true;
  mensagemErro = "";
  enviarComando("join_lobby#" + username);
}

void sairDaFila() {
  naFila = false;
}

// ── Raio a partir da massa ────────────────────────────────────────
float raioFromMassa(float massa) {
  return sqrt(massa / PI);
}
