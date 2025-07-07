package controller;

import module.Musica;
import module.SpotifUM;
import module.user.UserListings;
import module.user.Utilizador;
import view.RepMusicaView;

public class RepMusicaController {
    public static void playMusic(Utilizador user) {
        Utilizador realUser = UserListings.getInstance().getUser(user.getEmail());

        String nomeMusica = RepMusicaView.menurepmusica();

        if (nomeMusica == null) return;

        Musica musica = SpotifUM.getMusicaPorNome(nomeMusica);
        if (musica != null) {
            musica.reproduzir();
            atribuirPontosPorMusica(realUser); 

            FicheiroController.save("utilizadores.dat"); 
            RepMusicaView.mostrarDetalhesMusica(musica, realUser.isPremium());

            int opcao = RepMusicaView.obterOpcaoReproducao(realUser.isPremium());

            switch (opcao) {
                case 1:
                    UserController.menu(realUser); 
                    break;
                case 2:
                    if (realUser.isPremium()) {
                        realUser.adicionarAFavoritos(musica);
                    } else {
                        RepMusicaView.mostrarMensagemNaoPermitido();
                    }
                    break;
                default:
                    RepMusicaView.mostrarMensagemOpcaoInvalida();
            }
        } else {
            RepMusicaView.mostrarMensagemMusicaNaoEncontrada();
        }

        UserController.menu(realUser); 
    }

    
    public static void atribuirPontosPorMusica(Utilizador user) {
        double pontosAtuais = user.getPontos();
        double pontosAdicionar;

        switch (user.getPlano()) {
            case FREE:
                pontosAdicionar = 5;
                break;
            case PREMIUM_BASE:
                pontosAdicionar = 10;
                break;
            case PREMIUM_TOP:
                pontosAdicionar = pontosAtuais * 0.025;
                break;
            default:
                pontosAdicionar = 0;
        }

        user.adicionarPontos(pontosAdicionar);
    }
}
