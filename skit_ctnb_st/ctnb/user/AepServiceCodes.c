/*///////////////////////////////////////////////////////////////////
#include <ctype.h>
#include "AepServiceCodes.h"

//无符号整型16位  
uint_16 aep_htons(uint_16 source)  
{  

	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
		return (uint_16)( 0
		| ((source & 0x00ff) << 8)
		| ((source & 0xff00) >> 8) );  
}  

//无符号整型32位
uint_32 aep_htoni(uint_32 source)  
{  
	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
		return 0
		| ((source & 0x000000ff) << 24)
		| ((source & 0x0000ff00) << 8)
		| ((source & 0x00ff0000) >> 8)
		| ((source & 0xff000000) >> 24);  
}

//无符号整型64位
uint_64 aep_htonl(uint_64 source)  
{  
	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
		return 0
		| ((source & (uint_64)(0x00000000000000ff)) << 56)
		| ((source & (uint_64)(0x000000000000ff00)) << 40)
		| ((source & (uint_64)(0x0000000000ff0000)) << 24)
		| ((source & (uint_64)(0x00000000ff000000)) << 8)
		| ((source & (uint_64)(0x000000ff00000000)) >> 8)
		| ((source & (uint_64)(0x0000ff0000000000)) >> 24)
		| ((source & (uint_64)(0x00ff000000000000)) >> 40)
		| ((source & (uint_64)(0xff00000000000000)) >> 56);
}

//float
float aep_htonf(float source)  
{  
	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
	{
		uint_32 t= 0
			| ((*(uint_32*)&source & 0x000000ff) << 24)
			| ((*(uint_32*)&source & 0x0000ff00) << 8)
			| ((*(uint_32*)&source & 0x00ff0000) >> 8)
			| ((*(uint_32*)&source & 0xff000000) >> 24);
		return *(float*)&t;
	} 
}

//double
double aep_htond(double source)  
{  
	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
	{
		uint_64 t= 0
			| ((*(uint_64*)&source & (uint_64)(0x00000000000000ff)) << 56)
			| ((*(uint_64*)&source & (uint_64)(0x000000000000ff00)) << 40)
			| ((*(uint_64*)&source & (uint_64)(0x0000000000ff0000)) << 24)
			| ((*(uint_64*)&source & (uint_64)(0x00000000ff000000)) << 8)
			| ((*(uint_64*)&source & (uint_64)(0x000000ff00000000)) >> 8)
			| ((*(uint_64*)&source & (uint_64)(0x0000ff0000000000)) >> 24)
			| ((*(uint_64*)&source & (uint_64)(0x00ff000000000000)) >> 40)
			| ((*(uint_64*)&source & (uint_64)(0xff00000000000000)) >> 56);
		return *(double*)&t;
	}
}

//16进制转字符串
void HexToStr(char *pbDest, char *pbSrc, int nLen)
{
	unsigned char ddl,ddh;
	int i;

	for (i=0; i<nLen; i++)
	{
		ddh = 48 + (unsigned char)pbSrc[i] / 16;
		ddl = 48 + (unsigned char)pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i*2] = ddh;
		pbDest[i*2+1] = ddl;
	}

	//pbDest[nLen*2] = '\0';
}

//字符串转16进制
void StrToHex(char *pbDest, char *pbSrc, int nLen)
{
	unsigned char h1,h2;
	unsigned char s1,s2;
	int i;

	for (i=0; i<nLen; i++)
	{
		h1 = pbSrc[2*i];
		h2 = pbSrc[2*i+1];

		s1 = toupper(h1) - 0x30;
		if (s1 > 9) 
			s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9) 
			s2 -= 7;

		pbDest[i] = s1*16 + s2;
	}
}

//指令下发:电机控制
int motor_control_DecodeCmdDown (char* source, motor_control* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 4;


	memset(dest, 0, sizeof(motor_control));

	StrToHex((char *)&dest->control_int, index, 4);
	dest->control_int = aep_htoni(dest->control_int);
	index += 4 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

//指令下发响应:电机控制响应
AepString motor_control_ack_CodeCmdResponse (motor_control_ack srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 4;
	resultStruct.len = (1 + 2 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);

	srcStruct.control_int = aep_htoni(srcStruct.control_int);

	index = resultStruct.str;

	memcpy(index, "86", 2);
	index += 1 * 2;

	tempLen = aep_htons(9001);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(srcStruct.taskId);//taskID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.control_int, 4);
	index += 4 * 2;


	return resultStruct;
}

//指令下发:上报控制
int report_control_DecodeCmdDown (char* source, report_control* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 4;


	memset(dest, 0, sizeof(report_control));

	StrToHex((char *)&dest->control_int, index, 4);
	dest->control_int = aep_htoni(dest->control_int);
	index += 4 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

//指令下发响应:上报控制响应
AepString report_control_ack_CodeCmdResponse (report_control_ack srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 4;
	resultStruct.len = (1 + 2 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);

	srcStruct.control_int = aep_htoni(srcStruct.control_int);

	index = resultStruct.str;

	memcpy(index, "86", 2);
	index += 1 * 2;

	tempLen = aep_htons(9002);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(srcStruct.taskId);//taskID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.control_int, 4);
	index += 4 * 2;


	return resultStruct;
}

//指令下发:参数配置
int report_period_set_DecodeCmdDown (char* source, report_period_set* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 4;


	memset(dest, 0, sizeof(report_period_set));

	StrToHex((char *)&dest->set_int, index, 4);
	dest->set_int = aep_htoni(dest->set_int);
	index += 4 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

//指令下发响应:参数配置响应
AepString report_period_set_ack_CodeCmdResponse (report_period_set_ack srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 4;
	resultStruct.len = (1 + 2 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);

	srcStruct.set_int = aep_htoni(srcStruct.set_int);

	index = resultStruct.str;

	memcpy(index, "86", 2);
	index += 1 * 2;

	tempLen = aep_htons(9003);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(srcStruct.taskId);//taskID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.set_int, 4);
	index += 4 * 2;


	return resultStruct;
}

//数据上报:传感器上报
AepString sensor_report_CodeDataReport (sensor_report srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 3;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "02", 2);
	index += 1 * 2;

	tempLen = aep_htons(1);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)srcStruct.sensor_data, 3);
	index += 3 * 2;


	return resultStruct;
}

//数据上报:温湿度上报
AepString temperatute_humidity_report_CodeDataReport (temperatute_humidity_report srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 8;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);

	srcStruct.temperature_data = aep_htonf(srcStruct.temperature_data);
	srcStruct.humidity_data = aep_htonf(srcStruct.humidity_data);

	index = resultStruct.str;

	memcpy(index, "02", 2);
	index += 1 * 2;

	tempLen = aep_htons(4);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.temperature_data, 4);
	index += 4 * 2;

	HexToStr(index, (char *)&srcStruct.humidity_data, 4);
	index += 4 * 2;


	return resultStruct;
}

AepCmdData decodeCmdDownFromStr(char* source)
{
	char* index;
	AepCmdData result;
	char cmdType;
	unsigned short serviceId;
	unsigned short payloadLen;

	memset(&result, 0, sizeof(AepCmdData));

	index = source;

	//解析指令类型
	StrToHex(&cmdType, index, 1);
	index += 1 * 2;
	if (cmdType != 0x06)
	{
		result.code = AEP_CMD_INVALID_DATASET_TYPE;
	}

	//服务Id解析
	StrToHex((char *)&serviceId, index, 2);
	serviceId = aep_htons(serviceId);
	index += 2 * 2;

	StrToHex((char *)&result.taskId, index, 2);
	result.taskId = aep_htons(result.taskId);
	index += 2 * 2;

	//payload长度解析
	StrToHex((char *)&payloadLen, index, 2);
	payloadLen = aep_htons(payloadLen);
	index += 2 * 2;

	if (strlen(index) < payloadLen * 2)
	{
		result.code = AEP_CMD_PAYLOAD_PARSING_FAILED;
		return result;
	}


	if (serviceId == 8001)
	{
		result.serviceIdentifier = "motor_control";
		result.data = malloc(sizeof(motor_control));
		memset(result.data, 0, sizeof(motor_control));
		result.code = motor_control_DecodeCmdDown(index, (motor_control*)result.data);
	}
	else if (serviceId == 8002)
	{
		result.serviceIdentifier = "report_control";
		result.data = malloc(sizeof(report_control));
		memset(result.data, 0, sizeof(report_control));
		result.code = report_control_DecodeCmdDown(index, (report_control*)result.data);
	}
	else if (serviceId == 8003)
	{
		result.serviceIdentifier = "report_period_set";
		result.data = malloc(sizeof(report_period_set));
		memset(result.data, 0, sizeof(report_period_set));
		result.code = report_period_set_DecodeCmdDown(index, (report_period_set*)result.data);
	}
	else 
	{
		result.serviceIdentifier = NULL;
		result.data = malloc(payloadLen);
		memset(result.data, 0, sizeof(payloadLen));
		StrToHex((char *)result.data, index, payloadLen);
		result.code = AEP_CMD_INVALID_DATASET_IDENTIFIER;
	}

	return result;
}

AepCmdData decodeCmdDownFromBytes(char* source, int len)
{
	char * str = malloc(len * 2 + 1);
	AepCmdData result;
	HexToStr(str, source, len);
	str[len * 2] = 0;
	
	result = decodeCmdDownFromStr(str);
	free(str);
	return result;
}

AepString codeDataReportByIdToStr (int serviceId, void * srcStruct)
{
	if (serviceId == 9001)
	{
		return motor_control_ack_CodeCmdResponse(*(motor_control_ack*)srcStruct);
	}
	else if (serviceId == 9002)
	{
		return report_control_ack_CodeCmdResponse(*(report_control_ack*)srcStruct);
	}
	else if (serviceId == 9003)
	{
		return report_period_set_ack_CodeCmdResponse(*(report_period_set_ack*)srcStruct);
	}
	else if (serviceId == 1)
	{
		return sensor_report_CodeDataReport(*(sensor_report*)srcStruct);
	}
	else if (serviceId == 4)
	{
		return temperatute_humidity_report_CodeDataReport(*(temperatute_humidity_report*)srcStruct);
	}
	else 
	{
		AepString result = {0};
		return result;
	}
}

AepBytes codeDataReportByIdToBytes(int serviceId, void * srcStruct)
{
	AepString temp = codeDataReportByIdToStr(serviceId, srcStruct);
	AepBytes result = {0};
	result.len = temp.len / 2;
	if (result.len > 0)
	{
		result.str = malloc(result.len);
		StrToHex(result.str, temp.str, result.len);
		free(temp.str);
	}
	return result;
}

AepString codeDataReportByIdentifierToStr (char* serviceIdentifier, void * srcStruct)
{
	if (strcmp(serviceIdentifier, "motor_control_ack") == 0)
	{
		return motor_control_ack_CodeCmdResponse(*(motor_control_ack*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "report_control_ack") == 0)
	{
		return report_control_ack_CodeCmdResponse(*(report_control_ack*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "report_period_set_ack") == 0)
	{
		return report_period_set_ack_CodeCmdResponse(*(report_period_set_ack*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "sensor_report") == 0)
	{
		return sensor_report_CodeDataReport(*(sensor_report*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "temperatute_humidity_report") == 0)
	{
		return temperatute_humidity_report_CodeDataReport(*(temperatute_humidity_report*)srcStruct);
	}
	else 
	{
		AepString result = {0};
		return result;
	}
}

AepBytes codeDataReportByIdentifierToBytes(char* serviceIdentifier, void * srcStruct)
{
	AepString temp = codeDataReportByIdentifierToStr(serviceIdentifier, srcStruct);
	AepBytes result = {0};
	result.len = temp.len / 2;
	if (result.len > 0)
	{
		result.str = malloc(result.len);
		StrToHex(result.str, temp.str, result.len);
		free(temp.str);
	}
	return result;
}
///////////////////////////////////////////////////////////////////*/

