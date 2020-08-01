#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ip_subnet.h"

static int nearest_power_of_two(int num) {
    int pos = 0;

    if (!(num & (num - 1))) {
        return num;
    }

    while (num) {
        pos++;
        num >>= 1;
    }
    return (1 << pos);

}

int IP_GetFixedSubnetNumberFromStr(
        char *str,
        u_int32_t *fixed_subnet_num,
        int class) {

    *fixed_subnet_num = atoi(str);

    /* Check if number of subnets if not zero */
    if (!(*fixed_subnet_num)) {
        return 1;
    }
    /* Check if number of subnets required is power of two */
    if ((*fixed_subnet_num) & ((*fixed_subnet_num) - 1)) {
        return 1;
    }

    /* Check if the requirement is satisfiable */
    if (class == IP_CLASS_A) {
        if (*fixed_subnet_num > IP_CLASS_A_MAX_SUBNETS) {
            return 1;
        }
    }
    else if (class == IP_CLASS_B) {
        if (*fixed_subnet_num > IP_CLASS_B_MAX_SUBNETS) {
            return 1;
        }
    }
    else if (class == IP_CLASS_C) {
        if (*fixed_subnet_num > IP_CLASS_C_MAX_SUBNETS) {
            return 1;
        }
    }

    return 0;
}

void IP_PrintFixedSubnetTable(
        u_int32_t network_id,
        int class,
        int sub_num) {

    int i = 0;
    int sub_size;
    u_int32_t sub_mask;
    u_int32_t ip = network_id;

    /* Get the subnet size */
    if (class == IP_CLASS_A) {
        sub_size = IP_CLASS_A_MAX_ADDRESSES / sub_num;
    }
    else if (class == IP_CLASS_B) {
        sub_size = IP_CLASS_B_MAX_ADDRESSES / sub_num;
    }
    else if (class == IP_CLASS_C) {
        sub_size = IP_CLASS_C_MAX_ADDRESSES / sub_num;
    }
    else {
        return;
    }

    /* Find the subnet mask */
    sub_mask = ~(sub_size - 1);

    printf("Subnet No.\tSubnet ID\t\tBroadcast ID\t\tSubnet Mask\n");
    printf("==========\t=========\t\t============\t\t===========\n");

    /* Print the subnet IDs and broadcast IDs */
    while (i < sub_num) {
        /* Print the Subnet number */
        printf("%d\t\t", i + 1);

        /* Print the Subnet Id */
        printf("%u.%u.%u.%u\t\t",
               GET_OCTET_3(ip),
               GET_OCTET_2(ip),
               GET_OCTET_1(ip),
               GET_OCTET_0(ip));

        /* Get the Broadcast Id */
        ip += sub_size - 1;
        /* Print the Broadcast Id */
        printf("%u.%u.%u.%u\t\t",
               GET_OCTET_3(ip),
               GET_OCTET_2(ip),
               GET_OCTET_1(ip),
               GET_OCTET_0(ip));

        /* Print the Subnet Mask */
        printf("%u.%u.%u.%u\n",
               GET_OCTET_3(sub_mask),
               GET_OCTET_2(sub_mask),
               GET_OCTET_1(sub_mask),
               GET_OCTET_0(sub_mask));

        /* Get the next subnet Id */
        ip += 1;

        /* Increase the subnet count */
        i++;
    }
}

