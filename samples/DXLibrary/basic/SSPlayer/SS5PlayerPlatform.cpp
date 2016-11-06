﻿// 
//  SS5Platform.cpp
//
#include "SS5PlayerPlatform.h"

/**
* 各プラットフォームに合わせて処理を作成してください
* DXライブラリ用に作成されています。
*/
#include "DxLib.h"


namespace ss
{
	/**
	* テクスチャの読み込み
	*/
	long SSTextureLoad(const char* pszFileName, SsTexWrapMode::_enum  wrapmode, SsTexFilterMode::_enum filtermode)
	{
		/**
		* テクスチャ管理用のユニークな値を返してください。
		* テクスチャの管理はゲーム側で行う形になります。
		* テクスチャにアクセスするハンドルや、テクスチャを割り当てたバッファ番号等になります。
		*
		* プレイヤーはここで返した値とパーツのステータスを引数に描画を行います。
		*/
		long rc = 0;

		rc = (long)LoadGraph(pszFileName);
		//SpriteStudioで設定されたテクスチャ設定を反映させるための分岐です。
		switch (wrapmode)
		{
		case SsTexWrapMode::clamp:	//クランプ
			break;
		case SsTexWrapMode::repeat:	//リピート
			break;
		case SsTexWrapMode::mirror:	//ミラー
			break;
		}
		switch (filtermode)
		{
		case SsTexFilterMode::nearlest:	//ニアレストネイバー
			break;
		case SsTexFilterMode::linear:	//リニア、バイリニア
			break;
		}

		return rc;
	}
	
	/**
	* テクスチャの解放
	*/
	bool SSTextureRelese(long handle)
	{
		/// 解放後も同じ番号で何度も解放処理が呼ばれるので、例外が出ないように作成してください。
		bool rc = true;

		if ( DeleteGraph((int)handle) == -1 )
		{
			rc = false;
		}

		return rc ;
	}

	/**
	* テクスチャのサイズを取得
	* テクスチャのUVを設定するのに使用します。
	*/
	bool SSGetTextureSize(long handle, int &w, int &h)
	{
		if (GetGraphSize(handle, &w, &h) == -1)
		{
			return false;
		}
		return true;
	}

	//DXライブラリ用頂点バッファ作成関数
	VERTEX_3D vertex3Dfrom(const ss::SSV3F_C4B_T2F &vct)
	{
		VERTEX_3D v = {
			{ vct.vertices.x, vct.vertices.y, vct.vertices.z },
			vct.colors.b, vct.colors.g, vct.colors.r, vct.colors.a,
			vct.texCoords.u, vct.texCoords.v
		};
		return v;
	}
	/**
	* スプライトの表示
	*/
	void SSDrawSprite(State state)
	{
		//未対応機能
		//ステータスから情報を取得し、各プラットフォームに合わせて機能を実装してください。
		//X回転、Y回転、カラーブレンド（一部のみ）

		//描画ファンクション
		//
		switch (state.blendfunc)
		{
			case BLEND_MIX:		///< 0 ブレンド（ミックス）
				if (state.opacity == 255)
				{
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, state.opacity);
				}
				else
				{
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, state.opacity);
				}
				break;
			case BLEND_MUL:		///< 1 乗算
				SetDrawBlendMode(DX_BLENDMODE_MULA, state.opacity);
				break;
			case BLEND_ADD:		///< 2 加算
				SetDrawBlendMode(DX_BLENDMODE_ADD, state.opacity);
				break;
			case BLEND_SUB:		///< 3 減算
				SetDrawBlendMode(DX_BLENDMODE_SUB, state.opacity);
				break;

		}

		if (state.flags & PART_FLAG_COLOR_BLEND)
		{
			//RGBのカラーブレンドを設定
			//厳密に再現するには専用のシェーダーを使い、テクスチャにカラー値を合成する必要がある
			//作成する場合はssShader_frag.h、CustomSpriteのコメントとなってるシェーダー処理を参考にしてください。
			if (state.colorBlendType == VERTEX_FLAG_ONE)
			{
				//単色カラーブレンド
			}
			else
			{
				//頂点カラーブレンド
				//未対応
			}
			switch (state.colorBlendFunc)
			{
			case BLEND_MIX:
				break;
			case BLEND_MUL:		///< 1 乗算
				// ブレンド方法は乗算以外未対応
				// とりあえず左上の色を反映させる
				SetDrawBright(state.quad.tl.colors.r, state.quad.tl.colors.g, state.quad.tl.colors.b);
				break;
			case BLEND_ADD:		///< 2 加算
				break;
			case BLEND_SUB:		///< 3 減算
				break;
			}
//			DrawModiGraph

		}

