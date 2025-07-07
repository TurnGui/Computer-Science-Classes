package view;

import module.Musica;
import module.user.Utilizador;

import java.util.Scanner;

public class LeaderboardView {
    private static Scanner sc = new Scanner(System.in);

    public static int menuLeaderboard() {
        System.out.println("\n--- Leaderboard ---");
        System.out.println("1. Música mais reproduzida");
        System.out.println("2. Intérprete mais escutado");
        System.out.println("3. Género mais reproduzido");
        System.out.println("4. Utilizador que mais músicas ouviu");
        System.out.println("0. Voltar");
        System.out.print("Escolha: ");
        return Integer.parseInt(sc.nextLine());
    }

    public static void showMusicaMaisReproduzida(Musica m) {
        if (m != null) {
            System.out.println(" Música mais reproduzida: " + m.getNome() + " (" + m.getNumReproducoes() + "x)");
        } else {
            System.out.println("Nenhuma música foi reproduzida ainda.");
        }
    }

    public static void showInterpreteMaisEscutado(String nome, int vezes) {
        if (nome != null) {
            System.out.println(" Intérprete mais escutado: " + nome + " (" + vezes + "x)");
        } else {
            System.out.println("Nenhum intérprete encontrado.");
        }
    }

    public static void showGeneroMaisReproduzido(String genero, int vezes) {
        if (genero != null) {
            System.out.println(" Género mais reproduzido: " + genero + " (" + vezes + "x)");
        } else {
            System.out.println("Nenhum género encontrado.");
        }
    }

    public static void showUtilizadorMaisAtivo(Utilizador u) {
        if (u != null) {
            System.out.println(" Utilizador mais ativo: " + u.getNome() + " (" + u.getMusicasOuvidas() + " músicas)");
        } else {
            System.out.println("Nenhum utilizador encontrado.");
        }
    }
}
