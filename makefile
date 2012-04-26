# Variabili d'ambiente modificabili

ServiceRegisterAddress      := "127.0.0.1"
ServiceRegisterPort         := "12345"
ImageManipulationServerPort := "17171"
ImageStoringServerPort      := "34343"
ClientIterations            := "1"

################## Non modificare quanto segue ##################

COMPILE=@g++ -Wall -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -lboost_thread-mt SOA/*.cpp Application/*.cpp
COMPILING_MESSAGE=Compiling

# Varibili d'uso
ImageManipulationServerCommand :="ImageManipulationServer ${ImageManipulationServerPort} ${ServiceRegisterAddress} ${ServiceRegisterPort}"
ImageStoringServerCommand      :="ImageStoringServer ${ImageStoringServerPort} ${ServiceRegisterAddress} ${ServiceRegisterPort}"
ServiceRegisterCommand         :="ServiceRegister ${ServiceRegisterPort}"
ClientCommand                  :="Client ${ClientIterations} ${ServiceRegisterAddress} ${ServiceRegisterPort}"

# Settaggio dell'ambiente di xterm
XtermCommand=@xterm -hold -fg green -bg black -leftbar -geometry

all:
	@clear
	@echo "***********************************************"
	@echo "*        Service Oriented Architecture        *"
	@echo "***********************************************"
	@echo ""
	@make -s compile
	@make -s directories
	@make -s execution
	@echo ""
	@echo "***********************************************"
	@echo ""

compile:
	@echo "\033[1;34mCompiling sources\033[0m"
	@make ImageManipulationServer ImageStoringServer ServiceRegister Client
	@echo "${COMPILING_MESSAGE} process complete."
	@echo ""

directories: Clients Images Servers Servers/ImageManipulationServer Servers/ImageStoringServer

Clients:
	@mkdir -p Clients
Images:
	@mkdir -p Images
Servers:
	@mkdir -p Servers
Servers/ImageManipulationServer:
	@mkdir -p Servers/ImageManipulationServer
Servers/ImageStoringServer:
	@mkdir -p Servers/ImageStoringServer

ImageManipulationServer: SOA/*.cpp Application/*.cpp ImageManipulationServer.cpp
	@echo "${COMPILING_MESSAGE} Image Manipulation Server"
	${COMPILE} ImageManipulationServer.cpp -o ImageManipulationServer

ImageStoringServer: SOA/*.cpp Application/*.cpp ImageStoringServer.cpp
	@echo "${COMPILING_MESSAGE} Image Storing Server"
	${COMPILE} ImageStoringServer.cpp -o ImageStoringServer

ServiceRegister: SOA/*.cpp Application/*.cpp ServiceRegister.cpp
	@echo "${COMPILING_MESSAGE} Service Register"
	${COMPILE} ServiceRegister.cpp -o ServiceRegister

Client: SOA/*.cpp Application/*.cpp Client.cpp
	@echo "${COMPILING_MESSAGE} Client"
	${COMPILE} Client.cpp -o Client

execution: ImageManipulationServer ImageStoringServer ServiceRegister Client
	@echo "\033[1;34mExecuting application\033[0m"
	@xterm -e "echo Pre-fetching xterm"
	@echo "Launching Service Register"
	${XtermCommand} 70x45+0+0 -e "./${ServiceRegisterCommand}" &
	@sleep 2
	@echo "Launching Image Manipulation Server"
	${XtermCommand} 74x23+400+0 -e "./${ImageManipulationServerCommand}" &
	@sleep 0.1
	@echo "Launching Image Storing Server"
	${XtermCommand} 70x23+800+0 -e "./${ImageStoringServerCommand}" &
	@sleep 2
	@echo "Launching Clients"
	${XtermCommand} 80x30+100+100 -e "./${ClientCommand}" & sleep 0.1
	${XtermCommand} 80x30+200+120 -e "./${ClientCommand}" & sleep 0.1
	${XtermCommand} 80x30+300+130 -e "./${ClientCommand}" & sleep 0.1
	${XtermCommand} 80x30+400+140 -e "./${ClientCommand}" & sleep 0.1
	${XtermCommand} 80x30+500+150 -e "./${ClientCommand}" & sleep 0.1