#include <ctype.h>
#include "AepServiceCodes.h"

//无符号整型16位  
uint_16 aep_htons(uint_16 source)  
{  

	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
		return (uint_16)( 0
		| ((source & 0x00ff) << 8)
		| ((source & 0xff00) >> 8) );  
}  

//无符号整型32位
uint_32 aep_htoni(uint_32 source)  
{  
	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
		return 0
		| ((source & 0x000000ff) << 24)
		| ((source & 0x0000ff00) << 8)
		| ((source & 0x00ff0000) >> 8)
		| ((source & 0xff000000) >> 24);  
}

//无符号整型64位
uint_64 aep_htonl(uint_64 source)  
{  
	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
		return 0
		| ((source & (uint_64)(0x00000000000000ff)) << 56)
		| ((source & (uint_64)(0x000000000000ff00)) << 40)
		| ((source & (uint_64)(0x0000000000ff0000)) << 24)
		| ((source & (uint_64)(0x00000000ff000000)) << 8)
		| ((source & (uint_64)(0x000000ff00000000)) >> 8)
		| ((source & (uint_64)(0x0000ff0000000000)) >> 24)
		| ((source & (uint_64)(0x00ff000000000000)) >> 40)
		| ((source & (uint_64)(0xff00000000000000)) >> 56);
}

