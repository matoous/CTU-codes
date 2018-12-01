package cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils;

import cz.cvut.fel.agents.pdv.dsand.Message;

/**
 * Systemova zprava. Obsahuje typ pro lepsi manipulaci + casovou znamku
 * Typ V je genericky, s generikou jste se jiz setkali v C++ v podobe templatu. My zde pouzivame genericky typ pro
 * vyuziti dvou typu hodin
 */
public class SystemMessage<V extends IClock<?>> extends Message {

    // Druhy zprav v nasem cloudu. U PRINT kauzalitu sledovat nebudeme
    public enum Messages {
        SAVE, READ, REFRESH, PRINT, REPLICATE
    }

    private final Messages type;
    private final V timestamp;
    private final int content;

    public SystemMessage(Messages type, V timestamp, int content) {
        this.type = type;
        this.timestamp = timestamp;
        this.content = content;
    }

    public Messages getType() {
        return type;
    }

    public V getTimestamp() {
        return timestamp;
    }

    public int getContent() {
        return content;
    }
}
