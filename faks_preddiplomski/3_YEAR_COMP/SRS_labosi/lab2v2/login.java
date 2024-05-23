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

public class login {
    
     public static final String filepath = System.getProperty("user.dir") + "/obj";
    
    public static void main(String[] args) {
        String user;
        UserBase base;

        if(args.length < 1) {
            throw new IllegalArgumentException("Nedovoljno podataka za izvrsavanje akcije!");
        }

        try {
            base = loadUserBase();
        } catch (Exception e) {
            base = new UserBase();
        }

        user = args[0];

        System.out.println(loginUser(user, base));

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

    private static String loginUser(String user, UserBase base) {
        Console cons;
        cons = System.console();

        String passwd;

        passwd = new String(cons.readPassword("%s:", "Password"));

        while(!base.compare(user, passwd)) {
            System.out.println("Username or password incorrect.");
            passwd = new String(cons.readPassword("%s:", "Password"));
        }
        
        if (base.hasToChange(user) != null && base.hasToChange(user) == true) {
            String repeatPasswd;
            passwd = new String(cons.readPassword("%s:", "New password"));
            while(!isGoodPassword(passwd)) {
                passwd = new String(cons.readPassword("%s:", "Password"));
            }
            repeatPasswd = new String(cons.readPassword("%s:", "Repeat Password"));

            if (!passwd.equals(repeatPasswd)) {
                return "Login failed. Password mismatch.";
            } else if (base.compare(user, passwd)) {
                return "Old password can't be the same as new password.";
            }

            base.put(user, passwd);
        }


        return "Login successful.";
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
