package controller;

import module.user.UserListings;
import module.user.Utilizador;
import view.PlaylistsView;
import view.UserView;

public class UserController {

    public static void menu(Utilizador user) {

        boolean ativo = true;
        while (true) {
            int option = UserView.menu();
    
            switch (option) {
                case 1:
                    RepMusicaController.playMusic(user);
                    break;
                case 2:
                    PlaylistsView.showPlaylists(user);
                    break;
                case 3:
                    PlaylistsController.plmenu(user);
                    break;
                case 4:
                    while (true) {
                        int subOption = UserView.showPontosMenu(user.getPontos());
                        if (subOption == 0) break;
                        if (subOption == 1) {
                            Utilizador maxUser = null;
                            double maxPontos = -1;
                            for (Utilizador u : UserListings.getInstance().getUtilizador().values()) {
                                if (u.getPontos() > maxPontos) {
                                    maxPontos = u.getPontos();
                                    maxUser = u;
                                }
                            }
                            if (maxUser != null)
                                UserView.showUtilizadorComMaisPontos(maxUser.getNome(), maxUser.getPontos());
                        }
                    }
                    break;
                case 5:
                    LeaderboardController.menu(); 
                    break;
                case 6:
                    PlaylistsView.showPlaylistExplicitas(user);
                    break;
                case 7:
                    UserController.logout();
                    ativo = false; 
                    break;
                case 8:
                    System.exit(0);
                    break;
            }
        }
    }
    
    public static void logout() {
        UserView.logout();
        MenuController.menu();
    }



}

