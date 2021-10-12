#include "omp.h"
#include <iostream>
#include <inttypes.h>
#include <chrono>
#include <vector>
#include <string>
#include <random>

using namespace std;
using namespace std::chrono;

using MatrixType = int64_t;
using Row = vector<MatrixType>;
using Matrix = vector<Row>;

Matrix Product(const Matrix& A, const Matrix& B) {
	if (B.size() == 0) {
		return {};
	}

	const size_t abCommonCount = B.size();
	const size_t cRowCount = A.size();
	const size_t cColCount = B.at(0).size();

	Matrix C(cRowCount, Row(cColCount, 0));

	MatrixType item;

	for (size_t row = 0; row < cRowCount; ++row) {
		for (size_t col = 0; col < cColCount; ++col) {
			item = 0;
			for (size_t ab = 0; ab < abCommonCount; ++ab) {
				item += A.at(row).at(ab) * B.at(ab).at(col);
			}
			C[row][col] = item;
		}
	}
    return C;
}

Matrix OmpProduct(const Matrix& A, const Matrix& B) {
	if (B.size() == 0) {
		return {};
	}

	const size_t abCommonCount = B.size();
	const size_t cRowCount = A.size();
	const size_t cColCount = B.at(0).size();

	Matrix C(cRowCount, Row(cColCount, 0));

	MatrixType item;
	size_t row, col, ab;

#pragma omp parallel shared(A, B, C)
{
#pragma omp for private(row, col, ab, item)
	for (row = 0; row < cRowCount; ++row) {
		for (col = 0; col < cColCount; ++col) {
			item = 0;
			for (ab = 0; ab < abCommonCount; ++ab) {
				item += A.at(row).at(ab) * B.at(ab).at(col);
			}
			C[row][col] = item;
		}
	}
}
    return C;
}

Matrix GenerateMatrix(size_t rowCount, size_t colCount) {
	Matrix matrix(rowCount, Row(colCount));

	random_device dev;
	mt19937 rng(dev());
	uniform_int_distribution<mt19937::result_type> dist(1, 10);

	for (Row& row : matrix) {
		for (MatrixType& num : row) {
			num = dist(rng);
		}
	}	

	return matrix;
}

int main()
{
	while(true) {
		system("clear");

		size_t aRowCount, bRowCount, aColCount, bColCount;

		{
			string aRowCountS, bRowCountS, aColCountS, bColCountS;
			cout << "Operation A * B" << '\n';
			cout << "Row count of A = ";
			cin >> aRowCountS;
			cout << "Col count of A = ";
			cin >> aColCountS;
			cout << "Row count of B = ";
			cout << (bRowCountS = aColCountS) << '\n';
			cout << "Col count of B = ";
			cin >> bColCountS;

			try {
				aRowCount = stoi(aRowCountS);
				bRowCount = stoi(bRowCountS);
				aColCount = stoi(aColCountS);
				bColCount = stoi(bColCountS);
				if (aRowCount <= 0 || bRowCount <= 0 || aColCount <= 0 || bColCount <= 0) {
					continue;
				}
			}
			catch(...) {continue;}
		}

		cout << '\n' << "Generating matrices..." << endl;

		const Matrix A = GenerateMatrix(aRowCount, aColCount);
		const Matrix B = GenerateMatrix(bRowCount, bColCount);

		cout << '\n' << "Computing product..." << endl;

		const auto [time, res] = [&A, &B]()-> pair<uint64_t, Matrix> {
			Matrix res;
			uint32_t time;

			auto start_time = chrono::high_resolution_clock::now();

			res = Product(A, B);

			auto finish_time = std::chrono::high_resolution_clock::now();
			time = duration_cast<milliseconds>(finish_time - start_time).count();

			return {time, move(res)};
		}();

		cout << '\n' << "Computing omp product..." << endl;

		const auto [timeOmp, resOmp] = [&A, &B]() -> pair<uint64_t, Matrix> {
			Matrix res;
			uint32_t time;

			auto start_time = chrono::high_resolution_clock::now();

			res = OmpProduct(A, B);

			auto finish_time = std::chrono::high_resolution_clock::now();
			time = duration_cast<milliseconds>(finish_time - start_time).count();

			return {time, move(res)};
		}();

		const bool equal = res == resOmp;

		cout << '\n';

		if (equal) cout << "Products are equal";
		else cout << "Products are NOT equal";
		
		cout << '\n';

		cout << "Product computing time: " << time << " ms\n";
		cout << "Product omp computing time: " << timeOmp << " ms\n";

		int c;
        while((c = getchar()) != '\n' && c != EOF);
        cin.get();
        break;
	}
}
