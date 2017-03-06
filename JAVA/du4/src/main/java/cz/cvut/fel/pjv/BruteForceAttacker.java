package cz.cvut.fel.pjv;

public class BruteForceAttacker extends Thief {
    
    @Override
    public void breakPassword(int sizeOfPassword) {
        char[] letters = getCharacters();
        char[] did = new char[sizeOfPassword];
        if(sizeOfPassword == 0){
            tryOpen(did);
        }else{
            int did_len = 0;
            for(int i = 0; i < letters.length; i++){
                did[0] = letters[i];
                if(proceedInRecursion(did, did_len + 1, sizeOfPassword-1))break;
            }
        }
    }
    
    private boolean proceedInRecursion(char[] did, int did_len, int todo){
        if(todo == 0){
            if(tryOpen(did)){
                return true;
            }
        }
        else{
            char[] letters = getCharacters();
            for (int i = 0; i < letters.length; i++) {
                did[did_len] = letters[i];
                if(proceedInRecursion(did, did_len+1, todo-1)) return true;
            }
        }
        return false;
    }
    
}