#ifdef UP_MINUS
		/**
		* DXライブラリのスプライト表示機能ではXとY同時拡大なので、とりあえずXスケールを使用する
		* Y反転できないので未対応
		* DrawRectRotaGraphはxとyが中心になるように、テクスチャの矩形を表示します。
		* DXライブラリのスプライト表示機能は上方向がマイナスになります。
		*/
		SetDrawBright(state.quad.bl.colors.r, state.quad.bl.colors.g, state.quad.bl.colors.b);
		DrawRectRotaGraph(
			(int)x, (int)y,	//この座標が画像の中心になります。
			(int)state.rect.origin.x, (int)state.rect.origin.y, (int)state.rect.size.width, (int)state.rect.size.height,
			scaleX, SSRadianToDegree(rotationZ),
			state.texture.handle, TRUE, state.flipX
			);
		SetDrawBright(255, 255, 255);
#else
		/**
		* DXライブラリの3D機能を使用してスプライトを表示します。
		* DXライブラリの3D機能は上方向がプラスになります。
		* 3Dを使用する場合頂点情報を使用して再現すると頂点変形やUV系のアトリビュートを反映させる事ができます。
		*/
		//描画用頂点情報を作成
		SSV3F_C4B_T2F_Quad quad;
		quad = state.quad;

#ifdef USE_VERTEX
		//原点補正
		float cx = ((state.rect.size.width) * -(state.pivotX - 0.5f));
#ifdef UP_MINUS
		float cy = ((state.rect.size.height) * -(state.pivotY - 0.5f));
#else
		float cy = ((state.rect.size.height) * +(state.pivotY - 0.5f));
#endif

		quad.tl.vertices.x += cx;
		quad.tl.vertices.y += cy;
		quad.tr.vertices.x += cx;
		quad.tr.vertices.y += cy;
		quad.bl.vertices.x += cx;
		quad.bl.vertices.y += cy;
		quad.br.vertices.x += cx;
		quad.br.vertices.y += cy;

		float x, y;
		SSMatrix t;
		t.setupTranslation(quad.tl.vertices.x, quad.tl.vertices.y, 0.0f);
		t *= state.mat;
		t.getTranslation(&x, &y);
		quad.tl.vertices.x = x;
		quad.tl.vertices.y = y;

		t.setupTranslation(quad.tr.vertices.x, quad.tr.vertices.y, 0.0f);
		t *= state.mat;
		t.getTranslation(&x, &y);
		quad.tr.vertices.x = x;
		quad.tr.vertices.y = y;

		t.setupTranslation(quad.bl.vertices.x, quad.bl.vertices.y, 0.0f);
		t *= state.mat;
		t.getTranslation(&x, &y);
		quad.bl.vertices.x = x;
		quad.bl.vertices.y = y;

		t.setupTranslation(quad.br.vertices.x, quad.br.vertices.y, 0.0f);
		t *= state.mat;
		t.getTranslation(&x, &y);
		quad.br.vertices.x = x;
		quad.br.vertices.y = y;
#else
		float x, y;
		state.mat.getTranslation(&x, &y);	/// 表示座標はマトリクスから取得します。		float rotationZ = state.Calc_rotationZ;		/// 回転値
		float scaleX = state.Calc_scaleX;							/// 拡大率
		float scaleY = state.Calc_scaleY;							/// 拡大率


		//原点計算を行う
		float cx = ((state.rect.size.width * scaleX) * -(state.pivotX - 0.5f));
