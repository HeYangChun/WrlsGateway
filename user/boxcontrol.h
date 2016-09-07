#ifndef BOXCONTROL_H_
#define BOXCONTROL_H_

#define BOX_NUM_MAX 60  // ���֧�ֵ�������

//******************************************************************************
//������  ��Box_Init
//������������ʼ�����Ĵ���Ŀ��ƽӿ�
//����    ����
//���    ����
//����ֵ  ����
//******************************************************************************
extern void Box_Init(void);


//******************************************************************************
//������  ��Box_OpenDoor
//���������������ţ�ͬʱ������
//����    ��box_num   ���ӱ��
//���    ����
//����ֵ  ����
//******************************************************************************
extern void Box_OpenDoor(u8 box_num);


//******************************************************************************
//������  ��Box_OpenDoors
//������������ָ�������ţ�ͬʱ������
//����    ��box_nums[]   Ҫ�򿪵����ӱ��
//          len          ���ӵĸ���
//���    ����
//����ֵ  ����
//******************************************************************************
extern void Box_OpenDoors(u8 box_nums[], u8 len);


//******************************************************************************
//������  ��Box_SetLockDly
//�������������ÿ���ͨ��ʱ��
//����    ��dly   ͨ��ʱ�䣬��λ��
//���    ����
//����ֵ  ����
//******************************************************************************
extern void Box_SetLockDly(u8 dly);


//******************************************************************************
//������  ��Box_GetLockDly
//������������ȡ����ͨ��ʱ��
//����    ����
//���    ����
//����ֵ  ��ͨ��ʱ�䣬��λ��
//******************************************************************************
extern u8 Box_GetLockDly(void);


//******************************************************************************
//������  ��Box_OpenLight
//���������������������Ƶ�״̬
//����    ��box_num   ���ӱ��
//���    ����
//����ֵ  ����
//******************************************************************************
extern void Box_OpenLight(u8 box_num);


//******************************************************************************
//������  ��Box_CloseLight
//�����������ر����������Ƶ�״̬
//����    ��box_num   ���ӱ��
//���    ����
//����ֵ  ����
//******************************************************************************
extern void Box_CloseLight(u8 box_num);


//******************************************************************************
//������  ��Box_SetLightDly
//������������������ʱ��
//����    ��dly   ����ʱ�䣬��λ��
//���    ����
//����ֵ  ����
//******************************************************************************
extern void Box_SetLightDly(u8 dly);


//******************************************************************************
//������  ��Box_GetLightDly
//������������ȡ����ʱ��
//����    ����
//���    ����
//����ֵ  ������ʱ�䣬��λ��
//******************************************************************************
extern u8 Box_GetLightDly(void);


//******************************************************************************
//������  ��Box_IsDoorClosed
//����������������ŵ�״̬
//����    ��box_num   ���ӱ��
//���    ����
//����ֵ  ��0�����Ӵ�  1�����ӹر�
//******************************************************************************
extern u8 Box_IsDoorClosed(u8 box_num);


//******************************************************************************
//������  ��Box_IsAllDoorClosed
//��������������������ӵ�����״̬
//����    ����
//���    ��sta   ������ӵ�����״̬
//                STA[0]��1~8�����ӵ�����״̬��bit0��ʾ1�����ӵ�����״̬��
//                bit7��ʾ8�����ӵ�����״̬��0:���Ŵ�  1:���Źر�
//                STA[N]��8*N+1~8*(N+1)�����ӵ�����״̬����������ͬ��
//����ֵ  ��sta�е����ݸ���
//******************************************************************************
extern u8 Box_IsAllDoorClosed(u8 sta[]);


//******************************************************************************
//������  ��Box_IsLightOn
//���������������������״̬
//����    ��box_num   ���ӱ��
//���    ����
//����ֵ  ��0�������ر�  1��������
//******************************************************************************
extern u8 Box_IsLightOn(u8 box_num);


//******************************************************************************
//������  ��Box_IsAllLightOn
//��������������������ӵ�����״̬
//����    ����
//���    ��sta   ������ӵ�����״̬
//                STA[0]��1~8�����ӵ�����״̬��bit0��ʾ1�����ӵ�����״̬��
//                bit7��ʾ8�����ӵ�����״̬��0�������ر�  1��������
//                STA[N]��8*N+1~8*(N+1)�����ӵ�����״̬����������ͬ��
//����ֵ  ��sta�е����ݸ���
//******************************************************************************
extern u8 Box_IsAllLightOn(u8 sta[]);


//******************************************************************************
//������  ��Box_HaveObject
//������������������Ƿ�������
//����    ��box_num   ���ӱ��
//���    ����
//����ֵ  ��0������������  1������������
//******************************************************************************
extern u8 Box_HaveObject(u8 box_num);


//******************************************************************************
//������  ��Box_IsAllHaveObject
//����������������������Ƿ�������
//����    ����
//���    ��sta   ������ӵ�����״̬
//                STA[0]��1~8�����ӵ�����״̬��bit0��ʾ1�����ӵ�����״̬��
//                bit7��ʾ8�����ӵ�����״̬��0������������  1������������
//                STA[N]��8*N+1~8*(N+1)�����ӵ�����״̬����������ͬ��
//����ֵ  ��sta�е����ݸ���
//******************************************************************************
extern u8 Box_IsAllHaveObject(u8 sta[]);


//******************************************************************************
//������  ��Box_SetBoxRemap
//�����������������ӵ���ӳ��
//����    ��box_num   ���ӱ��
//          remap     ��ӳ��ı��
//���    ����
//����ֵ  ����
//******************************************************************************
extern void Box_SetBoxRemap(u8 box_num, u8 remap);


//******************************************************************************
//������  ��Box_GetRemapTable
//������������ȡ���ӵ�ӳ���
//����    ����
//���    ��table   ��ȡ����ӳ���
//����ֵ  ��table�����ݵĸ���
//******************************************************************************
extern u8 Box_GetRemapTable(u8 table[]);


//******************************************************************************
//������  ��Box_ResetRemap
//�����������������ӵ�ӳ���
//����    ����
//���    ����
//����ֵ  ����
//******************************************************************************
extern void Box_ResetRemap(void);

//******************************************************************************
//������  ��Box_RunPer100MS
//������������Ȧͨ�缰�����ļ�ʱ������ÿ0.1��ִ��һ��
//����    ����
//���    ����
//����ֵ  ����
//******************************************************************************
extern void Box_RunPer100MS(void);

#endif