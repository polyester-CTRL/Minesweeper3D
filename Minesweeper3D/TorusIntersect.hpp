# include <Siv3D.hpp> // OpenSiv3D v0.6.2
double TorusIntersect_implXZ(const Ray& ray, const Vec2& tor)
{
	const Vec3 ro = ray.origin.xyz();
	const Vec3 rd = ray.direction.xyz();

	double po = 1.0;
	double Ra2 = tor.x * tor.x;
	double ra2 = tor.y * tor.y;
	double m = ro.dot(ro);
	double n = ro.dot(rd);
	double k = (m + Ra2 - ra2) / 2.0;
	double k3 = n;
	double k2 = n * n - Ra2 * rd.xz().dot(rd.xz()) + k;
	double k1 = n * k - Ra2 * rd.xz().dot(ro.xz());
	double k0 = k * k - Ra2 * ro.xz().dot(ro.xz());

	if (std::abs(k3 * (k3 * k3 - k2) + k1) < 0.01)
	{
		po = -1.0;
		double tmp = k1; k1 = k3; k3 = tmp;
		k0 = 1.0 / k0;
		k1 = k1 * k0;
		k2 = k2 * k0;
		k3 = k3 * k0;
	}

	double c2 = k2 * 2.0 - 3.0 * k3 * k3;
	double c1 = k3 * (k3 * k3 - k2) + k1;
	double c0 = k3 * (k3 * (c2 + 2.0 * k2) - 8.0 * k1) + 4.0 * k0;
	c2 /= 3.0;
	c1 *= 2.0;
	c0 /= 3.0;
	double Q = c2 * c2 + c0;
	double R = c2 * c2 * c2 - 3.0 * c2 * c0 + c1 * c1;
	double h = R * R - Q * Q * Q;

	if (h >= 0.0)
	{
		h = sqrt(h);
		double v = Math::Sign(R + h) * pow(abs(R + h), 1.0 / 3.0); // cube root
		double u = Math::Sign(R - h) * pow(abs(R - h), 1.0 / 3.0); // cube root
		const Vec2 s = Vec2((v + u) + 4.0 * c2, (v - u) * std::sqrt(3.0));
		double y = sqrt(0.5 * (s.length() + s.x));
		double x = 0.5 * s.y / y;
		double r = 2.0 * c1 / (x * x + y * y);
		double t1 = x - r - k3; t1 = (po < 0.0) ? 2.0 / t1 : t1;
		double t2 = -x - r - k3; t2 = (po < 0.0) ? 2.0 / t2 : t2;
		double t = 1e20;
		if (t1 > 0.0) t = t1;
		if (t2 > 0.0) t = Min(t, t2);
		return t;
	}

	double sQ = sqrt(Q);
	double w = sQ * cos(acos(-R / (sQ * Q)) / 3.0);
	double d2 = -(w + c2); if (d2 < 0.0) return -1.0;
	double d1 = sqrt(d2);
	double h1 = sqrt(w - 2.0 * c2 + c1 / d1);
	double h2 = sqrt(w - 2.0 * c2 - c1 / d1);
	double t1 = -d1 - h1 - k3; t1 = (po < 0.0) ? 2.0 / t1 : t1;
	double t2 = -d1 + h1 - k3; t2 = (po < 0.0) ? 2.0 / t2 : t2;
	double t3 = d1 - h2 - k3; t3 = (po < 0.0) ? 2.0 / t3 : t3;
	double t4 = d1 + h2 - k3; t4 = (po < 0.0) ? 2.0 / t4 : t4;
	double t = 1e20;
	if (t1 > 0.0) t = t1;
	if (t2 > 0.0) t = Min(t, t2);
	if (t3 > 0.0) t = Min(t, t3);
	if (t4 > 0.0) t = Min(t, t4);
	return t;
}

