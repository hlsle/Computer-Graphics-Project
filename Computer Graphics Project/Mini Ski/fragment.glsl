#version 330
//--- out_Color: ���ؽ� ���̴����� �Է¹޴� ���� ��
//--- FragColor: ����� ������ ������ ������ ���۷� ���� ��.

in vec3 color;
out vec4 FragColor; //--- ���� ���
void main(void)
{
FragColor = vec4 (color, 1.0);
}