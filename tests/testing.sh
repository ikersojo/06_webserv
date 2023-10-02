# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    testing.sh                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: isojo-go <isojo-go@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/01 17:27:17 by isojo-go          #+#    #+#              #
#    Updated: 2023/10/02 10:37:01 by isojo-go         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# GET to be tested using web browser. Additional tests:
curl -v http://localhost:61000

# POST tests:
curl -i POST -d "xxxxxxxxxxxxxx" http://localhost:61000

# DELETE posts