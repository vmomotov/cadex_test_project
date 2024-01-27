#pragma once

struct Vect3D {
	double x = 0;
	double y = 0;
	double z = 0;

	Vect3D() = default;

	Vect3D(double _x, double _y, double _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{}

	Vect3D(const Vect3D& vect)
		: x(vect.x)
		, y(vect.y)
		, z(vect.z)
	{}
};

class Curve {
public:
	Curve(const Vect3D& centerPoint)
		: m_centerPoint(centerPoint)
	{}

	virtual Vect3D GetPoint(double angle) const = 0;
	virtual Vect3D GetFirstDerivative(double angle) const = 0;
	Vect3D GetCenter() const { return m_centerPoint; }
	virtual ~Curve() = default;
protected:
	Vect3D m_centerPoint;
};

class Circle : public Curve {
public:
	Circle(const Vect3D& centerPoint, double radius)
		: Curve(centerPoint)
		, m_radius(radius)
	{}

	virtual Vect3D GetPoint(double angle) const override {
		return Vect3D(m_centerPoint.x + m_radius * cos(angle)
					, m_centerPoint.y + m_radius * sin(angle)
					, m_centerPoint.z);
	}

	virtual Vect3D GetFirstDerivative(double angle) const override {
		return Vect3D(-m_radius * sin(angle)
					, m_radius * cos(angle)
					, 0);
	}

	double GetRadius() const { return m_radius; }

private:
	double m_radius;
};

class Ellipse : public Curve {
public:
	Ellipse(const Vect3D& centerPoint, double xRadius, double yRadius)
		: Curve(centerPoint)
		, m_xRadius(xRadius)
		, m_yRadius(yRadius)
	{}

	virtual Vect3D GetPoint(double angle) const override {
		return Vect3D(m_centerPoint.x + m_xRadius * cos(angle)
					, m_centerPoint.y + m_yRadius * sin(angle)
					, m_centerPoint.z);
	}

	virtual Vect3D GetFirstDerivative(double angle) const override {
		return Vect3D( - m_xRadius * sin(angle)
					, m_yRadius * cos(angle)
					, 0);
	}

	std::pair<double, double> GetRadii() const { return std::pair<double, double>(m_xRadius, m_yRadius); }

private:
	double m_xRadius;
	double m_yRadius;
};

class Helix : public Curve {
public:
	Helix(const Vect3D& centerPoint, double radius, double step)
		: Curve(centerPoint)
		, m_radius(radius)
		, m_step(step)
	{}

	virtual Vect3D GetPoint(double angle) const override {
		return Vect3D(m_centerPoint.x + m_radius * cos(angle)
					, m_centerPoint.y + m_radius * sin(angle)
					, m_centerPoint.z + m_step * angle / (2 * M_PI));
	}

	virtual Vect3D GetFirstDerivative(double angle) const override {
		return Vect3D( - m_radius * sin(angle)
					, m_radius * cos(angle)
					, m_step / (2 * M_PI));
	}

	double GetRadius() const { return m_radius; }
	double GetStep() const { return m_step; }

private:
	double m_radius;
	double m_step;
};