
import java.io.Serializable;
import java.util.Arrays;
import java.util.HashMap;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.KeySpec;

import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;

public class UserBase implements Serializable {
    private HashMap<String, Password> map;

    public UserBase() {
        map = new HashMap<String, Password>();
    }

    public HashMap<String, Password> getMap() {
        return map;
    }

    public void setMap(HashMap<String, Password> map) {
        this.map = map;
    }

    public void put(String key, String val) {
        try {
            byte[] salt, hash;
            salt = generateSalt();
            hash = generateHash(val, salt);
            map.put(key, new Password(hash, salt));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public Boolean exists(String key) {
        return map.get(key) != null;
    }

    public Boolean compare(String key, String newPass) {
        try {
            Password pass = map.get(key);
            if (pass == null) return false;
            byte[] newHash = generateHash(newPass, pass.getSalt());
            if (Arrays.equals(pass.getHash(), newHash)) return true;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    public void remove(String key) {
        map.remove(key);
    }

    public void forceChange(String key) {
        Password pass = map.get(key);
        if (pass != null) map.get(key).forceChange();
    }

    public Boolean hasToChange(String key) {
        Password pass = map.get(key);
        if (pass == null) return null;
        return map.get(key).getHasToChange();
    }

    private static byte[] generateHash(String password, byte[] salt) throws NoSuchAlgorithmException, InvalidKeySpecException {
        KeySpec spec = new PBEKeySpec(password.toCharArray(), salt, 65536, 128);
        SecretKeyFactory factory = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA1");
        byte[] hash = factory.generateSecret(spec).getEncoded();
        return hash;
    }

    private static byte[] generateSalt() {
        SecureRandom random = new SecureRandom();
        byte[] salt = new byte[16];
        random.nextBytes(salt);

        return salt;
    }
}