//float
float aep_htonf(float source)  
{  
	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
	{
		uint_32 t= 0
			| ((*(uint_32*)&source & 0x000000ff) << 24)
			| ((*(uint_32*)&source & 0x0000ff00) << 8)
			| ((*(uint_32*)&source & 0x00ff0000) >> 8)
			| ((*(uint_32*)&source & 0xff000000) >> 24);
		return *(float*)&t;
	} 
}

//double
double aep_htond(double source)  
{  
	if(AEP_ENDIANNESS == AEP_BIG_ENDIAN)
		return source;
	else
	{
		uint_64 t= 0
			| ((*(uint_64*)&source & (uint_64)(0x00000000000000ff)) << 56)
			| ((*(uint_64*)&source & (uint_64)(0x000000000000ff00)) << 40)
			| ((*(uint_64*)&source & (uint_64)(0x0000000000ff0000)) << 24)
			| ((*(uint_64*)&source & (uint_64)(0x00000000ff000000)) << 8)
			| ((*(uint_64*)&source & (uint_64)(0x000000ff00000000)) >> 8)
			| ((*(uint_64*)&source & (uint_64)(0x0000ff0000000000)) >> 24)
			| ((*(uint_64*)&source & (uint_64)(0x00ff000000000000)) >> 40)
			| ((*(uint_64*)&source & (uint_64)(0xff00000000000000)) >> 56);
		return *(double*)&t;
	}
}

