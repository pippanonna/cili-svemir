import java.io.Serializable;

public class EncryptedData implements Serializable {
    private String encryptedAddress;
    private String encryptedPassword;
    private byte[] salt;
    private byte[] iv;

    public EncryptedData(String encryptedAddress, String encryptedPassword, byte[] salt, byte[] iv) {
        this.encryptedAddress = encryptedAddress;
        this.encryptedPassword = encryptedPassword;
        this.salt = salt;
        this.iv = iv;
    }

    public String getEncryptedAddress() {
        return encryptedAddress;
    }

    public void setEncryptedAddress(String encryptedAddress) {
        this.encryptedAddress = encryptedAddress;
    }

    public String getEncryptedPassword() {
        return encryptedPassword;
    }

    public void setEncryptedPassword(String encryptedPassword) {
        this.encryptedPassword = encryptedPassword;
    }

    public byte[] getSalt() {
        return salt;
    }

    public void setSalt(byte[] salt) {
        this.salt = salt;
    }

    public byte[] getIv() {
        return iv;
    }

    public void setIv(byte[] iv) {
        this.iv = iv;
    }

}

