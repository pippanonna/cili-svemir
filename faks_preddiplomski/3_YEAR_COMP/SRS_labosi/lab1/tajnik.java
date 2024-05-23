


import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.KeySpec;
import java.util.Base64;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.PBEKeySpec;
import javax.crypto.spec.SecretKeySpec;



/**
 * tajnik
 */
public class tajnik {
    public static final String filepath = System.getProperty("user.dir") + "/obj";
    public static final String test = "hana";


    public static void main(String[] args) {
        String command, masterPass;

        if(args.length < 2) {
            throw new IllegalArgumentException("Nedovoljno podataka za izvrsavanje akcije!");
        }

        command = args[0];
        masterPass = args[1];

        if (command.equals("init")) {
            System.out.println(initSecure(masterPass));
        } else if (command.equals("put")) {
            if (args.length < 4) {
                throw new IllegalArgumentException("Nedovoljno podataka za izvrsavanje akcije!");
            }
            String address = args[2];
            String pass = args[3];
            System.out.println(putSecure(masterPass, address, pass));
        } else if (command.equals("get")) {
            if (args.length < 3) {
                throw new IllegalArgumentException("Nedovoljno podataka za izvrsavanje akcije!");
            }
            String address = args[2];
            System.out.println(getSecure(masterPass, address));
        } else {
            // error
        }
    }

    private static String initSecure(String masterPassword) {
        try {

            String algorithm = "AES/CBC/PKCS5Padding";
            byte[] salt = generateSalt();
            SecretKey sk = getSecretKey(masterPassword, salt);
            IvParameterSpec iv  = generateIv();
            String cipherTest = encrypt(algorithm, test, sk, iv);

            PasswordManager manager = new PasswordManager(cipherTest, salt, iv.getIV());
            
            savePasswordManager(manager);
            return "Password manager initialized.";
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "Unable to initialize password manager.";
    }

    private static String putSecure(String masterPassword, String address, String password) {
        try {
            PasswordManager manager = loadPasswordManager();
            String algorithm = "AES/CBC/PKCS5Padding";
            try {
                decrypt(algorithm, manager.getInit(), getSecretKey(masterPassword, manager.getSalt()), new IvParameterSpec(manager.getIv()));
            } catch (BadPaddingException e) {
                return "Master password incorrect or integrity check failed.";
            }

            String key = encrypt(algorithm, address, getSecretKey(masterPassword, manager.getSalt()), new IvParameterSpec(manager.getIv()));

            EncryptedData val = manager.get(key);

            
            byte[] salt = generateSalt();
            SecretKey sk  = getSecretKey(masterPassword, salt);
            IvParameterSpec iv = generateIv();
            
            String cipherAddress = encrypt(algorithm, address, sk, iv);
            String cipherPassword = encrypt(algorithm, password, sk, iv);

            if (val == null) {
                val = new EncryptedData(cipherAddress, cipherPassword, salt, iv.getIV());
            } else {
                val.setEncryptedAddress(cipherAddress);
                val.setEncryptedPassword(cipherPassword);
                val.setSalt(salt);
                val.setIv(iv.getIV());
            }

            manager.put(key, val);
            savePasswordManager(manager);
            return "Stored password for " + address + ".";

            
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "Master password incorrect or integrity check failed.";
    }

    private static String getSecure(String masterPassword, String address) {
        try {
            String algorithm = "AES/CBC/PKCS5Padding";

            PasswordManager manager = loadPasswordManager();
            try {
                decrypt(algorithm, manager.getInit(), getSecretKey(masterPassword, manager.getSalt()), new IvParameterSpec(manager.getIv()));
            } catch (BadPaddingException e) {
                return "Master password incorrect or integrity check failed.";
            }

            
            String key = encrypt(algorithm, address, getSecretKey(masterPassword, manager.getSalt()), new IvParameterSpec(manager.getIv()));

            EncryptedData val = manager.get(key);

            if (val == null) {
                return "Master password incorrect or integrity check failed.";
            }
            
            String storedAddress = decrypt(algorithm, val.getEncryptedAddress(), getSecretKey(masterPassword, val.getSalt()
            ), new IvParameterSpec(val.getIv()));
            if (address.compareTo(storedAddress) != 0) {
                return "Master password incorrect or integrity check failed.";
            }
            String password = decrypt(algorithm, val.getEncryptedPassword(), getSecretKey(masterPassword, val.getSalt()
            ), new IvParameterSpec(val.getIv()));
            return "Password for " + address + " is: " + password + ".";
            
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "Master password incorrect or integrity check failed.";
    }

    private static void savePasswordManager(PasswordManager passManager) throws FileNotFoundException, IOException {
        
        FileOutputStream fos = new FileOutputStream(filepath);
        ObjectOutputStream oos = new ObjectOutputStream(fos);
        oos.writeObject(passManager);
        oos.close();
    }

    private static PasswordManager loadPasswordManager() throws FileNotFoundException, IOException, ClassNotFoundException {
        File obj = new File(filepath);
        obj.createNewFile();
        FileInputStream fis = new FileInputStream(filepath);
        ObjectInputStream ois = new ObjectInputStream(fis);
        PasswordManager manager = (PasswordManager) ois.readObject();
        ois.close();
        return manager;
    }

    public static String encrypt(String algorithm, String input, SecretKey key,
    IvParameterSpec iv) throws NoSuchPaddingException, NoSuchAlgorithmException,
    InvalidAlgorithmParameterException, InvalidKeyException,
    BadPaddingException, IllegalBlockSizeException {
        Cipher cipher = Cipher.getInstance(algorithm);
        cipher.init(Cipher.ENCRYPT_MODE, key, iv);
        byte[] cipherText = cipher.doFinal(input.getBytes());
        return Base64.getEncoder()
            .encodeToString(cipherText);
    }

    public static String decrypt(String algorithm, String cipherText, SecretKey key,
        IvParameterSpec iv) throws NoSuchPaddingException, NoSuchAlgorithmException,
        InvalidAlgorithmParameterException, InvalidKeyException,
        BadPaddingException, IllegalBlockSizeException {
        
        Cipher cipher = Cipher.getInstance(algorithm);
        cipher.init(Cipher.DECRYPT_MODE, key, iv);
        byte[] plainText = cipher.doFinal(Base64.getDecoder().decode(cipherText));
        return new String(plainText);
    }

    private static SecretKey getSecretKey(String masterPassword, byte[] salt) throws NoSuchAlgorithmException, InvalidKeySpecException {
        KeySpec spec = new PBEKeySpec(masterPassword.toCharArray(), salt, 65536, 128);
        SecretKeyFactory factory = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA256");
        SecretKey sk = new SecretKeySpec(factory.generateSecret(spec).getEncoded(), "AES");
        return sk;
    }

    private static byte[] generateSalt() {
        SecureRandom random = new SecureRandom();
        byte[] salt = new byte[16];
        random.nextBytes(salt);

        return salt;
    }

    private static IvParameterSpec generateIv() {
        byte[] iv = new byte[16];
        new SecureRandom().nextBytes(iv);
        return new IvParameterSpec(iv);
    }

}