//text
	bool GetCharDesc(wchar_t Char, CharDesc* Desc)
	{
		//insert font_char.h

		//no char
		return false;
	}

	Vector2 GetTextSize(const wchar_t* String, CharDesc* DescArray, int* DescSize)
	{
		wchar_t WChar;
		int DescArrayOffset = 0;
		Vector2 TextSize = { 0.f, 0.f };
		for (int i = 0; (WChar = String[i]); ++i)
		{
			//get char description
			if (GetCharDesc(WChar, &DescArray[DescArrayOffset]))
			{
				//get char size
				CharDesc CDesc = DescArray[DescArrayOffset++];
				float YSize = float(CDesc.yOff + CDesc.srcH);
				TextSize.x += (float)CDesc.xAdv;
				TextSize.y = max(YSize, TextSize.y);
			}
		}

		//save size & ret text size
		*DescSize = DescArrayOffset;
		return TextSize;
	}

Vector2 String(Vector2 Start, const wchar_t* String, bool Center = false, FColor Color = FColor(255, 255, 255))
	{
		//get char desc & string size (in px)
		if (String == nullptr) return Vector2{ 0.f, 0.f };
		int CharDescSize = 0; CharDesc CharDesc[256];
		Vector2 TextSize = GetTextSize(String, CharDesc, &CharDescSize);
		if (Center) { Start -= (TextSize / 2.f); }

		auto drawTexture = [&](const Vector2& Start, uint32_t* Texture, const Vector2& TexOff, const Vector2& RectSize)/* [[msvc::forceinline]]*/ {
			for (int y = 0; y < RectSize.y; y++) {
				for (int x = 0; x < RectSize.x; x++) {
					//int off1 = (Start.x + x) + ((Start.y + y) * w);
					int off2 = (TexOff.x + x) + ((TexOff.y + y) * 170);

					//if (!FColor(Texture[off2]).RGBA[0] && !FColor(Texture[off2]).RGBA[1] && !FColor(Texture[off2]).RGBA[2])
					{
						//
						//auto gg = FColor(Texture[off2]);
						//auto colorAvg =/* 1.f -*/ ((float)((gg.RGBA[0] + gg.RGBA[1] + gg.RGBA[2]) / 3) / 255.f);
						//auto newg = FColor((float)Color.RGBA[0] * colorAvg, (float)Color.RGBA[1] * colorAvg, (float)Color.RGBA[2] * colorAvg, FColor(Texture[off2]).RGBA[3]);


						SetPixel(Start.x + x, Start.y + y, Texture[off2], 255 - FColor(Texture[off2]).RGBA[3]);

						//printf("%d\n\n", FColor(Texture[off2]).RGBA[3]);
						//Arr[off1] = Texture[off2];
					}
					//else 
					{
						//SetPixel(Start.x + x, Start.y + y, ColorBlend(Texture[off2], Color), 0);
					}
				}
			}
		};

		//draw chars
		for (int i = 0; i < CharDescSize; ++i)
		{
			//add vertex to draw list
			//auto FillChar = RenderToolKit::AddDrawCmd(4, 6);
			float srcX = (float)CharDesc[i].srcX, srcY = (float)CharDesc[i].srcY,
				srcW = (float)CharDesc[i].srcW, srcH = (float)CharDesc[i].srcH,
				xOff = (float)CharDesc[i].xOff, yOff = (float)CharDesc[i].yOff,
				xAdv = (float)CharDesc[i].xAdv;



			drawTexture({ Start.x + (xOff /** MP*/), Start.y + (yOff /** MP*/) }, (uint32_t*)&FontBytes[0], { srcX, srcY }, { srcW, srcH });



			//add vertexes
			//auto _VtxWritePtr = FillChar.VBuffer;

			//_VtxWritePtr[0] = { { Start.x + (xOff * MP), //top left
			//	Start.y + (yOff * MP) }, { srcX, srcY }, Color.DXColor };
			//
			//_VtxWritePtr[1] = { { Start.x + ((xOff + srcW) * MP), //upper right
			//	Start.y + (yOff * MP) }, { srcX + srcW, srcY }, Color.DXColor };
			//
			//_VtxWritePtr[2] = { { Start.x + (xOff * MP), //bottom left
			//	Start.y + ((yOff + srcH) * MP) }, { srcX, srcY + srcH }, Color.DXColor };
			//
			//_VtxWritePtr[3] = { { Start.x + ((xOff + srcW) * MP), //bottom right
			//	Start.y + ((yOff + srcH) * MP) }, { srcX + srcW, srcY + srcH }, Color.DXColor };

			//add indexes
			//FillChar.IBuffer[0] = 0; FillChar.IBuffer[1] = 1; FillChar.IBuffer[2] = 2;
			//FillChar.IBuffer[3] = 3; FillChar.IBuffer[4] = 1; FillChar.IBuffer[5] = 2;

			//set char spacing
			Start.x += (xAdv /** MP*/);
		}

		return TextSize;
	}

void DecompressFont()
{
	//decompress font bytes
	struct CByte { BYTE Series; BYTE DataByte; };
	CByte* CCByte = (CByte*)&FontBytesComp[0];
	int CCBytesSize = (sizeof(FontBytesComp) / sizeof(CByte));
	for (int Pos = 0, Off = 0; Pos < CCBytesSize; ++Pos, ++CCByte) {
		__stosb(&FontBytes[Off], CCByte->DataByte, CCByte->Series);
		Off += CCByte->Series;
	}
}