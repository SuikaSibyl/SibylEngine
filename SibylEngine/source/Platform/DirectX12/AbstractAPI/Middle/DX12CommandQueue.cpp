#include "SIByLpch.h"
#include "DX12CommandQueue.h"

#include "Platform/DirectX12/Common/DX12Utility.h"
#include "Platform/DirectX12/Common/DX12Context.h"

#include "Platform/DirectX12/AbstractAPI/Bottom/DX12ResourceStateTracker.h"
#include "Platform/DirectX12/AbstractAPI/Middle/DX12CommandList.h"

namespace SIByL
{
    DX12CommandQueue::DX12CommandQueue(D3D12_COMMAND_LIST_TYPE type)
        : m_FenceValue(0)
        , m_CommandListType(type)
        , m_bProcessInFlightCommandLists(true)
    {
        auto device = DX12Context::GetDevice();

        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = type;
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 0;

        DXCall(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_d3d12CommandQueue)));
        DXCall(device->CreateFence(m_FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_d3d12Fence)));

        switch (type)
        {
        case D3D12_COMMAND_LIST_TYPE_COPY:
            m_d3d12CommandQueue->SetName(L"Copy Command Queue");
            break;
        case D3D12_COMMAND_LIST_TYPE_COMPUTE:
            m_d3d12CommandQueue->SetName(L"Compute Command Queue");
            break;
        case D3D12_COMMAND_LIST_TYPE_DIRECT:
            m_d3d12CommandQueue->SetName(L"Direct Command Queue");
            break;
        }

        m_ProcessInFlightCommandListsThread = std::thread(&DX12CommandQueue::ProccessInFlightCommandLists, this);
    }

    DX12CommandQueue::~DX12CommandQueue()
    {
        m_bProcessInFlightCommandLists = false;
        m_ProcessInFlightCommandListsThread.join();
    }

    uint64_t DX12CommandQueue::Signal()
    {
        uint64_t fenceValue = ++m_FenceValue;
        m_d3d12CommandQueue->Signal(m_d3d12Fence.Get(), fenceValue);
        return fenceValue;
    }

    bool DX12CommandQueue::IsFenceComplete(uint64_t fenceValue)
    {
        return m_d3d12Fence->GetCompletedValue() >= fenceValue;
    }

    void DX12CommandQueue::WaitForFenceValue(uint64_t fenceValue)
    {
        if (!IsFenceComplete(fenceValue))
        {
            auto event = ::CreateEvent(NULL, FALSE, FALSE, NULL);
            assert(event && "Failed to create fence event handle.");

            // Is this function thread safe?
            m_d3d12Fence->SetEventOnCompletion(fenceValue, event);
            ::WaitForSingleObject(event, DWORD_MAX);

            ::CloseHandle(event);
        }
    }

    void DX12CommandQueue::Flush()
    {
        std::unique_lock<std::mutex> lock(m_ProcessInFlightCommandListsThreadMutex);
        m_ProcessInFlightCommandListsThreadCV.wait(lock, [this] { return m_InFlightCommandLists.Empty(); });

        // In case the command queue was signaled directly 
        // using the DX12CommandQueue::Signal method then the 
        // fence value of the command queue might be higher than the fence
        // value of any of the executed command lists.
        WaitForFenceValue(m_FenceValue);
    }

    Ref<DX12CommandList> DX12CommandQueue::GetCommandList()
    {
        Ref<DX12CommandList> commandList;

        // If there is a command list on the queue.
        if (!m_AvailableCommandLists.Empty())
        {
            m_AvailableCommandLists.TryPop(commandList);
        }
        else
        {
            if (m_InFlightCommandLists.Size() > 3)
            {
                while (m_AvailableCommandLists.Empty())
                {

                }
                m_AvailableCommandLists.TryPop(commandList);
            }
            else
            {
                // Otherwise create a new command list.
                commandList = CreateRef<DX12CommandList>(m_CommandListType);
            }
        }

        return commandList;
    }

    // Execute a command list.
    // Returns the fence value to wait for for this command list.
    uint64_t DX12CommandQueue::ExecuteCommandList(Ref<DX12CommandList> commandList)
    {
        return ExecuteCommandLists(std::vector<std::shared_ptr<DX12CommandList> >({ commandList }));
    }

    uint64_t DX12CommandQueue::ExecuteCommandLists(const std::vector<Ref<DX12CommandList> >& commandLists)
    {
        DX12ResourceStateTracker::Lock();

        // Command lists that need to put back on the command list queue.
        std::vector<std::shared_ptr<DX12CommandList> > toBeQueued;
        toBeQueued.reserve(commandLists.size() * 2);        // 2x since each command list will have a pending command list.

        // Generate mips command lists.
        std::vector<std::shared_ptr<DX12CommandList> > generateMipsCommandLists;
        generateMipsCommandLists.reserve(commandLists.size());

        // Command lists that need to be executed.
        std::vector<ID3D12CommandList*> d3d12CommandLists;
        d3d12CommandLists.reserve(commandLists.size() * 2); // 2x since each command list will have a pending command list.

        for (auto commandList : commandLists)
        {
            auto pendingCommandList = GetCommandList();
            bool hasPendingBarriers = commandList->Close(*pendingCommandList);
            pendingCommandList->Close();
            // If there are no pending barriers on the pending command list, there is no reason to 
            // execute an empty command list on the command queue.
            if (hasPendingBarriers)
            {
                d3d12CommandLists.push_back(pendingCommandList->GetGraphicsCommandList().Get());
            }
            d3d12CommandLists.push_back(commandList->GetGraphicsCommandList().Get());

            toBeQueued.push_back(pendingCommandList);
            toBeQueued.push_back(commandList);

            //auto generateMipsCommandList = commandList->GetGenerateMipsCommandList();
            //if (generateMipsCommandList)
            //{
            //    generateMipsCommandLists.push_back(generateMipsCommandList);
            //}
        }

        UINT numCommandLists = static_cast<UINT>(d3d12CommandLists.size());
        m_d3d12CommandQueue->ExecuteCommandLists(numCommandLists, d3d12CommandLists.data());
        uint64_t fenceValue = Signal();

        DX12ResourceStateTracker::Unlock();

        // Queue command lists for reuse.
        for (auto commandList : toBeQueued)
        {
            m_InFlightCommandLists.Push({ fenceValue, commandList });
        }

        //// If there are any command lists that generate mips then execute those
        //// after the initial resource command lists have finished.
        //if (generateMipsCommandLists.size() > 0)
        //{
        //    auto computeQueue = Application::Get().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE);
        //    computeQueue->Wait(*this);
        //    computeQueue->ExecuteCommandLists(generateMipsCommandLists);
        //}

        return fenceValue;
    }

    void DX12CommandQueue::Wait(const DX12CommandQueue& other)
    {
        m_d3d12CommandQueue->Wait(other.m_d3d12Fence.Get(), other.m_FenceValue);
    }

    ComPtr<ID3D12CommandQueue> DX12CommandQueue::GetD3D12CommandQueue() const
    {
        return m_d3d12CommandQueue;
    }

    void DX12CommandQueue::ProccessInFlightCommandLists()
    {
        std::unique_lock<std::mutex> lock(m_ProcessInFlightCommandListsThreadMutex, std::defer_lock);

        while (m_bProcessInFlightCommandLists)
        {
            CommandListEntry commandListEntry;

            lock.lock();
            while (m_InFlightCommandLists.TryPop(commandListEntry))
            {
                auto fenceValue = std::get<0>(commandListEntry);
                auto commandList = std::get<1>(commandListEntry);

                WaitForFenceValue(fenceValue);

                commandList->Reset();

                m_AvailableCommandLists.Push(commandList);
            }
            lock.unlock();
            m_ProcessInFlightCommandListsThreadCV.notify_one();

            std::this_thread::yield();
        }
    }

}