#include "stdafx.h"
#include "suanfa.h"
#include "DIBAPI.h"

#include <math.h>
#include <direct.h>
BOOL WINAPI ContourDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
		// ָ��Դͼ���ָ��
	LPSTR	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst;
	
	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIB;

	//ѭ������
	long i;
	long j;
	unsigned char n,e,s,w,ne,se,nw,sw;

	//����ֵ
	unsigned char pixel;

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIB = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIB == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	lpNewDIBBits = (char * )LocalLock(hNewDIB);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (char *)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (char *)lpDIBBits + lWidth * j + i;
			
			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
			lpDst = (char *)lpNewDIBBits + lWidth * j + i;
			
			//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
			pixel = (unsigned char)*lpSrc;

			//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
	//		if(pixel != 255 && pixel != 0)
	//		return FALSE;
			if(pixel == 0)
			{
				*lpDst = (unsigned char)0;
				nw = (unsigned char)*(lpSrc + lWidth -1);
				n  = (unsigned char)*(lpSrc + lWidth );
				ne = (unsigned char)*(lpSrc + lWidth +1);
				w = (unsigned char)*(lpSrc -1);
				e = (unsigned char)*(lpSrc +1);
				sw = (unsigned char)*(lpSrc - lWidth -1);
				s  = (unsigned char)*(lpSrc - lWidth );
				se = (unsigned char)*(lpSrc - lWidth +1);
				//������ڵİ˸��㶼�Ǻڵ�
				if(nw+n+ne+w+e+sw+s+se==0)
				{
					*lpDst = (unsigned char)255;
				}
			}
		}
	}

	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIB);
	LocalFree(hNewDIB);

	// ����
	return TRUE;
}
/************************************************************************************/
BOOL WINAPI Template(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
					 int iTempH, int iTempW, 
					 int iTempMX, int iTempMY,
					 FLOAT * fpArray, FLOAT fCoef)
{
	// ָ����ͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;
	
	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;
	
	// ָ��Ҫ���������ָ��
	unsigned char*	lpDst;
	
	// ѭ������
	LONG	i;
	LONG	j;
	LONG	k;
	LONG	l;
	
	// ������
	FLOAT	fResult;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	
	// �ж��Ƿ��ڴ����ʧ��
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);
	
	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);
	
	// ��(��ȥ��Ե����)
	for(i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		// ��(��ȥ��Ե����)
		for(j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			fResult = 0;
			
			// ����
			for (k = 0; k < iTempH; k++)
			{
				for (l = 0; l < iTempW; l++)
				{
					// ָ��DIB��i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)
						+ j - iTempMX + l;
					
					// ��������ֵ
					fResult += (* lpSrc) * fpArray[k * iTempW + l];
				}
			}
			
			// ����ϵ��
			fResult *= fCoef;
			
			// ȡ����ֵ
			fResult = (FLOAT ) fabs(fResult);
			
			// �ж��Ƿ񳬹�255
			if(fResult > 255)
			{
				// ֱ�Ӹ�ֵΪ255
				* lpDst = 255;
			}
			else
			{
				// ��ֵ
				* lpDst = (unsigned char) (fResult + 0.5);
			}
			
		}
	}
	
	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	
	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	// ����
	return TRUE;

}