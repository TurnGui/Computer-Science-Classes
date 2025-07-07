package controller;

import view.MenuView;

public class MenuController {
    public static void menu() {
        int option = MenuView.menu();

        switch (option) {
            case 1:
                LoginController.login();
                break;
            case 2:
                LoginController.signUp();
                break;
            case 3:
                System.exit(0);
        }
    }
}