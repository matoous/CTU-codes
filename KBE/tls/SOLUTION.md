# Solutions

I am sorry. Only on task 4 I noticed that we should write in python. I hope the code is readable,
I tried to make it as similar to the original python code including the tests as possible.
Also, there should be comments everywhere and I did my best to make the code readable.

Whole homework can be verified using:

```shell
go test ./...
```

## Task 1: Diffie–Hellman key exchange

See `agent.go` and `main.go` for implementation and `task1/task1_test.go` for test.

```shell
go test ./task1
```

## Task 2: Diffie–Hellman key

See `agent.go` and `main.go` for implementation and `task2/task2_test.go` for test.

```shell
go test ./task2
```

## Task 3: Bulk cipher

See `agent.go` and `main.go` for implementation and `task3/task3_test.go` for test.

```shell
go test ./task3
```

## Task 4: Implement simple SSL/TLS setup

See `agent.go` and `main.go` for implementation and `task4/task4_test.go` for test.

```shell
go test ./task4
```

## Task 5: Man-in-the-middle

See `mitm.go` for implementation and `task5/task5_test.go` for test.

```shell
go test ./task5
```

## Task 6: RSA

See `rsa.go` for implementation and `task6/task6_test.go` for test.

```shell
go test ./task6
```

## Task 7: RSA broadcast attack

See `rsa.go` for implementation and `task7/task7_test.go` for the implementation of the attack and test.

```shell
go test ./task7
```

## Task 8: Bleichenbacher's RSA attack

See `rsa.go` for implementation and `task8/task8_test.go` for the implementation of the attack and test.

```shell
go test ./task8
```

## Task 9: DSA

Implementation in `dsa.go`, test in `task9/task9_test.go`. I didn't manage to make it work...
sadly, the recovered private key still doesn't match the one from the task. I even checked
against https://github.com/tintinweb/DSAregenK/blob/master/src/DSAregenK.py and couldn't find
anything.
