package cz.cvut.fel.pjv;

import java.util.Arrays;

public abstract class Thief {

    private char[] characters;
    private char[] password;
    
    private boolean initialized = false;
    private boolean opened = false;

    public abstract void breakPassword(int sizeOfPassword);
    
    public boolean tryOpen(char[] input) {
        if (password.length != input.length) {
            System.err.println("Given password must be of the "
                    + "same size as password of the vault ("+ password.length
                    + "), but was: "+ input.length);
            return false;
        }
        
        opened = Arrays.equals(password, input);
        
        if (opened) {
            System.out.println("...click!...");
        }
        
        return opened;        
    }
    
    public char[] getCharacters() {
        return characters;
    }
    
    public final void init(char[] charactersArray, String passwordString) {
        if (initialized) {
            System.err.println("Function init has already been called");
            return; 
        }
        initialized = true;
        
        Arrays.sort(charactersArray);

        characters = Arrays.copyOf(charactersArray, charactersArray.length);
        password = passwordString.toCharArray();
    }

    public final boolean isOpened() {
        return opened;
    }
}
