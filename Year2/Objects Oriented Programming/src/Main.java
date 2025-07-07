import controller.MenuController;
import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import module.Musica;
import module.PlanoSubscricao;
import module.SpotifUM;
import module.user.UserListings;
import module.user.Utilizador;
import view.SerializationView;

public class Main {
    public static void main(String[] args) {
        inicializarUtilizadores("utilizadores.dat");
        inicializarMusicas("musicas.dat");

        // Iniciar menu principal
        MenuController.menu();

        // Guardar estado final
        SerializationView.guardarUtilizadores("utilizadores.dat", UserListings.getInstance());
        SerializationView.guardarMusicas("musicas.dat", SpotifUM.getMusicas());
    }

    private static void inicializarUtilizadores(String caminho) {
        File file = new File(caminho);

        if (file.exists()) {
            UserListings users = SerializationView.carregarUtilizadores(caminho);
            if (users != null) {
                UserListings.setInstance(users);
            } else {
                criarUtilizadoresDefault(caminho);
            }
        } else {
            criarUtilizadoresDefault(caminho);
        }
    }

    private static void criarUtilizadoresDefault(String caminho) {
        UserListings defaultUsers = UserListings.getInstance();

        PlanoSubscricao plano = PlanoSubscricao.PREMIUM_TOP;
        Utilizador defaultUser = new Utilizador(
                "Admin User",
                "admin@spotifum.com",
                "Rua XPTO Mix 123, City",
                plano);

        defaultUsers.addUser(defaultUser);
        UserListings.setInstance(defaultUsers);
        SerializationView.guardarUtilizadores(caminho, defaultUsers);
    }

    private static void inicializarMusicas(String caminho) {
        File file = new File(caminho);

        if (file.exists()) {
            List<Musica> musicas = SerializationView.carregarMusicas(caminho);
            if (musicas != null) {
                SpotifUM.setMusicas(musicas);
            } else {
                criarMusicasDefault(caminho);
            }
        } else {
            criarMusicasDefault(caminho);
        }
    }

    private static void criarMusicasDefault(String caminho) {
        List<Musica> defaultMusicas = new ArrayList<>();

       

        

        SpotifUM.setMusicas(defaultMusicas);
        SerializationView.guardarMusicas(caminho, defaultMusicas);
    }
}
