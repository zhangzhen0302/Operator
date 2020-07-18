#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MaxSize 100

/*����һ���ṹ�壬���������������ȼ�*/
struct Operation
{
	char ch;	//�����
	int pri;	//��Ӧ�����ȼ�
};

/*����������������飬�ֱ𴢴������������Ҳ�����ȼ�*/
struct Operation left[] = { {'=',0},{'(',1},{'*',5},{'/',5},{'+',3},{'-',3},{')',9},{'#',6} };
struct Operation right[] = { {'=',0},{'(',9},{'*',4},{'/',4},{'+',2},{'-',2},{')',1},{'#',6} };

/*���崿���������*/
char Operation[7] = { '+','-','*','/','#','(',')' };

/*�������������������ŵ����ȼ�*/
int leftpri(char lp) {
	int i;
	for (i = 0; i < MaxSize; i++)
		if (left[i].ch == lp)
			return left[i].pri;
}

/*�������������������ŵ����ȼ�*/
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

/*�ж�������ַ��Ƿ�Ϊ����������š����ǣ��򷵻�true�����򷵻�false*/
bool InOp(char ch) {
	int i = 0;
	bool a = false;
	for (i = 0; i < 7; i++) {
		if (ch == Operation[i])
			a = true;
	}
	return a;
}

/*�ж�������ַ��Ƿ�Ϊ˫Ŀ����������ǣ��򷵻�true�����򷵻�false*/
bool InOp2(char ch) {
	int i = 0;
	bool a = false;
	for (i = 0; i < 5; i++) {
		if (ch == Operation[i])
			a = true;
	}
	return a;
}

/*�ж���������������ȼ���
��op1�����ȼ�����op2���򷵻�1����op1�����ȼ�����op2���򷵻�0����op1�����ȼ�С��op2���򷵻�-1
*/
int Precede(char op1, char op2) { //the judge between the two marks
	if (leftpri(op1) == rightpri(op2)) return 0;
	else if (leftpri(op1) < rightpri(op2)) return -1;
	else return 1;
}

/*��ԭ���ʽת��Ϊ��׺���ʽ*/
void trans(char* exp, char postexp[]) {
	/*����һ���洢�������ջ*/
	struct {
		char data[MaxSize];
		int top;            //top����ջ��ָ��
	} op;
	int i = 0;
	bool fuhao = false;
	bool right = true;
	op.top = -1;
	op.top++;                  //��push��=��
	op.data[op.top] = '=';
	while (*exp != '\0') {     //exp���ʽδɨ����ʱѭ��
		if (!InOp(*exp) && InEx(*exp)) {        //Ϊ�����ַ������
			while ((*exp >= '0' && *exp <= '9') || *exp == '.') { //�ж�Ϊ���ֻ���С����
				postexp[i++] = *exp;
				fuhao = InOp2(*exp);
				exp++;
			}
			postexp[i++] = ' ';   //��' '��ʶһ����ֵ������
		}
		else if (fuhao && InOp2(*exp)) {
			printf("error:the expression is illegal!\n");
			//			postexp[0] = '\0';
			right = false;
			break;
		}
		else if (InEx(*exp)) {   //Ϊ����������
			fuhao = InOp2(*exp);
			switch (Precede(op.data[op.top], *exp)) {
			case -1:           //ջ������������ȼ���:��ջ
				op.top++;
				op.data[op.top] = *exp;
				exp++;     //����ɨ�������ַ�
				break;
			case 0:        //ֻ�����������������
				op.top--;      //��(��ջ
				exp++;     //����ɨ�������ַ�
				break;
			case 1:             //��ջ�������postexp��
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
	if (right)postexp[i] = '\0';    //��postexp���ʽ��ӽ�����ʶ
	else postexp[0] = '\0';
}

/*�����׺���ʽ��ֵ��*/
float compvalue(char exp[])
{
	if (exp == 0) {
		return 0;
	}
	struct {
		float data[MaxSize];    //�����ֵ
		int top;            //ջָ��
	} st;               //������ֵջ
	float d;
	int kh = 0;
	char ch;
	int t = 0; //t��Ϊexp���±�
	st.top = -1;
	ch = exp[t];
	t++;
	while (ch != '\0')    //exp�ַ���δɨ����ʱѭ��
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
			d = 0; //�������ַ�ת������ֵ��ŵ�d��
			if (ch >= '0' && ch <= '9') {
				int flag_point = 0;				//С����־λ��Ϊ0ʱ��������Ϊ1ʱ��С�������������С����������
				int decimal_place = -1;			//С��λ��
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
						printf("error:wrong number input!\n");//С����������1�Ĵ������
					}
				}
			}
			else if (ch == '.') {			//��λ��С����Ĵ������
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
	printf("please enter a expression,print 'q' to quit the calcular(no use of space) �r����ߦ壩�q \n");
	gets_s(exp);
	char postexp[MaxSize];
	float a = 0;
	while (1) {
		if (InOp(exp[0]) || (exp[0] >= '0' && exp[0] <= '9' && postexp[0] != '\0')) {
			trans(exp, postexp);
			a = compvalue(postexp);
			printf("��׺���ʽ:%s\n", exp);
			printf("��׺���ʽ:%s\n", postexp);
			printf("���ʽ��ֵ:%g\n", a);
			printf("please enter a expression,print 'q' to quit the calcular(no use of space) �r����ߦ壩�q \n");
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