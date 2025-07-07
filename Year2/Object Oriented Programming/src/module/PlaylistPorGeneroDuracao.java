package module;

import java.io.Serializable;

public class PlaylistPorGeneroDuracao extends Playlist implements Serializable {
    public PlaylistPorGeneroDuracao(String nome) {
        super(nome);
    }

    
    public static PlaylistPorGeneroDuracao criarAuto(String nome, 
                                                     Iterable<Musica> todas, 
                                                     String genero, 
                                                     int duracaoMax) {
        PlaylistPorGeneroDuracao p = new PlaylistPorGeneroDuracao(nome);
        int duracaoAtual = 0;
        for (Musica m : todas) {
            if (m.getGenero().equalsIgnoreCase(genero) &&
                duracaoAtual + m.getDuracao() <= duracaoMax) {
                p.adicionarMusica(m);
                duracaoAtual += m.getDuracao();
            }
        }
        return p;
    }

    @Override
    public void reproduzir() {
        for (Musica m : musicas) {
            m.reproduzir();
        }
    }

    @Override
    public Playlist clone() {
        PlaylistPorGeneroDuracao copia = new PlaylistPorGeneroDuracao(this.nome);
        for (Musica m : this.musicas) {
            copia.adicionarMusica(m.clone());
        }
        return copia;
    }
}
