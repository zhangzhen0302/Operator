#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MaxSize 100

/*定义一个结构体，存放运算符和其优先级*/
struct Operation
{
	char ch;	//运算符
	int pri;	//对应的优先级
};

/*定义两个运算符数组，分别储存其在左侧和在右侧的优先级*/
struct Operation left[] = { {'=',0},{'(',1},{'*',5},{'/',5},{'+',3},{'-',3},{')',9},{'#',6} };
struct Operation right[] = { {'=',0},{'(',9},{'*',4},{'/',4},{'+',2},{'-',2},{')',1},{'#',6} };

/*定义纯运算符数组*/
char Operation[7] = { '+','-','*','/','#','(',')' };

/*查表，返回左运算符或括号的优先级*/
int leftpri(char lp) {
	int i;
	for (i = 0; i < MaxSize; i++)
		if (left[i].ch == lp)
			return left[i].pri;
}

/*查表，返回右运算符或括号的优先级*/
int rightpri(char rp) {
	int i;
	for (i = 0; i < MaxSize; i++)
		if (right[i].ch == rp) return right[i].pri;
}

bool InEx(char ch) {
	int i = 0;
	bool a = false;
	for (i = 0; i < 7; i++) {
		if (ch == Operation[i])
			a = true;
	}
	if ((ch >= '0' && ch <= '9') || ch == '.')
		a = true;
	return a;
}

/*判断输入的字符是否为运算符或括号。若是，则返回true；否则返回false*/
bool InOp(char ch) {
	int i = 0;
	bool a = false;
	for (i = 0; i < 7; i++) {
		if (ch == Operation[i])
			a = true;
	}
	return a;
}

/*判断输入的字符是否为双目运算符。若是，则返回true；否则返回false*/
bool InOp2(char ch) {
	int i = 0;
	bool a = false;
	for (i = 0; i < 5; i++) {
		if (ch == Operation[i])
			a = true;
	}
	return a;
}

/*判断两个运算符的优先级。
若op1的优先级大于op2，则返回1；若op1的优先级等于op2，则返回0；若op1的优先级小于op2，则返回-1
*/
int Precede(char op1, char op2) { //the judge between the two marks
	if (leftpri(op1) == rightpri(op2)) return 0;
	else if (leftpri(op1) < rightpri(op2)) return -1;
	else return 1;
}

/*将原表达式转化为后缀表达式*/
void trans(char* exp, char postexp[]) {
	/*定义一个存储运算符的栈*/
	struct {
		char data[MaxSize];
		int top;            //top用作栈顶指针
	} op;
	int i = 0;
	bool fuhao = false;
	bool right = true;
	op.top = -1;
	op.top++;                  //先push“=”
	op.data[op.top] = '=';
	while (*exp != '\0') {     //exp表达式未扫描完时循环
		if (!InOp(*exp) && InEx(*exp)) {        //为数字字符的情况
			while ((*exp >= '0' && *exp <= '9') || *exp == '.') { //判定为数字或者小数点
				postexp[i++] = *exp;
				fuhao = InOp2(*exp);
				exp++;
			}
			postexp[i++] = ' ';   //用' '标识一个数值串结束
		}
		else if (fuhao && InOp2(*exp)) {
			printf("error:the expression is illegal!\n");
			//			postexp[0] = '\0';
			right = false;
			break;
		}
		else if (InEx(*exp)) {   //为运算符的情况
			fuhao = InOp2(*exp);
			switch (Precede(op.data[op.top], *exp)) {
			case -1:           //栈顶运算符的优先级低:进栈
				op.top++;
				op.data[op.top] = *exp;
				exp++;     //继续扫描其他字符
				break;
			case 0:        //只有括号满足这种情况
				op.top--;      //将(退栈
				exp++;     //继续扫描其他字符
				break;
			case 1:             //退栈并输出到postexp中
				postexp[i++] = op.data[op.top];
				op.top--;
				fuhao = false;
				break;
			}
		}
		else if (*exp != '\n') {
			printf("error:the expression is illegal!\n");
			//			postexp[0] = '\0';
			right = false;
			break;
		}

	} //while (*exp!='\0')
	while (op.data[op.top] != '=') {//judge the expression is over
		postexp[i++] = op.data[op.top];
		op.top--;
	}
	if (right)postexp[i] = '\0';    //给postexp表达式添加结束标识
	else postexp[0] = '\0';
}

