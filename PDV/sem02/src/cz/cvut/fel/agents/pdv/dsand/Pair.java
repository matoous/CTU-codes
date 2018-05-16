package cz.cvut.fel.agents.pdv.dsand;

import java.io.Serializable;
import java.util.Objects;

public class Pair<T extends Serializable, U extends Serializable> implements Serializable {

  T first;
  U second;

  public Pair(T first, U second) {
    this.first = first;
    this.second = second;
  }

  @Override
  public boolean equals(Object o) {
    if (this == o) {
      return true;
    }
    if (o == null || getClass() != o.getClass()) {
      return false;
    }
    Pair<?, ?> pair = (Pair<?, ?>) o;
    return Objects.equals(first, pair.first) &&
        Objects.equals(second, pair.second);
  }

  @Override
  public int hashCode() {

    return Objects.hash(first, second);
  }

  public T getFirst() {
    return first;
  }

  public U getSecond() {
    return second;
  }

  public void setSecond(U second) {
    this.second = second;
  }
}
