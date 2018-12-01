package cz.cvut.fel.agents.pdv.dsand;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Random;
import java.util.Set;

public class CommunicationLayer {

  private final DSConfig config;

  private long currentTick = 0;
  private final PriorityQueue<ChannelInfo> leastMessages;
  private final Map<Pair<String, String>, Queue<MessageWrapper>> channels;
  private final Map<String, Queue<Message>> inboxes;

  private final Random rnd;

  CommunicationLayer(DSConfig config, String... agents) {
    this.config = config;

    leastMessages = new PriorityQueue<>();
    channels = new HashMap<>();
    inboxes = new HashMap<>();
    rnd = new Random();

    for (String dst : agents) {
      for (String src : agents) {
        channels.put(new Pair<>(src, dst), new LinkedList<>());
      }
      inboxes.put(dst, new LinkedList<>());
    }
  }

  public void deliveryCycle() {
    currentTick++;

    ArrayList<ChannelInfo> toDeliver = new ArrayList<>();
    while (!leastMessages.isEmpty()
        && Objects.requireNonNull(leastMessages.peek()).leastMessage <= currentTick) {
      toDeliver.add(leastMessages.poll());
    }
    int count = toDeliver.size();

    while (count > 0) {
      int idx = rnd.nextInt(count);
      ChannelInfo currInfo = toDeliver.get(idx);

      if (!currInfo.channel.isEmpty()
          && Objects.requireNonNull(currInfo.channel.peek()).deliveryTime <= currentTick) {
        inboxes.get(currInfo.dst).add(Objects.requireNonNull(currInfo.channel.poll()).unpack());
      } else {
        toDeliver.set(idx, toDeliver.get(--count));
        if (!currInfo.channel.isEmpty()) {
          currInfo.update();
          leastMessages.add(currInfo);
        }
      }
    }
  }

  void send(String sender, String recipient, Message message, List<Set<String>> partitioning,
      Set<String> deadProcesses) {
    if (deadProcesses.contains(sender) || deadProcesses.contains(recipient)
        || rnd.nextDouble() > config.getReliability(sender, recipient, partitioning)) {
      return;
    }

    message.recipient = recipient;
    message.sender = sender;

    MessageWrapper wrappedMessage = new MessageWrapper(message,
        currentTick + config.getDeliveryDelay(sender, recipient));
    Queue<MessageWrapper> channel = channels.get(new Pair<>(sender, recipient));

    if (channel.isEmpty()) {
      leastMessages.add(new ChannelInfo(sender, recipient, channel, wrappedMessage.deliveryTime));
    }

    channel.add(wrappedMessage);
  }

  Queue<Message> getInboxOfAgent(String id) {
    return inboxes.get(id);
  }

  private static class ChannelInfo implements Comparable<ChannelInfo> {

    String src;
    String dst;
    Queue<MessageWrapper> channel;

    long leastMessage;

    ChannelInfo(String src, String dst, Queue<MessageWrapper> channel, long leastMessage) {
      this.src = src;
      this.dst = dst;
      this.channel = channel;
      this.leastMessage = leastMessage;
    }

    @Override
    public int compareTo(ChannelInfo o) {
      return Long.compareUnsigned(leastMessage, o.leastMessage);
    }

    void update() {
      if (channel.isEmpty()) {
        leastMessage = Long.MAX_VALUE;
      } else {
        leastMessage = Objects.requireNonNull(channel.peek()).deliveryTime;
      }
    }
  }
}
