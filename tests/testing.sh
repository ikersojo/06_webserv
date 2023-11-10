# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    testing.sh                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/01 17:27:17 by isojo-go          #+#    #+#              #
#    Updated: 2023/11/10 17:59:07 by jdasilva         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#! bin/zsh

#Check
./bin/webserv conf/prueba/Address_error_port.yml
echo "\033[0;93m ---------- Puerto mal escrito ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Address_error.yml
echo "\033[0;93m ---------- Direccion mal escrita ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Allow_empty.yml
echo "\033[0;93m ---------- Allow vacio ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Allow_error_1.yml
echo "\033[0;93m ---------- Opcion de Allow invalida (UP) ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Allow_error.yml
echo "\033[0;93m ---------- Mal configuracion de Allow ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Allow_repeated.yml
echo "\033[0;93m ---------- Configuracion repetida (GET) ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Bad_configuration_1.yml
echo "\033[0;93m ---------- No tiene listen: ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Bad_configuration.yml
echo "\033[0;93m ---------- No tiene location: ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Buffer_size_empty.yml
echo "\033[0;93m ---------- Buffer_size vacio ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Buffer_size_negativo.yml
echo "\033[0;93m ---------- Buffer_size numero negativo ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Buffer_size_no_number.yml
echo "\033[0;93m ---------- Buffer_size no es un numero ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Cgi_empty.yml
echo "\033[0;93m ---------- CGI vacio ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Empty.yml
echo "\033[0;93m ---------- 🖕 ARCHIVO VACIO 🖕 ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Error_page_empty_file.yml
echo "\033[0;93m ---------- El archivo Error_page vacio ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Error_page_empty_number.yml
echo "\033[0;93m ---------- Error_page no tiene numero ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Error_page_negative.yml
echo "\033[0;93m ---------- El numero de Error_page es negativo ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Error_page_no_number.yml
echo "\033[0;93m ---------- Error_page no es un numero ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/File_empty.yml
echo "\033[0;93m ---------- File: vacio ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/File_error.yml
echo "\033[0;93m ---------- File: error en la direccion del fichero ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Handle_delete_empty.yml
echo "\033[0;93m ---------- Handle_delete vacio ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Handle_post_empty.yml
echo "\033[0;93m ---------- Handle_post vacio ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Listen_empty.yml
echo "\033[0;93m ---------- Listen: vacio ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Location_empty.yml
echo "\033[0;93m ---------- Location: vacio ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Location_error.yml
echo "\033[0;93m ---------- La direccion de location mal configurada ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/No_empty_line.yml
echo "\033[0;93m ---------- Noy hay un espacio entre los servidores ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/No_server:.yml
echo "\033[0;93m ---------- No tiene server: al inicio de la configuracion ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Redirect_empty.yml
echo "\033[0;93m ---------- Redirect: vacio ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Repeated_port.yml
echo "\033[0;93m ---------- Se repite el puerto ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Root_empty.yml
echo "\033[0;93m ---------- Root: vacio ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Root_error_1.yml
echo "\033[0;93m ---------- La direccion root mal configurada ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Root_error.yml
echo "\033[0;93m ---------- La direccion root mal configurada ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Servername_empty.yml
echo "\033[0;93m ---------- Servername: vacio ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans
./bin/webserv conf/prueba/Wrong_space.yml
echo "\033[0;93m ---------- La linea tiene un error de espacio o tabulacion ----------\033[0;39m"
echo "\033[0;96m Press a key to continue\033[0;39m"
read ans

#Limpio la pantalla
clear 
 
echo "\033[0;92m ====== ☠ All Error tests completed! ☠️ ======"