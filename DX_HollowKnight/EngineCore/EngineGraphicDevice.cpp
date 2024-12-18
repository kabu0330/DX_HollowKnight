#include "PreCompile.h"
#include "EngineGraphicDevice.h"

UEngineGraphicDevice::UEngineGraphicDevice()
{
}

UEngineGraphicDevice::~UEngineGraphicDevice()
{
    Release();
}

void UEngineGraphicDevice::Release()
{
    if (nullptr != RTV)
    {
        RTV->Release();
        RTV = nullptr;
    }

    if (nullptr != DXBackBufferTexture)
    {
        DXBackBufferTexture->Release();
        DXBackBufferTexture = nullptr;
    }

    if (nullptr != SwapChain)
    {
        SwapChain->Release();
        SwapChain = nullptr;
    }

    if (nullptr != Context)
    {
        Context->Release();
        Context = nullptr;
    }

    if (nullptr != Device)
    {
        Device->Release();
        Device = nullptr;
    }
}

IDXGIAdapter* UEngineGraphicDevice::GetHighPerFormanceAdapter()
{
    IDXGIFactory* Factory = nullptr;
    unsigned __int64 MaxVideoMemory = 0;
    IDXGIAdapter* ResultAdapter = nullptr;

    HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&Factory));

    if (nullptr == Factory)
    {
        MSGASSERT("�׷���ī�� ����� ���丮 ������ �����߽��ϴ�.");
        return nullptr;
    }

    for (int Index = 0;; ++Index)
    {
        IDXGIAdapter* CurAdapter = nullptr;
        Factory->EnumAdapters(Index, &CurAdapter);

        if (nullptr == CurAdapter)
        {
            break;
        }

        // ��������ü�� �ְ�
        DXGI_ADAPTER_DESC Desc;

        // ��ġ �ڵ鿡�� ������ ���Դϴ�.
        CurAdapter->GetDesc(&Desc);

        // �� ũ�Ⱑ ũ�� ���ɵ� ������.
        if (MaxVideoMemory <= Desc.DedicatedVideoMemory)
        {
            MaxVideoMemory = Desc.DedicatedVideoMemory;
            if (nullptr != ResultAdapter)
            {
                ResultAdapter->Release();
            }
            ResultAdapter = CurAdapter;
            continue;
        }

        CurAdapter->Release();
    }

    if (nullptr != Factory)
    {
        Factory->Release();
    }

    if (nullptr == ResultAdapter)
    {
        MSGASSERT("�׷���ī�尡 �޷����� ���� ��ǻ���Դϴ�.");
        return nullptr;
    }

     int Test = MaxVideoMemory / (1024 * 1024 * 1024);

    return ResultAdapter;
}

void UEngineGraphicDevice::CreateDeviceAndContext()
{
    // ���� ���� ���� �׷��� ī�带 ã�Ҵ�.
    MainAdapter = GetHighPerFormanceAdapter();

    int iFlag = 0;

#ifdef _DEBUG
    // ����� ����϶���
//  D3D11_CREATE_DEVICE_SINGLETHREADED = 0x1,
//	D3D11_CREATE_DEVICE_DEBUG = 0x2,
//	D3D11_CREATE_DEVICE_SWITCH_TO_REF = 0x4,
//	D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS = 0x8,
//	D3D11_CREATE_DEVICE_BGRA_SUPPORT = 0x20,
//	D3D11_CREATE_DEVICE_DEBUGGABLE = 0x40,
//	D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY = 0x80,
//	D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT = 0x100,
//	D3D11_CREATE_DEVICE_VIDEO_SUPPORT = 0x800
    iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif


    //D3D_DRIVER_TYPE DriverType,
    // D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN ���� �־������� �װɷ� ��
    // D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE �ϰ� �˾Ƽ� �׷��� ī�� ã����.
    // D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_SOFTWARE �׷���ī�� �Ⱦ���.
    // �׷���ī�带 �Ⱦ��ڴ�.

    //HMODULE Software, // Ư�� �ܰ��(������ ������������ �Ϻθ� ���� ���� �ڵ�� ��ü�ϱ� ���� dll �ڵ�)

    //UINT Flags, // �ɼ�

    //_In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    //UINT FeatureLevels,
    //UINT SDKVersion,
    //_COM_Outptr_opt_ ID3D11Device** ppDevice,
    //_Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
    // D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN ���� �� ��ͷ� �ض�.

    D3D_FEATURE_LEVEL ResultLevel;

    // _COM_Outptr_opt_ ID3D11Device** ppDevice,
    // _Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
    // _COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext

    HRESULT Result = D3D11CreateDevice(
        MainAdapter,
        D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,
        nullptr, // Ư�� �ܰ踦 ���� § �ڵ�� ��ü�ϰڴ�.
        iFlag,
        nullptr, // �������� ���� 11�� ����Ŵϱ�. �迭�� �־��ټ�
        0, // ���� ������ ��ó���� ����
        D3D11_SDK_VERSION, // ���� ���̷�Ʈx sdk ����
        &Device,
        &ResultLevel,
        &Context);

    if (nullptr == Device)
    {
        MSGASSERT("�׷��� ����̽� ������ �����߽��ϴ�.");
        return;
    }

    if (nullptr == Context)
    {
        MSGASSERT("�׷��� ���ؽ�Ʈ ������ �����߽��ϴ�.");
        return;
    }

    if (Result != S_OK)
    {
        MSGASSERT("���� �߸���.");
        return;
    }

    if (ResultLevel != D3D_FEATURE_LEVEL_11_0
        && ResultLevel != D3D_FEATURE_LEVEL_11_1)
    {
        MSGASSERT("���̷�Ʈ 11������ �������� �ʴ� �׷���ī�� �Դϴ�.");
        return;
    }

    // ���̷�Ʈ x�� �⺻������ ������ �������� ��ì���ش�.
    // ��� �������� ���������� ������� �ʼ��̱� ������
    // �����带 ����ϰڴٴ� ���� �̸� ������ټ� �ִ�.
    Result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    if (Result != S_OK)
    {
        MSGASSERT("������ ������ ���뿡 ������ ������ϴ�.");
        return;
    }
    // �ʱ�ȭ ����
}

