package view;

public class MenuView {
    public static int menu () {
        view.Util.clearScreen();
        Util.printHeader();
        Util.println("\n[1] LOGIN");
        Util.println("[2] SIGNUP");
        Util.println("[3] Exit");

        return Util.inputOption(3);
    }
}