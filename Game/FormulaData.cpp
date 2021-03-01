using namespace EquationHelper;
using namespace Numerics;
typedef EquationSolver ES;
/**
* Private constructor - does nothing.
*/
ES::EquationSolver(){}
/**
* Performs the specified operation against the
* argument strings. The operation is dependant on
* the value of op.
*/
std::string ES::doOperation(const std::string& lhs, char op, const std::string& rhs){

	Double bdLhs = lhs;
	Double bdRhs = rhs;
	Double temp;
	switch (op){
	case '^':
		temp(pow(bdLhs, bdRhs));
		break;
	case '*':
		temp(bdLhs * bdRhs);
		break;
	case '/':
		temp(bdLhs / bdRhs);
		break;
	case '+':
		temp(bdLhs + bdRhs);
		break;
	case '%':
		temp(fmod(bdLhs, bdRhs));
		break;
	}
	return temp.getString();
}
/**
* Returns the string with its enclosing paranthesis
* stripped from it.
*/
void ES::correctedString(std::string& arg){

	size_t pos1 = arg.find_first_of("(");
	size_t pos2 = arg.find_last_of(")");
	if (pos1 >= 0 && pos1 < arg.length() && pos2 >= 0 && pos2 <= arg.length())
		arg[pos1] = arg[pos2] = ' ';
}
/**
* Remove spaces from the argument string.
*/
void ES::removeSpaces(std::string& argu){
	std::string temp = "";
	for (size_t i = 0; i < argu.length(); i++)
		if (argu[i] != ' ')
			temp += argu[i];
	argu = temp;
}
/**
* The brains of the program.
* Solves expressions by using recursion for complex expressions.
*/
std::string ES::parse(const std::string& param){

	std::string expression = param;
	correctedString(expression);
	removeSpaces(expression);
	std::string finalExpression = "";
	bool operatorEncountered = true;
	for (size_t i = 0; i < expression.length(); i++){
		if (expression[i] == '('){
			std::string placeHolder = "(";
			int valuesCounted = 1;
			operatorEncountered = false;
			for (size_t j = i + 1; valuesCounted != 0; j++){
				if (expression[j] == '(')
					valuesCounted++;
				else if (expression[j] == ')')
					valuesCounted--;
				placeHolder += expression[j];
			}
			std::string evaluatedString = parse(placeHolder);
			finalExpression += evaluatedString;
			i += (placeHolder.length() - 1);
		}
		else{
			if (expression[i] == '-' && operatorEncountered == false)
				finalExpression += '+';
			finalExpression += expression[i];
			if ((expression[i] == '+'
				|| expression[i] == '/'
				|| expression[i] == '^'
				|| expression[i] == '*'
				|| expression[i] == '%'
				|| expression[i] == '-'))
				operatorEncountered = true;
			else if (expression[i] != ' ')
				operatorEncountered = false;
		}
	}
	removeSpaces(finalExpression);
	std::string perfectExpression = "";
	for (size_t i = 0; i < finalExpression.length(); i++){
		if ((i + 1) < finalExpression.length())
			if (finalExpression[i] == '-' && finalExpression[i + 1] == '-')
				i += 2;
		perfectExpression += finalExpression[i];
	}
	finalExpression = perfectExpression;
	std::vector<std::string> totalNumbers;
	std::vector<char> totalOperations;
	//std::cout << finalExpression << std::endl;
	for (size_t i = 0; i < finalExpression.length(); i++){
		if (finalExpression[i] >= '0' && finalExpression[i] <= '9'
			|| finalExpression[i] == '-' || finalExpression[i] == '.'){
			std::string temp = "";	//
			for (size_t j = i; j < finalExpression.length(); j++){
				if (finalExpression[j] >= '0' && finalExpression[j] <= '9'
					|| finalExpression[j] == '-' || finalExpression[j] == '.'){
					temp += finalExpression[j];
				}
				else break;
			}
			totalNumbers.push_back(temp);
			i += temp.length() == 0 ? 0 : (temp.length() - 1);
		}
		else if (finalExpression[i] == '*'
			|| finalExpression[i] == '/'
			|| finalExpression[i] == '^'
			|| finalExpression[i] == '+'
			|| finalExpression[i] == '%'
			){
			totalOperations.push_back(finalExpression[i]);
		}
	}
	ES::calculate(totalNumbers, totalOperations, "^");
	ES::calculate(totalNumbers, totalOperations, "*/%");
	ES::calculate(totalNumbers, totalOperations, "+");
	return totalNumbers[0];
}
/**
* Calculates the numbers in the first vector using the operands in the 2nd vector,
* based on the expressions allowed which are determined by the string argument.
*/
void ES::calculate(std::vector<std::string>& totalNumbers, std::vector<char>& totalOperations,
	const std::string& arg){
	for (int i = 0; i < static_cast<int>(totalOperations.size()); i++){
		if (arg.find(totalOperations[i]) != arg.npos){
			totalNumbers[i] = doOperation(totalNumbers[i], totalOperations[i], totalNumbers[i + 1]);

			size_t oldNumberLength = totalNumbers.size();
			size_t oldOperatorLength = totalOperations.size();
			size_t nextNumberLength = oldNumberLength - 1;
			size_t nextOperatorLength = oldOperatorLength - 1;
			size_t sCount = 0;
			size_t oCount = 0;
			std::vector<std::string> temp1(nextNumberLength);
			std::vector<char> temp2(nextOperatorLength);

			for (size_t j = 0; j < oldNumberLength; j++){
				if (j != static_cast<int>(i + 1)){
					temp1[sCount++] = totalNumbers[j];
				}
				if (j != i && j < oldOperatorLength){
					temp2[oCount++] = totalOperations[j];
				}
			}
			totalNumbers = temp1;
			totalOperations = temp2;
			i--;
		}
	}
}
/**
* Returns true if the equation is solvable (not really),
* returns false otherwise.
*
* This function is truly a misnomer, because more restrictions
* should be put in place.
*/
bool ES::isSolvable(const std::string& eq){

	int paranthesisCount = 0;
	for (size_t i = 0; i < eq.length(); i++){
		if (eq[i] == '(')
			paranthesisCount++;
		else if (eq[i] == ')')
			paranthesisCount--;
		if (paranthesisCount < 0)
			return false;
	}
	return paranthesisCount == 0;
}
/**
* An attempt to solve a string-expression, given
* a precision value.
*/
std::string ES::solve(const std::string& eq, int prec){
	if (isSolvable(eq)){
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		//std::cout << eq << std::endl;
		std::string value;
		value += '(';
		value += eq;
		value += ')';

		ss.setf(0, std::ios::floatfield);
		ss.precision(prec);
		ss << parse(value);
		return ss.str();
	}
	else return "";
}

