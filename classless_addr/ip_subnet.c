#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ip_subnet.h"

static int nearest_power_of_two(int num)
{
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
        CidrBlock cidr_ip)
{

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
    if (*fixed_subnet_num > GET_CIDR_MAX_SUBNETS(cidr_ip.cidr)) {
        return 1;
    }

    return 0;
}

void IP_PrintFixedSubnetTable(
        CidrBlock cidr_ip,
        int sub_num)
{

    int i = 0;
    int sub_size;
    u_int32_t sub_mask;
    u_int32_t ip = cidr_ip.ip;

    /* Get the subnet size */
    sub_size = GET_CIDR_BLOCK_SIZE(cidr_ip.cidr) / sub_num;

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
        CidrBlock cidr_ip)
{

    int total_hosts = 0;
    int i = 0;

    /* Set the variable subnet host numbers + 2 (subnet Id, Broadcast ID) in
     * variable subnet list */
    while (i < var_num) {

        /* Get the requested number of hosts + 2 */
        var_list[i] = atoi(str_list[i]) + 2;

        /* Check if the number of required addresses is more than 2 */
        if (var_list[i] <= 2) {
            return 1;
        }

        /* Round to upper power of two */
        var_list[i] = nearest_power_of_two(var_list[i]);;

        /* Update the total number of addresses required */
        total_hosts += var_list[i];

        i++;
    }

    /* Check if requirement is valid */
    if (total_hosts > GET_CIDR_BLOCK_SIZE(cidr_ip.cidr)) {
        return 1;
    }

    /* Comparison function required for qsort */
    int cmp(const void *ptr1, const void *ptr2)
    {
        int *p1 = (int *)ptr1;
        int *p2 = (int *)ptr2;

        return *p2 - *p1;
    }

    /* Sort the list in reverse order */
    qsort(var_list, var_num, sizeof(int), cmp);

    return 0;
}

void IP_PrintVariableSubnetTable(
        CidrBlock cidr_ip,
        int *var_list,
        int var_num)
{

    /* For each of the host request print the subnet ID, mask and broadcast */
    int i = 0;
    u_int32_t ip = cidr_ip.ip;
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

int IP_GetCidrBlockAddrFromStr(
        char *cidr_ip_str,
        CidrBlock *cidr_ip)
{

    u_int8_t ip_arr[4];
    u_int8_t cidr;
    int ip_idx = 4;
    char *token;

    /* Tokenize the octets from the given ip string */
    token = strtok(cidr_ip_str, ".");
    while ((token) && (ip_idx)) {
        /* Convert the string octet to integer */
        ip_idx--;
        ip_arr[ip_idx] = atoi(token);
        /* Tokenize on '.' if there are more octets else tokenize on '/' */
        if (ip_idx > 1) {
            token = strtok(NULL, ".");
        }
        else {
            token = strtok(NULL, "/");
        }
    }

    /* Get the cidr number */
    if (token) {
        cidr = atoi(token);
    }
    else {
        return 1;
    }

    /* Check if token and ip_idx are both weared out */
    token = strtok(NULL, " ");
    if (token || ip_idx) {
        return 1;
    }

    /* Set the IP */
    cidr_ip->ip = (*(u_int32_t *)ip_arr);
    cidr_ip->cidr = cidr;

    return 0;
}


int IP_GetCidrBlockAddr(
        char *cidr_ip_str,
        CidrBlock *cidr_ip)
{

    /* Get the raw cidr block address */
    if (IP_GetCidrBlockAddrFromStr(cidr_ip_str, cidr_ip)) {
        return 1;
    }

    /* Check if the cidr number is valid */
    if (cidr_ip->cidr > 32) {
        return 1;
    }

    /* Set the suffix bits in the address to all zero */
    cidr_ip->ip = cidr_ip->ip & GET_CIDR_BLOCK_MASK(cidr_ip->cidr);

    return 0;
}
