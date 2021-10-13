test-serveur:
	@echo "Lancement du serveur"
	cd RBC/ && make test

test-client: 
	@echo "Lancement du client"
	cd EVC/ && make test

test-client-fake: 
	@echo "Lancement du client"
	cd EVC/ && make test-fake

clean: 
	@echo "Cleaning..."
	cd RBC/ && make clean && cd ../EVC/ && make clean