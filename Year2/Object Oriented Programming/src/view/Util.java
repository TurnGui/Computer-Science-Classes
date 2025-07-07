package view;


import java.util.Scanner;

public class Util {


    public static void println(String message) {
        System.out.println(message);
    }

    public static void print(String message) {System.out.print(message);}

    public static void giveError(String message) {
        System.err.println(message);
    }

    public static String input(String message) {
        print(message);
        Scanner scanner = new Scanner(System.in); 
        return scanner.nextLine();
    }


    public static int inputInt(String message) {
        Integer num = null;
        String text;
        Scanner scanner = new Scanner(System.in);

        do {
            print(message);
            text = scanner.nextLine();

            try {
                num = Integer.parseInt(text);
            } catch (NumberFormatException e) {
                giveError(text + " is not a valid Integer.");
            }

        } while (num == null);

        return num;
    }

    public static int inputOption(int options) {
        Integer num = null;
        String text = "";
        boolean state = true;
        Scanner scanner = new Scanner(System.in);

        do {
            print("Select one of the options above: ");
            try {
                text = scanner.nextLine();
                num = Integer.parseInt(text);
            } catch (NumberFormatException e) {
                giveError(text + " is not a valid option!");
                state = false;
            }

            if ((num == null || num < 0 || num > options) && state) {
                num = null;
                giveError(text + " is not a valid option!");
            }
            state = true;
        } while (num == null);

        return num;
    }



    public static void waitInput(){
        Integer a = 0;
        do{
            inputInt("Prima 0 para regressar: ");
        }while(a != 0);
    }

    public static void printHeader() {

        System.out.println("  _____   ____    ____   _____   _    _____ ");
        System.out.println(" / ____| |    |  / __ \\ |_   _| | |  |  ___|");
        System.out.println("| (___   |    | | |  | |  | |   | |  | |_   ");
        System.out.println(" \\___ \\  | ___| | |  | |  | |   | |  |  _|  ");
        System.out.println(" ____) | | |    | |__| |  | |   | |  | | ");
        System.out.println("|_____/  |_|     \\____/   |_|   |_|  |_|");

    }

    public static String inputEmail() {
        String emailRegex = "^[\\w-.]+@([\\w-]+\\.)+[\\w-]{2,4}$";
        String email = Util.input("Email: ");

        while (!email.matches(emailRegex) && !email.equals("0")) {
            email = Util.input("You need to type a correct email: ");
        }
        return email;
    }


    public static void clearScreen() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
    }

}