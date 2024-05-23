

import java.io.Serializable;
import java.util.HashMap;
import java.util.Set;



public class PasswordManager implements Serializable {
    public HashMap<String, EncryptedData> map;
    private String init;
    private byte[] salt;
    private byte[] iv;

    public PasswordManager(String init, byte[] salt, byte[] iv) {
        map = new HashMap<String, EncryptedData>();
        this.init = init;
        this.salt = salt;
        this.iv = iv;
    }

    public EncryptedData get(String address) {
        return map.get(address);
    }

    public void put(String address, EncryptedData password) {
        map.put(address, password);
    }

    public Set<String> getKeys() {
        return map.keySet();
    }

    public String getInit() {
        return init;
    }

    public byte[] getSalt() {
        return salt;
    }

    public byte[] getIv() {
        return iv;
    }

}
