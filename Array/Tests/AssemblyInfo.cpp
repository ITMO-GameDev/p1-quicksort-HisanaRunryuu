#include "stdafx.h"

using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;

//
// ����� �������� �� ���� ������ ��������������� ��������� �������
// ���������. �������������� �������� ���� ���������, ����� ��������
// ����� �������� �� ���� ������.
//
[assembly:AssemblyTitleAttribute("Tests")];
[assembly:AssemblyDescriptionAttribute("")];
[assembly:AssemblyConfigurationAttribute("")];
[assembly:AssemblyCompanyAttribute("")];
[assembly:AssemblyProductAttribute("Tests")];
[assembly:AssemblyCopyrightAttribute("Copyright (c)  2019")];
[assembly:AssemblyTrademarkAttribute("")];
[assembly:AssemblyCultureAttribute("")];

//
// �������� � ������ ������ ������� �� ��������� ������� ��������:
//
//      �������� ����� ������
//      �������������� ����� ������
//   ����� ������
//      ��������
//
// ����� ������ ��� �������� ��� ������� ������ ������ � �������� �� ���������
// ��������� "*", ��� �������� ����:

[assembly:AssemblyVersionAttribute("1.0.*")];
[assembly:ComVisible(false)];
