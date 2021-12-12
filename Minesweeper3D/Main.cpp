# include <Siv3D.hpp> // OpenSiv3D v0.6.2
# include "Array3D.hpp"
// # include "TorusIntersect.hpp"


void Main()
{
	Window::Resize(1280, 720);
	const ColorF backgroundColor = ColorF{ 0.4, 0.6, 0.8 }.removeSRGBCurve();
	const Texture uvChecker{ U"example/texture/uv.png", TextureDesc::MippedSRGB };
	const Texture earthTexture{ U"example/texture/earth.jpg", TextureDesc::MippedSRGB };
	const Texture woodTexture{ U"example/texture/wood.jpg", TextureDesc::MippedSRGB };
	const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };
	BasicCamera3D camera{ renderTexture.size(), 30_deg, Vec3{ 9, 19, -29 } };
	FontAsset::Register(U"Number", 10, Typeface::Medium);

	const int numOfBlock = 7;

	OrientedBox box{ 0,4,0,1 }, box2{4};
	
	Quaternion rotation;
	bool grabbedX = false;
	bool grabbedY = false;


	const Disc diY{ box.center, 1 };
	

	const Cylinder cY{ box.center, 6.0, 1 };
	const Cylinder cX{ box.center, 6.0, 1, Quaternion::RotateZ(90_deg) };
	const Mesh torusY{ MeshData::Torus(box.center, 6.0, 0.6) };
	const Mesh torusX{ MeshData::Torus(6.0, 0.6) };

	const Mesh billboard{ MeshData::Billboard() };

	bool isGameover = false;
	Array<size_t> checked(numOfBlock * numOfBlock * numOfBlock);

	Array3D<std::pair<int32, OrientedBox>> boxes(numOfBlock, numOfBlock, numOfBlock);
	for (int32 i = 0; i < numOfBlock; i++)
	{
		for (int32 j = 0; j < numOfBlock; j++)
		{
			for (int32 k = 0; k < numOfBlock; k++)
			{
				boxes.set(i, j, k, { (int32)RandomBool(0.2), OrientedBox{ Vec3{i - numOfBlock / 2, j - numOfBlock / 2, k - numOfBlock / 2} * rotation + Vec3{0, 4, 0},
								 Vec3{0.9, 0.9, 0.9}, rotation} });
			}
		}
	}

	while (System::Update())
	{
		ClearPrint();
		// camera.update(2.0);
		Graphics3D::SetCameraTransform(camera);
		// Print << camera.getEyePosition();  // デバッグ用カメラの位置を取得

		{
			const Ray ray = camera.screenToRay(Cursor::PosF());
			const ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };
			
			Plane{ 64 }.draw(uvChecker);

			// 交差し、もっとも手前にあるボックスのインデックス
			size_t boxIndex = 998244353;
			double minDistance = Math::Inf;

			for (int i = 0; i < numOfBlock; i++) {
				for (int j = 0; j < numOfBlock; j++) {
					for (int k = 0; k < numOfBlock; k++) {
						auto& tmpbox = boxes.get(i, j, k);
						tmpbox.second.setPos(Vec3{ i - numOfBlock / 2, j - numOfBlock / 2, k - numOfBlock / 2 } *rotation + Vec3{ 0, 4, 0 });
						tmpbox.second.setOrientation(rotation);
						if (tmpbox.first >= 0)
						{
							if (const auto distance = ray.intersects(tmpbox.second))
							{
								if (*distance < minDistance)
								{
									minDistance = *distance;
									boxIndex = boxes.get_index(i, j, k);
								}
							}
							if(checked[boxes.get_index(i, j, k)])
							{
								tmpbox.second.draw(ColorF{ 1.0, 0.8, 0.0, 0.8 });
							}
							else
							{
								tmpbox.second.draw(ColorF{ 1.0, 1.0, 1.0, 0.8 });
							}
							
							// FontAsset(U"Number")(U"0").draw(camera.billboard(Vec3{i - numOfBlock / 2, j - numOfBlock / 2, k - numOfBlock / 2} *rotation + Vec3{0, 4, 0}, 1));
						}
						boxes.set(i, j, k, tmpbox);
						
					}
				}
			}

			if (minDistance != Math::Inf && boxIndex < 998244353)
			{
				auto& tmpbox = boxes.get(boxIndex);
				if (MouseL.down())
				{
					if (tmpbox.first > 0)
					{
						// Print << U"Game Over";
						isGameover = true;
					}
					else
					{
						boxes.set(boxIndex, { -1, tmpbox.second });
					}
				}
				// 印を付ける
				else if (MouseR.down())
				{
					// 状態を反転
					checked[boxIndex] ^= 1;
				}
			}

			if (isGameover)
			{
				Print << U"Game Over";
			}


			// torusY.draw(ColorF{ (grabbedY ? 0.8 : 0.5), 0.0, 0.0 }.removeSRGBCurve());
			// torusX.draw(box.center, Quaternion::RotateZ(90_deg), ColorF{ 0.0, (grabbedX ? 0.8 : 0.5), 0.0 }.removeSRGBCurve());

			/*if (grabbedX)
			{
				rotation *= Quaternion::RotateX(-1_deg * Cursor::DeltaF().y);
			}

			if (grabbedY)
			{
				rotation *= Quaternion::RotateY(-1_deg * Cursor::DeltaF().x);
			}*/

			if (KeyA.pressed())
			{
				rotation *= Quaternion::RotateY(1_deg);
			}
			if (KeyD.pressed())
			{
				rotation *= Quaternion::RotateY(-1_deg);
			}
			if (KeyW.pressed())
			{
				rotation *= Quaternion::RotateX(1_deg);
			}
			if (KeyS.pressed())
			{
				rotation *= Quaternion::RotateX(-1_deg);
			}
			// const Optional<float> cyd = TorusXYIntersect(ray, { box.center, 6.0}, box.center);
			//const Optional<float> cyd = ray.intersects(cY);
			//const Optional<float> cxd = ray.intersects(cX);
			//// torusX.boundingSphere().draw();

			//Print << U"cyd: " << cyd;
			//Print << U"cxd: " << cxd;

			//if (cyd || cxd)
			//{
			//	Cursor::RequestStyle(CursorStyle::Hand);

			//	if (MouseL.down())
			//	{
			//		if (cxd && cyd)
			//		{
			//			((cxd < cyd) ? grabbedX : grabbedY) = true;
			//		}
			//		else
			//		{
			//			((cxd) ? grabbedX : grabbedY) = true;
			//		}
			//	}
			//}

			//if (MouseL.up())
			//{
			//	grabbedY = grabbedX = false;
			//}
		}

		{
			Graphics3D::Flush();
			renderTexture.resolve();
			Shader::LinearToScreen(renderTexture);
		}
	}
}
