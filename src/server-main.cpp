#include "proxy-server.hpp"

int main(){
	ProxyServer myServer;
	myServer.run();
	myServer.shutdown();
}