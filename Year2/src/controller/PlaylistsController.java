package controller;
import module.user.Utilizador;
import view.PlaylistsView;
import view.Util;

public class PlaylistsController {
    public static void plmenu(Utilizador user) {
        int option = PlaylistsView.playlistsmenu();

        switch (option) {
            case 1:
                PlaylistsView.createPlaylistConstruida(user);
                break;
            case 2:
                PlaylistsView.createPlaylistAleatoria(user);
                break;
            case 3:
                PlaylistsView.createPlaylistGeneroDuracao(user);
                break;
            case 4:
                PlaylistsView.createPlaylistFavoritos(user);
                break;
            case 5:
                UserController.menu(user);
            default:
                System.out.println("Opção inválida.");
                Util.waitInput();
        }
    }
}