void UEngineGraphicDevice::CreateBackBuffer(const UEngineWindow& _Window)
{
    FVector Size = _Window.GetWindowSize();

    DXGI_SWAP_CHAIN_DESC ScInfo = { 0 };

    ScInfo.BufferCount = 2;
    ScInfo.BufferDesc.Width = Size.iX();
    ScInfo.BufferDesc.Height = Size.iY();
    ScInfo.OutputWindow = _Window.GetWindowHandle();
    // ��üȭ��
    // false�� ��üȭ��
    // true�� âȭ��
    ScInfo.Windowed = true;

    // �� ���� ����Ϳ� �󸶳� ������ �����Ұų�
    // �Ҽ� ������ �ض�.
    ScInfo.BufferDesc.RefreshRate.Denominator = 1;
    ScInfo.BufferDesc.RefreshRate.Numerator = 60;
    // ScInfo.BufferDesc.RefreshRate.Numerator = 144;

    // ������� ��������
    // 65536 �ܰ�
    // ������ ������ �� ū ����Ϳ����� �ǹ̰� �ִ�.
    //                                     8 8 8 8 32��Ʈ �������� ����۸� �����
    ScInfo.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    // ����� ������ �ǹ� ����. HDR ����͸� �ǹ̸� �������� �ִ�.
    // ScInfo.BufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

    // ����ͳ� �����쿡 �ȼ��� ���ŵǴ� ������ ��� 
    // �׳� ���� �����ɷ� ����
    ScInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    // ��¥ ���ȳ� �ƿ� 
    ScInfo.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // �뵵
    // DXGI_USAGE_RENDER_TARGET_OUTPUT ȭ�鿡 �׷����� �뵵�� ����Ѵ�.
    //                   ���⿡ �׸��� ����                  ���̴����� �����ͷε� ����Ҽ� ����
    ScInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;

    // ���ø�
    // ����Ƽ�� �ʿ����
    ScInfo.SampleDesc.Quality = 0;
    // �� ������ 1���� ����ϴ�.
    ScInfo.SampleDesc.Count = 1;

    // ���� n�� �������?
    // n���� ���ۿ� ���� 
    ScInfo.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    // ���� ���ȳ�
    ScInfo.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    //MainAdapter->Release();

    IDXGIFactory* pF = nullptr;

    // �� ���� ���丮�� ���ü� �ִ�.
    MainAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pF));

    // IUnknown* pDevice,
    // DXGI_SWAP_CHAIN_DESC* pDesc,
    // IDXGISwapChain** ppSwapChain

    pF->CreateSwapChain(Device, &ScInfo, &SwapChain);

    pF->Release();
    MainAdapter->Release();

    if (nullptr == SwapChain)
    {
        MSGASSERT("����ü�� ���ۿ� �����߽��ϴ�.");
    }

    // HDC��� ���� �˴ϴ�.
    // ����ü�� ���ο� �����ϴ� 
    // HDC�ȿ� bitmap�� ����ִ� �����̾���?
    // bitmap => ��¥ ���� �迭�� ���� �ڵ�
    // FColor Arr[100][100];
    // directx������ �̷� bitmap�� id3d11texture2d*

    // SwapChain���ο� id3d11texture2d*��� �ִ�.
    // DXBackBufferTexture => �� BITMAP�Դϴ�.
    DXBackBufferTexture = nullptr;
    if (S_OK != SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>
        (&DXBackBufferTexture)))
    {
        MSGASSERT("����� �ؽ�ó�� �����µ� �����߽��ϴ�.");
    };

    // id3d11texture2d* �̳༮ �����δ� �Ҽ� �ִ°� ���� �����ϴ�.
    // �ִ� �̹����� 2���� �����͸� ��Ÿ���� ���������� ���� �����Դϴ�.
    // �̹����� �����ϰų� ����Ҽ� �ִ� ������ id3d11texture2d*�� ���� �մϴ�.
    // WINAPI���� HDC ���� ��ó�� id3d11texture2d* ����������
    // �ؽ�ó���� ������ �մϴ�.

    //                             HBITMAP                       HDC
    if (S_OK != Device->CreateRenderTargetView(DXBackBufferTexture, nullptr, &RTV))
    {
        MSGASSERT("�ؽ�ó �������� ȹ�濡 �����߽��ϴ�");
    }

}

void UEngineGraphicDevice::RenderStart()
{
    FVector ClearColor;

    ClearColor = FVector(0.0f, 0.0f, 1.0f, 1.0f);

    // �̹��� �Ķ������� ä������.
    Context->ClearRenderTargetView(RTV, ClearColor.Arr1D);
}

void UEngineGraphicDevice::RenderEnd()
{
    // ���� ������ hwnd�� ���̷�Ʈ ������ ����� ����ض�.
    HRESULT Result = SwapChain->Present(0, 0);

    //             ����̽��� ������ ���� ����          ����̽��� ���µǾ������
    if (Result == DXGI_ERROR_DEVICE_REMOVED || Result == DXGI_ERROR_DEVICE_RESET)
    {
        MSGASSERT("�ػ� �����̳� ����̽� ���� ������ ��Ÿ�� ���� �����Ǿ����ϴ�");
        return;
    }
}