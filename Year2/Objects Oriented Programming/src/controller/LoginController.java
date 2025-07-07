package controller;

import module.user.Utilizador;
import module.user.UserListings;
import view.AuthView;
import view.UserView;
import module.PlanoSubscricao;

import java.util.Map;

public class LoginController {
    public static void login() {
        Map<String, String> userMap = AuthView.login();

        if (userMap.get("email").equals("0")) MenuController.menu();

        Utilizador user = UserListings.getInstance().getUser(userMap.get("email"));

        if (user == null) {
            AuthView.noUser();
            MenuController.menu();
        }

        UserController.menu(user);
    }

    public static void signUp() {
        Map <String, String> userMap = AuthView.signUp();

        if (UserListings.getInstance().checkUser(userMap.get("email"))) {
            AuthView.userExists();
            MenuController.menu();
        }
        
        PlanoSubscricao plano = UserView.selectPlano();

        if (plano == null) {
            AuthView.invalidPlano();
            signUp(); 
            return;
        }

        Utilizador user = registerUser(userMap, plano);
        FicheiroController.save("utilizadores.dat"); 
        
         UserController.menu(user);

    }
    

    private static Utilizador registerUser(Map<String, String> user, PlanoSubscricao plano) {
        if (UserListings.getInstance().checkUser(user.get("email"))) return null;

        Utilizador newUser = new Utilizador(user.get("name"), user.get("email"), user.get("address"), plano);
        UserListings.getInstance().addUser(newUser);

        return newUser;
    }
    
}
