/******************************************************************************
*	�ļ���	:	sprite.h
*	��������	:	7/2/2016
*	����		:	Loser
*	����		:	
******************************************************************************/

#ifndef __MLT_D3D_SPRITE_H__
#define __MLT_D3D_SPRITE_H__

namespace mlt
{
	/**************************************************************************
	* ������
	**************************************************************************/

	class MD3DSprite : public ID3DSprite, public has_slots<>
	{
		friend class MD3DDevice;

	public:
		// ����������
		virtual ~MD3DSprite();

		// ����任
		virtual BOOL SetTransform(const D3DXMATRIX* pMatrix);

		// ��ʼ����
		virtual BOOL BeginDraw(DWORD dwFlags = 0);
		// ��������
		virtual BOOL EndDraw();

		// ����
		virtual BOOL Draw(SP_ID3DTEXTURE spITex, 
						const D3DXVECTOR3* pPosition = NULL,
						const RECT* pSrcRect = NULL, 
						const D3DXVECTOR3* pCenter = NULL,
						D3DCOLOR dwColor = 0xFFFFFFFF);

	public:
		// �豸��ʧ�¼�
		virtual void OnLostDevice();
		// �豸�����¼�
		virtual void OnResetDevice();

	protected:
		// ���캯��
		MD3DSprite(LPD3DXSPRITE pD3DXSprite);
		
		

	};
}


#endif // __MLT_D3D_SPRITE_H__