//16进制转字符串
void HexToStr(char *pbDest, char *pbSrc, int nLen)
{
	unsigned char ddl,ddh;
	int i;

	for (i=0; i<nLen; i++)
	{
		ddh = 48 + (unsigned char)pbSrc[i] / 16;
		ddl = 48 + (unsigned char)pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i*2] = ddh;
		pbDest[i*2+1] = ddl;
	}

	//pbDest[nLen*2] = '\0';
}

//字符串转16进制
void StrToHex(char *pbDest, char *pbSrc, int nLen)
{
	unsigned char h1,h2;
	unsigned char s1,s2;
	int i;

	for (i=0; i<nLen; i++)
	{
		h1 = pbSrc[2*i];
		h2 = pbSrc[2*i+1];

		s1 = toupper(h1) - 0x30;
		if (s1 > 9) 
			s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9) 
			s2 -= 7;

		pbDest[i] = s1*16 + s2;
	}
}

//数据上报:业务数据上报
AepString data_report_CodeDataReport (data_report srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 2;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "02", 2);
	index += 1 * 2;

	tempLen = aep_htons(1);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.full_state, 1);
	index += 1 * 2;

	HexToStr(index, (char *)&srcStruct.incline_state, 1);
	index += 1 * 2;


	return resultStruct;
}

//指令下发:开门控制下发1
int door_control_1_DecodeCmdDown (char* source, door_control_1* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 1;


	memset(dest, 0, sizeof(door_control_1));

	StrToHex((char *)&dest->open_signal1, index, 1);
	index += 1 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

//指令下发:开门控制下发2
int door_control_2_DecodeCmdDown (char* source, door_control_2* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 1;


	memset(dest, 0, sizeof(door_control_2));

	StrToHex((char *)&dest->open_signal2, index, 1);
	index += 1 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

//指令下发:开门控制下发3
int door_control_3_DecodeCmdDown (char* source, door_control_3* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 1;


	memset(dest, 0, sizeof(door_control_3));

	StrToHex((char *)&dest->open_signal3, index, 1);
	index += 1 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

//事件上报:故障上报
AepString error_code_report_CodeEventReport (error_code_report srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 1;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "07", 2);
	index += 1 * 2;

	tempLen = aep_htons(1005);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.error_code, 1);
	index += 1 * 2;


	return resultStruct;
}

//事件上报:火灾状态上报
AepString fire_state_alarm_CodeEventReport (fire_state_alarm srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 1;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "07", 2);
	index += 1 * 2;

	tempLen = aep_htons(1002);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.fire_state, 1);
	index += 1 * 2;


	return resultStruct;
}

//事件上报:满溢状态上报
AepString full_state_alarm_CodeEventReport (full_state_alarm srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 1;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "07", 2);
	index += 1 * 2;

	tempLen = aep_htons(1001);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.full_state, 1);
	index += 1 * 2;


	return resultStruct;
}

//数据上报:湿度值上报
AepString humi_signal_up_CodeDataReport (humi_signal_up srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 4;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);

	srcStruct.humi_signal = aep_htonf(srcStruct.humi_signal);

	index = resultStruct.str;

	memcpy(index, "02", 2);
	index += 1 * 2;

	tempLen = aep_htons(104);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.humi_signal, 4);
	index += 4 * 2;


	return resultStruct;
}

//事件上报:倾倒状态上报
AepString incline_state_alarm_CodeEventReport (incline_state_alarm srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 1;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "07", 2);
	index += 1 * 2;

	tempLen = aep_htons(1003);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.incline_state, 1);
	index += 1 * 2;


	return resultStruct;
}

//数据上报:厕纸存量信号上报
AepString paper_signal_up_CodeDataReport (paper_signal_up srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 1;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "02", 2);
	index += 1 * 2;

	tempLen = aep_htons(106);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)srcStruct.paper_signal, 1);
	index += 1 * 2;


	return resultStruct;
}

//事件上报:电量状态上报
AepString power_state_alarm_CodeEventReport (power_state_alarm srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 1;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "07", 2);
	index += 1 * 2;

	tempLen = aep_htons(1004);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.power_state, 1);
	index += 1 * 2;


	return resultStruct;
}