/* Add 2, get to the nearest power of two, check if it is valid and sort */
int IP_GetVariableSubnetNumberFromStr(
        char **str_list,
        int *var_list,
        int var_num,
        int class) {

    int total_hosts = 0;
    int i = 0;

    /* var_list = (int *)malloc(sizeof(int) * var_num); */

    /* Set the variable subnet host numbers + 2 (subnet Id, Broadcast ID) in
     * variable subnet list */
    while (i < var_num) {

        /* Get the requested number of hosts + 2 */
        var_list[i] = atoi(str_list[i]) + 2;

        /* Check if the number of required addresses is more than 2 */
        if (var_list[i] <= 2) {
            return 1;
        }

        var_list[i] = nearest_power_of_two(var_list[i]);;

        total_hosts += var_list[i];

        i++;
    }

    /* Check if requirement is valid */
    if (class == IP_CLASS_A) {
        if (total_hosts > IP_CLASS_A_MAX_ADDRESSES) {
            return 1;
        }
    }
    else if (class == IP_CLASS_B) {
        if (total_hosts > IP_CLASS_B_MAX_ADDRESSES) {
            return 1;
        }
    }
    else if (class == IP_CLASS_C) {
        if (total_hosts > IP_CLASS_C_MAX_ADDRESSES) {
            return 1;
        }
    }

    /* Comparison function required for qsort */
    int cmp(const void *ptr1, const void *ptr2) {
        int *p1 = (int *)ptr1;
        int *p2 = (int *)ptr2;

        return *p2 - *p1;
    }

    /* Sort the list in reverse order */
    qsort(var_list, var_num, sizeof(int), cmp);

    return 0;
}

void IP_PrintVariableSubnetTable(
        int net_ip,
        int class,
        int *var_list,
        int var_num) {

    /* For each of the host request print the subnet ID, mask and broadcast */
    int i = 0;
    u_int32_t ip = net_ip;
    u_int32_t sub_mask;

    printf("Subnet No.\tSubnet Size\tSubnet ID\t\tBroadcast ID\t\tSubnet Mask\n");
    printf("==========\t===========\t=========\t\t============\t\t===========\n");

    while (i < var_num) {
        /* Print the subnet number and subnet size */
        printf("%d\t\t%d\t\t", i + 1, var_list[i]);

        /* Print the subnet ID */
        printf("%u.%u.%u.%u\t\t",
               GET_OCTET_3(ip),
               GET_OCTET_2(ip),
               GET_OCTET_1(ip),
               GET_OCTET_0(ip));

        /* Get the subnet size */
        ip += var_list[i] - 1;

        /* Print the broadcast ID */
        printf("%u.%u.%u.%u\t\t",
               GET_OCTET_3(ip),
               GET_OCTET_2(ip),
               GET_OCTET_1(ip),
               GET_OCTET_0(ip));

        /* Get the subnet mask */
        sub_mask = ~(var_list[i] - 1);

        /* Print the subnet mask */
        printf("%u.%u.%u.%u\n",
               GET_OCTET_3(sub_mask),
               GET_OCTET_2(sub_mask),
               GET_OCTET_1(sub_mask),
               GET_OCTET_0(sub_mask));

        ip += 1;

        i++;
    }

}

int IP_GetIPFromStr(
        char *ip_str,
        u_int32_t *ip) {

    u_int8_t ip_arr[4];
    int ip_idx = 4;
    char *token;

    /* Tokenize the octets from the given ip string */
    token = strtok(ip_str, ".");
    while ((token) && (ip_idx)) {
        ip_arr[ip_idx - 1] = atoi(token);
        token = strtok(NULL, ".");
        ip_idx--;
    }

    if (token || ip_idx) {
        return 1;
    }

    /* Set the IP */
    *ip = (*(u_int32_t *)ip_arr);

    return 0;
}


int IP_GetNetworkIPAddr(char *ip_str, u_int32_t *ip, int *class) {

    /* Get the raw IP */
    if (IP_GetIPFromStr(ip_str, ip)) {
        return 1;
    }

    /* Check for the class of ip using the MSB octet */
    if (IS_IP_CLASS_A(*ip)) { /* Class A */
        /* Set the rest of the octets to zero to get the network ID */
        *ip = SET_OCTET_0(*ip, 0);
        *ip = SET_OCTET_1(*ip, 0);
        *ip = SET_OCTET_2(*ip, 0);
        *class = IP_CLASS_A;
    }
    else if (IS_IP_CLASS_B(*ip)) { /* Class B */
        /* Set the last two octets to zero */
        *ip = SET_OCTET_0(*ip, 0);
        *ip = SET_OCTET_1(*ip, 0);
        *class = IP_CLASS_B;
    }
    else if (IS_IP_CLASS_C(*ip)) { /* Class C */
        /* Set the last octet to zero */
        *ip = SET_OCTET_0(*ip, 0);
        *class = IP_CLASS_C;
    }
    else {
        return 1;
    }

    return 0;
}
