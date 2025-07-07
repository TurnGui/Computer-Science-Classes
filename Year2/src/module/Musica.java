package module;

import java.util.List;
import java.io.Serializable;

public class Musica implements Serializable {
    private static final long serialVersionUID = 1L;

    private String nome;
    private String interprete;
    private String editora;
    private String letra;
    private List<String> textomusica;
    private String genero;
    private int duracao;
    private boolean explicita;
    private boolean multimedia;
    private int contadorReproducao;
    private boolean favorita;

    public Musica(String nome, String interprete, String editora, String letra, List<String> textomusica,
                  String genero, int duracao, boolean explicita, boolean multimedia) {
        this.nome = nome;
        this.interprete = interprete;
        this.editora = editora;
        this.letra = letra;
        this.textomusica = textomusica;
        this.genero = genero;
        this.duracao = duracao;
        this.explicita = explicita;
        this.multimedia = multimedia;
        this.contadorReproducao = 0;
    }

    public Musica(Musica mu){
        this.nome = mu.getNome();
        this.interprete = mu.getInterprete();
        this.editora = mu.getEditora();
        this.letra = mu.getLetra();
        this.textomusica = mu.gettextomusica();
        this.genero = mu.getGenero();
        this.duracao = mu.getDuracao();
        this.explicita = mu.isExplicita();
        this.multimedia = mu.ismultimedia();
        this.contadorReproducao = mu.getContadorReproducao();
        this.favorita = mu.isFavorita();
    }

    public void reproduzir() {
        contadorReproducao++;
        System.out.println("Reproduzindo: " + nome + " por " + interprete);
        System.out.println("Letra: " + letra);
    }

    public int getContadorReproducao() {
        return this.contadorReproducao;
    }

    public int getNumReproducoes() {
        return contadorReproducao;
    }

    public String getNome() {
        return this.nome;
    }

    public void setNome(String nome) {
        this.nome = new String(nome);
    }

    public boolean isFavorita() {
        return favorita;
    }

    public void setFavorita(boolean favorita) {
        this.favorita = favorita;
    }

    public String getInterprete() {
        return this.interprete;
    }

    public void setInterprete(String interprete) {
        this.interprete = new String(interprete);
    }

    public String getEditora() {
        return this.editora;
    }

    public void setEditora(String editora) {
        this.editora = new String(editora);
    }

    public String getLetra() {
        return this.letra;
    }

    public void setLetra(String letra) {
        this.letra = new String(letra);
    }

    public List<String> gettextomusica() {
        return this.textomusica;
    }

    public void settextomusica(List<String> textomusica) {
        this.textomusica = textomusica;
    }

    public String getGenero() {
        return this.genero;
    }

    public void setGenero(String genero) {
        this.genero = new String(genero);
    }

    public int getDuracao() {
        return this.duracao;
    }

    public void setDuracao(int duracao) {
        this.duracao = duracao;
    }

    public boolean isExplicita() {
        return this.explicita;
    }

    public void setExplicita(boolean explicita) {
        this.explicita = explicita;
    }

    public boolean ismultimedia() {
        return this.multimedia;
    }

    public void setmultimedia(boolean multimedia) {
        this.multimedia = multimedia;
    }

    @Override
    public String toString(){
        StringBuffer sb = new StringBuffer();

        sb.append("\nMúsica: ").append(this.nome);
        sb.append("\nIntérprete: ").append(this.interprete);
        sb.append("\nEditora: ").append(this.editora);
        sb.append("\nGénero Musical: ").append(this.genero);
        sb.append("\nDuração: ").append(this.duracao);
        if (this.explicita) sb.append("\nMusica Explícita");
        if (this.multimedia) sb.append("\nMusica Multimédia");
        sb.append("\nNúmero de Reproduções: ").append(this.contadorReproducao);
        sb.append("\nLetra: ").append(this.letra);

        return sb.toString();
    }

    @Override
    public Musica clone(){
        return new Musica(this);
    }
}

