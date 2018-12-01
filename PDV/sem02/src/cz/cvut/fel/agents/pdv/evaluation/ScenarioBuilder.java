package cz.cvut.fel.agents.pdv.evaluation;

/**
 * Rozhrani pro Stavitele konfigurace scenare. Diky navrhovemu vzoru Stavitel nemusime mit tak
 * dlouhe konstruktory. Parametry uz maji nastavene zakladni hodnoty (pokud hodnotu neprepisete pri
 * vytvareni instance scenare pomoci stavitele, vezme se tato hodnota).
 */
public abstract class ScenarioBuilder<T extends Scenario<?>> {

  // pocet procesu v clusteru - N
  protected int numberOfProcessesInCluster = 5;
  // spolehlivost spojeni mezi procesy - (0,1>
  protected double reliabilityOfNetworkConnections = 1.0;
  // kolik procesu se ma ukoncit - <0,N>
  protected int processToKill = 0;
  // maji se ukoncene procesy restartovat
  protected boolean restartProcess = false;
  // dojde k opetovnemu spojeni site
  protected boolean reconnectProcess = false;
  // pocet procesu, ktere ocekavame, ze budou v jedne podsiti.
  // tyto procesy komunikuji jen v ramci podsite - <0,numberOfProcessesInCluster>
  protected int expectedNumberOfProcessesInPartition = 5;
  // pocet kroku simulace - <1,N>
  protected long stepsOfSimulation = 100;
  // spozdeni zprav v siti - pokud je 0, zpravy se nespozduji - <0,N>
  protected int networkDelays = 0;
  // pocet kroku simulace trvani nehody - <0,N>
  protected int lengthOfFailure = 0;
  // pocet kroku simulace mezi nehodami - <1,N>
  protected int spanBetweenFailures = 1;
  // pocet nehod - <0,N>
  protected int countOfFailures = 0;
  // pocet klientu - <1,N>
  protected int numberOfClients = 2;
  // perioda posilani pozadavku z klientu - <0,N>
  protected int requestPeriod = 10;
  // kolik kroku pred koncem simulace prestat s posilanim pozadavku - <1,N>
  protected int stopRequestBeforeEndOfSimulation = 20;
  // po jak dlouhe dobe se pokusit poslat pozadavek znovu - <1,N>
  protected int retryInterval = 5;
  // maximalni pocet pozadavku pro jeden krok simulace - <1,N>
  protected int maxRequestsPerTick = 1;
  // maximalni pocet nezpracovanych pozadavku - <0,N>
  protected int maxUncommittedRequest = 0;

  /**
   * Vrati instanci scenare
   */
  public abstract T build();

  /**
   * Pocet procesu v clusteru - N
   */
  public ScenarioBuilder<T> numberOfProcessesInCluster(int numberOfProcessesInCluster) {
    this.numberOfProcessesInCluster = numberOfProcessesInCluster;
    return this;
  }

  /**
   * Spolehlivost spojeni mezi procesy - (0,1>
   */
  public ScenarioBuilder<T> reliabilityOfNetworkConnections(
      double reliabilityOfNetworkConnections) {
    this.reliabilityOfNetworkConnections = reliabilityOfNetworkConnections;
    return this;
  }

  /**
   * Kolik procesu se ma ukoncit - <0,N>
   */
  public ScenarioBuilder<T> processToKill(int processToKill) {
    this.processToKill = processToKill;
    return this;
  }

  /**
   * Maji se ukoncene procesy restartovat
   */
  public ScenarioBuilder<T> restartProcess(boolean restartProcess) {
    this.restartProcess = restartProcess;
    return this;
  }

  /**
   * Dojde k opetovnemu spojeni site po rozdeleni
   */
  public ScenarioBuilder<T> reconnectProcess(boolean reconnectProcess) {
    this.reconnectProcess = reconnectProcess;
    return this;
  }

  /**
   * Pocet procesu, ktere ocekavame, ze budou v jedne podsiti. Tyto procesy komunikuji jen v ramci
   * podsite - <0,numberOfProcessesInCluster>
   */
  public ScenarioBuilder<T> expectedNumberOfProcessesInPartition(
      int expectedNumberOfProcessesInPartition) {
    this.expectedNumberOfProcessesInPartition = expectedNumberOfProcessesInPartition;
    return this;
  }

  /**
   * Pocet kroku simulace - <1,N>
   */
  public ScenarioBuilder<T> stepsOfSimulation(long stepsOfSimulation) {
    this.stepsOfSimulation = stepsOfSimulation;
    return this;
  }

  /**
   * Spozdeni zprav v siti - pokud je 0, zpravy se nespozduji - <0,N>
   */
  public ScenarioBuilder<T> networkDelays(int networkDelays) {
    this.networkDelays = networkDelays;
    return this;
  }

  /**
   * Pocet kroku simulace trvani nehody - <0,N>
   */
  public ScenarioBuilder<T> lengthOfFailure(int lengthOfFailure) {
    this.lengthOfFailure = lengthOfFailure;
    return this;
  }

  /**
   * Pocet kroku simulace mezi nehodami - <1,N>
   */
  public ScenarioBuilder<T> spanBetweenFailures(int spanBetweenFailures) {
    this.spanBetweenFailures = spanBetweenFailures;
    return this;
  }

  /**
   * Pocet nehod - <0,N>
   */
  public ScenarioBuilder<T> countOfFailures(int countOfFailures) {
    this.countOfFailures = countOfFailures;
    return this;
  }

  /**
   * Pocet klientu - <1,N>
   */
  public ScenarioBuilder<T> numberOfClients(int numberOfClients) {
    this.numberOfClients = numberOfClients;
    return this;
  }

  /**
   * Perioda posilani pozadavku z klientu - <0,N>
   */
  public ScenarioBuilder<T> requestPeriod(int requestPeriod) {
    this.requestPeriod = requestPeriod;
    return this;
  }

  /**
   * Kolik kroku pred koncem simulace prestat s posilanim pozadavku - <1,N>
   */
  public ScenarioBuilder<T> stopRequestBeforeEndOfSimulation(int stopRequestBeforeEndOfSimulation) {
    this.stopRequestBeforeEndOfSimulation = stopRequestBeforeEndOfSimulation;
    return this;
  }

  /**
   * Po jak dlouhe dobe se pokusit poslat pozadavek znovu - <1,N>
   */
  public ScenarioBuilder<T> retryInterval(int retryInterval) {
    this.retryInterval = retryInterval;
    return this;
  }

  /**
   * Maximalni pocet pozadavku pro jeden krok simulace - <1,N>
   */
  public ScenarioBuilder<T> maxRequestsPerTick(int maxRequestsPerTick) {
    this.maxRequestsPerTick = maxRequestsPerTick;
    return this;
  }

  /**
   * maximalni pocet nezpracovanych pozadavku - <0,N>
   */
  public ScenarioBuilder<T> maxUncommittedRequest(
      int maxUncommittedRequest) {
    this.maxUncommittedRequest = maxUncommittedRequest;
    return this;
  }

}
