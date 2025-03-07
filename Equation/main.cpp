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

			// 계수 문자열을 입력 받는다. // Receive the count string.
			fgets(arrInput, 4096, stdin);

			CFLString<char> flstrInput(arrInput);
			if(flstrInput.IsEmpty())
				break;

			// 계수 값을 담기위해 CFLArray<std::complex<double>> 생성 // Create CFLArray<std::complex<double>> to hold coefficient values
			CFLArray<std::complex<double>> flaCoef;
			int64_t i64TokenIdx = 0;

			// 입력 받은 문자열을 ',' 으로 구분하여 double 값으로 변환한다. // Separate the input string with ',' and convert it to a double value.
			while(true)
			{
				CFLString<char> flstrToken = flstrInput.Tokenize(",", i64TokenIdx);
				if(flstrToken.IsEmpty())
					break;

				if(flstrToken == "\n")
					break;

				flaCoef.PushBack(flstrToken.ToDouble());
			}

			// 최상위 계수가 0 이면 제거해준다. // If the top coefficient is 0, remove it.
			while(flaCoef.GetCount())
			{
				double f64Coef = flaCoef.GetAt(0).real();
				if(f64Coef != 0.)
					break;

				flaCoef.PopFront();
			}

			// 입력 받은 계수로 수식을 만들어서 표시한다. // Create and display a formula with the entered coefficients.
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

			// 방정식의 해를 얻기위해 CFLArray<std::complex<double>> 생성 // Create CFLArray<std::complex<double>> to get solution of equation
			CFLArray<std::complex<double>> flaEquationResult;

			// 방정식의 해를 얻어온다. // Get the solution of the equation.
			CEquation::Solve(flaCoef, &flaEquationResult);

			// 방정식의 해를 표시한다. // Display the solution of the equation.
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