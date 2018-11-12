package cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils;

import java.io.Serializable;

/**
 * Kontrakt pro tridy pracujici s hodinami - pro hodiny v procesu a casovou znamku
 * Typ T je genericky, s generikou jste se jiz setkali v C++ v podobe templatu. My zde pouzivame genericky typ pro
 * vyuziti dvou typu hodin.
 */
public interface IClock<T> extends Serializable {
    T getTime();

    String toString();

}
