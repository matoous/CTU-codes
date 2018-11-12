package cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils;

import cz.cvut.fel.agents.pdv.dsand.DSProcess;
import cz.cvut.fel.agents.pdv.dsand.Message;

import java.util.Queue;
import java.util.Random;
import java.util.function.BiConsumer;

/**
 * Abstraktnio trida pro nase procesy pracujici s hodinami
 * Typ T je genericky, s generikou jste se jiz setkali v C++ v podobe templatu. My zde pouzivame genericky typ pro
 * vyuziti dvou typu hodin
 */
abstract class CloudProcess<T extends IProcessClock<?>> extends DSProcess {

    // hodiny majici genericky typ, ktery rozsiruje IProcessClock
    final T clock;

    // generator nahodnych cisel
    static final Random RANDOM = new Random();

    CloudProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox, T clock) {
        super(id, inbox, outbox);
        this.clock = clock;
    }
}
