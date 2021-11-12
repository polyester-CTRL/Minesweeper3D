# include <Siv3D.hpp> // OpenSiv3D v0.6.2

void Main()
{
	Window::Resize(1280, 720);
	const ColorF backgroundColor = ColorF{ 0.4, 0.6, 0.8 }.removeSRGBCurve();
	const Texture uvChecker{ U"example/texture/uv.png", TextureDesc::MippedSRGB };
	const Texture earthTexture{ U"example/texture/earth.jpg", TextureDesc::MippedSRGB };
	const Texture woodTexture{ U"example/texture/wood.jpg", TextureDesc::MippedSRGB };
	const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };
	DebugCamera3D camera{ renderTexture.size(), 30_deg, Vec3{ 10, 16, -32 } };

	const OrientedBox box{ 0,4,0,4 };
	Quaternion rotation;
	bool grabbedX = false;
	bool grabbedY = false;

	const Cylinder cY{ box.center, 4.6, 1 };
	const Cylinder cX{ box.center, 4.5, 1, Quaternion::RotateZ(90_deg) };

	while (System::Update())
	{
		ClearPrint();
		camera.update(2.0);
		Graphics3D::SetCameraTransform(camera);

		{
			const Ray ray = camera.screenToRay(Cursor::PosF());
			const ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };
			Plane{ 64 }.draw(uvChecker);
			box.draw(rotation, woodTexture);

			cY.draw(ColorF{ (grabbedY ? 0.8 : 0.5), 0.0, 0.0 }.removeSRGBCurve());
			cX.draw(ColorF{ 0.0, (grabbedX ? 0.8 : 0.5), 0.0 }.removeSRGBCurve());

			if (grabbedX)
			{
				rotation *= Quaternion::RotateX(-1_deg * Cursor::DeltaF().y);
			}

			if (grabbedY)
			{
				rotation *= Quaternion::RotateY(-1_deg * Cursor::DeltaF().x);
			}

			const Optional<float> cyd = ray.intersects(cY);
			const Optional<float> cxd = ray.intersects(cX);

			Print << U"cyd: " << cyd;
			Print << U"cxd: " << cxd;

			if (cyd || cxd)
			{
				Cursor::RequestStyle(CursorStyle::Hand);

				if (MouseL.down())
				{
					if (cxd && cyd)
					{
						((cxd < cyd) ? grabbedX : grabbedY) = true;
					}
					else
					{
						((cxd) ? grabbedX : grabbedY) = true;
					}
				}
			}

			if (MouseL.up())
			{
				grabbedY = grabbedX = false;
			}
		}

		{
			Graphics3D::Flush();
			renderTexture.resolve();
			Shader::LinearToScreen(renderTexture);
		}
	}
}
