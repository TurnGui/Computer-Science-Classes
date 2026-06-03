// ════════════════════════════════════════════════════════════════
//  ECRÃ 0 — LOGIN / REGISTO
// ════════════════════════════════════════════════════════════════
void desenharMenu() {
  background(40, 45, 60);
  fill(255); textSize(40); textAlign(CENTER, CENTER);
  text("JOGO CONCORRENTE", width/2f, 90);

  fill(255); textSize(16); textAlign(CENTER, CENTER);
  text("Username:", width/2f, 170);
  if (campoAtivo==1) stroke(100,200,100); else noStroke();
  strokeWeight(2); fill(255); rect(width/2f-125, 188, 250, 40, 5); noStroke();
  fill(0); textAlign(LEFT, CENTER);
  text(username+(campoAtivo==1&&frameCount%60<30?"|":""), width/2f-115, 208);

  textAlign(CENTER,CENTER); fill(255); text("Password:", width/2f, 255);
  if (campoAtivo==2) stroke(100,200,100); else noStroke();
  strokeWeight(2); fill(255); rect(width/2f-125, 273, 250, 40, 5); noStroke();
  fill(0); textAlign(LEFT, CENTER);
  text("*".repeat(password.length())+(campoAtivo==2&&frameCount%60<30?"|":""), width/2f-115, 293);

  noStroke();
  // Botões LOGIN + REGISTAR
  float yB=355, lB=115;
  float xL=width/2f-130;
  if (mouseX>xL&&mouseX<xL+lB&&mouseY>yB&&mouseY<yB+44) fill(100,200,100); else fill(50,150,50);
  rect(xL,yB,lB,44,10); fill(255); textAlign(CENTER,CENTER); textSize(15); text("LOGIN",xL+lB/2,yB+22);

  float xR=width/2f+10;
  if (mouseX>xR&&mouseX<xR+lB&&mouseY>yB&&mouseY<yB+44) fill(100,150,255); else fill(50,80,200);
  rect(xR,yB,lB,44,10); fill(255); text("REGISTAR",xR+lB/2,yB+22);

  // Botão CANCELAR REGISTO (pequeno, abaixo)
  float xU=width/2f-100; float yU=yB+60; float lU=200;
  if (mouseX>xU&&mouseX<xU+lU&&mouseY>yU&&mouseY<yU+34) fill(180,60,60); else fill(120,40,40);
  rect(xU,yU,lU,34,8); fill(255); textSize(13); text("Cancelar Registo",xU+lU/2,yU+17);

  if (!mensagemErro.isEmpty())    { fill(255,100,100); textSize(14); textAlign(CENTER,CENTER); text(mensagemErro,    width/2f, 460); }
  if (!mensagemSucesso.isEmpty()) { fill(100,220,100); textSize(14); textAlign(CENTER,CENTER); text(mensagemSucesso, width/2f, 460); }
}

// ════════════════════════════════════════════════════════════════
//  ECRÃ 1 — LOBBY
// ════════════════════════════════════════════════════════════════
void desenharLobby() {
  background(30,35,50);
  fill(255); textSize(30); textAlign(CENTER,CENTER); text("JOGO CONCORRENTE", width/2f, 50);
  fill(160); textSize(15); text("Bem-vindo, "+username+"!", width/2f, 85);

  // painel top scores
  int pX=40, pY=115, pW=340, pH=390;
  fill(40,45,62); noStroke(); rect(pX,pY,pW,pH,10);
  fill(255,200,50); textSize(17); textAlign(CENTER,CENTER); text("Top Pontuações", pX+pW/2f, pY+28);
  stroke(70); strokeWeight(1); line(pX+15,pY+50,pX+pW-15,pY+50); noStroke();

  if (topScores.isEmpty()) {
    fill(120); textSize(13); textAlign(CENTER,CENTER); text("Ainda não há pontuações.", pX+pW/2f, pY+pH/2f);
  } else {
    textAlign(LEFT,CENTER);
    for (int i=0; i<min(topScores.size(),10); i++) {
      String[] e = topScores.get(i);
      int rY = pY+65+i*30;
      if      (i==0) fill(255,215,0);
      else if (i==1) fill(192,192,192);
      else if (i==2) fill(205,127,50);
      else           fill(190,190,190);
      textSize(14); text((i+1)+".  "+e[0], pX+20, rY);
      textAlign(RIGHT,CENTER); text(e[1]+" pts", pX+pW-20, rY);
      textAlign(LEFT,CENTER);
    }
  }

  // painel estado
  int dX=width-40-340, dY=115, dW=340, dH=390;
  fill(40,45,62); noStroke(); rect(dX,dY,dW,dH,10);
  fill(255); textSize(17); textAlign(CENTER,CENTER); text("Estado", dX+dW/2f, dY+28);
  stroke(70); strokeWeight(1); line(dX+15,dY+50,dX+dW-15,dY+50); noStroke();

  if (naFila) {
    pushMatrix(); translate(dX+dW/2f, dY+150);
    noFill(); stroke(100,180,255); strokeWeight(4);
    float a=(frameCount*5)%360;
    arc(0,0,50,50,radians(a),radians(a+270)); noStroke(); popMatrix();
    fill(100,180,255); textSize(14); textAlign(CENTER,CENTER);
    text("À espera de jogadores...", dX+dW/2f, dY+210);
    fill(160); textSize(12); text("Mínimo 3, máximo 4", dX+dW/2f, dY+238);

    float bX=dX+(dW-180)/2f, bY=dY+dH-65;
    if (mouseX>bX&&mouseX<bX+180&&mouseY>bY&&mouseY<bY+40) fill(200,60,60); else fill(160,40,40);
    rect(bX,bY,180,40,8); fill(255); textSize(14); textAlign(CENTER,CENTER); text("Sair da Fila",bX+90,bY+20);
  } else {
    fill(180); textSize(13); textAlign(CENTER,CENTER); text("Clica em Jogar para\nentrar na fila.", dX+dW/2f, dY+150);
    float bX=dX+(dW-180)/2f, bY=dY+dH-65;
    if (mouseX>bX&&mouseX<bX+180&&mouseY>bY&&mouseY<bY+40) fill(80,200,80); else fill(50,160,50);
    rect(bX,bY,180,40,8); fill(255); textSize(14); textAlign(CENTER,CENTER); text("Jogar!",bX+90,bY+20);
  }
}

