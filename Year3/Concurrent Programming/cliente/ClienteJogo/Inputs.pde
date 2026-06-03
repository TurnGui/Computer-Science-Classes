void mousePressed() {

  // ── LOGIN ────────────────────────────────────────────────────
  if (estadoAtual == 0) {
    if (mouseX > width/2f-125 && mouseX < width/2f+125 && mouseY > 188 && mouseY < 228)
      campoAtivo = 1;
    else if (mouseX > width/2f-125 && mouseX < width/2f+125 && mouseY > 273 && mouseY < 313)
      campoAtivo = 2;
    else
      campoAtivo = 0;

    float yB = 355; float lB = 115;
    // LOGIN
    if (mouseX > width/2f-130 && mouseX < width/2f-130+lB && mouseY > yB && mouseY < yB+44)
      enviarDados("LOGIN");
    // REGISTAR
    if (mouseX > width/2f+10 && mouseX < width/2f+10+lB && mouseY > yB && mouseY < yB+44)
      enviarDados("REGISTER");
    // CANCELAR REGISTO
    float xU=width/2f-100; float yU=yB+60; float lU=200;
    if (mouseX > xU && mouseX < xU+lU && mouseY > yU && mouseY < yU+34)
      enviarDados("UNREGISTER");
  }

  // ── LOBBY ────────────────────────────────────────────────────
  if (estadoAtual == 1) {
    int dX = width-40-340; int dY = 115;
    int dW = 340;          int dH = 390;
    float bX = dX + (dW-180)/2f;
    float bY = dY + dH - 65;
    if (mouseX > bX && mouseX < bX+180 && mouseY > bY && mouseY < bY+40) {
      if (!naFila) entrarNaFila();
      else         sairDaFila();
    }
  }

  // ── FIM DE JOGO ──────────────────────────────────────────────
  if (estadoAtual == 3) {
    float bX = width/2f - 100;
    float bY = height/2f + 130;
    if (mouseX > bX && mouseX < bX+200 && mouseY > bY && mouseY < bY+45) {
      naFila      = false;
      keyLeft     = false;
      keyRight    = false;
      keyForward  = false;
      estadoAtual = 1;
    }
  }
}

void keyPressed() {
  // ── LOGIN — escrita nos campos ────────────────────────────────
  if (estadoAtual == 0) {
    mensagemErro    = "";
    mensagemSucesso = "";

    if (campoAtivo == 1) {
      if      (keyCode == BACKSPACE && username.length() > 0)
        username = username.substring(0, username.length()-1);
      else if (keyCode == TAB)   campoAtivo = 2;
      else if (keyCode == ENTER || keyCode == RETURN) enviarDados("LOGIN");
      else if (key >= ' ' && key <= '~') username += key;

    } else if (campoAtivo == 2) {
      if      (keyCode == BACKSPACE && password.length() > 0)
        password = password.substring(0, password.length()-1);
      else if (keyCode == TAB)   campoAtivo = 1;
      else if (keyCode == ENTER || keyCode == RETURN) enviarDados("LOGIN");
      else if (key >= ' ' && key <= '~') password += key;
    }
  }

  // ── JOGO — teclas de movimento ────────────────────────────────
  if (estadoAtual == 2) {
    if (keyCode == LEFT  && !keyLeft)    { keyLeft    = true; enviarComando("key_press#" + username + "#LEFT"); }
    if (keyCode == RIGHT && !keyRight)   { keyRight   = true; enviarComando("key_press#" + username + "#RIGHT"); }
    if (keyCode == UP    && !keyForward) { keyForward = true; enviarComando("key_press#" + username + "#FORWARD"); }
  }
}

void keyReleased() {
  if (estadoAtual == 2) {
    if (keyCode == LEFT)  { keyLeft    = false; enviarComando("key_release#" + username + "#LEFT"); }
    if (keyCode == RIGHT) { keyRight   = false; enviarComando("key_release#" + username + "#RIGHT"); }
    if (keyCode == UP)    { keyForward = false; enviarComando("key_release#" + username + "#FORWARD"); }
  }
}
