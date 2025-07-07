package module;

import java.util.Arrays;
import java.util.List;

import module.user.UserListings;
import module.user.Utilizador;
import view.SerializationView;
import controller.FicheiroController;

public class Teste {
    // Método auxiliar de teste
    public static void carregarDadosTeste() {
        UserListings userListings = UserListings.getInstance();

        
        Utilizador user1 = new Utilizador("Maria", "maria@gmail.com","Rua da Luz", PlanoSubscricao.PREMIUM_TOP);
        Utilizador user2 = new Utilizador("João", "joao@gmail.com","Rua SLAY", PlanoSubscricao.FREE);

        Musica musica1 = new Musica(
            "Chuva de Estrelas",
            "Luna Silva",
            "Estrela Records",
            "Cai a noite com brilhos no céu...",
            Arrays.asList("Cai", "a", "noite", "com", "brilhos", "no", "céu"),
            "Pop",
            210,
            false,
            true
        );
        Musica musica2 = new Musica(
            "Slay U all",
            "Periodt",
            "Estrela Records",
            "Go go Slay...",
            Arrays.asList("Go", "GO", "U", "SLay"),
            "GIRLY POP",
            280,
            true,
            true
        );
        SpotifUM.adicionarMusica(musica1);
        SpotifUM.adicionarMusica(musica2);

        // Adicionar à lista
        userListings.addUser(user1);
        userListings.addUser(user2);
        musica1.reproduzir();

        // Guardar ficheiro de teste
        //SerializationView.guardarEstado("teste", userListings);
    }
}
