package cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils;

/**
 * Interface pro hodiny procesu
 * Typ T je genericky, s generikou jste se jiz setkali v C++ v podobe templatu. My zde pouzivame genericky typ pro
 * vyuziti dvou typu hodin
 */
public interface IProcessClock<T> extends IClock<T> {

    // aktualizuj cas na zaklade casove znamky ze zpravy
    void update(IClock<T> timestamp);

    // aktualizuj cas na zaklade nove udalosti
    void onNewEvent();

    // vrati aktualni snapshot hodin
    IClock<T> getAsTimeStamp();

    // metoda pro overeni poruseni kauzality na zaklade lokalnich hodin a casove znamky
    default boolean isCausalityForProcessViolated(IClock<T> timestamp, int process) {
        return false;
    }

}
