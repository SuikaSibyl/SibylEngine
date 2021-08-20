#pragma once

#include "SIByLpch.h"
#include "DX12Shader.h"

#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Platform/DirectX12/Common/DX12Utility.h"
#include "Platform/DirectX12/Common/DX12Context.h"

namespace SIByL
{
	DX12Shader::DX12Shader()
	{

	}

	DX12Shader::DX12Shader(std::string vFile, std::string pFile)
	{
		m_VsBytecode = CompileFromFile(AnsiToWString(vFile), nullptr, "VS", "vs_5_1");
		m_PsBytecode = CompileFromFile(AnsiToWString(pFile), nullptr, "PS", "ps_5_1");
	}

	void DX12Shader::Use()
	{

	}

	ComPtr<ID3DBlob> DX12Shader::CompileFromFile(
		const std::wstring& fileName,
		const D3D_SHADER_MACRO* defines,
		const std::string& enteryPoint,
		const std::string& target)
	{
		// If in debug mode, compile using DEBUG Mode
		UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // defined(DEBUG) || defined(_DEBUG)
		
		HRESULT hr = S_OK;

		ComPtr<ID3DBlob> byteCode = nullptr;
		ComPtr<ID3DBlob> errors;
		hr = D3DCompileFromFile(fileName.c_str(), //hlslԴ�ļ���
			defines,	//�߼�ѡ�ָ��Ϊ��ָ��
			D3D_COMPILE_STANDARD_FILE_INCLUDE,	//�߼�ѡ�����ָ��Ϊ��ָ��
			enteryPoint.c_str(),	//��ɫ������ڵ㺯����
			target.c_str(),		//ָ��������ɫ�����ͺͰ汾���ַ���
			compileFlags,	//ָʾ����ɫ���ϴ���Ӧ����α���ı�־
			0,	//�߼�ѡ��
			&byteCode,	//����õ��ֽ���
			&errors);	//������Ϣ

		if (errors != nullptr)
		{
			SIByL_CORE_ERROR("DX12 Shader Compile From File Failed!");
		}
		DXCall(hr);

		return byteCode;
	}
}