#ifdef UP_MINUS
		float cy = ((state.rect.size.height * scaleY) * -(state.pivotY - 0.5f));
#else
		float cy = ((state.rect.size.height * scaleY) * +(state.pivotY - 0.5f));
#endif
		get_uv_rotation(&cx, &cy, 0, 0, rotationZ);

		x += cx;
		y += cy;

		quad.tl.vertices.x *= scaleX;
		quad.tl.vertices.y *= scaleY;
		quad.tr.vertices.x *= scaleX;
		quad.tr.vertices.y *= scaleY;
		quad.bl.vertices.x *= scaleX;
		quad.bl.vertices.y *= scaleY;
		quad.br.vertices.x *= scaleX;
		quad.br.vertices.y *= scaleY;

		//頂点の回転、3D描画はY方向が逆なので角度をマイナスで計算する
		get_uv_rotation(&quad.tl.vertices.x, &quad.tl.vertices.y, 0, 0, rotationZ);
		get_uv_rotation(&quad.tr.vertices.x, &quad.tr.vertices.y, 0, 0, rotationZ);
		get_uv_rotation(&quad.bl.vertices.x, &quad.bl.vertices.y, 0, 0, rotationZ);
		get_uv_rotation(&quad.br.vertices.x, &quad.br.vertices.y, 0, 0, rotationZ);

		quad.tl.vertices.x += x;
		quad.tl.vertices.y += y;
		quad.tr.vertices.x += x;
		quad.tr.vertices.y += y;
		quad.bl.vertices.x += x;
		quad.bl.vertices.y += y;
		quad.br.vertices.x += x;
		quad.br.vertices.y += y;
#endif
		//頂点カラーにアルファを設定
		quad.tl.colors.a = quad.bl.colors.a * state.Calc_opacity / 255;
		quad.tr.colors.a = quad.bl.colors.a * state.Calc_opacity / 255;
		quad.bl.colors.a = quad.bl.colors.a * state.Calc_opacity / 255;
		quad.br.colors.a = quad.bl.colors.a * state.Calc_opacity / 255;

		//DXライブラリ用の頂点バッファを作成する
		VERTEX_3D vertex[4] = {
			vertex3Dfrom(quad.tl),
			vertex3Dfrom(quad.bl),
			vertex3Dfrom(quad.tr),
			vertex3Dfrom(quad.br)
		};
		//3Dプリミティブの表示
		DrawPolygon3DBase(vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP, state.texture.handle, true);
#endif

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//ブレンドステートを通常へ戻す
	}

	/**
	* ユーザーデータの取得
	*/
	void SSonUserData(Player *player, UserData *userData)
	{
		//ゲーム側へユーザーデータを設定する関数を呼び出してください。
	}

	/**
	* ユーザーデータの取得
	*/
	void SSPlayEnd(Player *player)
	{
		//ゲーム側へアニメ再生終了を設定する関数を呼び出してください。
	}


	/**
	* 文字コード変換
	*/ 
	std::string utf8Togbk(const char *src)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
		unsigned short * wszGBK = new unsigned short[len + 1];
		memset(wszGBK, 0, len * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, (LPWSTR)wszGBK, len);

		len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
		char *szGBK = new char[len + 1];
		memset(szGBK, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
		std::string strTemp(szGBK);
		if (strTemp.find('?') != std::string::npos)
		{
			strTemp.assign(src);
		}
		delete[]szGBK;
		delete[]wszGBK;
		return strTemp;
	}

	/**
	* windows用パスチェック
	*/ 
	bool isAbsolutePath(const std::string& strPath)
	{
		std::string strPathAscii = utf8Togbk(strPath.c_str());
		if (strPathAscii.length() > 2
			&& ((strPathAscii[0] >= 'a' && strPathAscii[0] <= 'z') || (strPathAscii[0] >= 'A' && strPathAscii[0] <= 'Z'))
			&& strPathAscii[1] == ':')
		{
			return true;
		}
		return false;
	}

};
