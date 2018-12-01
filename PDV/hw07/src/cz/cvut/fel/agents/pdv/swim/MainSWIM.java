package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Simulation;

/**
 * Trida obsahujici metodu main, ktera spousti ukazkovy testovaci scenar.
 */
public class MainSWIM {

    public static void main(String[] args) {
        // Spusteni simulace:
        // V evaluaci nas zajima, jak dobre obstoji vase implementace v prostredi, ve kterem dochazi ke ztrate a
        // zpozdovani zprav. Take nas zajima to, jak moc vytezuje vase reseni komunikacni sit a jak je presne.
        // U zprav, kde je prijemce/odesilatel DisseminationProcess mate zaruceno, ze se zpravy neztraceji.

        // Parametry SWIM Configu, ktere muzete pouzit pro ladeni protokolu:
        // maxDelay             .... maximalni delay, nez je poslana zprava dorucena
        // processesToKill      .... pocet procesu, ktere se ukonci
        // pptOfFailure         .... pravdepodobnost, ze v danem casovem okamziku nejaky proces selze
        // processes            .... pocet procesu v siti
        // upperBoundOnMessages .... maximalni pocet zprav, ktery muze proces poslat behem celeho algoritmu
        // minReliability       .... minimalni pravdepodobnost doruceni zpravy
        // accuracy             .... maximalni pocet procesu mylne prohlasenych za mrtve

        Simulation sim = new Simulation(new SWIMDSConfig(10, 2, 0.1, 100, 400, 0.1, 2));
        sim.run();
    }

}
