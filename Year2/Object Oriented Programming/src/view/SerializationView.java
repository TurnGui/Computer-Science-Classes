
package view;

import java.io.*;
import java.util.List;
import module.user.UserListings;
import module.Musica;

public class SerializationView {

    public static void guardarUtilizadores(String caminho, UserListings users) {
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(caminho))) {
            oos.writeObject(users);
            System.out.println("Utilizadores guardados com sucesso.");
        } catch (IOException e) {
            System.out.println("Erro ao guardar utilizadores: " + e.getMessage());
        }
    }

    public static UserListings carregarUtilizadores(String caminho) {
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(caminho))) {
            UserListings loaded = (UserListings) ois.readObject();
            UserListings.setInstance(loaded);
            System.out.println("Utilizadores carregados com sucesso.");
            return loaded;
        } catch (IOException | ClassNotFoundException e) {
            System.out.println("Erro ao carregar utilizadores: " + e.getMessage());
            return null;
        }
    }

    public static void guardarMusicas(String caminho, List<Musica> musicas) {
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(caminho))) {
            oos.writeObject(musicas);
            System.out.println("Músicas guardadas com sucesso.");
        } catch (IOException e) {
            System.out.println("Erro ao guardar músicas: " + e.getMessage());
        }
    }

    public static List<Musica> carregarMusicas(String caminho) {
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(caminho))) {
            return (List<Musica>) ois.readObject();
        } catch (IOException | ClassNotFoundException e) {
            System.out.println("Erro ao carregar músicas: " + e.getMessage());
            return null;
        }
    }
}