double TorusIntersect_implXY(const Ray& ray, const Vec2& tor)
{
	const Vec3 ro = ray.origin.xyz();
	const Vec3 rd = ray.direction.xyz();

	double po = 1.0;
	double Ra2 = tor.x * tor.x;
	double ra2 = tor.y * tor.y;
	double m = ro.dot(ro);
	double n = ro.dot(rd);
	double k = (m + Ra2 - ra2) / 2.0;
	double k3 = n;
	double k2 = n * n - Ra2 * rd.xy().dot(rd.xy()) + k;
	double k1 = n * k - Ra2 * rd.xy().dot(ro.xy());
	double k0 = k * k - Ra2 * ro.xy().dot(ro.xy());

	if (std::abs(k3 * (k3 * k3 - k2) + k1) < 0.01)
	{
		po = -1.0;
		double tmp = k1; k1 = k3; k3 = tmp;
		k0 = 1.0 / k0;
		k1 = k1 * k0;
		k2 = k2 * k0;
		k3 = k3 * k0;
	}

	double c2 = k2 * 2.0 - 3.0 * k3 * k3;
	double c1 = k3 * (k3 * k3 - k2) + k1;
	double c0 = k3 * (k3 * (c2 + 2.0 * k2) - 8.0 * k1) + 4.0 * k0;
	c2 /= 3.0;
	c1 *= 2.0;
	c0 /= 3.0;
	double Q = c2 * c2 + c0;
	double R = c2 * c2 * c2 - 3.0 * c2 * c0 + c1 * c1;
	double h = R * R - Q * Q * Q;

	if (h >= 0.0)
	{
		h = sqrt(h);
		double v = Math::Sign(R + h) * pow(abs(R + h), 1.0 / 3.0); // cube root
		double u = Math::Sign(R - h) * pow(abs(R - h), 1.0 / 3.0); // cube root
		const Vec2 s = Vec2((v + u) + 4.0 * c2, (v - u) * std::sqrt(3.0));
		double y = sqrt(0.5 * (s.length() + s.x));
		double x = 0.5 * s.y / y;
		double r = 2.0 * c1 / (x * x + y * y);
		double t1 = x - r - k3; t1 = (po < 0.0) ? 2.0 / t1 : t1;
		double t2 = -x - r - k3; t2 = (po < 0.0) ? 2.0 / t2 : t2;
		double t = 1e20;
		if (t1 > 0.0) t = t1;
		if (t2 > 0.0) t = Min(t, t2);
		return t;
	}

	double sQ = sqrt(Q);
	double w = sQ * cos(acos(-R / (sQ * Q)) / 3.0);
	double d2 = -(w + c2); if (d2 < 0.0) return -1.0;
	double d1 = sqrt(d2);
	double h1 = sqrt(w - 2.0 * c2 + c1 / d1);
	double h2 = sqrt(w - 2.0 * c2 - c1 / d1);
	double t1 = -d1 - h1 - k3; t1 = (po < 0.0) ? 2.0 / t1 : t1;
	double t2 = -d1 + h1 - k3; t2 = (po < 0.0) ? 2.0 / t2 : t2;
	double t3 = d1 - h2 - k3; t3 = (po < 0.0) ? 2.0 / t3 : t3;
	double t4 = d1 + h2 - k3; t4 = (po < 0.0) ? 2.0 / t4 : t4;
	double t = 1e20;
	if (t1 > 0.0) t = t1;
	if (t2 > 0.0) t = Min(t, t2);
	if (t3 > 0.0) t = Min(t, t3);
	if (t4 > 0.0) t = Min(t, t4);
	return t;
}

bool TorusXYIntersect(const Ray& ray, const Vec2& tor, const Vec3& torusPos)
{
	Ray r = ray;
	r.origin = SIMD_double4{ r.origin.xyz() - torusPos, 0.0f };
	return (0.0 < TorusIntersect_implXY(r, tor));
}

bool TorusXZIntersect(const Ray& ray, const Vec2& tor, const Vec3& torusPos)
{
	Ray r = ray;
	r.origin = SIMD_double4{ r.origin.xyz() - torusPos, 0.0f };
	return (0.0 < TorusIntersect_implXZ(r, tor));
}
