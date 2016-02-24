/**********************************************************************
* 版权所有 (C)2016, Zhou Zhaoxiong。
*
* 文件名称： ReverseTheString.c
* 文件标识： 无
* 内容摘要： 将字符串中的单词反序
* 其它说明： 例如, 将"I'm a student"转变为"student a I'm"
* 当前版本： V1.0
* 作    者： Zhou Zhaoxiong
* 完成日期： 20160215
*
**********************************************************************/
#include <stdio.h>
#include <stdlib.h>

// 重新定义数据类型
typedef signed   char       INT8;
typedef unsigned short int  UINT16;
typedef          int        INT32;
typedef unsigned int        UINT32;

// 函数声明
INT32 GetFieldFromString(INT8 *pszIn, INT8 cSplitter, UINT16 iIdx, INT8 *pszOut, UINT16 iLen);
void CombineSpace(INT8 *pszOldStr, INT8 *pszNewStr);


/**********************************************************************
* 功能描述： 主函数
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 0-执行成功   其它-执行失败
* 其它说明： 无
* 修改日期        版本号     修改人            修改内容
* ---------------------------------------------------------------------
* 20160215        V1.0     Zhou Zhaoxiong        创建
***********************************************************************/
INT32 main()
{
    INT8   szTestStr[500]   = {0};
    INT8   szTmpStr[500]    = {0};  // 用于存放经过规范化处理之后的字符串
    INT8   szResultStr[500] = {0};
    INT8   szFieldVal[100]  = {0};
    INT32  iPosFlag         = 0;
    INT32  iRetFlag         = 0;
    UINT32 iBlankCount      = 0;
    
    printf("Please input the string: \n");
    gets(szTestStr);
    printf("TestStr=%s\n", szTestStr);
    
    // 去除测试字符串中多余的空格及前后的空格
    CombineSpace(szTestStr, szTmpStr);
    
    // 获取测试字符串中的空格字符的个数
    iPosFlag = 0;
    while (szTmpStr[iPosFlag] != '\0')
    {
        if (szTmpStr[iPosFlag] == ' ')
        {
            iBlankCount ++;
        }
        
        iPosFlag ++;
    }
    
    // 将测试字符串中的单词按照反序来拼接
    strcat(szTmpStr, " ");   // 在测试字符串最后面添加空格, 用于解析各个单词
    for (iPosFlag = iBlankCount; iPosFlag >= 0; iPosFlag --)
    {
        memset(szFieldVal, 0x00, sizeof(szFieldVal));
        iRetFlag = GetFieldFromString(szTmpStr, ' ', iPosFlag, szFieldVal, sizeof(szFieldVal)-1);
        if (iRetFlag != 0)
        {
            printf("Exec GetFieldFromString to get FieldVal failed!\n");
            return -1;
        }
        
        if (strlen(szResultStr) > sizeof(szResultStr)-1)     // 字符串超长, 直接退出
        {
            break;
        }
        else
        {
            strcat(szResultStr, szFieldVal);     // 将各个单词拼接起来
            strcat(szResultStr, " ");            // 在各个单词后面添加空格
        }
    }
    
    if (szResultStr[strlen(szResultStr)-1] == ' ')
    {
        szResultStr[strlen(szResultStr)-1] = '\0';   // 去除最后面的空格
    }
    
    printf("ResultStr=%s\n", szResultStr);

    return 0;            
}


/**********************************************************************
* 功能描述： 从字符串中获取字段的值
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 0-执行成功   其它-执行失败
* 其它说明： 无
* 修改日期        版本号     修改人            修改内容
* ---------------------------------------------------------------------
* 20160215        V1.0     Zhou Zhaoxiong        创建
***********************************************************************/
INT32 GetFieldFromString(INT8 *pszIn, INT8 cSplitter, UINT16 iIdx, INT8 *pszOut, UINT16 iLen)
{
    INT8   *pszStart    = NULL;
    INT8   *pszEnd      = NULL;
    UINT16  iCount      = 0;
    UINT16  iFiledLen   = 0;
    INT8    szBuf[1024] = {0};

    if (NULL == pszIn || NULL == pszOut)
    {
        return -1;
    }

    pszStart = pszIn;
    for (iCount = 0; iIdx != iCount; iCount++)   // for循环用于查找当前字段的起始位置
    {
        pszStart = strchr(pszStart, cSplitter);
        if (NULL == pszStart)
        {
            break;
        }
        else
        {
            pszStart ++;
        }
    }

    if (NULL == pszStart)
    {
        return -2;
    }

    pszEnd = strchr(pszStart, cSplitter);
    if (NULL == pszEnd)
    {
        return -3;
    }

    // 判断长度大小, 防止拷贝字符串时越界
    if (pszEnd - pszStart > sizeof(szBuf)-1)
    {
        iFiledLen = sizeof(szBuf)-1;
    }
    else
    {
        iFiledLen = pszEnd - pszStart;
    }
    
    strncpy(szBuf, pszStart, iFiledLen);

    if (iLen < iFiledLen)   // 传入的长度太小
    {
        return -4;
    }

    strncpy(pszOut, szBuf, iFiledLen);

    return 0;
}


/**********************************************************************
* 功能描述：合并多个连续空格为一个, 并去掉首位的空格
* 输入参数：pszOldStr-合并空格之前的字符串
* 输出参数：pszNewStr-合并空格之后的字符串
* 返 回 值：无
* 其它说明：无
* 修改日期       版本号        修改人          修改内容
* -------------------------------------------------------------------
* 20160215       V1.0      Zhou Zhaoxiong       创建
***********************************************************************/
void CombineSpace(INT8 *pszOldStr, INT8 *pszNewStr)
{
    UINT32 iOldIndex        = 0;
    UINT32 iNewIndex        = 0;
    INT8   chSingleChar     = '\0';
    INT8   iIsSpace         = 0;
    INT8   szTmpNewStr[100] = {0};

    if ((NULL == pszOldStr) || (NULL == pszNewStr))
    {
        return;
    }

    while ('\0' != (chSingleChar = pszOldStr[iOldIndex++]))
    {
        if (chSingleChar == ' ')   // 多个连续空格只保留一个
        {
            if (iIsSpace == 0)
            {
                iIsSpace = 1;
                szTmpNewStr[iNewIndex++] = chSingleChar;
            }
        }
        else
        {
            iIsSpace = 0;
            szTmpNewStr[iNewIndex++] = chSingleChar;
        }
    }

    // 如果最后一个字符是空格, 则将其去掉
    if (szTmpNewStr[strlen(szTmpNewStr)-1] == ' ')
    {
        szTmpNewStr[strlen(szTmpNewStr)-1] = '\0';
    }

    // 如果第一个字符是空格, 则将其去掉
    if (szTmpNewStr[0] == ' ')
    {
        memcpy(pszNewStr, szTmpNewStr+1, strlen(szTmpNewStr)-1);
    }
    else
    {
        memcpy(pszNewStr, szTmpNewStr, strlen(szTmpNewStr));
    }

    return;
}
