package controller;

import module.user.UserListings;
import view.SerializationView;

public class FicheiroController {

    public static void save(String caminho) {
        SerializationView.guardarUtilizadores(caminho, UserListings.getInstance());
    }

    public static void load(String caminho) {
        UserListings users = SerializationView.carregarUtilizadores(caminho);
        if (users != null) {
            UserListings.setInstance(users);
        }
    }
}
