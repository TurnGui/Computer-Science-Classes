package module;
import java.io.Serializable;
import java.util.*;

public class PlaylistConstruida extends Playlist implements Serializable{
    private boolean aleatoria;
    private int posAtual;

    public PlaylistConstruida(String nome, boolean aleatoria) {
        super(nome);
        this.aleatoria = aleatoria;
        this.posAtual = 0;
    }

    @Override
    public void reproduzir() {
        List<Musica> ordem = new ArrayList<>(musicas);
        if (aleatoria) Collections.shuffle(ordem);

        for (Musica m : ordem) {
            m.reproduzir();
        }
    }

    public void avancar() {
        if (posAtual < musicas.size() - 1) posAtual++;
    }

    public void retroceder() {
        if (posAtual > 0) posAtual--;
    }

    public Musica getAtual() {
        return musicas.get(posAtual);
    }

    @Override
    public Playlist clone() {
        PlaylistConstruida nova = new PlaylistConstruida(this.nome, this.aleatoria);
        nova.musicas = new ArrayList<>();
        for (Musica m : this.musicas) nova.musicas.add(m.clone());
        return nova;
    }
}
