
import java.io.Console;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class usermgmt {
    public static final String filepath = System.getProperty("user.dir") + "/obj";
    
    public static void main(String[] args) {
        String command, user;
        UserBase base;

        if(args.length < 2) {
            throw new IllegalArgumentException("Nedovoljno podataka za izvrsavanje akcije!");
        }

        try {
            base = loadUserBase();
        } catch (Exception e) {
            base = new UserBase();
        }

        command = args[0];
        user = args[1];

        if (command.equals("add")) {
            System.out.println(add(user, base));
        } else if (command.equals("passwd")) {
            System.out.println(passwd(user, base));
        } else if (command.equals("forcepass")) {
            System.out.println(forcepass(user, base));
        } else if (command.equals("del")) {
            System.out.println(del(user, base));
        } else {
            System.out.println("Command " + command + " not found!");
        }

        try {
            saveUserBase(base);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void saveUserBase(UserBase base) throws FileNotFoundException, IOException {
        
        FileOutputStream fos = new FileOutputStream(filepath);
        ObjectOutputStream oos = new ObjectOutputStream(fos);
        oos.writeObject(base);
        oos.close();
    }

    private static UserBase loadUserBase() throws FileNotFoundException, IOException, ClassNotFoundException {
        File obj = new File(filepath);
        obj.createNewFile();
        FileInputStream fis = new FileInputStream(filepath);
        ObjectInputStream ois = new ObjectInputStream(fis);
        UserBase base = (UserBase) ois.readObject();
        ois.close();
        return base;
    }

    private static String add(String user, UserBase base) {
        if (base.exists(user)) {
            return "User " + user + " already exists.";
        }

        Console cons;
        String passwd, repeatPasswd;

        cons = System.console();
        passwd = new String(cons.readPassword("%s:", "Password"));
        while(!isGoodPassword(passwd)) {
            passwd = new String(cons.readPassword("%s:", "Password"));
        }
        repeatPasswd = new String(cons.readPassword("%s:", "Repeat Password"));

        if (!passwd.equals(repeatPasswd)) {
            return "User add failed. Password mismatch.";
        }

        base.put(user, passwd);

        return "User " + user + " successfuly added.";
    }

    private static String passwd(String user, UserBase base) {
        if (!base.exists(user)) {
            return "No user " + user + ".";
        }

        Console cons;
        String passwd, repeatPasswd;

        cons = System.console();
        passwd = new String(cons.readPassword("%s:", "Password"));
        while(!isGoodPassword(passwd)) {
            passwd = new String(cons.readPassword("%s:", "Password"));
        }
        repeatPasswd = new String(cons.readPassword("%s:", "Repeat Password"));

        if (!passwd.equals(repeatPasswd)) {
            return "Password change failed. Password mismatch.";
        }

        base.put(user, passwd);

        return "Password change successful.";
    }

    private static String forcepass(String user, UserBase base) {
        if (!base.exists(user)) {
            return "No user " + user + ".";
        }
        
        base.forceChange(user);

        return "User will be requested to change password on next login.";
    }

    private static String del(String user, UserBase base) {
        if (!base.exists(user)) {
            return "No user " + user + ".";
        }
        
        base.remove(user);

        return "User successfuly removed.";
    }

    private static Boolean isGoodPassword(String passwd) {
        Boolean strongPassword = true;
        if (passwd.length() < 8) {
            System.out.println("Password must be at least 8 characters.");
            strongPassword = false;
        }
        Pattern letter = Pattern.compile("[a-zA-z]");
        Pattern digit = Pattern.compile("[0-9]");
        Pattern special = Pattern.compile ("[!@#$%&*()_+=|<>?{}\\[\\]~-]");


        Matcher hasLetter = letter.matcher(passwd);
        Matcher hasDigit = digit.matcher(passwd);
        Matcher hasSpecial = special.matcher(passwd);
        
        if (!hasLetter.find()) {
            System.out.println("Password must contain at least one letter.");
            strongPassword = false;
        }
        if (!hasDigit.find()) {
            System.out.println("Password must contain at least one digit.");
            strongPassword = false;
        }
        if (!hasSpecial.find()) {
            System.out.println("Password must contain at least one special character.");
            strongPassword = false;
        }
        
        return strongPassword;
    }

}
