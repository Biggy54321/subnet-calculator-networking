#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ip_subnet.h"

int main(int argc, char *argv[]) {

    CidrBlock cidr_ip;
    int subnet_type;
    int fixed_subnet_num;
    int variable_subnet_num;
    int *variable_subnet_list;

    /* Get the cidr block address */
    if (IP_GetCidrBlockAddr(argv[1], &cidr_ip)) {
            printf("Error Occurred\n");
            return 0;
    }

    /* Get the type of subnet */
    if (!strcmp(argv[2], "Fixed")) {
        subnet_type = 0;
    }
    else if (!strcmp(argv[2], "Variable")) {
        subnet_type = 1;
    }
    else {
        printf("Error Occurred\n");
        return 0;
    }

    if (subnet_type == 0) {
        if (IP_GetFixedSubnetNumberFromStr(argv[3], &fixed_subnet_num, cidr_ip)) {
            printf("Error Occurred\n");
            return 0;
        }

        IP_PrintFixedSubnetTable(cidr_ip, fixed_subnet_num);
    }
    else if (subnet_type == 1) {

        /* Create the variable subnet request array */
        variable_subnet_num = argc - 3;
        variable_subnet_list = (int *)malloc(sizeof(int) * variable_subnet_num);

        if (IP_GetVariableSubnetNumberFromStr(argv + 3,
                                              variable_subnet_list,
                                              variable_subnet_num,
                                              cidr_ip)) {
            printf("Error Occurred\n");
            return 0;
        }

        IP_PrintVariableSubnetTable(cidr_ip,
                                    variable_subnet_list,
                                    variable_subnet_num);

        free(variable_subnet_list);
    }

    return 0;
}