/*计算后缀表达式的值。*/
float compvalue(char exp[])
{
	if (exp == 0) {
		return 0;
	}
	struct {
		float data[MaxSize];    //存放数值
		int top;            //栈指针
	} st;               //定义数值栈
	float d;
	int kh = 0;
	char ch;
	int t = 0; //t作为exp的下标
	st.top = -1;
	ch = exp[t];
	t++;
	while (ch != '\0')    //exp字符串未扫描完时循环
	{
		switch (ch)
		{
		case'+':
			st.data[st.top - 1] = st.data[st.top - 1] + st.data[st.top];
			st.top--;
			break;
		case '-':
			st.data[st.top - 1] = st.data[st.top - 1] - st.data[st.top];
			st.top--;
			break;
		case '*':
			st.data[st.top - 1] = st.data[st.top - 1] * st.data[st.top];
			st.top--;
			break;
		case '/':
			if (st.data[st.top] != 0)
				st.data[st.top - 1] = st.data[st.top - 1] / st.data[st.top];
			else
			{
				printf("error:ZERO cannot devide a number!\n");
				break;
			}
			st.top--;
			break;
		case '#':
			st.data[st.top] = 0 - st.data[st.top];
			break;
		case'(':
			kh++;
			break;
		case')':
			kh--;
			break;

		default:
			d = 0; //将数字字符转换成数值存放到d中
			if (ch >= '0' && ch <= '9') {
				int flag_point = 0;				//小数标志位。为0时是整数，为1时是小数，其他情况是小数点数错误
				int decimal_place = -1;			//小数位数
				float temp = 114;
				while ((ch >= '0' && ch <= '9') || ch == '.') {
					if (ch == '.') {
						flag_point++;
						ch = exp[t];
						t++;
						continue;
					}
					switch (flag_point) {
					case 0:
						d = 10 * d + ch - '0';
						ch = exp[t];
						t++;
						break;
					case 1:
						temp = float(pow(10, decimal_place));
						d = d + (ch - '0') * temp;
						decimal_place--;
						ch = exp[t];
						t++;
						break;
					default:
						printf("error:wrong number input!\n");//小数点数多于1的错误情况
					}
				}
			}
			else if (ch == '.') {			//首位是小数点的错误情况
				printf("error:wrong number input!\n");
			}
			st.top++;
			st.data[st.top] = d;
		}
		ch = exp[t];
		t++;
	}
	if (kh != 0) {
		printf("error:brackets not match!\n");
		exp[0] = '\0';
		return 0;
	}

	return st.data[st.top];
}

int main()
{
	char exp[MaxSize];
	printf("please enter a expression,print 'q' to quit the calcular(no use of space) ╮（﹀＿﹀）╭ \n");
	gets_s(exp);
	char postexp[MaxSize];
	float a = 0;
	while (1) {
		if (InOp(exp[0]) || (exp[0] >= '0' && exp[0] <= '9' && postexp[0] != '\0')) {
			trans(exp, postexp);
			a = compvalue(postexp);
			printf("中缀表达式:%s\n", exp);
			printf("后缀表达式:%s\n", postexp);
			printf("表达式的值:%g\n", a);
			printf("please enter a expression,print 'q' to quit the calcular(no use of space) ╮（﹀＿﹀）╭ \n");
			gets_s(exp);
		}
		else if (exp[0] == 'q') break;
		else {
			printf("error:wrong intput! \n");
			gets_s(exp);
			trans(exp, postexp);
			a = compvalue(postexp);
		}
	}
	return 0;
}