/***
*AepServiceCodes.h - 定义上下行数据的结构体，还有提供组装上行报文的函数和解析下行报文的函数
*
*Purpose:
*	1.数据结构体命名和平台定义的服务标识一致
*	2.codeDataReportByIdToStr、codeDataReportByIdToBytes、codeDataReportByIdentifierToStr、codeDataReportByIdentifierToBytes为组装上报数据的函数，具体说明见函数前的注释
*	3.decodeCmdDownFromStr、decodeCmdDownFromBytes为解析平台发送过来数据的函数，具体说明见函数前的注释
****/
#ifndef AEPSERVICECODES_H
#define AEPSERVICECODES_H
/*///////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <string.h>


#define AEP_BIG_ENDIAN 'b'
#define AEP_LITTLE_ENDIAN 'l'

static union { char c[4]; unsigned long mylong; } endian_test = {{ 'l', '?', '?', 'b' } };
#define AEP_ENDIANNESS ((char)endian_test.mylong)


typedef unsigned long long uint_64;
typedef unsigned int uint_32;  
typedef unsigned short uint_16;

//命令解析响应码
#define AEP_CMD_SUCCESS 0						//执行成功
#define AEP_CMD_FAILED 1						//执行失败
#define AEP_CMD_INVALID_DATASET_TYPE 2			//无效数据集类型
#define AEP_CMD_INVALID_DATASET_IDENTIFIER 3	//无效数据集标识
#define AEP_CMD_PAYLOAD_PARSING_FAILED 4		//指令数据集Payload解析失败,紧凑二进制编码内容长度不符等


typedef struct AepStrStruct
{
	unsigned short len;
	char* str;
} AepString;
typedef AepString AepBytes;

//无符号整型16位  
uint_16 aep_htons(uint_16 source);

//无符号整型32位
uint_32 aep_htoni(uint_32 source);

//无符号整型64位
uint_64 aep_htonl(uint_64 source);

//float
float aep_htonf(float source);

//double
double aep_htond(double source);

//16进制转字符串
void HexToStr(char *pbDest, char *pbSrc, int nLen);

//字符串转16进制
void StrToHex(char *pbDest, char *pbSrc, int nLen);


//根据服务id生成上报数据的十六进制字符串,srcStruct需要根据服务定义传入对应的类型,返回结果为字符串
AepString codeDataReportByIdToStr(int serviceId, void * srcStruct);

//根据服务id生成上报数据的字节流,srcStruct需要根据服务定义传入对应的类型,返回结果为字节流
AepBytes codeDataReportByIdToBytes(int serviceId, void * srcStruct);

//根据服务标识生成上报数据的十六进制字符串,srcStruct需要根据服务定义传入对应的类型,返回结果为字符串
AepString codeDataReportByIdentifierToStr(char * serviceIdentifier, void * srcStruct);

//根据服务标识生成上报数据的字节流,srcStruct需要根据服务定义传入对应的类型,返回结果为字节流
AepBytes codeDataReportByIdentifierToBytes(char * serviceIdentifier, void * srcStruct);

//指令解析返回结构体，data在使用时需要根据serviceId强转为对应类型
typedef struct CmdStruct 
{
	char* serviceIdentifier;
	unsigned short taskId;
	void * data;
	int code;
} AepCmdData;
//解析接受到的报文数据,入参为十六进制字符串
AepCmdData decodeCmdDownFromStr(char* source);
//解析接受到的报文数据,入参为原始字节流
AepCmdData decodeCmdDownFromBytes(char* source, int len);



typedef struct motor_controlStruct 
{
	int control_int;
} motor_control;
//指令下发:电机控制
int motor_control_DecodeCmdDown (char* source, motor_control* dest);


typedef struct motor_control_ackStruct 
{
	unsigned short taskId;
	int control_int;
} motor_control_ack;
//指令下发响应:电机控制响应
AepString motor_control_ack_CodeCmdResponse (motor_control_ack srcStruct);


typedef struct report_controlStruct 
{
	int control_int;
} report_control;
//指令下发:上报控制
int report_control_DecodeCmdDown (char* source, report_control* dest);


typedef struct report_control_ackStruct 
{
	unsigned short taskId;
	int control_int;
} report_control_ack;
//指令下发响应:上报控制响应
AepString report_control_ack_CodeCmdResponse (report_control_ack srcStruct);


typedef struct report_period_setStruct 
{
	int set_int;
} report_period_set;
//指令下发:参数配置
int report_period_set_DecodeCmdDown (char* source, report_period_set* dest);


typedef struct report_period_set_ackStruct 
{
	unsigned short taskId;
	int set_int;
} report_period_set_ack;
//指令下发响应:参数配置响应
AepString report_period_set_ack_CodeCmdResponse (report_period_set_ack srcStruct);


typedef struct sensor_reportStruct 
{
	char sensor_data[3];
} sensor_report;
//数据上报:传感器上报
AepString sensor_report_CodeDataReport (sensor_report srcStruct);


typedef struct temperatute_humidity_reportStruct 
{
	float temperature_data;
	float humidity_data;
} temperatute_humidity_report;
//数据上报:温湿度上报
AepString temperatute_humidity_report_CodeDataReport (temperatute_humidity_report srcStruct);

///////////////////////////////////////////////////////////////////*/

