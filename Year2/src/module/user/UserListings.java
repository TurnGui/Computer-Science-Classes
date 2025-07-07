package module.user;

import java.io.*;
import java.util.HashMap;
import java.util.Map;

public class UserListings implements Serializable {
    private static UserListings instance = null;
    private Map<String, Utilizador> users;

    public static UserListings getInstance() {
        if (instance == null) {
            instance = new UserListings();
        }
        return instance;
    }

    private UserListings() {
        this.users = new HashMap<String, Utilizador>();
    }

    public static void setInstance(UserListings instancia) {
        instance = instancia;
    }

    public Map<String, Utilizador> getUtilizador() {
        return new HashMap<>(this.users);
    }

    public void addUser(Utilizador user) {
        users.put(user.getEmail(), user);
    }

    public boolean deleteUser(String email) {
        if (checkUser(email)) {
            users.remove(email);
            return true;
        } else return false;
    }

    public boolean checkUser(String email) {
        return users.containsKey(email);
    }

    public Utilizador getUser(String email) {
        return users.get(email);
    }

    public Utilizador getUserByName(String name) {
        for (Utilizador user : users.values()) {
            if (user.getNome().equals(name)) return user;
        }
        return null;
    }

    public boolean isEmailAvailable(String email) {
        return !users.containsKey(email);
    }

    public void saveUsers(String folderName) {
        try {
            File file = new File("saves/" + folderName + "/users.ser");
            file.getParentFile().mkdirs(); 
            file.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
        try (FileOutputStream fileOut = new FileOutputStream("saves/" + folderName + "/users.ser");
             ObjectOutputStream out = new ObjectOutputStream(fileOut)) {
            out.writeObject(this);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void loadUsers(String folderName) {
        try (FileInputStream fileIn = new FileInputStream("saves/" + folderName + "/users.ser");
             ObjectInputStream in = new ObjectInputStream(fileIn)) {
            UserListings userListings = (UserListings) in.readObject();
            instance = userListings;
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }
}
