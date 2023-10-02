# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    testing.sh                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/01 17:27:17 by isojo-go          #+#    #+#              #
#    Updated: 2023/10/01 17:29:47 by isojo-go         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# GET to be tested using web browser

# POST tests:
curl -i -X POST -d "param1=value1&param2=value2" http://localhost:61000
curl -i -X POST -H "Content-Type: application/json" -d '{"key":"value"}' http://localhost:61000

# DELETE posts