#include <stdlib.h>
#include <string.h>


#define AEP_BIG_ENDIAN 'b'
#define AEP_LITTLE_ENDIAN 'l'

static union { char c[4]; unsigned long mylong; } endian_test = {{ 'l', '?', '?', 'b' } };
#define AEP_ENDIANNESS ((char)endian_test.mylong)


typedef unsigned long long uint_64;
typedef unsigned int uint_32;  
typedef unsigned short uint_16;

//命令解析响应码
#define AEP_CMD_SUCCESS 0						//执行成功
#define AEP_CMD_FAILED 1						//执行失败
#define AEP_CMD_INVALID_DATASET_TYPE 2			//无效数据集类型
#define AEP_CMD_INVALID_DATASET_IDENTIFIER 3	//无效数据集标识
#define AEP_CMD_PAYLOAD_PARSING_FAILED 4		//指令数据集Payload解析失败,紧凑二进制编码内容长度不符等


typedef struct AepStrStruct
{
	unsigned short len;
	char* str;
} AepString;
typedef AepString AepBytes;

//无符号整型16位  
uint_16 aep_htons(uint_16 source);

//无符号整型32位
uint_32 aep_htoni(uint_32 source);

//无符号整型64位
uint_64 aep_htonl(uint_64 source);

//float
float aep_htonf(float source);

//double
double aep_htond(double source);

//16进制转字符串
void HexToStr(char *pbDest, char *pbSrc, int nLen);

//字符串转16进制
void StrToHex(char *pbDest, char *pbSrc, int nLen);


//根据服务id生成上报数据的十六进制字符串,srcStruct需要根据服务定义传入对应的类型,返回结果为字符串
AepString codeDataReportByIdToStr(int serviceId, void * srcStruct);

//根据服务id生成上报数据的字节流,srcStruct需要根据服务定义传入对应的类型,返回结果为字节流
AepBytes codeDataReportByIdToBytes(int serviceId, void * srcStruct);

//根据服务标识生成上报数据的十六进制字符串,srcStruct需要根据服务定义传入对应的类型,返回结果为字符串
AepString codeDataReportByIdentifierToStr(char * serviceIdentifier, void * srcStruct);

//根据服务标识生成上报数据的字节流,srcStruct需要根据服务定义传入对应的类型,返回结果为字节流
AepBytes codeDataReportByIdentifierToBytes(char * serviceIdentifier, void * srcStruct);

//指令解析返回结构体，data在使用时需要根据serviceId强转为对应类型
typedef struct CmdStruct 
{
	char* serviceIdentifier;
	unsigned short taskId;
	void * data;
	int code;
} AepCmdData;
//解析接受到的报文数据,入参为十六进制字符串
AepCmdData decodeCmdDownFromStr(char* source);
//解析接受到的报文数据,入参为原始字节流
AepCmdData decodeCmdDownFromBytes(char* source, int len);


