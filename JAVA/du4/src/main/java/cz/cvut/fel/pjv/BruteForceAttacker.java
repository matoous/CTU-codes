package cz.cvut.fel.pjv;

public class BruteForceAttacker extends Thief {
    
    char[] letters = getCharacters();
    int sp;
    
    @Override
    public void breakPassword(int sizeOfPassword) {
        sp = sizeOfPassword;
        char[] did = new char[sizeOfPassword];
        if(sizeOfPassword == 0) tryOpen(did);
        else{
            for(int i = 0; i < letters.length; i++){
                did[0] = letters[i];
                if(proceedInRecursion(did, did_len + 1)) break;
            }
        }
    }
    
    private boolean proceedInRecursion(char[] did, int did_len){
        if(did_len == sp) return tryOpen(did);
        else{
            for (int i = 0; i < letters.length; i++) {
                did[did_len] = letters[i];
                if(proceedInRecursion(did, did_len+1, todo-1)) return true;
            }
        }
        return false;
    }
}
