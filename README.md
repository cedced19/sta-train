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

`RBC.exe` require no argument, `EVC.exe` will require IP address and port (and eventualy ID of train): `./EVC.exe 192.168.1.101 4242 2`

See Makefile to more information.

[Youtube Videos](https://www.youtube.com/playlist?list=PLRohp2lBQ573M8j11N9RYrsu51QHPHMRh)