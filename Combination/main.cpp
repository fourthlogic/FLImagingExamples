#include <cstdio>

#include <FLImaging.h>


int main()
{
	// ���� ��ü ���� // Declare a combination object
	CCombination C;

	while(true)
	{
		CFLString<char> flstrResult = "";

		do
		{
			char arrInput[4096] = { 0, };

			printf("Please input n and k as n, k.\n");
			printf("Combination : k objects are selected from a set of n objects to produce subsets without ordering\n");
			printf("ex) 6, 2\n");
			printf("Input: ");

			// ��� ���ڿ��� �Է� �޴´�. // Receive the count string.
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

			// nCk, n ������ k ���� �����ϴ� ���� // nCk, a combination of selecting k objects from n objects
			C.SetMax(n);
			C.SetSelection(k);

			// ������ ��� // Calculate combinations
			C.Calculate();

			// ���� ����� ��� // Get combination result
			CFLArray<CFLArrayL> flaCombination = C.GetResult();

			CFLString<char> flstrCombination = "";
			int64_t i64CombinationCnt = 0;

			for(int64_t i = 0; i < flaCombination.GetCount(); i++)
			{
				flstrCombination += "(";

				for(int64_t j = 0; j < flaCombination.GetAt(i).GetCount(); j++)
				{
					flstrCombination += CFLString<char>().Format(" %d ", flaCombination.GetAt(i).GetAt(j));
				}
				flstrCombination += ")\n";
				i64CombinationCnt++;
			}

			CFLString<char> flstrCnt = CFLString<char>().Format("\nCount : %d", i64CombinationCnt);

			flstrResult = flstrCombination + flstrCnt;
		} while(false);

		if(flstrResult.IsEmpty())
			flstrResult = "Please check the input.\n";

		flstrResult += "\n\n";

		printf(flstrResult);
	}

	return 0;
}