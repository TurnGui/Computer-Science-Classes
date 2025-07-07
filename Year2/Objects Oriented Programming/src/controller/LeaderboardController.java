package controller;

import module.Musica;
import module.SpotifUM;
import module.user.UserListings;
import module.user.Utilizador;
import view.LeaderboardView;

import java.util.*;

public class LeaderboardController {

    public static void menu() {
        while (true) {
            int op = LeaderboardView.menuLeaderboard();
            switch (op) {
                case 1: musicaMaisReproduzida(); break;
                case 2: interpreteMaisEscutado(); break;
                case 3: generoMaisReproduzido(); break;
                case 4: utilizadorMaisAtivo(); break;
                case 0: return;
            }
        }
    }

    private static void musicaMaisReproduzida() {
        List<Musica> musicas = SpotifUM.getMusicas();
        Musica top = null;
        int max = -1;
        for (Musica m : musicas) {
            if (m.getNumReproducoes() > max) {
                max = m.getNumReproducoes();
                top = m;
            }
        }
        LeaderboardView.showMusicaMaisReproduzida(top);
    }

    private static void interpreteMaisEscutado() {
        Map<String, Integer> contagem = new HashMap<>();
        for (Musica m : SpotifUM.getMusicas()) {
            contagem.put(m.getInterprete(),
                contagem.getOrDefault(m.getInterprete(), 0) + m.getNumReproducoes());
        }

        String top = null;
        int max = -1;
        for (Map.Entry<String, Integer> e : contagem.entrySet()) {
            if (e.getValue() > max) {
                max = e.getValue();
                top = e.getKey();
            }
        }
        LeaderboardView.showInterpreteMaisEscutado(top, max);
    }

    private static void generoMaisReproduzido() {
        Map<String, Integer> contagem = new HashMap<>();
        for (Musica m : SpotifUM.getMusicas()) {
            contagem.put(m.getGenero(),
                contagem.getOrDefault(m.getGenero(), 0) + m.getNumReproducoes());
        }

        String top = null;
        int max = -1;
        for (Map.Entry<String, Integer> e : contagem.entrySet()) {
            if (e.getValue() > max) {
                max = e.getValue();
                top = e.getKey();
            }
        }
        LeaderboardView.showGeneroMaisReproduzido(top, max);
    }

    private static void utilizadorMaisAtivo() {
        Utilizador top = null;
        int max = -1;
        for (Utilizador u : UserListings.getInstance().getUtilizador().values()) {
            if (u.getMusicasOuvidas() > max) {
                max = u.getMusicasOuvidas();
                top = u;
            }
        }
        LeaderboardView.showUtilizadorMaisAtivo(top);
    }
}
