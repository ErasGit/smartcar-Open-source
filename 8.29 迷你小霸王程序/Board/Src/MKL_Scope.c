/*
* @file         MKL_Scope.c
  * @brief      ɽ������ʾ��������
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-23
*/

#include "common.h"
#include "MKL_Scope.h"
#include "MKL_uart.h"

/*
 *  @brief      ����ʾ������������
    wareaddr    Ҫ���͵�����ĵ�ַ
    waresize    Ҫ���͵�����ĳ���
    example :   vcan_scope((uint8_t *)var, sizeof(var)); 
 *  @since      v1.0
*/
void vcan_scope(uint8 *wareaddr, uint32 waresize)
{
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //���ڵ��� ʹ�õ�ǰ����
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //���ڵ��� ʹ�õĺ�����

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //�ȷ���ǰ����
    uart_putbuff(VCAN_PORT, wareaddr, waresize);    //��������
    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //���ͺ�����

}