//指令下发:设置火灾阈值
int set_fire_threshold_DecodeCmdDown (char* source, set_fire_threshold* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 4;


	memset(dest, 0, sizeof(set_fire_threshold));

	StrToHex((char *)&dest->fire_threshold, index, 4);
	dest->fire_threshold = aep_htoni(dest->fire_threshold);
	index += 4 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

//指令下发响应:设置火灾阈值响应
AepString set_fire_threshold_resp_CodeCmdResponse (set_fire_threshold_resp srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 4;
	resultStruct.len = (1 + 2 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);

	srcStruct.fire_threshold = aep_htoni(srcStruct.fire_threshold);

	index = resultStruct.str;

	memcpy(index, "86", 2);
	index += 1 * 2;

	tempLen = aep_htons(9002);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(srcStruct.taskId);//taskID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.fire_threshold, 4);
	index += 4 * 2;


	return resultStruct;
}

//指令下发:设置满溢阈值
int set_full_threshold_DecodeCmdDown (char* source, set_full_threshold* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 4;


	memset(dest, 0, sizeof(set_full_threshold));

	StrToHex((char *)&dest->full_threshold, index, 4);
	dest->full_threshold = aep_htoni(dest->full_threshold);
	index += 4 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

//指令下发响应:设置满溢阈值响应
AepString set_full_threshold_resp_CodeCmdResponse (set_full_threshold_resp srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 4;
	resultStruct.len = (1 + 2 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);

	srcStruct.full_threshold = aep_htoni(srcStruct.full_threshold);

	index = resultStruct.str;

	memcpy(index, "86", 2);
	index += 1 * 2;

	tempLen = aep_htons(9001);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(srcStruct.taskId);//taskID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.full_threshold, 4);
	index += 4 * 2;


	return resultStruct;
}

//指令下发:设置电量阈值
int set_power_threshold_DecodeCmdDown (char* source, set_power_threshold* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 4;


	memset(dest, 0, sizeof(set_power_threshold));

	StrToHex((char *)&dest->power_threshold, index, 4);
	dest->power_threshold = aep_htoni(dest->power_threshold);
	index += 4 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

//指令下发响应:设置电量阈值响应
AepString set_power_threshold_resp_CodeCmdResponse (set_power_threshold_resp srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 4;
	resultStruct.len = (1 + 2 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);

	srcStruct.power_threshold = aep_htoni(srcStruct.power_threshold);

	index = resultStruct.str;

	memcpy(index, "86", 2);
	index += 1 * 2;

	tempLen = aep_htons(9004);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(srcStruct.taskId);//taskID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.power_threshold, 4);
	index += 4 * 2;


	return resultStruct;
}

//指令下发:设置倾倒阈值
int set_tilt_threshold_DecodeCmdDown (char* source, set_tilt_threshold* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 4;


	memset(dest, 0, sizeof(set_tilt_threshold));

	StrToHex((char *)&dest->tilt_threshold, index, 4);
	dest->tilt_threshold = aep_htoni(dest->tilt_threshold);
	index += 4 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

//指令下发响应:设置倾倒阈值响应
AepString set_tilt_threshold_resp_CodeCmdResponse (set_tilt_threshold_resp srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 4;
	resultStruct.len = (1 + 2 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);

	srcStruct.tilt_threshold = aep_htoni(srcStruct.tilt_threshold);

	index = resultStruct.str;

	memcpy(index, "86", 2);
	index += 1 * 2;

	tempLen = aep_htons(9003);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(srcStruct.taskId);//taskID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.tilt_threshold, 4);
	index += 4 * 2;


	return resultStruct;
}

//数据上报:信号数据上报
AepString signal_report_CodeDataReport (signal_report srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 20;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);

	srcStruct.rsrp = aep_htoni(srcStruct.rsrp);
	srcStruct.sinr = aep_htoni(srcStruct.sinr);
	srcStruct.pci = aep_htoni(srcStruct.pci);
	srcStruct.ecl = aep_htoni(srcStruct.ecl);
	srcStruct.cell_id = aep_htoni(srcStruct.cell_id);

	index = resultStruct.str;

	memcpy(index, "02", 2);
	index += 1 * 2;

	tempLen = aep_htons(2);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.rsrp, 4);
	index += 4 * 2;

	HexToStr(index, (char *)&srcStruct.sinr, 4);
	index += 4 * 2;

	HexToStr(index, (char *)&srcStruct.pci, 4);
	index += 4 * 2;

	HexToStr(index, (char *)&srcStruct.ecl, 4);
	index += 4 * 2;

	HexToStr(index, (char *)&srcStruct.cell_id, 4);
	index += 4 * 2;


	return resultStruct;
}

//数据上报:温度值上报
AepString temp_signal_up_CodeDataReport (temp_signal_up srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 4;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);

	srcStruct.temp_signal = aep_htonf(srcStruct.temp_signal);

	index = resultStruct.str;

	memcpy(index, "02", 2);
	index += 1 * 2;

	tempLen = aep_htons(103);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.temp_signal, 4);
	index += 4 * 2;


	return resultStruct;
}

