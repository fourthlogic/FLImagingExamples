#include <cstdio>

#include <FLImaging.h>


int main()
{
	// ���� ��ü ���� // Declare permutation object
	CPermutation P;

	while(true)
	{
		CFLString<char> flstrResult = "";

		do
		{
			char arrInput[4096] = { 0, };

			printf("Please input n and k as n, k.\n");
			printf("Permutation : k objects are selected from a set of n objects to produce subsets with ordering\n");
			printf("ex) 6, 2\n");
			printf("Input: ");

			// n, k ���ڿ��� �Է� �޴´�. // Receive n, k strings.
			fgets(arrInput, 4096, stdin);

			CFLString<char> flstrInput(arrInput);
			if(flstrInput.IsEmpty())
				break;

			// ��� ���� ������� CFLArray<int> ���� // Create CFLArray<int> to hold coefficient values
			CFLArray<int> flaNK;
			int32_t i32TokenIdx = 0;

			// �Է� ���� ���ڿ��� ',' ���� �����Ͽ� int ������ ��ȯ�Ѵ�. // Separates the input string with ',' and converts it to an int value.
			while(true)
			{
				CFLString<char> flstrToken = flstrInput.Tokenize(",", i32TokenIdx);
				if(flstrToken.IsEmpty())
					break;

				if(flstrToken == "\n")
					break;

				flaNK.PushBack(flstrToken.ToInt32());

				if(flaNK.GetCount() == 2)
					break;
			}

			if(flaNK.GetCount() < 2)
				break;

			int32_t n = flaNK.GetAt(0);
			int32_t k = flaNK.GetAt(1);

			if(k == 0 || n < k)
			{
				flstrResult = "\nCount : 0";
				break;
			}

			// nPk, n ������ k ���� �����ϴ� ���� // nPk, a permutation of selecting k objects from n objects
			P.SetMax(n);
			P.SetSelection(k);

			// ������ ��� // Calculate the permutation
			P.Calculate();

			// ���� ����� ��� // Get permutation result
			CFLArray<CFLArrayL> flaPermutation = P.GetResult();

			CFLString<char> flstrPermutation = "";
			int64_t i64PermutationCnt = 0;

			for(int64_t i = 0; i < flaPermutation.GetCount(); i++)
			{
				flstrPermutation += "(";

				for(int64_t j = 0; j < flaPermutation.GetAt(i).GetCount(); j++)
					flstrPermutation += CFLString<char>().Format(" %d ", flaPermutation.GetAt(i).GetAt(j));

				flstrPermutation += ")\n";

				i64PermutationCnt++;
			}

			CFLString<char> flstrCnt = CFLString<char>().Format("\nCount : %d", i64PermutationCnt);

			flstrResult = flstrPermutation + flstrCnt;
		}
		while(false);

		if(flstrResult.IsEmpty())
			flstrResult = "Please check the input.\n";

		flstrResult += "\n\n";

		printf(flstrResult);
	}

	return 0;
}