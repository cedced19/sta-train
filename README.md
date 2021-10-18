# STA Train Autonome

To test on Raspberry:
```
make test-client
make test-serveur
```

To test on Computer:
```
make test-client-fake
```

## Code 

Even number: Server -> Train
Odd number: Train -> Server

* 1 : Add train request
* 2 : Add train ack
* 3 : Send position and speed
* 4 : /
* 5 : Moving ack
* 6 : Moving command
