import java.io.Serializable;

public class Password implements Serializable {
    private byte[] hash;
    private Boolean hasToChange;
    private byte[] salt;

    public Password(byte[] hash, byte[] salt) {
        this.hash = hash;
        this.hasToChange = false;
        this.salt = salt;
    }

    public byte[] getHash() {
        return hash;
    }

    public byte[] getSalt() {
        return salt;
    }

    public Boolean getHasToChange() {
        return hasToChange;
    }

    public void forceChange() {
        this.hasToChange = true;
    }


    public void setNewPassword(byte[] hash, byte[] salt) {
        this.hash = hash;
        this.hasToChange = false;
        this.salt = salt;
    }

}