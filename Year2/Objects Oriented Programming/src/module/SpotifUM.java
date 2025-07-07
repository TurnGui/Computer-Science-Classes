package module;

import java.util.ArrayList;
import java.util.List;

import module.Musica;

import module.user.Utilizador;

public class SpotifUM {
    private static List<Musica> musicas = new ArrayList<>();
    private static List<Utilizador> utilizadores = new ArrayList<>();

    
    public static void adicionarMusica(Musica musica) {
        musicas.add(musica);
    }

    
    public static List<Musica> getMusicas() {
        return musicas;
    }

    public static void setMusicas(List<Musica> lista) {
        musicas = lista;
    }

    
    public static void adicionarUtilizador(Utilizador utilizador) {
        utilizadores.add(utilizador);
    }

   
    public static List<Utilizador> getUtilizadores() {
        return utilizadores;
    }

    
    public static Musica getMusicaPorNome(String nome) {
        for (Musica m : musicas) {
            if (m.getNome().equalsIgnoreCase(nome)) {
                return m;
            }
        }
        return null;
    }
}
