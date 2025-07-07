package view;

import java.text.Normalizer;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Random;
import java.util.Scanner;
import java.util.Set;
import java.util.stream.Collectors;

import module.Musica;
import module.Playlist;
import module.PlaylistAleatoria;
import module.PlaylistConstruida;
import module.PlaylistFavoritos;
import module.PlaylistPorGeneroDuracao;
import module.SpotifUM;
import module.user.UserListings;
import module.user.Utilizador;
import controller.RepMusicaController;
import controller.UserController;

public class PlaylistsView {
    public static void showPlaylists(Utilizador user) {
        view.Util.clearScreen();
        Util.printHeader();
        System.out.println("\n");

        List<Playlist> playlists = user.getPlaylists();

        if (playlists.isEmpty()) {
            System.out.println("Não existem playlists.");
            Util.waitInput();
            UserController.menu(user);
            return;
        }

        System.out.println("Playlists disponíveis:");
        for (int i = 0; i < playlists.size(); i++) {
            System.out.println((i + 1) + ". " + playlists.get(i).getNome());
        }

        int escolha = Util.inputInt("Escolhe uma playlist para ver as músicas (0 para voltar): ");

        if (escolha == 0) {
            UserController.menu(user);
            return;
        }

        if (escolha < 0 || escolha > playlists.size()) {
            System.out.println("Opção inválida.");
            Util.waitInput();
            UserController.menu(user);
            return;
        }

        Playlist selecionada = playlists.get(escolha - 1);
        List<Musica> musicas = selecionada.getMusicas();

        if (musicas.isEmpty()) {
            System.out.println("A playlist está vazia.");
            Util.waitInput();
            UserController.menu(user);
            return;
        }
        view.Util.clearScreen();
        Util.printHeader();
        System.out.println("\n");

        /*System.out.println("Músicas na playlist:");
        for (int i = 0; i < musicas.size(); i++) {
            System.out.println((i + 1) + ". " + musicas.get(i).getNome());
        }
         * 
         */
        

        if (selecionada instanceof PlaylistAleatoria) {
            showPlaylistAleatoria(user, (PlaylistAleatoria) selecionada);
        } else if (selecionada instanceof PlaylistConstruida) {
            showPlaylistConstruida(user, (PlaylistConstruida) selecionada);
        } else if (selecionada instanceof PlaylistPorGeneroDuracao) {
            showPlaylistGeneroDuracao(user, (PlaylistPorGeneroDuracao) selecionada);
        } else if (selecionada instanceof PlaylistFavoritos) {
            showPlaylistFavoritos(user, (PlaylistFavoritos) selecionada);
        } else {
            Util.waitInput();
            UserController.menu(user);
        }
    }

    private static void playRandomMusica(Utilizador user, List<Musica> musicas) {
        List<Musica> musicasRestantes = new ArrayList<>(musicas);
        Random rand = new Random();

        while (!musicasRestantes.isEmpty()) {
            int indexAleatorio = rand.nextInt(musicasRestantes.size());
            Musica musicaAtual = musicasRestantes.get(indexAleatorio);
            RepMusicaView.mostrarDetalhesMusica(musicaAtual, user.isPremium());
            musicasRestantes.remove(indexAleatorio);

            System.out.println("\n1. Reproduzir próxima música\n2. Voltar atrás");
            int opcao = Util.inputInt("Escolha uma opção: ");

            if (opcao != 1) {
                break;
            }
            RepMusicaController.atribuirPontosPorMusica(user);
        }

        UserController.menu(user);
    }

    public static void showPlaylistAleatoria(Utilizador user, PlaylistAleatoria playlist) {
        List<Musica> musicas = playlist.getMusicas();
        if (musicas.isEmpty()) {
            System.out.println("A playlist está vazia.");
            Util.waitInput();
            UserController.menu(user);
            return;
        }

        System.out.println("\nMúsicas na playlist Aleatoria:");
        for (int i = 0; i < musicas.size(); i++) {
            System.out.println((i + 1) + ". " + musicas.get(i).getNome());
        }

        System.out.println("\n1. Reproduzir a primeira música (aleatória)\n2. Voltar atrás");
        int opcao = Util.inputInt("Escolha uma opção: ");

        if (opcao == 1) {
            playRandomMusica(user, musicas);
        } else {
            UserController.menu(user);
        }
    }

    public static void showPlaylistConstruida(Utilizador user, PlaylistConstruida playlist) {
        List<Musica> musicas = playlist.getMusicas();
        if (musicas.isEmpty()) {
            System.out.println("A playlist está vazia.");
            Util.waitInput();
            UserController.menu(user);
            return;
        }

        System.out.println("\nMúsicas na playlist construída:");
        for (int i = 0; i < musicas.size(); i++) {
            System.out.println((i + 1) + ". " + musicas.get(i).getNome());
        }

        System.out.println("\n1. Reproduzir playlist\n2. Voltar atrás");
        int opcao = Util.inputInt("Escolha uma opção: ");

        if (opcao == 1) {
            playPlaylistSequencial(user, musicas);
        } else {
            UserController.menu(user);
        }
    }

