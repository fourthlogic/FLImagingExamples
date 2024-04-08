#include <cstdio>
#include <locale.h>

#include <FLImaging.h>


int main()
{
	char* astrEquation[] = {
		"Linear equation",
		"Quadratic equation",
		"Cubic equation",
		"Quartic equation",
		"Quintic equation"
	};

	while(true)
	{
		CFLString<char> flstrResult = "";

		do 
		{
			char arrInput[4096] = { 0, };

			printf("Please input equation coefficient.\n");
			printf("ex) 7.2, 3.8, 10, 2.4\n");
			printf("    7.2*x^3 + 3.8*x^2 + 10*x + 2.4\n\n");
			printf("Input: ");

			// ��� ���ڿ��� �Է� �޴´�. // Receive the count string.
			fgets(arrInput, 4096, stdin);

			CFLString<char> flstrInput(arrInput);
			if(flstrInput.IsEmpty())
				break;

			// ��� ���� ������� CFLArray<std::complex<double>> ���� // Create CFLArray<std::complex<double>> to hold coefficient values
			CFLArray<std::complex<double>> flaCoef;
			int64_t i64TokenIdx = 0;

			// �Է� ���� ���ڿ��� ',' ���� �����Ͽ� double ������ ��ȯ�Ѵ�. // Separate the input string with ',' and convert it to a double value.
			while(true)
			{
				CFLString<char> flstrToken = flstrInput.Tokenize(",", i64TokenIdx);
				if(flstrToken.IsEmpty())
					break;

				if(flstrToken == "\n")
					break;

				flaCoef.PushBack(flstrToken.ToDouble());
			}

			// �ֻ��� ����� 0 �̸� �������ش�. // If the top coefficient is 0, remove it.
			while(flaCoef.GetCount())
			{
				double f64Coef = flaCoef.GetAt(0).real();
				if(f64Coef != 0.)
					break;

				flaCoef.PopFront();
			}

			// �Է� ���� ����� ������ ���� ǥ���Ѵ�. // Create and display a formula with the entered coefficients.
			CFLString<char> flstrEquation = "";

			int64_t i64Count = flaCoef.GetCount();
			if(i64Count < 2)
				break;

			CFLString<char> flstrDegree = "";
			if(i64Count < 7)
				flstrDegree = astrEquation[i64Count-2];
			else
				flstrDegree.Format("%dth degree equation", i64Count - 1);

			flstrDegree += "\n";
			printf("\n");
			printf(flstrDegree);

			for(int64_t i = 0; i < i64Count; ++i)
			{
				double f64Coef = flaCoef.GetAt(i).real();
				if(f64Coef == 0.)
					continue;

				if(!flstrEquation.IsEmpty() && f64Coef > 0.)
					flstrEquation += " + ";

				CFLString<char> flstrFormat = "";
				if(i == i64Count - 2)
				{
					flstrFormat.Format("%lf*x", f64Coef);
				}
				else if(i == i64Count - 1)
				{
					flstrFormat.Format("%lf", f64Coef);
				}
				else
				{
					flstrFormat.Format("%lf*x^%d", f64Coef, i64Count - 1 - i);
				}

				flstrEquation += flstrFormat;
			}

			printf(flstrEquation);
			printf("\n\n");

			// �������� �ظ� ������� CFLArray<std::complex<double>> ���� // Create CFLArray<std::complex<double>> to get solution of equation
			CFLArray<std::complex<double>> flaEquationResult;

			// �������� �ظ� ���´�. // Get the solution of the equation.
			CEquation::Solve(flaCoef, &flaEquationResult);

			// �������� �ظ� ǥ���Ѵ�. // Display the solution of the equation.
			flstrResult = "Result \n";

			for(int64_t i = 0; i < flaEquationResult.GetCount(); ++i)
			{
				std::complex<double> cp = flaEquationResult.GetAt(i);
				CFLString<char> flstrCP = "";
				if(cp.imag() == 0.)
					flstrCP.Format("%lf", cp.real());
				else if(cp.imag() > 0.)
					flstrCP.Format("%.15lf+%.15lfi", cp.real(), cp.imag());
				else
					flstrCP.Format("%.15lf%.15lfi", cp.real(), cp.imag());

				flstrResult += flstrCP + "\n";
			}
		}
		while(false);

		if(flstrResult.IsEmpty())
			flstrResult = "Please check the input.\n";

		flstrResult += "\n\n";

		printf(flstrResult);
	}

	return 0;
}