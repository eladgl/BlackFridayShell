# BlackFridayShell
shell command line, supports all commands from /bin directory. Allow companies to create coupons to their store and clients to make orders

The main files are Advanced_Shell.c Black.c and Black.h
This project works only if you have the directory /home/braude/ you can either change it in each file to whatever directory you want

Advanced_Shell is the shell where the user/company will create orders/couponss. it supports all /bin commands and will exit if and only if exit wsa entered.
Upon run it creates a directory called Black_Friday in home/braude/ and upon exit deletes it.
The extra commands the shell support: CreateFlyer, GetFlyer, MakeOrder, GetNumComp, GetOrderNum, GetSummary and exit.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CreateFlyer - Takes two agruments. Company name and a discount in int.
             Can create a single coupon for each company. The coupon will be saved in a directory named as the company.
             The company name will be added to a file named camp_partics.txt as a company registered in the campain.
             The coupon has a unique print so there will be a csv file for each company coupon made saving the products and their prices in a nice manner to work with
             later on.
             Upon saving the coupon the coupon maker could choose a nice ending to it at the end from aa given list or something of their own.
------------------------------------------------------------------------------------------------------------------------------------------------------------------------
GetFlyer    - Takes company name, if coupn exitsts prints the coupon other wise print Copany Not Found!
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
MakeOrder    Takes company name and who orders. Creates a csv file with the order information, what was ordered and how much of it. Also creates a file in the Order 
             of the company.
             At the end shows how much the total is and asks for conirmation.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
GetNumComp  - Prints how many companies are in the campaign
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
GetOrderNum - Prints how many orders a company has
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
GetSummary  - Prints how many orders each company has and the total of orders overall
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
exit        - Exits from the shell, recursively deleting everything the system created
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------


This project denonstartes capabilities of working with the fcntl.h library whereas also working with multiple processes (forks and execs that are made along the way) and working with different authorization levels in linux.

There is also a file named Advanced_Shell.sh that compiles all the projects together and gives authorization to run and execute all the commands needed for the ordereing system.
