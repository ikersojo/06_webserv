# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    testing2.sh                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/10 19:12:53 by jdasilva          #+#    #+#              #
#    Updated: 2023/11/10 19:16:32 by jdasilva         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#! bin/zsh

primero=true

for archivo in $(find conf/prueba -name "*.yml"); do
  if [ "$primero" = true ]; then
    ./bin/webserv "$archivo" > tests/test.logs
    primero=false
  else
    ./bin/webserv "$archivo" >> tests/test.logs
  fi
done
echo "\033[0;92m ====== ☠ All Error tests completed! ☠️ ======"