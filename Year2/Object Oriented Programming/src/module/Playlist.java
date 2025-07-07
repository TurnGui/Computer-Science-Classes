package module;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public abstract class Playlist implements Serializable {
    public String nome;
    public List<Musica> musicas;

    public Playlist(String nome) {
        this.nome = nome;
        this.musicas = new ArrayList<>();
    }

    public List<Musica> getMusicas() {
        return new ArrayList<>(musicas);
    }
    public void setMusicas(List<Musica> musicas) {
        this.musicas = new ArrayList<>(musicas);
    }

    public String getNome() {
        return this.nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public void adicionarMusica(Musica m) {
        musicas.add(m.clone());
    }

    public int getDuracaoTotal() {
        int total = 0;
        for (Musica m : musicas) {
            total += m.getDuracao();
        }
        return total;
    }

    public void reproduzir() {
        for (Musica m : musicas) {
            m.reproduzir();
        }
    }
    @Override
    public String toString() {
        StringBuffer sb = new StringBuffer();
        sb.append("\nPlaylist: "); sb.append(this.nome);
        for (Musica m : musicas) {
            sb.append(m.toString()).append("\n");
        }
        return sb.toString();
    }

    public abstract Playlist clone();
}
