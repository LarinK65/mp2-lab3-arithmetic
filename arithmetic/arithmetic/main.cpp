#include <iostream>
#include <cmath>
#include "Expression.h"

using namespace std;



int main()
{
	int n;
	cout << "enter var count: " << std::endl;

	cin >> n;

	if (n > 0)
	cout << "enter variables: " << std::endl;

	std::vector<std::string> vars(n);

	for (int i = 0; i < n; i++)
	{
		cin >> vars[i];
	}

	cout << "enter expression" << std::endl;

	std::string expression;
	getline(cin, expression);
	if (expression.empty()) {
		getline(cin, expression);
	}



	try {
		Expression e(vars.begin(), vars.end(), expression);


		while (true)
		{
			if (vars.size() > 0)
			cout << "enter var values" << std::endl;

			std::vector<double> vals(vars.size());

			for (int i = 0; i < vars.size(); i++)
			{
				cout << vars[i] << ": ";
				cout.flush();

				cin >> vals[i];
			}

			try {
				std::cout << "result is " << e.calculate(vals.begin(), vals.end()) << std::endl;
			}
			catch (std::exception e)
			{
				std::cout << e.what();
			}
			if (vars.size() == 0) break;
		}

	}
	catch (std::exception e)
	{
		std::cout << e.what();
	}

	return 0;
}
