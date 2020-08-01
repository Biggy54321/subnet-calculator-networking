/*
 * Header Guard
 */
#ifndef _IP_SUBNET_H_
#define _IP_SUBNET_H

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
    ((((u_int32_t)ip_addr) & OCTET_0_MASK) >> OCTET_0_BASE_POS)
#define GET_OCTET_1(ip_addr)                    \
    ((((u_int32_t)ip_addr) & OCTET_1_MASK) >> OCTET_1_BASE_POS)
#define GET_OCTET_2(ip_addr)                    \
    ((((u_int32_t)ip_addr) & OCTET_2_MASK) >> OCTET_2_BASE_POS)
#define GET_OCTET_3(ip_addr)                    \
    ((((u_int32_t)ip_addr) & OCTET_3_MASK) >> OCTET_3_BASE_POS)

/*
 * IP Class A Information
 */
#define IP_CLASS_A                (0)
#define IP_CLASS_A_START          (0x00)
#define IP_CLASS_A_END            (0x7f)
#define IP_CLASS_A_MAX_ADDRESSES  (1 << 24)
#define IP_CLASS_A_MAX_SUBNETS    (1 << (24 - 2))

/*
 * IP Class B Information
 */
#define IP_CLASS_B                (1)
#define IP_CLASS_B_START          (0x80)
#define IP_CLASS_B_END            (0xbf)
#define IP_CLASS_B_MAX_ADDRESSES  (1 << 16)
#define IP_CLASS_B_MAX_SUBNETS    (1 << (16 - 2))

/*
 * IP Class C Information
 */
#define IP_CLASS_C                (2)
#define IP_CLASS_C_START          (0xc0)
#define IP_CLASS_C_END            (0xdf)
#define IP_CLASS_C_MAX_ADDRESSES  (1 << 8)
#define IP_CLASS_C_MAX_SUBNETS    (1 << (8 - 2))

/*
 * IP CLASS CHECK PREDICATES
 */
#define IS_IP_CLASS_A(ip_addr)                          \
    ((GET_OCTET_3(ip_addr) >= IP_CLASS_A_START) &&      \
     (GET_OCTET_3(ip_addr) <= IP_CLASS_A_END))
#define IS_IP_CLASS_B(ip_addr)                          \
    ((GET_OCTET_3(ip_addr) >= IP_CLASS_B_START) &&      \
     (GET_OCTET_3(ip_addr) <= IP_CLASS_B_END))
#define IS_IP_CLASS_C(ip_addr)                          \
    ((GET_OCTET_3(ip_addr) >= IP_CLASS_C_START) &&      \
     (GET_OCTET_3(ip_addr) <= IP_CLASS_C_END))

int IP_GetFixedSubnetNumberFromStr(
        char *str,
        u_int32_t *fixed_subnet_num,
        int class);

void IP_PrintFixedSubnetTable(
        u_int32_t network_id,
        int class,
        int sub_num);

int IP_GetVariableSubnetNumberFromStr(
        char **str_list,
        int *var_list,
        int var_num,
        int class);

void IP_PrintVariableSubnetTable(
        int net_ip,
        int class,
        int *var_list,
        int var_num);

int IP_GetIPFromStr(
        char *ip_str,
        u_int32_t *ip);

int IP_GetNetworkIPAddr(
        char *ip_str
        , u_int32_t *ip,
        int *class);

#endif