typedef struct data_reportStruct 
{
	char full_state;
	char incline_state;
} data_report;
//数据上报:业务数据上报
AepString data_report_CodeDataReport (data_report srcStruct);

typedef struct clean_signal_downStruct 
{
	char clean_signal;
} clean_signal_down;
//指令下发:清洁标记下发
int clean_signal_down_DecodeCmdDown (char* source, clean_signal_down* dest);

typedef struct door_control_1Struct 
{
	char open_signal1;
} door_control_1;
//指令下发:开门控制下发1
int door_control_1_DecodeCmdDown (char* source, door_control_1* dest);


typedef struct door_control_2Struct 
{
	char open_signal2;
} door_control_2;
//指令下发:开门控制下发2
int door_control_2_DecodeCmdDown (char* source, door_control_2* dest);


typedef struct door_control_3Struct 
{
	char open_signal3;
} door_control_3;
//指令下发:开门控制下发3
int door_control_3_DecodeCmdDown (char* source, door_control_3* dest);


typedef struct error_code_reportStruct 
{
	char error_code;
} error_code_report;
//事件上报:故障上报
AepString error_code_report_CodeEventReport (error_code_report srcStruct);


typedef struct fire_state_alarmStruct 
{
	char fire_state;
} fire_state_alarm;
//事件上报:火灾状态上报
AepString fire_state_alarm_CodeEventReport (fire_state_alarm srcStruct);


typedef struct full_state_alarmStruct 
{
	char full_state;
} full_state_alarm;
//事件上报:满溢状态上报
AepString full_state_alarm_CodeEventReport (full_state_alarm srcStruct);


typedef struct humi_signal_upStruct 
{
	float humi_signal;
} humi_signal_up;
//数据上报:湿度值上报
AepString humi_signal_up_CodeDataReport (humi_signal_up srcStruct);


typedef struct incline_state_alarmStruct 
{
	char incline_state;
} incline_state_alarm;
//事件上报:倾倒状态上报
AepString incline_state_alarm_CodeEventReport (incline_state_alarm srcStruct);


typedef struct paper_signal_upStruct 
{
	char paper_signal[1];
} paper_signal_up;
//数据上报:厕纸存量信号上报
AepString paper_signal_up_CodeDataReport (paper_signal_up srcStruct);


typedef struct power_state_alarmStruct 
{
	char power_state;
} power_state_alarm;
//事件上报:电量状态上报
AepString power_state_alarm_CodeEventReport (power_state_alarm srcStruct);


typedef struct set_fire_thresholdStruct 
{
	int fire_threshold;
} set_fire_threshold;
//指令下发:设置火灾阈值
int set_fire_threshold_DecodeCmdDown (char* source, set_fire_threshold* dest);


typedef struct set_fire_threshold_respStruct 
{
	unsigned short taskId;
	int fire_threshold;
} set_fire_threshold_resp;
//指令下发响应:设置火灾阈值响应
AepString set_fire_threshold_resp_CodeCmdResponse (set_fire_threshold_resp srcStruct);


typedef struct set_full_thresholdStruct 
{
	int full_threshold;
} set_full_threshold;
//指令下发:设置满溢阈值
int set_full_threshold_DecodeCmdDown (char* source, set_full_threshold* dest);


typedef struct set_full_threshold_respStruct 
{
	unsigned short taskId;
	int full_threshold;
} set_full_threshold_resp;
//指令下发响应:设置满溢阈值响应
AepString set_full_threshold_resp_CodeCmdResponse (set_full_threshold_resp srcStruct);


typedef struct set_power_thresholdStruct 
{
	int power_threshold;
} set_power_threshold;
//指令下发:设置电量阈值
int set_power_threshold_DecodeCmdDown (char* source, set_power_threshold* dest);


