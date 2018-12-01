package cz.cvut.fel.agents.pdv.bank;

import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;
import cz.cvut.fel.agents.pdv.clocked.ClockedProcess;
import cz.cvut.fel.agents.pdv.exclusion.ExclusionPrimitive;

import java.util.Queue;
import java.util.Random;
import java.util.function.BiConsumer;

/**
 * Proces bankovniho urednika/urednice postupne zpracovava nahodne vygenerovane prikazy k prevodu penez
 * mezi bankovnimi ucty.
 */
public class BankOfficerProcess extends ClockedProcess {

    // Synchronizacni primitiva chranici pristup k jednotlivym bankovnim uctum. Pro pristup k uctu
    // musi vsichni ostatni odsouhlasit, ze do kriticke sekce (odpovidajici danemu bankovnimu uctu)
    // mohu vstoupit.
    private ExclusionPrimitive mutexes[];

    // Identifikator procesu, ktery spravuje databazi zustatku na jednotlivych uctech
    private String datastore;

    // Zdrojovy a cilovy bankovni ucet. Zaporne cislo znaci, ze src a dst nebylo jeste zvoleno.
    private int src = -1;
    private int dst = -1;

    // Aktualni zustatky na zdrojovym a cilovym bankovnim uctu.
    int srcValue = -1;
    int dstValue = -1;

    // Pocet odpovedi na dotaz READ zustatku bankovnich uctu. Hodnota 2 znaci, ze oba zustatky byly nacteny.
    int numReads = 0;

    // Pocet odpovedi na dotaz WRITE zustatku bankovnich uctu. Hodnota 2 znaci, ze oba zustatky byly uspesne
    // zapsany.
    int numWrites = 0;

    // Hodnota 'true' znaci, ze transakce byla zahajena. Tj. pozadavky READ na zustatek uctu src a dst byly
    // odeslane.
    boolean transactionStarted = false;

    // Zamkova struktura, ktera zajisti, ze dojde k zamknuti mutexu pro oba ucty (src a dst). Navic zajisti,
    // ze pri spravne implementaci ExclusionPrimitive nemuze dojit k deadlocku.
    MultiLock multiLock = null;

    private Random rnd = new Random();

    public BankOfficerProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox, int numAccounts,
                              String[] officers, String datastore) {
        super(id, inbox, outbox);
        mutexes = new ExclusionPrimitive[numAccounts];
        for(int i = 0 ; i < numAccounts ; i++) {
            // Vsechny procesy mohou pristupovat ke zdroji 'account'i. Proto si vytvari svoji instanci mutexu
            // pro danou kritickou sekci. O vstupu do kriticke sekce rozhoduji vsichni urednici ('officers').
            mutexes[i] = new ExclusionPrimitive(this, "account"+i, officers);
        }

        this.datastore = datastore;
    }

    @Override
    public void act() {

        if(src < 0) {
            // Pokud nemame zadny prikaz k prevodu penez, tak si ho vytvorime.
            src = rnd.nextInt(mutexes.length);
            dst = rnd.nextInt(mutexes.length - 1);
            if(dst >= src) dst += 1;

            numReads = 0;                // Dosud nam neprisly zadne odpovedi
            numWrites = 0;
            transactionStarted = false;  // a transakce nebyla zahajena

            // Abychom prevod mohli uskutecnit, tak si musime zamknout oba ucty (src a dst)
            multiLock = new MultiLock(mutexes[src], mutexes[dst]);
        }

        outer:
        while(!inbox.isEmpty()) {
            ClockedMessage msg = receive();

            // Nejprve se zeptame, zda prijata zprava nenalezi nekteremu ExclusionPrimitive. Pokud ano
            // tak metoda accept(...) vrati true, a muzeme tak pokracovat dalsi zpravou.
            for(int i = 0 ; i < mutexes.length ; i++) {
                if(mutexes[i].accept(msg)) continue outer;
            }

            if(msg instanceof ValueMessage) {

                // Prisla nam odpoved na pozadavek READ. Hodnotu si ulozime do srcValue/dstValue
                ValueMessage vm = (ValueMessage) msg;
                if(vm.key == src) srcValue = vm.value;
                else if(vm.key == dst) dstValue = vm.value;
                else {
                    throw new RuntimeException("I haven't asked for " + vm.key + "!");
                }
                numReads++;

                if(numReads == 2) {
                    // Pokud nam uz dosly obe odpovedi na dotaz READ, muzeme uskutecnit prevod penez.
                    // Nejprve rozhodneme, kolik penez budeme prevadet.
                    int amount = Math.min(srcValue, rnd.nextInt(5));
                    srcValue -= amount;
                    dstValue += amount;

                    // Na zaver zasleme WRITE pozadavky procesu 'datastore'. Vsimnete si, ze pred odeslanim
                    // zprav inkrementujeme logicky cas procesu.
                    increaseTime();
                    send(datastore, new WriteMessage(src, srcValue));

                    increaseTime();
                    send(datastore, new WriteMessage(dst, dstValue));
                }
            } else if(msg instanceof WriteAcknowledgedMessage) {
                if(++numWrites == 2) {
                    // Pokud nam dorazily obe odpovedi na dotazy WRITE, vime, ze jsme dokoncili celou transakci.
                    // V tu chvili muzeme uvolnit oba zamky (pro ucty src a dst).
                    multiLock.exit();
                    multiLock = null;

                    src = -1;
                    dst = -1;
                }
            } else {
                throw new RuntimeException("Unexpected message: " + msg);
            }
        }

        if(src >= 0 && multiLock.isHeld() && !transactionStarted) {
            // Pokud jsme uz ziskali zamek pro oba ucty, muzeme zahajit transakci.
            transactionStarted = true;

            // To udelame tak, ze zasleme pozadavek READ pro precteni aktualnich zustatku.
            increaseTime();
            send(datastore, new ReadMessage(src));

            increaseTime();
            send(datastore, new ReadMessage(dst));
        }
    }

}