FormulaMgr::FormulaMgr()
{

}

FormulaMgr::~FormulaMgr()
{
	LIST_CLEAR(FormulaList::iterator, this->m_FormulaList);
}

void FormulaMgr::Load()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Formula Data...");

	LIST_CLEAR(FormulaList::iterator, this->m_FormulaList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM formula_data");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			FormulaData* pData = new FormulaData;
			pData->SetType(fields[0].GetUInt8());
			pData->SetID(fields[1].GetUInt16());
			pData->SetFormula(fields[2].GetString());

			this->m_FormulaList.push_back(pData);

			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u formula data definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

float FormulaMgr::GetValue(uint8 type, uint16 id, int32 variable)
{
	for (FormulaList::const_iterator itr = this->m_FormulaList.begin(); itr != this->m_FormulaList.end(); ++itr)
	{
		FormulaData const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetType() == type && pData->GetID() == id)
		{
			std::string formula = pData->GetFormula();
			Util::ReplaceString(formula, "%d", std::to_string(variable));

			std::string value = ES::solve(formula, 3);

			int32 int_value = static_cast<int32>(static_cast<float>(atof((char*)value.c_str())) * 100 + 0.5f);
			float float_value = static_cast<float>(int_value);

			return (float)(float_value / 100);
		}
	}

	return 0;
}

float FormulaMgr::GetOrCreateValue(uint8 type, uint16 id, int32 variable)
{
	for (FormulaList::const_iterator itr = this->m_FormulaList.begin(); itr != this->m_FormulaList.end(); ++itr)
	{
		FormulaData* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetType() == type && pData->GetID() == id)
		{
			FormulaDataValueMap::const_iterator it = pData->m_ValueMap.find(variable);

			if (it != pData->m_ValueMap.end())
			{
				return it->second;
			}
			else
			{
				std::string formula = pData->GetFormula();
				Util::ReplaceString(formula, "%d", std::to_string(variable));

				std::string value = ES::solve(formula, 3);

				int32 int_value = static_cast<int32>(static_cast<float>(atof((char*)value.c_str())) * 100 + 0.5f);
				float float_value = static_cast<float>(int_value) / 100;

				pData->m_ValueMap[variable] = float_value;

				return float_value;
			}
		}
	}

	return 0;
}