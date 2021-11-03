RBC_PORT = 4242
RBC_IP = 192.168.1.103


test-serveur:
	@echo "Lancement du serveur"
	cd rbc/ && RBC_PORT=$(RBC_PORT) make test

compile-client-1: 
	@echo "Compilation du client"
	sshpass -p "raspberry" ssh pi@192.168.1.21 "cd evc/ && cmake . && make"

test-client-1: 
	@echo "Lancement du client"
	sshpass -p "raspberry" ssh pi@192.168.1.21 "cd evc/ && ./EVC.exe $(RBC_IP) $(RBC_PORT)"

connect-client-1: 
	@echo "Connection au client"
	sshpass -p "raspberry" ssh pi@192.168.1.21

compile-client-2: 
	@echo "Compilation du client"
	sshpass -p "raspberry" ssh pi@192.168.1.31 "cd evc/ && cmake . && make"

test-client-2: 
	@echo "Lancement du client"
	sshpass -p "raspberry" ssh pi@192.168.1.31 "cd evc/ && ./EVC.exe $(RBC_IP) $(RBC_PORT)"

connect-client-2: 
	@echo "Connection au client"
	sshpass -p "raspberry" ssh pi@192.168.1.31

test-client-fake: 
	@echo "Lancement du client"
	cd evc/ && make test-fake

test-client-fake-2: 
	@echo "Lancement du client"
	cd evc/ && make test-fake-2

clean: 
	@echo "Cleaning..."
	cd rbc/ && make clean && cd ../evc/ && make clean