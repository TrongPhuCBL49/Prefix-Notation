#include <iostream>
#include <string>
#include <math.h>

using namespace std;

//Stack
template <typename Type> class NODE
{
public:
	Type Info;
	NODE* Next;
};
template <typename Type> class STACK
{
public:
	NODE<Type>* Top;
	void Init()
	{
		this->Top = NULL;
	}
	bool IsEmpty()
	{
		return (this->Top == NULL);
	}
	void Push(Type x)
	{
		NODE<Type>* p = CreateNode<Type>(x);
		if (p == NULL)
			return;
		p->Next = this->Top;
		this->Top = p;
	}
	Type Pop()
	{
		NODE<Type>* p = this->Top;
		Type x = p->Info;
		this->Top = this->Top->Next;
		delete p;
		return x;
	}
};
template <typename Type> NODE<Type>* CreateNode(Type x)
{
	NODE<Type>* p = new NODE<Type>;
	if (p == NULL)
		return NULL;
	p->Info = x;
	p->Next = NULL;
	return p;
}
//Set thứ tự ưu tiên cho các toán tử
int getPriority(string ope)
{
	if (ope == "s" || ope == "c" || ope == "t" || ope == "a") return 3;
	else if (ope == "*" || ope == "/") return 2;
	else if (ope == "+" || ope == "-") return 1;
	else return 0;
}
int getPriority(char ope)
{
	string Ope(1, ope);
	return getPriority(Ope);
}
//Kiểm tra xem là toán tử, toán hạng hay dấu ngoặc
int isOperator(string ope)
{
	if (getPriority(ope) == 0) //không phải toán tử
	{
		if (ope != "(" && ope != ")") return 0; //là toán hạng
		else return 1; //là ngoặc
	}
	return 2; //là toán tử
}
int isOperator(char ope)
{
	string Ope(1, ope);
	return isOperator(Ope);
}
//Chuyển biểu thức sang hậu tố - kí pháp ba lan
STACK<string> ConvertToPostfix(string exp)
{
	STACK<string> myStack, Result;
	myStack.Init();
	Result.Init();
	// Do có những toán hạng lớn hơn 10, hoặc số thập phân => Có nhiều hơn 1 ký tự
	// Ta cần phải add toàn bộ các kí tự số đó vào chuỗi number
	string number = "";
	for (int i = 0; i < exp.length(); i++)
	{
		string s(1, exp[i]); //Chuyển kiểu char sang string
		if (isOperator(s) == 0) number += s;
		else
		{
			// Push toán hạng vào Result
			if (number.length() > 0)
			{
				Result.Push(number);
				number = "";
			}
			if (isOperator(s) == 1) //1. Là ngoặc
			{
				if (s == "(") myStack.Push("(");
				else if (s == ")")
				{
					string pop = myStack.Pop(); //gán pop bằng kí tự cuối trong Stack
					while (pop != "(")
					{
						Result.Push(pop);
						pop = myStack.Pop();
					}
				}
			}
			else //là toán tử
			{
				//xét thứ tự ưu tiên cho toán tử
				while (!(myStack.IsEmpty()) && getPriority(myStack.Top->Info) >= getPriority(s))
					Result.Push(myStack.Pop());
				myStack.Push(s);
			}
		}
	}
	// Trường hợp còn sót lại toán hạng cuối cùng
	if (number.length() > 0)
	{
		Result.Push(number);
		number = "";
	}
	while (!myStack.IsEmpty())
		Result.Push(myStack.Pop());
	return Result;
}
//Tính toán biểu thức sau khi đã chuyển qua hậu tố
double Calc(STACK<string> inp)
{
	STACK<double> myStack;
	STACK<string> Input;
	myStack.Init();
	Input.Init();
	while (inp.Top != NULL)
		Input.Push(inp.Pop());
	while (Input.Top != NULL)
	{
		if (isOperator(Input.Top->Info) == 0)
			myStack.Push(stod(Input.Pop())); //Nếu là toán hạng thì đổi sang double rồi Push vào Stack
		else
		{
			//xử lý toán tử 2 ngôi
			if (getPriority(Input.Top->Info) != 3)
			{
				// Do ta cần quan tâm đến thứ tự các toán hạng
				// Nên ta phải Pop vế sau trước, sau đó vế trước mới Pop sau
				double a, b;
				b = myStack.Pop();
				a = myStack.Pop();

				if (Input.Top->Info == "+") myStack.Push(a + b);
				else if (Input.Top->Info == "-") myStack.Push(a - b);
				else if (Input.Top->Info == "*") myStack.Push(a * b);
				else if (Input.Top->Info == "/") myStack.Push(a / b);
			}
			else
				//xử lý toán tử một ngôi
			{
				double a = myStack.Pop();
				if (Input.Top->Info == "a") myStack.Push(-a);
				else
				{
					double PI = 3.1415926535897932384626344;
					a = a * PI / 180; //đổi từ độ sang radian
					if (Input.Top->Info == "s") myStack.Push(sin(a));
					else if (Input.Top->Info == "c") myStack.Push(cos(a));
					else if (Input.Top->Info == "t") myStack.Push(tan(a));
				}
			}
			Input.Pop(); //pop toán tử ra khỏi Input
		}
	}
	return myStack.Pop();
}
//thay dấu âm bằng chữ a
string ThayDauAm(string& inp, int vt)
{
	inp.insert(vt, "a");
	return inp.erase(vt + 1, 1);
}
//Kiểm tra dữ liệu đầu vào
bool KiemTraInput(string& inp)
{
	//xóa khoảng trắng trong chuỗi
	while (inp.find(' ') != -1) inp.erase(inp.find(' '), 1);
	//chuẩn hóa các phép toán sin cos tan
	while (inp.find("sin") != -1) inp.erase(inp.find("sin") + 1, 2);
	while (inp.find("cos") != -1) inp.erase(inp.find("cos") + 1, 2);
	while (inp.find("tan") != -1) inp.erase(inp.find("tan") + 1, 2);
	//nếu chứa các kí tự khác số, toán tử và ngoặc => không hợp lệ
	if (inp.find_first_not_of("0123456789.+-*/sct()") != -1)
		return false;
	//nếu bắt đầu là dấu + x / => không hợp lệ
	if (inp[0] == '+' || inp[0] == '*' || inp[0] == '/') return false;
	//nếu bắt đầu là dấu - => chuyển thành chữ "a" để phân biệt với toán tử 2 ngôi
	if (inp[0] == '-') inp = ThayDauAm(inp, 0);
	//nêu kết thúc là toán tử hoặc dấu mở ngoặc => không hợp lệ
	if (isOperator(inp[inp.length() - 1]) == 2 || inp[inp.length() - 1] == '(') return false;
	int demngoac = 0;
	if (inp[0] == '(') demngoac++;
	for (int i = 1; i < inp.length() - 1; i++)
	{
		//kiểm tra 2 toán tử liền nhau
		if (isOperator(inp[i]) == 2 && isOperator(inp[i + 1]) == 2)
		{
			if ((inp[i + 1] == '+' || inp[i + 1] == '*' || inp[i + 1] == '/')) return false;
			if (inp[i + 1] == '-')
				//nếu sau dấu - không phải là toán tử thì dấu - hợp lệ
				if (isOperator(inp[i + 2]) == 2 && getPriority(inp[i + 2]) != 3)
					return false; //nếu 2,3 toán tử liền nhau => không hợp lệ
				else inp = ThayDauAm(inp, i + 1);
		}
		if (inp[i] == '(')
			if (inp[i + 1] == '-')
				//nếu sau dấu - không phải là toán tử thì dấu - hợp lệ
				if (isOperator(inp[i + 2]) != 2)
				{
					inp = ThayDauAm(inp, i + 1);
					demngoac++;
				}
				else return false;
			else
				//trước ngoặc ( là số "123(" hoặc trong ngoặc rỗng "()" => không hợp lệ
				if ((isOperator(inp[i - 1]) == 0 || inp[i + 1] == ')') && i < inp.length()) return false;
				else demngoac++;
				if (inp[i] == ')')
					//trước ngoặc ) là toán tử "+)" hoặc giữa 2 ngoặc không có toán tử ")(" => không hợp lệ
					if ((isOperator(inp[i - 1]) == 2 || inp[i + 1] == '(') && i < inp.length()) return false;
					else demngoac--;
					if (demngoac < 0) return false; //mở ngoặc trước => không hợp lệ
	}
	if (inp[inp.length() - 1] == ')') demngoac--;
	if (demngoac != 0) return false; //mở ngoặc nhiều nhưng không đóng ngoặc => không hợp lệ
	return true;
}

void main()
{
	string BieuThuc;
	cout << "Nhap bieu thuc toan hoc: ";
	getline(cin, BieuThuc);
	if (!KiemTraInput(BieuThuc))
		cout << "Bieu thuc khong hop le!!!";
	else
		cout << "Ket qua: " << Calc(ConvertToPostfix(BieuThuc)) << endl;
	system("pause");
}