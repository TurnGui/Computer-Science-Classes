package module;
import java.io.Serializable;
import java.util.*;

public class PlaylistAleatoria extends Playlist implements Serializable{
    public PlaylistAleatoria(String nome, List<Musica> todas, int tamanho) {
        super(nome);
        Collections.shuffle(todas);
        for (int i = 0; i < Math.min(tamanho, todas.size()); i++) {
            musicas.add(todas.get(i).clone());
        }
    }

    @Override
    public void reproduzir() {
        for (Musica m : musicas) {
            m.reproduzir();
        }
    }

    @Override
    public Playlist clone() {
        
        List<Musica> copiaMusicas = new ArrayList<>();
        for (Musica m : this.musicas) {
            copiaMusicas.add(m.clone());
        }
    
        PlaylistAleatoria clone = new PlaylistAleatoria(this.nome, copiaMusicas, copiaMusicas.size());
        return clone;
    }
    
}