//事件上报:测试动作
AepString test_motion_CodeEventReport (test_motion srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 1;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "07", 2);
	index += 1 * 2;

	tempLen = aep_htons(1100);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)&srcStruct.Text_signal, 1);
	index += 1 * 2;


	return resultStruct;
}

//数据上报:厕所卫生情况信号上传
AepString toilet_hygiene_signal_up_CodeDataReport (toilet_hygiene_signal_up srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 5;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "02", 2);
	index += 1 * 2;

	tempLen = aep_htons(105);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)srcStruct.toilet_hygiene_signal, 5);
	index += 5 * 2;


	return resultStruct;
}

//数据上报:厕所使用情况上报1
AepString toilet_signal_up_1_CodeDataReport (toilet_signal_up_1 srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 1;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "02", 2);
	index += 1 * 2;

	tempLen = aep_htons(100);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)srcStruct.toilet_signal_1, 1);
	index += 1 * 2;


	return resultStruct;
}

//数据上报:厕所使用情况上报2
AepString toilet_signal_up_2_CodeDataReport (toilet_signal_up_2 srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 1;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "02", 2);
	index += 1 * 2;

	tempLen = aep_htons(101);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)srcStruct.toilet_signal_2, 1);
	index += 1 * 2;


	return resultStruct;
}

//数据上报:厕所使用情况上报3
AepString toilet_signal_up_3_CodeDataReport (toilet_signal_up_3 srcStruct)
{
	char* index;
	AepString resultStruct;
	unsigned short tempLen;

	unsigned short payloadLen = 1;
	resultStruct.len = (1 + 2 + 2 + payloadLen) * 2;
	resultStruct.str = (char *)malloc(resultStruct.len + 1);
	memset(resultStruct.str, 0, resultStruct.len + 1);


	index = resultStruct.str;

	memcpy(index, "02", 2);
	index += 1 * 2;

	tempLen = aep_htons(102);//服务ID
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	tempLen = aep_htons(payloadLen);
	HexToStr(index, (char *)&tempLen, 2);
	index += 2 * 2;

	HexToStr(index, (char *)srcStruct.toilet_signal_3, 1);
	index += 1 * 2;


	return resultStruct;
}