// ════════════════════════════════════════════════════════════════
//  ECRÃ 2 — JOGO
// ════════════════════════════════════════════════════════════════
void desenharJogo() {
  background(20);

  // bordas do mundo
  noFill(); stroke(80); strokeWeight(2);
  rect(0, 0, WORLD_W, WORLD_H);
  noStroke();

  // desenha objetos
  for (float[] obj : objetos.values()) {
    float ox    = obj[1];
    float oy    = obj[2];
    float om    = obj[3];
    boolean ven = obj[4] == 1.0f;
    float raioO = raioFromMassa(om);

    if (ven) fill(220, 50, 50);    // vermelho = venenoso
    else     fill(50, 200, 80);    // verde = comestível
    noStroke();
    ellipse(ox, oy, raioO*2, raioO*2);
  }

  // desenha cada jogador
  for (String user : jogadores.keySet()) {
    float[] d = jogadores.get(user);
    if (d == null) continue;
    float x     = d[0];
    float y     = d[1];
    float massa = d[2];
    float ang   = d[3];
    int   score = (int) d[4];
    float raio  = raioFromMassa(massa);
    boolean souEu = user.equals(username);

    // corpo preto
    fill(0); noStroke();
    ellipse(x, y, raio*2, raio*2);

    // borda: azul para o próprio, vermelha para os outros
    noFill();
    stroke(souEu ? color(52,152,219) : color(231,76,60));
    strokeWeight(3);
    ellipse(x, y, raio*2, raio*2);
    noStroke();

    // seta de direção
    float ax = x + cos(ang)*(raio+10);
    float ay = y + sin(ang)*(raio+10);
    stroke(souEu ? color(52,152,219) : color(231,76,60));
    strokeWeight(2);
    line(x, y, ax, ay);

    // ponta da seta
    float ts = 6;
    float pa = ang + HALF_PI;
    fill(souEu ? color(52,152,219) : color(231,76,60)); noStroke();
    triangle(
      ax, ay,
      ax - cos(ang)*ts + cos(pa)*(ts/2), ay - sin(ang)*ts + sin(pa)*(ts/2),
      ax - cos(ang)*ts - cos(pa)*(ts/2), ay - sin(ang)*ts - sin(pa)*(ts/2)
    );

    // nome + score
    noStroke(); fill(souEu ? color(52,152,219) : color(255));
    textAlign(CENTER, BOTTOM); textSize(12);
    text(user+" ["+score+"]", x, y-raio-5);
  }

  // HUD
  desenharHUD();
}

void desenharHUD() {
  fill(0,0,0,160); noStroke(); rect(0,0,width,40);

  int t = tempoRestante;
  String tempo = String.format("%02d:%02d", t/60, t%60);
  fill(t<=30 ? color(231,76,60) : color(255));
  textAlign(CENTER,CENTER); textSize(18);
  text(tempo, width/2f, 20);

  float[] eu = jogadores.get(username);
  if (eu != null) {
    textAlign(LEFT,CENTER); textSize(13); fill(52,152,219);
    text("Score: "+(int)eu[4]+"   Massa: "+(int)eu[2], 15, 20);
  }

  // legenda objetos
  noStroke(); fill(50,200,80); ellipse(width-130, 22, 10, 10);
  fill(180); textAlign(LEFT,CENTER); textSize(11); text("comestível", width-122, 22);
  fill(220,50,50); ellipse(width-50, 22, 10, 10);
  fill(180); text("venenoso", width-42, 22); // ficará fora se janela pequena, ok

  textAlign(RIGHT,BOTTOM); textSize(11); fill(120);
  text("← → girar   ↑ avançar", width-10, height-10);
}

// ════════════════════════════════════════════════════════════════
//  ECRÃ 3 — FIM DE JOGO
// ════════════════════════════════════════════════════════════════
void desenharFimJogo() {
  background(20);

  // mostra scores finais
  fill(255); textAlign(CENTER,CENTER); textSize(30);
  text("Fim de Jogo!", width/2f, height/2f-80);
  fill(200); textSize(18);
  text(mensagemFimJogo, width/2f, height/2f-30);

  // tabela de scores finais
  int i = 0;
  for (String user : jogadores.keySet()) {
    float[] d = jogadores.get(user);
    if (d == null) continue;
    boolean souEu = user.equals(username);
    fill(souEu ? color(52,152,219) : color(200));
    textSize(15); textAlign(CENTER,CENTER);
    text(user + " — " + (int)d[4] + " capturas", width/2f, height/2f + 20 + i*28);
    i++;
  }

  float bX=width/2f-100, bY=height/2f+130;
  if (mouseX>bX&&mouseX<bX+200&&mouseY>bY&&mouseY<bY+45) fill(80,130,220); else fill(50,90,180);
  rect(bX,bY,200,45,8);
  fill(255); textSize(15); textAlign(CENTER,CENTER);
  text("Voltar ao Lobby", bX+100, bY+22);
}