    private static void playPlaylistSequencial(Utilizador user, List<Musica> musicas) {
        int index = 0;
        while (index < musicas.size()) {
            Musica musicaAtual = musicas.get(index);
            RepMusicaView.mostrarDetalhesMusica(musicaAtual, user.isPremium());

            System.out.println("\n1. Reproduzir próxima música\n2. Voltar atrás");
            int opcao = Util.inputInt("Escolha uma opção: ");

            if (opcao != 1) {
                break;
            }
            RepMusicaController.atribuirPontosPorMusica(user);

            index++;
        }

        UserController.menu(user);
    }

    public static void showPlaylistGeneroDuracao(Utilizador user, PlaylistPorGeneroDuracao playlist) {
        List<Musica> musicas = playlist.getMusicas();
        if (musicas.isEmpty()) {
            System.out.println("A playlist está vazia.");
            Util.waitInput();
            UserController.menu(user);
            return;
        }

        System.out.println("\nMúsicas na playlist por género e duração:");
        for (int i = 0; i < musicas.size(); i++) {
            System.out.println((i + 1) + ". " + musicas.get(i).getNome());
        }

        System.out.println("\n1. Reproduzir playlist (aleatório)\n2. Voltar atrás");
        int opcao = Util.inputInt("Escolha uma opção: ");

        if (opcao == 1) {
            playRandomMusica(user, musicas);
        } else {
            UserController.menu(user);
        }
    }

    public static void showPlaylistFavoritos(Utilizador user, PlaylistFavoritos playlist) {
        List<Musica> musicas = playlist.getMusicas();
        if (musicas.isEmpty()) {
            System.out.println("A playlist está vazia.");
            Util.waitInput();
            UserController.menu(user);
            return;
        }

        System.out.println("\nMúsicas na playlist de favoritos:");
        for (int i = 0; i < musicas.size(); i++) {
            System.out.println((i + 1) + ". " + musicas.get(i).getNome());
        }

        System.out.println("\n1. Reproduzir playlist\n2. Voltar atrás");
        int opcao = Util.inputInt("Escolha uma opção: ");

        if (opcao == 1) {
            playPlaylistSequencial(user, musicas); 
        } else {
            UserController.menu(user);
        }
    }

    public static int playlistsmenu() {
        view.Util.clearScreen();
        Util.printHeader();
        Util.println("\n Criação de Playlists:");
        Util.println("[1] Criar Playlist Construída (PREMIUM)");
        Util.println("[2] Criar Playlist Aleatória");
        Util.println("[3] Criar Playlist Por Género e Duração (PREMIUM)");
        Util.println("[4] Criar Playlist de Favoritos (PREMIUM)");
        Util.println("[5] Voltar atrás");

        return Util.inputOption(5);
    }

    public static void createPlaylistConstruida(Utilizador user) {
        if (!user.isPremium()) {
            UserView.notPremium(user);
            return;
        }

        Util.clearScreen();
        Util.printHeader();
        Util.println("\n=== Criar Playlist Construída ===");
        String nome = Util.input("Insere o nome da playlist: ");

        user.criarPlaylistConstruida(nome, false); // reprodução nunca aleatória aqui

        Playlist playlist = user.getPlaylistPorNome(nome);
        if (playlist == null) {
            System.out.println("Erro ao criar a playlist.");
            Util.waitInput();
            UserController.menu(user);
            return;
        }

        Set<String> nomesAdicionados = playlist.getMusicas().stream()
                .map(m -> removeSpecialChars(m.getNome()).toLowerCase())
                .collect(Collectors.toSet());

        List<Musica> todasMusicas = SpotifUM.getMusicas();

        while (true) {
            Util.clearScreen();
            Util.printHeader();
            Util.println("\nAdicionar músicas à playlist \"" + nome + "\" (escreve 0 para terminar):\n");

            todasMusicas.forEach(m -> Util.println("- " + m.getNome() + " por " + m.getInterprete()));

            String escolha = Util.input("\nNome da música a adicionar: ");
            if (escolha.equals("0")) {
                SerializationView.guardarUtilizadores("utilizadores.dat", UserListings.getInstance());
                Util.println("Playlist \"" + nome + "\" criada com sucesso!");
                Util.waitInput();
                UserController.menu(user);
                return;
            }

            Musica encontrada = todasMusicas.stream()
                    .filter(m -> removeSpecialChars(m.getNome()).equalsIgnoreCase(removeSpecialChars(escolha)))
                    .findFirst()
                    .orElse(null);

            if (encontrada == null) {
                Util.println("Música não encontrada.");
            } else if (nomesAdicionados.contains(removeSpecialChars(encontrada.getNome()).toLowerCase())) {
                Util.println("Essa música já está na playlist!");
            } else {
                playlist.adicionarMusica(encontrada);
                nomesAdicionados.add(removeSpecialChars(encontrada.getNome()).toLowerCase());
                Util.println("Música adicionada!");
            }

            Util.waitInput();
        }
    }

