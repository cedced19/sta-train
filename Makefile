test-serveur:
	@echo "Lancement du serveur"
	cd rbc/ && make test

test-client: 
	@echo "Lancement du client"
	cd evc/ && make test

test-client-fake: 
	@echo "Lancement du client"
	cd evc/ && make test-fake

clean: 
	@echo "Cleaning..."
	cd rbc/ && make clean && cd ../evc/ && make clean