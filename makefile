
# Variabili d'ambiente modificabili (sezione modificabile)

     ServiceRegisterAddress="127.0.0.1"
        ServiceRegisterPort="12345"
ImageManipulationServerPort="17171"
	 ImageStoringServerPort="34343"

####################################### Non modificare quanto segue ###

GCC=@g++
CFLAGS=-Wall -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -lboost_thread-mt SOA/*.cpp Application/*.cpp
COMPILING_MESSAGE=Compiling

# Varibili d'uso
ImageManipulationServerCommand="ImageManipulationServer ${ImageManipulationServerPort} ${ServiceRegisterAddress} ${ServiceRegisterPort}"
     ImageStoringServerCommand="ImageStoringServer ${ImageStoringServerPort} ${ServiceRegisterAddress} ${ServiceRegisterPort}"
        ServiceRegisterCommand="ServiceRegister ${ServiceRegisterPort}"
                 ClientCommand="Client ${ServiceRegisterAddress} ${ServiceRegisterPort}"

# Settaggio dell'ambiente di xterm
XtermCommand=@xterm -fg green -bg black -leftbar -geometry

all:
	@clear
	@echo "***********************************************"
	@echo "*        Service Oriented Architecture        *"
	@echo "***********************************************"
	@echo ""
	@make -s compile
	@make exec

compile: ImageManipulationServer ImageStoringServer ServiceRegister Client
	@echo "\033[1;34mCompiling source\033[0m"
	@make ImageManipulationServer ImageStoringServer ServiceRegister Client
	@echo "${COMPILING_MESSAGE} process complete."

ImageManipulationServer: ImageManipulationServer.cpp
	@echo "${COMPILING_MESSAGE} Image Manipulation Server"
	${GCC} ${CFLAGS} ImageManipulationServer.cpp -o ImageManipulationServer

ImageStoringServer: ImageStoringServer.cpp
	@echo "${COMPILING_MESSAGE} Image Storing Server"
	${GCC} ${CFLAGS} ImageStoringServer.cpp -o ImageStoringServer

ServiceRegister: ServiceRegister.cpp
	@echo "${COMPILING_MESSAGE} Service Register"
	${GCC} ${CFLAGS} ServiceRegister.cpp -o ServiceRegister

Client: Client.cpp
	@echo "${COMPILING_MESSAGE} Client"
	${GCC} ${CFLAGS} Client.cpp -o Client

exec: ImageManipulationServer ImageStoringServer ServiceRegister Client
	@echo "\033[1;34mExecuting application\033[0m"
	@xterm -e "echo Pre-fetching xterm"
	@echo "Launching Service Register"
	${XtermCommand} 70x45+0+0 -e "./${ServiceRegisterCommand}" &
	@sleep 2
	@echo "Launching Image Modification Server"
	${XtermCommand} 72x23+420+0 -e "./${ImageManipulationServerCommand}" &
	@sleep 0.1
	@echo "Launching Image Storing Server"
	${XtermCommand} 70x23+840+0 -e "./${ImageStoringServerCommand}" &
	@sleep 2
	@echo "Launching Clients"
	${XtermCommand} 100x30+100+100 -e "./${ClientCommand}; sleep 30" &
	${XtermCommand} 100x30+200+120 -e "./${ClientCommand}; sleep 30" &
	${XtermCommand} 100x30+300+130 -e "./${ClientCommand}; sleep 30" &
	${XtermCommand} 100x30+400+140 -e "./${ClientCommand}; sleep 30" &
	${XtermCommand} 100x30+500+150 -e "./${ClientCommand}; sleep 30" &
	@echo ""
