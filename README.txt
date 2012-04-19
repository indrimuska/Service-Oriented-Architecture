---------------------------------------
README.txt
---------------------------------------

Indice:

  1. makefile: compilazione dei file ed esecuzione dell'applicazione

  3. Informazioni utili per l'esecuzione

**************************************************************************************

1. makefile

Per la compilazione del progetto è presente un makefile che crea i seguenti file
eseguibili:
  - ImageManipulationServer
  - ImageStoringServer
  - ServiceRegister
  - Client
Attraverso il comando "make" è possibile automatizzare la compilazione dell'applica-
zione garantendo che tutte le opzioni di compilazione vengano inserite.

La dimostrazione (ottenibile direttamente attraverso il comando 'make exec') consi-
ste nell'avvio di un Service Register e dei due server Image Manipulation Server e
Image Storing Server, i quali si registrano nel registro dei servizi insieme ai ser-
vizi che offrono.
In seguito vengono avviati 5 client che in parallelo interrogano il Service Register
per l'indirizzo dei server in grado di fornire i servizi richiesti permettendo di in-
teragire direttamente con i Service Provider.

--------------------------------------------------------------------------------------

2. Informazioni utili per l'esecuzione

Per facilitare l'esecuzione dell'applicazione e velocizzare l'inserimento dei parame-
tri sono presenti le seguenti variabili:
  - ServiceRegisterAddress: identifica l'indirizzo IP del Service Register
  - ServiceRegisterPort: identifica la porta di ascolto del Service Register
  - ImageManipulationServerPort: identifica la porta di ascolto dell'Image Manipula-
                                 tion Server
  - ImageStoringServerPort: identifica la porta di ascolto dell'Image Storing Server
  - ClientIterations: numero di iterazioni da far eseguire a ciascun client

Questo possono essere modificate per permettere l'esecuzione dell'applicativo in tutti
quei casi in cui sono presenti problemi non dovuti al progetto (ad esempio, una chiu-
sura errata degli eseguibili può comportare che un rocket rimanga aperto e non sia
disponibile la porta di ascolto su cui impostare i server)

--------------------------------------------------------------------------------------

Progetto:
  Service Oriented Architecture

Autori:
  Indri Muska <indrimuska@gmail.com>
  Paolo Antonio Rossi <paoloantoniorossi@gmail.com>