//指令下发:冲水控制下发
int water_control_down_DecodeCmdDown (char* source, water_control_down* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 1;


	memset(dest, 0, sizeof(water_control_down));

	StrToHex((char *)&dest->hua_lalala, index, 1);
	index += 1 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

//指令下发:清洁标记下发
int clean_signal_down_DecodeCmdDown (char* source, clean_signal_down* dest)
{
	char* index = source;
	int srcStrLen = strlen(source);
	int len = 1;


	memset(dest, 0, sizeof(clean_signal_down));

	StrToHex((char *)&dest->clean_signal, index, 1);
	index += 1 * 2;



	if (len * 2 > srcStrLen)
	{
		return AEP_CMD_PAYLOAD_PARSING_FAILED;
	}
	return AEP_CMD_SUCCESS;
}

AepCmdData decodeCmdDownFromStr(char* source)
{
	char* index;
	AepCmdData result;
	char cmdType;
	unsigned short serviceId;
	unsigned short payloadLen;

	memset(&result, 0, sizeof(AepCmdData));

	index = source;

	//解析指令类型
	StrToHex(&cmdType, index, 1);
	index += 1 * 2;
	if (cmdType != 0x06)
	{
		result.code = AEP_CMD_INVALID_DATASET_TYPE;
	}

	//服务Id解析
	StrToHex((char *)&serviceId, index, 2);
	serviceId = aep_htons(serviceId);
	index += 2 * 2;

	StrToHex((char *)&result.taskId, index, 2);
	result.taskId = aep_htons(result.taskId);
	index += 2 * 2;

	//payload长度解析
	StrToHex((char *)&payloadLen, index, 2);
	payloadLen = aep_htons(payloadLen);
	index += 2 * 2;

	if (strlen(index) < payloadLen * 2)
	{
		result.code = AEP_CMD_PAYLOAD_PARSING_FAILED;
		return result;
	}

  if (serviceId == 8010)
	{
		result.serviceIdentifier = "clean_signal_down";
		result.data = malloc(sizeof(clean_signal_down));
		memset(result.data, 0, sizeof(clean_signal_down));
		result.code = clean_signal_down_DecodeCmdDown(index, (clean_signal_down*)result.data);
	}
	else if (serviceId == 8006)
	{
		result.serviceIdentifier = "door_control_1";
		result.data = malloc(sizeof(door_control_1));
		memset(result.data, 0, sizeof(door_control_1));
		result.code = door_control_1_DecodeCmdDown(index, (door_control_1*)result.data);
	}
	else if (serviceId == 8007)
	{
		result.serviceIdentifier = "door_control_2";
		result.data = malloc(sizeof(door_control_2));
		memset(result.data, 0, sizeof(door_control_2));
		result.code = door_control_2_DecodeCmdDown(index, (door_control_2*)result.data);
	}
	else if (serviceId == 8008)
	{
		result.serviceIdentifier = "door_control_3";
		result.data = malloc(sizeof(door_control_3));
		memset(result.data, 0, sizeof(door_control_3));
		result.code = door_control_3_DecodeCmdDown(index, (door_control_3*)result.data);
	}
	else if (serviceId == 8002)
	{
		result.serviceIdentifier = "set_fire_threshold";
		result.data = malloc(sizeof(set_fire_threshold));
		memset(result.data, 0, sizeof(set_fire_threshold));
		result.code = set_fire_threshold_DecodeCmdDown(index, (set_fire_threshold*)result.data);
	}
	else if (serviceId == 8001)
	{
		result.serviceIdentifier = "set_full_threshold";
		result.data = malloc(sizeof(set_full_threshold));
		memset(result.data, 0, sizeof(set_full_threshold));
		result.code = set_full_threshold_DecodeCmdDown(index, (set_full_threshold*)result.data);
	}
	else if (serviceId == 8004)
	{
		result.serviceIdentifier = "set_power_threshold";
		result.data = malloc(sizeof(set_power_threshold));
		memset(result.data, 0, sizeof(set_power_threshold));
		result.code = set_power_threshold_DecodeCmdDown(index, (set_power_threshold*)result.data);
	}
	else if (serviceId == 8003)
	{
		result.serviceIdentifier = "set_tilt_threshold";
		result.data = malloc(sizeof(set_tilt_threshold));
		memset(result.data, 0, sizeof(set_tilt_threshold));
		result.code = set_tilt_threshold_DecodeCmdDown(index, (set_tilt_threshold*)result.data);
	}
	else if (serviceId == 8005)
	{
		result.serviceIdentifier = "water_control_down";
		result.data = malloc(sizeof(water_control_down));
		memset(result.data, 0, sizeof(water_control_down));
		result.code = water_control_down_DecodeCmdDown(index, (water_control_down*)result.data);
	}
	else 
	{
		result.serviceIdentifier = NULL;
		result.data = malloc(payloadLen);
		memset(result.data, 0, sizeof(payloadLen));
		StrToHex((char *)result.data, index, payloadLen);
		result.code = AEP_CMD_INVALID_DATASET_IDENTIFIER;
	}

	return result;
}

AepCmdData decodeCmdDownFromBytes(char* source, int len)
{
	char * str = malloc(len * 2 + 1);
	AepCmdData result;
	HexToStr(str, source, len);
	str[len * 2] = 0;
	
	result = decodeCmdDownFromStr(str);
	free(str);
	return result;
}

AepString codeDataReportByIdToStr (int serviceId, void * srcStruct)
{
	if (serviceId == 1)
	{
		return data_report_CodeDataReport(*(data_report*)srcStruct);
	}
	else if (serviceId == 1005)
	{
		return error_code_report_CodeEventReport(*(error_code_report*)srcStruct);
	}
	else if (serviceId == 1002)
	{
		return fire_state_alarm_CodeEventReport(*(fire_state_alarm*)srcStruct);
	}
	else if (serviceId == 1001)
	{
		return full_state_alarm_CodeEventReport(*(full_state_alarm*)srcStruct);
	}
	else if (serviceId == 104)
	{
		return humi_signal_up_CodeDataReport(*(humi_signal_up*)srcStruct);
	}
	else if (serviceId == 1003)
	{
		return incline_state_alarm_CodeEventReport(*(incline_state_alarm*)srcStruct);
	}
	else if (serviceId == 106)
	{
		return paper_signal_up_CodeDataReport(*(paper_signal_up*)srcStruct);
	}
	else if (serviceId == 1004)
	{
		return power_state_alarm_CodeEventReport(*(power_state_alarm*)srcStruct);
	}
	else if (serviceId == 9002)
	{
		return set_fire_threshold_resp_CodeCmdResponse(*(set_fire_threshold_resp*)srcStruct);
	}
	else if (serviceId == 9001)
	{
		return set_full_threshold_resp_CodeCmdResponse(*(set_full_threshold_resp*)srcStruct);
	}
	else if (serviceId == 9004)
	{
		return set_power_threshold_resp_CodeCmdResponse(*(set_power_threshold_resp*)srcStruct);
	}
	else if (serviceId == 9003)
	{
		return set_tilt_threshold_resp_CodeCmdResponse(*(set_tilt_threshold_resp*)srcStruct);
	}
	else if (serviceId == 2)
	{
		return signal_report_CodeDataReport(*(signal_report*)srcStruct);
	}
	else if (serviceId == 103)
	{
		return temp_signal_up_CodeDataReport(*(temp_signal_up*)srcStruct);
	}
	else if (serviceId == 1100)
	{
		return test_motion_CodeEventReport(*(test_motion*)srcStruct);
	}
	else if (serviceId == 105)
	{
		return toilet_hygiene_signal_up_CodeDataReport(*(toilet_hygiene_signal_up*)srcStruct);
	}
	else if (serviceId == 100)
	{
		return toilet_signal_up_1_CodeDataReport(*(toilet_signal_up_1*)srcStruct);
	}
	else if (serviceId == 101)
	{
		return toilet_signal_up_2_CodeDataReport(*(toilet_signal_up_2*)srcStruct);
	}
	else if (serviceId == 102)
	{
		return toilet_signal_up_3_CodeDataReport(*(toilet_signal_up_3*)srcStruct);
	}
	else 
	{
		AepString result = {0};
		return result;
	}
}

AepBytes codeDataReportByIdToBytes(int serviceId, void * srcStruct)
{
	AepString temp = codeDataReportByIdToStr(serviceId, srcStruct);
	AepBytes result = {0};
	result.len = temp.len / 2;
	if (result.len > 0)
	{
		result.str = malloc(result.len);
		StrToHex(result.str, temp.str, result.len);
		free(temp.str);
	}
	return result;
}

AepString codeDataReportByIdentifierToStr (char* serviceIdentifier, void * srcStruct)
{
	if (strcmp(serviceIdentifier, "data_report") == 0)
	{
		return data_report_CodeDataReport(*(data_report*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "error_code_report") == 0)
	{
		return error_code_report_CodeEventReport(*(error_code_report*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "fire_state_alarm") == 0)
	{
		return fire_state_alarm_CodeEventReport(*(fire_state_alarm*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "full_state_alarm") == 0)
	{
		return full_state_alarm_CodeEventReport(*(full_state_alarm*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "humi_signal_up") == 0)
	{
		return humi_signal_up_CodeDataReport(*(humi_signal_up*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "incline_state_alarm") == 0)
	{
		return incline_state_alarm_CodeEventReport(*(incline_state_alarm*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "paper_signal_up") == 0)
	{
		return paper_signal_up_CodeDataReport(*(paper_signal_up*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "power_state_alarm") == 0)
	{
		return power_state_alarm_CodeEventReport(*(power_state_alarm*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "set_fire_threshold_resp") == 0)
	{
		return set_fire_threshold_resp_CodeCmdResponse(*(set_fire_threshold_resp*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "set_full_threshold_resp") == 0)
	{
		return set_full_threshold_resp_CodeCmdResponse(*(set_full_threshold_resp*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "set_power_threshold_resp") == 0)
	{
		return set_power_threshold_resp_CodeCmdResponse(*(set_power_threshold_resp*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "set_tilt_threshold_resp") == 0)
	{
		return set_tilt_threshold_resp_CodeCmdResponse(*(set_tilt_threshold_resp*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "signal_report") == 0)
	{
		return signal_report_CodeDataReport(*(signal_report*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "temp_signal_up") == 0)
	{
		return temp_signal_up_CodeDataReport(*(temp_signal_up*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "test_motion") == 0)
	{
		return test_motion_CodeEventReport(*(test_motion*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "toilet_hygiene_signal_up") == 0)
	{
		return toilet_hygiene_signal_up_CodeDataReport(*(toilet_hygiene_signal_up*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "toilet_signal_up_1") == 0)
	{
		return toilet_signal_up_1_CodeDataReport(*(toilet_signal_up_1*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "toilet_signal_up_2") == 0)
	{
		return toilet_signal_up_2_CodeDataReport(*(toilet_signal_up_2*)srcStruct);
	}
	else if (strcmp(serviceIdentifier, "toilet_signal_up_3") == 0)
	{
		return toilet_signal_up_3_CodeDataReport(*(toilet_signal_up_3*)srcStruct);
	}
	else 
	{
		AepString result = {0};
		return result;
	}
}

AepBytes codeDataReportByIdentifierToBytes(char* serviceIdentifier, void * srcStruct)
{
	AepString temp = codeDataReportByIdentifierToStr(serviceIdentifier, srcStruct);
	AepBytes result = {0};
	result.len = temp.len / 2;
	if (result.len > 0)
	{
		result.str = malloc(result.len);
		StrToHex(result.str, temp.str, result.len);
		free(temp.str);
	}
	return result;
}