    private static String removeSpecialChars(String input) {
        if (input == null)
            return null;
        String normalized = Normalizer.normalize(input, Normalizer.Form.NFD);
        return normalized.replaceAll("\\p{InCombiningDiacriticalMarks}+", "").toLowerCase();
    }

    public static void createPlaylistAleatoria(Utilizador user) {
        Util.clearScreen();
        Util.printHeader();
        Util.println("\n=== Criar Playlist Aleatória ===");

        String nome = Util.input("Nome da playlist: ");

        PlaylistAleatoria nova = new PlaylistAleatoria(nome, new ArrayList<>(), 0);

        List<Musica> musicas = SpotifUM.getMusicas();
        Set<String> nomesAdicionados = new HashSet<>();

        while (true) {
            Util.clearScreen();
            Util.printHeader();
            Util.println("\nAdicionar músicas à playlist \"" + nome + "\" (digite 0 para terminar):\n");

            for (Musica m : musicas) {
                Util.println("- " + m.getNome() + " por " + m.getInterprete());
            }

            String escolha = Util.input("\nNome da música a adicionar: ");
            if (escolha.equals("0")) {
                user.adicionarPlaylist(nova);
                SerializationView.guardarUtilizadores("utilizadores.dat", UserListings.getInstance());

                Util.println("Playlist \"" + nome + "\" criada com sucesso!");
                Util.waitInput();
                UserController.menu(user);
                return;
            }

            String escolhaNormalizada = removeSpecialChars(escolha);

            Musica encontrada = musicas.stream()
                    .filter(m -> removeSpecialChars(m.getNome()).equals(escolhaNormalizada))
                    .findFirst()
                    .orElse(null);

            if (encontrada == null) {
                Util.println("Música não encontrada.");
            } else if (nomesAdicionados.contains(removeSpecialChars(encontrada.getNome()))) {
                Util.println("Essa música já está na playlist!");
            } else {
                nova.adicionarMusica(encontrada);
                nomesAdicionados.add(removeSpecialChars(encontrada.getNome()));
                Util.println("Música adicionada!");
            }

            Util.waitInput();
        }
    }

    public static void createPlaylistGeneroDuracao(Utilizador user) {
        if (!user.isPremium()) {
            UserView.notPremium(user);
            return;
        }

        Util.clearScreen();
        Util.printHeader();
        Util.println("\n=== Criar Playlist Por Género e Duração ===");

        String nome = Util.input("Insere o nome da playlist: ");
        String generoEscolhido = Util.input("Insere o género que queres ouvir: ").toLowerCase();
        int duracaoMax = Util.inputInt("Insere a duração máxima total da playlist (em segundos): ");

        List<Musica> musicasGenero = SpotifUM.getMusicas().stream()
                .filter(m -> m.getGenero().equalsIgnoreCase(generoEscolhido))
                .collect(Collectors.toList());

        if (musicasGenero.isEmpty()) {
            Util.println("Não existem músicas disponíveis para esse género.");
            Util.waitInput();
            UserController.menu(user);
            return;
        }

        Util.println("\nMúsicas disponíveis com género \"" + generoEscolhido + "\":");
        musicasGenero.forEach(m -> Util.println("- " + m.getNome() + " (" + m.getDuracao() + "s)"));

        //PlaylistPorGeneroDuracao playlist = PlaylistPorGeneroDuracao.criarAuto(nome, SpotifUM.getMusicas(), generoEscolhido, duracaoMax);
        PlaylistPorGeneroDuracao playlist = new PlaylistPorGeneroDuracao(nome);

        Set<String> nomesAdicionados = new HashSet<>();
        int duracaoAtual = 0;

        while (true) {
            String nomeMusica = Util.input("\nNome da música a adicionar (0 para terminar): ");
            if (nomeMusica.equals("0"))
                break;

            Musica selecionada = musicasGenero.stream()
                    .filter(m -> removeSpecialChars(m.getNome()).equalsIgnoreCase(removeSpecialChars(nomeMusica)))
                    .findFirst()
                    .orElse(null);

            if (selecionada == null) {
                Util.println("Música não encontrada no género escolhido.");
            } else if (nomesAdicionados.contains(removeSpecialChars(selecionada.getNome()))) {
                Util.println("Essa música já está na playlist!");
            } else if (duracaoAtual + selecionada.getDuracao() > duracaoMax) {
                Util.println("Já não pode adicionar esta música. Limite de duração excedido!");
            } else {
                playlist.adicionarMusica(selecionada);
                nomesAdicionados.add(removeSpecialChars(selecionada.getNome()));
                duracaoAtual += selecionada.getDuracao();
                Util.println("Música adicionada! Duração atual: " + duracaoAtual + "s");
            }

            Util.waitInput();
        }

        if (playlist.getMusicas().isEmpty()) {
            Util.println("A playlist não tem músicas. Não foi criada.");
        } else {
            user.adicionarPlaylist(playlist);
            SerializationView.guardarUtilizadores("utilizadores.dat", UserListings.getInstance());
            Util.println("Playlist \"" + nome + "\" criada com sucesso!");
        }

        Util.waitInput();
        UserController.menu(user);
    }

