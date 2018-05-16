package cz.cvut.fel.agents.pdv.dsand;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

public class MessageWrapper {

  private Message message;
  protected long deliveryTime;

  protected MessageWrapper(Message message, long deliveryTime) {
    this.message = deepCopy(message);
    this.deliveryTime = deliveryTime;
  }

  protected Message unpack() {
    return message;
  }

  private static Message deepCopy(Message m) {
    try {
      ByteArrayOutputStream baos = new ByteArrayOutputStream();
      ObjectOutputStream oos = new ObjectOutputStream(baos);
      oos.writeObject(m);
      oos.close();

      ByteArrayInputStream bais = new ByteArrayInputStream(baos.toByteArray());
      ObjectInputStream ois = new ObjectInputStream(bais);
      m = (Message) ois.readObject();
      ois.close();
    } catch (Exception e) {
      e.printStackTrace();
      System.exit(1);
    }

    return m;
  }
}
