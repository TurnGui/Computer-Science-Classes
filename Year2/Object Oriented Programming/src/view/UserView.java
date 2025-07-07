package view;

import controller.UserController;
import module.PlanoSubscricao;
import module.user.Utilizador;

public class UserView {

    public static int menu() {
        Util.clearScreen();
        Util.printHeader();
        Util.println("\nMENU\n");
        Util.println("[1] Reproduzir Música");
        Util.println("[2] Ver Playlists");
        Util.println("[3] Criar Playlist");
        Util.println("[4] Ver Pontos");
        Util.println("[5] Leaderboard");
        Util.println("[6] Playlist Explícita (PREMIUM_TOP)");
        Util.println("[7] Logout");
        Util.println("[8] Exit\n");

        Integer option = Util.inputOption(8);
        return option;
    }

    
    public static int showPontosMenu(double pontos) {
        Util.clearScreen();
        Util.printHeader();
        Util.println("\nOs teus pontos:");
        Util.println("Tens " + pontos + " pontos.\n");
        Util.println("[0] Voltar");
        Util.println("[1] Ver utilizador com mais pontos");
    
        return Util.inputOption(1);
    }
    
    public static void showUtilizadorComMaisPontos(String nome, double pontos) {
        Util.clearScreen();
        Util.printHeader();
        Util.println("\nUtilizador com mais pontos:");
        Util.println(nome + " - " + pontos + " pontos.");
        Util.waitInput();
    }
    
    public static PlanoSubscricao selectPlano() {
        Util.clearScreen();
        Util.printHeader();
        Util.println("\nEscolha o seu plano de subscrição:\n");
        
        
        Util.println("[1] FREE");
        Util.println("[2] PREMIUM_BASE");
        Util.println("[3] PREMIUM_TOP");
        
        int escolha;
        while (true) {
            escolha = Util.inputOption(3);
            if (escolha == 1 || escolha == 2 || escolha == 3) {
                break;  
            } else {
                Util.println("\nOpção inválida. Por favor, tente novamente.");
            }
        }

        switch (escolha) {
            case 1: return PlanoSubscricao.FREE;
            case 2: return PlanoSubscricao.PREMIUM_BASE;
            case 3: return PlanoSubscricao.PREMIUM_TOP;
            default: return PlanoSubscricao.FREE; 
        }
    }

    public static void notPremium(Utilizador user) {
        Util.println("\nApenas utilizadores Premium podem criar playlists personalizadas.");
        Util.waitInput();
        UserController.menu(user);
    }

    public static void notPremiumtop(Utilizador user) {
        Util.println("\nApenas utilizadores Premiu_Top podem acessar à Playlist Explicita");
        Util.waitInput();
        UserController.menu(user);
    }

    public static void logout() {
        Util.clearScreen();
        Util.printHeader();
        Util.println("\nA realizar logout...");
        //Util.waitInput();
    }
}