    public static void createPlaylistFavoritos(Utilizador user) {
        if (!user.isPremium()) {
            UserView.notPremium(user);
            return;
        }

        Util.clearScreen();
        Util.printHeader();
        Util.println("\n=== Criar Playlist Favoritos ===");

        String nome = Util.input("Insere o nome da playlist: ");
        boolean aleatoria = Util.input("Reprodução aleatória? (s/n): ").equalsIgnoreCase("s");

        user.criarPlaylistConstruida(nome, aleatoria);

        Playlist playlist = user.getPlaylistPorNome(nome);
        if (playlist == null) {
            System.out.println("Erro ao criar a playlist.");
            Util.waitInput();
            UserController.menu(user);
            return;
        }

        Set<String> nomesAdicionados = new HashSet<>();
        for (Musica m : playlist.getMusicas()) {
            nomesAdicionados.add(removeSpecialChars(m.getNome()));
        }

        while (true) {
            Util.clearScreen();
            Util.printHeader();
            Util.println("\nAdicionar músicas à playlist \"" + nome + "\" (escreve 0 para terminar):\n");

            for (Musica m : SpotifUM.getMusicas()) {
                Util.println("- " + m.getNome() + " por " + m.getInterprete());
            }

            String escolha = Util.input("\nNome da música a adicionar: ");
            if (escolha.equals("0")) {
                SerializationView.guardarUtilizadores("utilizadores.dat", UserListings.getInstance());
                Util.println("Playlist \"" + nome + "\" criada com sucesso!");
                Util.waitInput();
                UserController.menu(user);
                return;
            }

            Musica encontrada = SpotifUM.getMusicas().stream()
                    .filter(m -> removeSpecialChars(m.getNome()).equalsIgnoreCase(removeSpecialChars(escolha)))
                    .findFirst()
                    .orElse(null);

            if (encontrada == null) {
                Util.println("Música não encontrada.");
            } else if (nomesAdicionados.contains(removeSpecialChars(encontrada.getNome()))) {
                Util.println("Essa música já está na playlist!");
            } else {
                playlist.adicionarMusica(encontrada);
                nomesAdicionados.add(removeSpecialChars(encontrada.getNome()));
                Util.println("Música adicionada!");
            }

            Util.waitInput();
        }
    }


    public static void showPlaylistExplicitas(Utilizador user) {
        if (!user.isPremiumtop()) {
            UserView.notPremiumtop(user);
            return;
        }
        Util.clearScreen();
        Util.printHeader();

        List<Musica> todas = SpotifUM.getMusicas();
        List<Musica> explicitas = todas.stream()
                .filter(Musica::isExplicita)
                .collect(Collectors.toList());

        if (explicitas.isEmpty()) {
            System.out.println("Não existem músicas explícitas no sistema.");
            return;
        }

        System.out.println("\n--- Playlist Automática: Músicas Explícitas ---");
        for (int i = 0; i < explicitas.size(); i++) {
            System.out.println((i + 1) + ". " + explicitas.get(i).getNome() + " - " + explicitas.get(i).getInterprete());
        }

        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("\nDigite o número da música para reproduzir ou 0 para voltar ao menu:");
            String input = scanner.nextLine();
            RepMusicaController.atribuirPontosPorMusica(user);

            if (input.equals("0")) break;

            try {
                int opcao = Integer.parseInt(input);
                if (opcao >= 1 && opcao <= explicitas.size()) {
                    Musica m = explicitas.get(opcao - 1);
                    m.reproduzir();
                } else {
                    System.out.println("Opção inválida.");
                }
            } catch (NumberFormatException e) {
                System.out.println("Por favor, insira um número válido.");
            }
        }
    }

}