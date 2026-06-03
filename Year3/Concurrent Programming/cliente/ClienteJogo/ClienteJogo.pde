import java.net.Socket;
import java.io.PrintWriter;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;

// ── Configuração ─────────────────────────────────────────────────
final String SERVER_HOST = "127.0.0.1";
final int    SERVER_PORT = 8080;
final int    WORLD_W     = 800;
final int    WORLD_H     = 600;

// ── Lock principal (Guião 3 — ReentrantLock) ─────────────────────
// Protege todo o estado partilhado entre a thread de rede
// (ConnectionServer) e a thread principal do Processing (draw).
// Sem este lock, ambas as threads podiam aceder ao mesmo estado
// ao mesmo tempo — race condition idêntica ao Counter do Guião 1.
final ReentrantLock lock = new ReentrantLock();

// Condition para o draw() esperar que o estado mude (Guião 5)
// Ex: draw() pode fazer await() até o jogo começar
final Condition estadoMudou = lock.newCondition();

// ── Estado global ─────────────────────────────────────────────────
// 0=Login  1=Lobby  2=Jogo  3=FimJogo
// ACESSO: sempre dentro de lock.lock() / lock.unlock()
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
// Usamos HashMap normal protegido pelo ReentrantLock
// (em vez de ConcurrentHashMap — o lock já garante a exclusão mútua)
HashMap<String, float[]> jogadores = new HashMap<String, float[]>();
HashMap<Integer, float[]> objetos   = new HashMap<Integer, float[]>();
int tempoRestante = 120;

// ── Teclas premidas ───────────────────────────────────────────────
// Só usadas pela thread principal — não precisam de lock
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
  // Adquire o lock antes de ler qualquer estado partilhado
  // Garante que o draw() nunca lê dados a meio de uma atualização
  // feita pela thread de rede — exclusão mútua (Guião 2 e 3)
  lock.lock();
  try {
    switch (estadoAtual) {
      case 0: desenharMenu();    break;
      case 1: desenharLobby();   break;
      case 2: desenharJogo();    break;
      case 3: desenharFimJogo(); break;
    }
  } finally {
    // finally garante que o lock é sempre libertado,
    // mesmo que ocorra uma exceção — padrão do Guião 3
    lock.unlock();
  }
}

// ── Enviar login/registo ──────────────────────────────────────────
void enviarDados(String tipo) {
  // Estas variáveis são só da thread principal aqui,
  // mas usamos o lock para consistência
  lock.lock();
  try {
    mensagemErro    = "";
    mensagemSucesso = "";

    if (username.isEmpty() || password.length() < 3) {
      mensagemErro = "Preenche os campos! (password min. 3 caracteres)";
      return;
    }
  } finally {
    lock.unlock();
  }

  try {
    Socket socket = new Socket(SERVER_HOST, SERVER_PORT); //abre a ligação TCP ao servidor Erlang na porta 8080
    serverOut = new PrintWriter(socket.getOutputStream(), true); //cria o canal de escrita. O true ativa o auto-flush cada println é enviado imediatamente sem ficar em buffer

    // Constrói o comando no protocolo # que o servidor Erlang espera
    String comando;
    if      (tipo.equals("LOGIN"))      comando = "login#"      + username + "#" + password;
    else if (tipo.equals("UNREGISTER")) comando = "unregister#" + username + "#" + password;
    else                                 comando = "register#"   + username + "#" + password;

    serverOut.println(comando); // envia o primeiro comando

    // Cria e arranca a thread de rede (Guião 1 — extends Thread)
    ConnectionServer ouvinte = new ConnectionServer(socket, this);
    ouvinte.start(); // a partir daqui a thread de rede corre em paralelo com o draw()

  } catch (Exception e) { //Se o servidor estiver offline
    lock.lock();
    try { mensagemErro = "Erro: Servidor offline."; }
    finally { lock.unlock(); }
  }
}

// ── Enviar comando genérico ───────────────────────────────────────
void enviarComando(String comando) { // envia qualquer string ao servidor se a ligação existir
  if (serverOut != null) serverOut.println(comando); //usada para join_lobby, key_press, e key_release
  //Não precisa de lock porque o PrintWriter é thread-safe
}

// ── Entrar/sair da fila ───────────────────────────────────────────
void entrarNaFila() {
  lock.lock();
  try {
    naFila = true; //Marca localmente que o jogador está na fila
    mensagemErro = ""; // (O draw() usa naFila para mostrar o spinner de espera no lobby — por isso precisa do lock)
  } finally {
    lock.unlock();
  }
  enviarComando("join_lobby#" + username); //e envia o comando ao servidor
}

void sairDaFila() { //Apenas marca que o jogador saiu da fila. Não envia nada ao servidor
  lock.lock();
  try { naFila = false; }
  finally { lock.unlock(); }
}

// ── Raio a partir da massa ────────────────────────────────────────
float raioFromMassa(float massa) { //Duplica a fórmula do servidor Erlang
  return sqrt(massa / PI); // calcular o raio para desenhar os círculos com o tamanho correto
}
