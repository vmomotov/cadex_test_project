#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <omp.h>

#include "curves.h"

using namespace std;

class CurvesRandomizer {
public:
	static vector<shared_ptr<Curve> > CreateCurvesVector(size_t minElementsCount, size_t maxElementsCount, size_t maxCoordinate, size_t maxRadius, size_t maxHelixStep) {
		srand(time(0));
		size_t outVectorSize = rand() % (maxElementsCount - minElementsCount + 1) + minElementsCount;
		vector<shared_ptr<Curve> > outVector = {};
		outVector.reserve(outVectorSize);
		for (size_t i = 0; i != outVectorSize; i++) {
			outVector.push_back(CreateCurve(maxCoordinate, maxRadius, maxHelixStep));
		}
		return outVector;
	}
private:
	// for simplification we initialize parameters with integer values (despite their type is double)
	static Circle CreateCircle(size_t maxCoordinate, size_t maxRadius) {
		return Circle(Vect3D(rand() % maxCoordinate
							, rand() % maxCoordinate
							, 0)
					, rand() % maxRadius);
	}
	static Ellipse CreateEllipse(size_t maxCoordinate, size_t maxRadius) {
		return Ellipse(Vect3D(rand() % maxCoordinate
							, rand() % maxCoordinate
							, 0)
					, rand() % maxRadius
					, rand() % maxRadius);
	}
	static Helix CreateHelix(size_t maxCoordinate, size_t maxRadius, size_t maxStep) {
		return Helix(Vect3D(rand() % maxCoordinate
							, rand() % maxCoordinate
							, rand() % maxCoordinate)
					, rand() % maxRadius
					, rand() % maxStep);
	}
	static shared_ptr<Curve> CreateCurve(size_t maxCoordinate, size_t maxRadius, size_t maxStep) {
		unsigned char curveType = rand() % 3;
		shared_ptr<Curve> pResult = nullptr;
		switch (curveType) {
			case 0:
				pResult = shared_ptr<Curve>(make_shared<Circle>(CreateCircle(maxCoordinate, maxRadius)));
				break;
			case 1:
				pResult = shared_ptr<Curve>(make_shared<Ellipse>(CreateEllipse(maxCoordinate, maxRadius)));
				break;
			case 2:
				pResult = shared_ptr<Curve>(make_shared<Helix>(CreateHelix(maxCoordinate, maxRadius, maxStep)));
				break;
			default:
				break;
		}
		return pResult;
	}
};

class CurvesPrinter {
public:
	static void PrintPointsAndDerivativesAtParameter(const vector<shared_ptr<Curve> >& curvesVector, double angleParameter) {
		for (const shared_ptr<Curve>& pCurve : curvesVector) {
			cout << "Point x: " << pCurve->GetPoint(angleParameter).x << " y: " << pCurve->GetPoint(angleParameter).y << " z: " << pCurve->GetPoint(angleParameter).z << endl;
			cout << "Derivate x: " << pCurve->GetFirstDerivative(angleParameter).x << " y: " << pCurve->GetFirstDerivative(angleParameter).y << " z: " << pCurve->GetFirstDerivative(angleParameter).z << endl;
		}
	}
};

class CirclesPerformer {
public:
	static vector<shared_ptr<Circle> > CreateCirclesVector(const vector<shared_ptr<Curve> >& curvesVector) {
		vector<shared_ptr<Circle> > outCirclesVector = {};
		outCirclesVector.reserve(curvesVector.size()); // in worst case all curves are circles
		for (const shared_ptr<Curve>& pCurve : curvesVector) {
			shared_ptr<Circle> pCircle = dynamic_pointer_cast<Circle>(pCurve); // if object does not belong to Circle class, dynamic_pointer_cast will return nullptr
			if (pCircle != nullptr) {
				outCirclesVector.push_back(pCircle);
			}
		}
		return outCirclesVector;
	}
	static void SortCirclesVector(vector<shared_ptr<Circle> >& circlesVector) {
		sort(circlesVector.begin()
			, circlesVector.end()
			, [](const shared_ptr<Circle>& p1, const shared_ptr<Circle>& p2) {
				return p1->GetRadius() < p2->GetRadius();
			});
	}

	static double GetSumOfRadii(const vector<shared_ptr<Circle> >& circlesVector) {
		double sum = 0;
		#pragma omp parallel for reduction (+:sum)
		for (const auto& pCircle : circlesVector) {
			sum += pCircle->GetRadius();
		};
		return sum;
	}
};

int main() {
	cout << "--- Test project for CAD Exchanger ---" << endl;

	size_t minElementsCount = 5;
	size_t maxElementsCount = 10;
	size_t maxCoordinate = 100;
	size_t maxRadius = 50;
	size_t maxHelixStep = 10;
	auto curvesVector = CurvesRandomizer::CreateCurvesVector(minElementsCount, maxElementsCount, maxCoordinate, maxRadius, maxHelixStep);
	cout << "--- Vector of curves created ---" << endl;

	double angle_t = M_PI_4;
	CurvesPrinter::PrintPointsAndDerivativesAtParameter(curvesVector, angle_t);
	cout << "--- Point and derivatives printed ---" << endl;

	auto circlesVector = CirclesPerformer::CreateCirclesVector(curvesVector);
	cout << "--- Vector of circles created ---" << endl;

	CirclesPerformer::SortCirclesVector(circlesVector);
	cout << "--- Vector of circles sorted ---" << endl;

	double sum = CirclesPerformer::GetSumOfRadii(circlesVector);
	cout << "--- Sum of radii is " << sum << " ---" << endl;

	system("pause");
	return 0;
}