typedef struct set_power_threshold_respStruct 
{
	unsigned short taskId;
	int power_threshold;
} set_power_threshold_resp;
//指令下发响应:设置电量阈值响应
AepString set_power_threshold_resp_CodeCmdResponse (set_power_threshold_resp srcStruct);


typedef struct set_tilt_thresholdStruct 
{
	int tilt_threshold;
} set_tilt_threshold;
//指令下发:设置倾倒阈值
int set_tilt_threshold_DecodeCmdDown (char* source, set_tilt_threshold* dest);


typedef struct set_tilt_threshold_respStruct 
{
	unsigned short taskId;
	int tilt_threshold;
} set_tilt_threshold_resp;
//指令下发响应:设置倾倒阈值响应
AepString set_tilt_threshold_resp_CodeCmdResponse (set_tilt_threshold_resp srcStruct);


typedef struct signal_reportStruct 
{
	int rsrp;
	int sinr;
	int pci;
	int ecl;
	int cell_id;
} signal_report;
//数据上报:信号数据上报
AepString signal_report_CodeDataReport (signal_report srcStruct);


typedef struct temp_signal_upStruct 
{
	float temp_signal;
} temp_signal_up;
//数据上报:温度值上报
AepString temp_signal_up_CodeDataReport (temp_signal_up srcStruct);


typedef struct test_motionStruct 
{
	char Text_signal;
} test_motion;
//事件上报:测试动作
AepString test_motion_CodeEventReport (test_motion srcStruct);


typedef struct toilet_hygiene_signal_upStruct 
{
	char toilet_hygiene_signal[5];
} toilet_hygiene_signal_up;
//数据上报:厕所卫生情况信号上传
AepString toilet_hygiene_signal_up_CodeDataReport (toilet_hygiene_signal_up srcStruct);


typedef struct toilet_signal_up_1Struct 
{
	char toilet_signal_1[1];
} toilet_signal_up_1;
//数据上报:厕所使用情况上报1
AepString toilet_signal_up_1_CodeDataReport (toilet_signal_up_1 srcStruct);


typedef struct toilet_signal_up_2Struct 
{
	char toilet_signal_2[1];
} toilet_signal_up_2;
//数据上报:厕所使用情况上报2
AepString toilet_signal_up_2_CodeDataReport (toilet_signal_up_2 srcStruct);


typedef struct toilet_signal_up_3Struct 
{
	char toilet_signal_3[1];
} toilet_signal_up_3;
//数据上报:厕所使用情况上报3
AepString toilet_signal_up_3_CodeDataReport (toilet_signal_up_3 srcStruct);


typedef struct water_control_downStruct 
{
	char hua_lalala;
} water_control_down;
//指令下发:冲水控制下发
int water_control_down_DecodeCmdDown (char* source, water_control_down* dest);

typedef struct temperatute_humidity_reportStruct 
{
	float temperature_data;
	float humidity_data;
} temperatute_humidity_report;
//数据上报:温湿度上报
AepString temperatute_humidity_report_CodeDataReport (temperatute_humidity_report srcStruct);

typedef struct sensor_reportStruct 
{
	char sensor_data[3];
} sensor_report;
//数据上报:传感器上报
AepString sensor_report_CodeDataReport (sensor_report srcStruct);

typedef struct motor_control_ackStruct 
{
	unsigned short taskId;
	int control_int;
} motor_control_ack;
//指令下发响应:电机控制响应
AepString motor_control_ack_CodeCmdResponse (motor_control_ack srcStruct);

typedef struct report_period_set_ackStruct 
{
	unsigned short taskId;
	int set_int;
} report_period_set_ack;
//指令下发响应:参数配置响应
AepString report_period_set_ack_CodeCmdResponse (report_period_set_ack srcStruct);

typedef struct report_control_ackStruct 
{
	unsigned short taskId;
	int control_int;
} report_control_ack;
//指令下发响应:上报控制响应
AepString report_control_ack_CodeCmdResponse (report_control_ack srcStruct);

#endif
