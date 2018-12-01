package cz.cvut.fel.agents.dsand;

import java.util.*;

public class CommunicationLayer {

    private final DSConfig config;

    private long currentTick = 0;
    private final PriorityQueue<ChannelInfo> leastMessages;
    private final Map<Pair<String, String>,Queue<MessageWrapper>> channels;
    private final Map<String,Queue<Message>> inboxes;

    private final Random rnd;

    public CommunicationLayer(DSConfig config, String... agents) {
        this.config = config;

        leastMessages = new PriorityQueue<>();
        channels = new HashMap<>();
        inboxes = new HashMap<>();
        rnd = new Random();

        for(String dst : agents) {
            for(String src : agents) {
                channels.put(new Pair<>(src,dst), new LinkedList<>());
            }
            inboxes.put(dst, new LinkedList<>());
        }
    }

    public void deliveryCycle() {
        currentTick++;

        ArrayList<ChannelInfo> toDeliver = new ArrayList<>();
        while(!leastMessages.isEmpty() && leastMessages.peek().leastMessage <= currentTick) {
            toDeliver.add(leastMessages.poll());
        }
        int count = toDeliver.size();

        while(count > 0) {
            int idx = rnd.nextInt(count);
            ChannelInfo currInfo = toDeliver.get(idx);

            if(!currInfo.channel.isEmpty() && currInfo.channel.peek().deliveryTime <= currentTick) {
                inboxes.get(currInfo.dst).add(currInfo.channel.poll().unpack());
            } else {
                toDeliver.set(idx, toDeliver.get(--count));
                if(!currInfo.channel.isEmpty()) {
                    currInfo.update();
                    leastMessages.add(currInfo);
                }
            }
        }
    }

    public void send(String sender, String recipient, Message message) {
        if(rnd.nextDouble() > config.getReliability(sender, recipient)) return;

        message.recipient = recipient;
        message.sender = sender;

        MessageWrapper wrappedMessage = new MessageWrapper(message, currentTick + config.getDeliveryDelay(sender, recipient));
        Queue<MessageWrapper> channel = channels.get(new Pair<>(sender, recipient));

        if(channel.isEmpty()) {
            leastMessages.add(new ChannelInfo(sender, recipient, channel, wrappedMessage.deliveryTime));
        }

        channel.add(wrappedMessage);
    }

    public Queue<Message> getInboxOfAgent(String id) {
        return inboxes.get(id);
    }

    private static class ChannelInfo implements Comparable<ChannelInfo> {
        String src;
        String dst;
        Queue<MessageWrapper> channel;

        long leastMessage;

        public ChannelInfo(String src, String dst, Queue<MessageWrapper> channel, long leastMessage) {
            this.src = src;
            this.dst = dst;
            this.channel = channel;
            this.leastMessage = leastMessage;
        }

        @Override
        public int compareTo(ChannelInfo o) {
            return Long.compareUnsigned(leastMessage, o.leastMessage);
        }

        public void update() {
            if(channel.isEmpty()) leastMessage = Long.MAX_VALUE;
            else leastMessage = channel.peek().deliveryTime;
        }
    }
}
