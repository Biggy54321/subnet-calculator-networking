# Subnet calculator

* The program prints the subnet parameters of all the possible subnets of a given network.

* The following parameters are printed
    + Subnet ID
    + Broadcast ID
    + Subnet Mask

* There are two such programs, one which calculates the subnets considering the input network address is a classful address and other for classless addressing. Both the programs handle fixed or variable size of subnetting.

* Usage:
```
$>  cc ip_subnet.c main.c -o subnet

$>  ./subnet <ip_addr> <Fixed/Variable> <list_of_sizes_of_the_subnets>...

```