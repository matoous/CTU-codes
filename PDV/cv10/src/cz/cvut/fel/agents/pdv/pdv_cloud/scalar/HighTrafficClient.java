package cz.cvut.fel.agents.pdv.pdv_cloud.scalar;

import cz.cvut.fel.agents.pdv.dsand.DSProcess;
import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils.IClock;
import cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils.IProcessClock;
import cz.cvut.fel.agents.pdv.pdv_cloud.cloud_utils.SystemMessage;

import java.util.Queue;
import java.util.function.BiConsumer;

public class HighTrafficClient<L, T extends IProcessClock<L>> extends DSProcess {
    T clock;
    String target;

    public HighTrafficClient(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox, T clock, String target) {
        super(id, inbox, outbox);
        this.clock = clock;
        this.target = target;
    }

    @Override
    public void act() {
        for(int i = 0 ; i < 100 ; i++) {
            clock.onNewEvent();
            send(target, new SystemMessage<IClock<L>>(SystemMessage.Messages.READ, clock.getAsTimeStamp(), 0));
        }
    }
}
