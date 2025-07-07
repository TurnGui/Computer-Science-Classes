package view;

import java.util.HashMap;
import java.util.Map;

public class AuthView {
    public static Map<String, String> login() {
        Util.clearScreen();
        Util.printHeader();
        Map<String, String> user = new HashMap<String, String>();

        Util.println("\nLogin\n");
        Util.println("Insere o teu email ou insere 0 para voltar atrás.");
        String email = Util.inputEmail();
        user.put("email", email);

        return user;
    }

    public static void noUser() {
        Util.println("\nO utilizador não existe, tenta fazer login com outro email.");
        Util.input("Pressione enter para voltar à página de autenticação");
    }

    public static Map<String, String> signUp() {
        Util.clearScreen();
        Util.printHeader();
        Util.println("SignUp");

        Map<String, String> userInput = new HashMap<String, String>();

        userInput.put("email", Util.inputEmail());
        userInput.put("nome", Util.input("Nome: "));
        userInput.put("morada", Util.input("Morada: "));

        

        return userInput;
    }

    public static void userExists() {
        Util.println("\nUtilizador já existe, tenta fazer registo com outro email.");
        Util.input("Pressione enter para voltar à página de autenticação");
    }

    public static void invalidPlano() {
        Util.clearScreen();
        Util.printHeader();
        Util.println("\nPlano de subscrição inválido. Tente novamente.");
        Util.input("Pressione enter para tentar novamente");
    }
    
}
