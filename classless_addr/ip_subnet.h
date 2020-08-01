/*
 * Header Guard
 */
#ifndef _IP_SUBNET_H_
#define _IP_SUBNET_H

/*
 * CIDR Block Structure
 */
typedef struct _CidrBlock {
    u_int32_t ip;
    u_int8_t cidr;
} CidrBlock;

/*
 * IP Octets Base Positions
 */
#define OCTET_0_BASE_POS          (0u)
#define OCTET_1_BASE_POS          (8u)
#define OCTET_2_BASE_POS          (16u)
#define OCTET_3_BASE_POS          (24u)

/*
 * IP Octets Masks
 */
#define OCTET_0_MASK              (0x000000ff)
#define OCTET_1_MASK              (0x0000ff00)
#define OCTET_2_MASK              (0x00ff0000)
#define OCTET_3_MASK              (0xff000000)

/*
 * Set IP Octets
 */
#define SET_OCTET_0(ip_addr, val)                       \
    (((ip_addr) & ~(OCTET_0_MASK)) | ((val) << OCTET_0_BASE_POS))
#define SET_OCTET_1(ip_addr, val)                       \
    (((ip_addr) & ~(OCTET_1_MASK)) | ((val) << OCTET_1_BASE_POS))
#define SET_OCTET_2(ip_addr, val)                       \
    (((ip_addr) & ~(OCTET_2_MASK)) | ((val) << OCTET_2_BASE_POS))
#define SET_OCTET_3(ip_addr, val)                       \
    (((ip_addr) & ~(OCTET_3_MASK)) | ((val) << OCTET_3_BASE_POS))

/*
 * Get IP Octets
 */
#define GET_OCTET_0(ip_addr)                    \
    ((((u_int32_t)(ip_addr)) & OCTET_0_MASK) >> OCTET_0_BASE_POS)
#define GET_OCTET_1(ip_addr)                    \
    ((((u_int32_t)(ip_addr)) & OCTET_1_MASK) >> OCTET_1_BASE_POS)
#define GET_OCTET_2(ip_addr)                    \
    ((((u_int32_t)(ip_addr)) & OCTET_2_MASK) >> OCTET_2_BASE_POS)
#define GET_OCTET_3(ip_addr)                    \
    ((((u_int32_t)(ip_addr)) & OCTET_3_MASK) >> OCTET_3_BASE_POS)

/*
 * CIDR Block specifics
 */
#define GET_CIDR_BLOCK_SIZE(cidr)               \
    (1 << (32 - (u_int32_t)(cidr)))
#define GET_CIDR_BLOCK_MASK(cidr)               \
    (~((GET_CIDR_BLOCK_SIZE(cidr)) - 1))
#define GET_CIDR_MAX_SUBNETS(cidr)              \
    (GET_CIDR_BLOCK_SIZE(cidr) >> 2)

/*
 * Functions
 */
int IP_GetFixedSubnetNumberFromStr(
        char *str,
        u_int32_t *fixed_subnet_num,
        CidrBlock cidr_ip);

void IP_PrintFixedSubnetTable(
        CidrBlock cidr_ip,
        int sub_num);

int IP_GetVariableSubnetNumberFromStr(
        char **str_list,
        int *var_list,
        int var_num,
        CidrBlock cidr_ip);

void IP_PrintVariableSubnetTable(
        CidrBlock cidr_ip,
        int *var_list,
        int var_num);

int IP_GetCidrBlockAddrFromStr(
        char *cidr_ip_str,
        CidrBlock *cidr_ip);

int IP_GetCidrBlockAddr(
        char *cidr_ip_str,
        CidrBlock *cidr_ip);

#endif
